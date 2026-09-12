# Itajai Drive 3.2.1 - Hamachi / Virtual LAN Multiplayer

This checkpoint makes the existing native UDP multiplayer comfortable to use over a private virtual LAN such as LogMeIn Hamachi.

## Player flow

1. Install/open Hamachi on both PCs.
2. Join the same Hamachi network.
3. Launch Itajai Drive on both PCs.
4. Press `F12` to open the Multiplayer panel.
5. The game scans Windows network adapters and displays the detected Hamachi IPv4 (normally a `25.x.x.x` address).
6. Host selects **CRIAR JOGO** and presses Enter.
7. Guest selects **ENTRAR EM JOGO**, types the host's Hamachi IPv4, then presses Enter.
8. `F8` or the **DESCONECTAR** option ends the direct session.

No public relay or router port-forward is required for this mode. Hamachi provides the virtual network path; Itajai Drive continues to use its native UDP port `27015` on that virtual interface.

## Controls

- `F12`: open/close Multiplayer panel
- Arrow keys or `W/S`: select menu item
- Digits + `.`: enter host IPv4 while **ENTRAR EM JOGO** is selected
- Backspace: edit host IPv4
- Enter: confirm selected action
- `F8`: disconnect direct session

Legacy developer shortcuts remain available:

- `F6`: direct host
- `F7`: join localhost
- `--host`
- `--join=<IPv4>`
- `--port=<port>`

## Adapter detection

The native client calls Windows `GetAdaptersInfo` and looks for adapter descriptions/names containing `Hamachi` or `LogMeIn`. As a fallback, it recognizes a usable `25.x.x.x` IPv4 as a probable Hamachi adapter.

If Hamachi is not detected, Direct IP still works from the panel; the host address can be typed manually.

## Firewall

Windows Firewall may prompt when the native executable first hosts UDP traffic. Allowing the game on the relevant private/virtual network profile is sufficient. Router port-forwarding is not part of this mode.

## Architecture

- Local vehicle physics remains unchanged.
- Host/client transport remains the 3.1 Winsock UDP implementation.
- Vehicle snapshots remain approximately 20 Hz.
- Remote vehicles remain presentation-only/non-collidable in this checkpoint.
- The 3.2 public relay implementation is retained as an optional future transport and is not required for Hamachi play.
