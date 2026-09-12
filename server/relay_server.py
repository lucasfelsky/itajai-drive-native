#!/usr/bin/env python3
"""Itajai Drive 3.2 UDP relay/rendezvous server.

Standard-library only. It creates two-player session codes and forwards validated
relay DATA packets between the registered host/client endpoints. The game payload
inside DATA stays the existing 3.1 vehicle snapshot protocol.
"""
from __future__ import annotations

import argparse
import secrets
import socket
import struct
import time
from dataclasses import dataclass
from typing import Dict, Optional, Tuple

MAGIC = 0x32524A49  # little-endian bytes: IJR2
PROTOCOL = 1
HEADER = struct.Struct("<IHHII")  # magic, protocol, type, sequence, nonce
CODE = struct.Struct("<6sH")
PING = struct.Struct("<f")
ERROR = struct.Struct("<I")

CREATE = 1
CREATED = 2
JOIN = 3
JOINED = 4
READY = 5
DATA = 6
PING_REQ = 7
PONG = 8
LEAVE = 9
ERROR_MSG = 10

ERR_BAD_PACKET = 1
ERR_ROOM_NOT_FOUND = 2
ERR_ROOM_FULL = 3
ERR_NOT_MEMBER = 4

ALPHABET = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"
MAX_PACKET = 768
SESSION_TTL = 45.0
MAX_PACKETS_PER_SECOND = 240

Endpoint = Tuple[str, int]


@dataclass
class Peer:
    addr: Endpoint
    nonce: int
    last_seen: float


@dataclass
class Session:
    code: str
    host: Peer
    guest: Optional[Peer]
    created: float


class RelayServer:
    def __init__(self, host: str = "0.0.0.0", port: int = 27016):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((host, port))
        self.sock.settimeout(0.25)
        self.sessions: Dict[str, Session] = {}
        self.rate: Dict[Endpoint, Tuple[int, int]] = {}
        self.sequence = 0

    @property
    def address(self) -> Endpoint:
        return self.sock.getsockname()

    def close(self) -> None:
        self.sock.close()

    def _next_seq(self) -> int:
        self.sequence = (self.sequence + 1) & 0xFFFFFFFF
        return self.sequence

    def _header(self, packet_type: int, nonce: int = 0) -> bytes:
        return HEADER.pack(MAGIC, PROTOCOL, packet_type, self._next_seq(), nonce)

    def _send_code(self, addr: Endpoint, packet_type: int, code: str, nonce: int = 0) -> None:
        self.sock.sendto(self._header(packet_type, nonce) + CODE.pack(code.encode("ascii"), 0), addr)

    def _send_error(self, addr: Endpoint, reason: int, nonce: int = 0) -> None:
        self.sock.sendto(self._header(ERROR_MSG, nonce) + ERROR.pack(reason), addr)

    def _new_code(self) -> str:
        for _ in range(100):
            code = "".join(secrets.choice(ALPHABET) for _ in range(6))
            if code not in self.sessions:
                return code
        raise RuntimeError("unable to allocate room code")

    def _allow(self, addr: Endpoint, now: float) -> bool:
        sec = int(now)
        old_sec, count = self.rate.get(addr, (sec, 0))
        if old_sec != sec:
            old_sec, count = sec, 0
        count += 1
        self.rate[addr] = (old_sec, count)
        return count <= MAX_PACKETS_PER_SECOND

    def _cleanup(self, now: float) -> None:
        stale = []
        for code, session in self.sessions.items():
            newest = session.host.last_seen
            if session.guest:
                newest = max(newest, session.guest.last_seen)
            if now - newest > SESSION_TTL:
                stale.append(code)
        for code in stale:
            self.sessions.pop(code, None)
        if len(self.rate) > 4096:
            self.rate.clear()

    @staticmethod
    def _decode_code(data: bytes, offset: int = HEADER.size) -> Optional[str]:
        if len(data) < offset + CODE.size:
            return None
        raw, _ = CODE.unpack_from(data, offset)
        try:
            code = raw.decode("ascii").upper()
        except UnicodeDecodeError:
            return None
        if len(code) != 6 or any(c not in ALPHABET for c in code):
            return None
        return code

    @staticmethod
    def _member(session: Session, addr: Endpoint, nonce: int) -> int:
        if session.host.addr == addr and session.host.nonce == nonce:
            return 1
        if session.guest and session.guest.addr == addr and session.guest.nonce == nonce:
            return 2
        return 0

    def handle(self, data: bytes, addr: Endpoint) -> None:
        now = time.monotonic()
        if not self._allow(addr, now):
            return
        if len(data) < HEADER.size or len(data) > MAX_PACKET:
            self._send_error(addr, ERR_BAD_PACKET)
            return
        magic, protocol, packet_type, sequence, nonce = HEADER.unpack_from(data)
        del sequence
        if magic != MAGIC or protocol != PROTOCOL:
            return

        if packet_type == CREATE:
            # Idempotent retry: reuse an existing room owned by the same endpoint+nonce.
            for session in self.sessions.values():
                if session.host.addr == addr and session.host.nonce == nonce:
                    session.host.last_seen = now
                    self._send_code(addr, CREATED, session.code, nonce)
                    return
            code = self._new_code()
            self.sessions[code] = Session(code, Peer(addr, nonce, now), None, now)
            self._send_code(addr, CREATED, code, nonce)
            return

        if packet_type == JOIN:
            code = self._decode_code(data)
            if not code or code not in self.sessions:
                self._send_error(addr, ERR_ROOM_NOT_FOUND, nonce)
                return
            session = self.sessions[code]
            if session.guest and not (session.guest.addr == addr and session.guest.nonce == nonce):
                self._send_error(addr, ERR_ROOM_FULL, nonce)
                return
            session.guest = Peer(addr, nonce, now)
            session.host.last_seen = now
            self._send_code(addr, JOINED, code, nonce)
            self._send_code(session.host.addr, READY, code, session.host.nonce)
            self._send_code(addr, READY, code, nonce)
            return

        if packet_type == PING_REQ:
            # Echo timestamp bytes verbatim so the client can calculate RTT.
            if len(data) >= HEADER.size + PING.size:
                self.sock.sendto(self._header(PONG, nonce) + data[HEADER.size:HEADER.size + PING.size], addr)
            return

        code = self._decode_code(data)
        if not code or code not in self.sessions:
            self._send_error(addr, ERR_ROOM_NOT_FOUND, nonce)
            return
        session = self.sessions[code]
        member = self._member(session, addr, nonce)
        if not member:
            self._send_error(addr, ERR_NOT_MEMBER, nonce)
            return
        if member == 1:
            session.host.last_seen = now
        elif session.guest:
            session.guest.last_seen = now

        if packet_type == LEAVE:
            if member == 1:
                if session.guest:
                    self._send_code(session.guest.addr, LEAVE, code, session.guest.nonce)
                self.sessions.pop(code, None)
            else:
                self._send_code(session.host.addr, LEAVE, code, session.host.nonce)
                session.guest = None
            return

        if packet_type != DATA:
            return
        payload_offset = HEADER.size + CODE.size
        if len(data) < payload_offset + 2:
            self._send_error(addr, ERR_BAD_PACKET, nonce)
            return
        payload_len = struct.unpack_from("<H", data, payload_offset)[0]
        payload_start = payload_offset + 2
        if payload_len <= 0 or payload_start + payload_len != len(data) or payload_len > 512:
            self._send_error(addr, ERR_BAD_PACKET, nonce)
            return
        target = session.guest.addr if member == 1 and session.guest else session.host.addr if member == 2 else None
        if target:
            self.sock.sendto(data, target)

    def serve_forever(self) -> None:
        print(f"Itajai Drive relay listening on UDP {self.address[0]}:{self.address[1]}", flush=True)
        last_cleanup = time.monotonic()
        try:
            while True:
                try:
                    data, addr = self.sock.recvfrom(MAX_PACKET)
                    self.handle(data, addr)
                except socket.timeout:
                    pass
                now = time.monotonic()
                if now - last_cleanup >= 5.0:
                    self._cleanup(now)
                    last_cleanup = now
        except KeyboardInterrupt:
            pass
        finally:
            self.close()


def main() -> None:
    parser = argparse.ArgumentParser(description="Itajai Drive UDP relay")
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--port", type=int, default=27016)
    args = parser.parse_args()
    RelayServer(args.host, args.port).serve_forever()


if __name__ == "__main__":
    main()
