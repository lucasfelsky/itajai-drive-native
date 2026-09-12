#!/usr/bin/env python3
import socket
import struct
import threading
import time
import unittest

from relay_server import (
    RelayServer, MAGIC, PROTOCOL, HEADER, CODE, CREATE, CREATED, JOIN, JOINED,
    READY, DATA, PING_REQ, PONG, LEAVE,
)


def header(packet_type: int, seq: int, nonce: int) -> bytes:
    return HEADER.pack(MAGIC, PROTOCOL, packet_type, seq, nonce)


def recv_until(sock: socket.socket, wanted: int, timeout: float = 2.0):
    end = time.monotonic() + timeout
    while time.monotonic() < end:
        data, _ = sock.recvfrom(1024)
        if len(data) >= HEADER.size and HEADER.unpack_from(data)[2] == wanted:
            return data
    raise TimeoutError(wanted)


class RelayIntegrationTest(unittest.TestCase):
    def setUp(self):
        self.server = RelayServer("127.0.0.1", 0)
        self.stop = False

        def worker():
            last_cleanup = time.monotonic()
            while not self.stop:
                try:
                    data, addr = self.server.sock.recvfrom(768)
                    self.server.handle(data, addr)
                except socket.timeout:
                    pass
                now = time.monotonic()
                if now - last_cleanup > 1.0:
                    self.server._cleanup(now)
                    last_cleanup = now

        self.thread = threading.Thread(target=worker, daemon=True)
        self.thread.start()
        self.host = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.guest = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.host.settimeout(2.0)
        self.guest.settimeout(2.0)
        self.addr = self.server.address

    def tearDown(self):
        self.stop = True
        self.host.close()
        self.guest.close()
        self.thread.join(timeout=1.0)
        self.server.close()

    def test_create_join_forward_ping_leave(self):
        host_nonce = 0x11111111
        guest_nonce = 0x22222222

        self.host.sendto(header(CREATE, 1, host_nonce), self.addr)
        created = recv_until(self.host, CREATED)
        code = CODE.unpack_from(created, HEADER.size)[0]
        self.assertEqual(len(code), 6)

        self.guest.sendto(header(JOIN, 1, guest_nonce) + CODE.pack(code, 0), self.addr)
        recv_until(self.guest, JOINED)
        recv_until(self.guest, READY)
        recv_until(self.host, READY)

        payload = b"vehicle-snapshot-test"
        data = header(DATA, 2, host_nonce) + CODE.pack(code, 0) + struct.pack("<H", len(payload)) + payload
        self.host.sendto(data, self.addr)
        forwarded = recv_until(self.guest, DATA)
        payload_offset = HEADER.size + CODE.size + 2
        self.assertEqual(forwarded[payload_offset:], payload)

        stamp = 12.25
        self.guest.sendto(header(PING_REQ, 3, guest_nonce) + struct.pack("<f", stamp), self.addr)
        pong = recv_until(self.guest, PONG)
        self.assertAlmostEqual(struct.unpack_from("<f", pong, HEADER.size)[0], stamp, places=3)

        self.guest.sendto(header(LEAVE, 4, guest_nonce) + CODE.pack(code, 0), self.addr)
        left = recv_until(self.host, LEAVE)
        self.assertEqual(CODE.unpack_from(left, HEADER.size)[0], code)


if __name__ == "__main__":
    unittest.main()
