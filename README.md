# Itajaí Drive Native

Mini engine 3D nativa para Windows, feita do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido.

O projeto começou como um protótipo de direção livre inspirado em Itajaí/SC e evoluiu para uma engine própria com OpenStreetMap, tráfego, grafo viário, A*, collision world, spatial grid, simulação de faixas, física veicular simcade, clima, horário, cache binário do mundo e updater nativo.

## Estado atual

- Jogo: **0.6.0 — Road & Vehicle Simulation**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Canal de releases: **GitHub Releases público**

### Implementado

- Janela Win32 nativa
- Renderização OpenGL 1.1
- Quatro perfis de veículo e quatro câmeras
- Ruas e construções via OpenStreetMap / Overpass
- Cache local do mundo (`itajai_world_v05.bin`)
- Grafo viário direcionado, mão única e `oneway=-1`
- A* para tráfego e GPS
- Spatial grid de 40 m com hash espacial
- Indexação espacial de colliders e segmentos de rua
- Colliders para construções e postes de semáforo
- Broad phase via spatial grid + narrow phase circle-vs-AABB
- Colisão jogador × cenário e jogador × tráfego
- PhysicsBody com posição, velocidade, yaw, velocidade angular, massa, restituição e aderência
- Superfícies: asfalto, meio-fio/calçada e grama/fora da via
- **Modelo de faixas derivado da largura da via**
- **Tráfego posicionado no centro da faixa do sentido correto**
- **Car-following com distância segura e frenagem progressiva**
- **Frenagem para semáforo vermelho baseada em distância de parada**
- **Luzes de freio no tráfego**
- **VehicleProfile com massa, wheelbase, track, esterço, freio, grip, aero e rolling resistance**
- **Bicycle model simcade com velocidade lateral/slip e yaw rate**
- **Amostragem independente das quatro rodas no spatial grid**
- **Roll/pitch visual e rodas dianteiras esterçando**
- **Marcações de faixa renderizadas a partir do mesmo lane model usado pela IA**
- Debug visual do spatial grid/colliders e telemetria física via Tab
- Updater com staging + tamanho + SHA-256
- Build/release automático via GitHub Actions

A fundação espacial está em `docs/FOUNDATION_0.5.md` e a simulação viária/veicular em `docs/SIMULATION_0_6.md`.

## Próximas etapas naturais

A base agora já separa mundo, colisão, faixas e dinâmica veicular. Os próximos blocos naturais são connectors de faixa em interseções/troca de faixa, GPS com reroute dinâmico, streaming de regiões e depois loader glTF + renderer moderno.

## Controles

| Tecla | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | esterço |
| Espaço | freio de mão / redução de grip |
| C | câmera |
| E | trocar carro |
| R | reset |
| M | GPS / A* |
| Tab | debug + grid/colliders/telemetria |
| T | horário |
| Y | clima |
| U | limpar cache do mapa |
| Esc | sair |

## Build e releases

O workflow `.github/workflows/release.yml` compila `ItajaiDriveNative.exe` e `ItajaiDriveUpdater.exe` em um runner Windows. Alterar o arquivo `VERSION` na branch `main` dispara uma build de release, gera o manifesto com SHA-256, cria a tag correspondente e publica os binários no GitHub Releases.

## Atualizador

O updater consulta automaticamente o `manifest.txt` da release mais recente, baixa primeiro para `.update/`, valida tamanho e SHA-256 e só então substitui os arquivos instalados. Se a rede falhar, a instalação atual permanece intacta e o jogo abre normalmente.

`itajai_osm_cache.json` não é gerenciado pelo updater. A 0.6 mantém o formato de mundo da 0.5 e reutiliza `itajai_world_v05.bin`, evitando invalidar o mapa apenas por uma mudança de simulação.

O canal de releases é público, então o updater não precisa de login nem token GitHub. Nenhum token pessoal é embutido no executável.

Dados de mapa: © OpenStreetMap contributors.
