# Itajaí Drive 3.2 — Internet Multiplayer

## Scope

3.2 keeps the validated 3.0.1/3.1 local vehicle simulation and adds an Internet transport around the existing 20 Hz vehicle snapshots. The first Internet milestone remains two-player and remote-player collision is intentionally disabled.

The relay is a rendezvous + UDP forwarding service. Both players establish outbound UDP mappings to the same public server, so normal home NAT does not require router port forwarding.

## Ports

- Direct/LAN multiplayer: UDP 27015
- Internet relay: UDP 27016 by default

## Run a relay on a public VPS

Requirements: Python 3.9+ and one public IPv4 address.

```bash
python3 server/relay_server.py --host 0.0.0.0 --port 27016
```

Allow the UDP port in the VPS firewall/security group. Example with UFW:

```bash
sudo ufw allow 27016/udp
```

### Optional systemd service

```ini
[Unit]
Description=Itajai Drive UDP Relay
After=network-online.target

[Service]
Type=simple
User=itajai
WorkingDirectory=/opt/itajai-drive
ExecStart=/usr/bin/python3 /opt/itajai-drive/server/relay_server.py --host 0.0.0.0 --port 27016
Restart=always
RestartSec=2

[Install]
WantedBy=multi-user.target
```

Then:

```bash
sudo systemctl daemon-reload
sudo systemctl enable --now itajai-relay
```

## Host a room

Either launch directly:

```powershell
.\ItajaiDriveNative.exe --relay=PUBLIC_RELAY_IP:27016 --internet-host
```

or launch with only `--relay=PUBLIC_RELAY_IP:27016` and press **F9** in game.

The HUD displays a six-character room code such as `K7DM4Q`. Share only that code with the other player.

## Join from another network

```powershell
.\ItajaiDriveNative.exe --relay=PUBLIC_RELAY_IP:27016 --internet-join=K7DM4Q
```

Both players must use the same relay. When connected, the HUD shows `CONECTADO` plus relay RTT. **F11** leaves the Internet session.

## Protocol behavior

1. Host sends CREATE to relay.
2. Relay allocates a random six-character room code.
3. Guest sends JOIN with the code.
4. Relay registers both public UDP endpoints and sends READY.
5. Existing 3.1 `Net301Snapshot` packets are wrapped in relay DATA packets and forwarded to the other registered endpoint.
6. Client-side interpolation renders the remote vehicle.
7. PING/PONG measures relay round-trip time and keeps the NAT mapping active.

The relay only forwards packets for endpoints registered in a room. It rate-limits each source and expires inactive sessions. The game state is not stored on the server.

## Current limitations

- two players per room;
- no multiplayer vehicle collision yet;
- traffic/weather/world authority is still local;
- room codes are rendezvous secrets, not account authentication;
- relay traffic is not encrypted in 3.2;
- a public relay host must exist before Internet mode can work without port forwarding.

## Automated validation

`server/test_relay.py` creates a local relay, creates/joins a room, forwards a test payload, validates ping/pong and validates leave notification. The Development Build Gate runs this integration test together with the native Windows build.
