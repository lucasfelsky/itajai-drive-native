# Itajaí Drive Native

Uma mini-engine 3D nativa para Windows, construída do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido. O objetivo é direção livre em uma versão comprimida e reconhecível de Itajaí/SC, usando OpenStreetMap como base de ruas, prédios e navegação.

## Estado atual

- Jogo: **1.1.0 — Driving Feel 2.0**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Distribuição: GitHub Releases público + updater nativo
- Build: GitHub Actions / Windows / clang-cl + lld-link

## Mundo

- OpenStreetMap / Overpass para ruas, prédios e semáforos
- corredor urbano de direção de arte: **Porto, Centro, Beira-Rio/Fazenda, Molhes, Atalaia, Cabeçudas e Praia Brava**
- footprints OSM extrudados para prédios próximos, com LOD volumétrico à distância
- relevo procedural, faixa costeira, praia e oceano
- spatial grid, collision world e streaming regional
- quality layer da Beira-Rio e conteúdo curado para Porto, Molhes e setores costeiros
- editor persistente de landmarks via `itajai_landmarks_v19.bin`

> Os setores são regiões aproximadas de direção de arte; não representam limites administrativos oficiais.

## Direção e simulação — 1.1

A 1.1 mantém o chassis simcade validado da 1.0 e acrescenta uma camada dinâmica por eixo:

- quatro rodas com estado próprio de superfície/suspensão/carga
- slip angle dianteiro/traseiro e saturação progressiva de pneu
- transferência longitudinal de carga
- comportamento distinto **FWD / RWD / AWD**
- perda de aderência dos pneus motrizes sob potência
- freio-motor por marcha
- TCS/ABS aproximados integrados ao limite de aderência
- handbrake com redução forte de grip traseiro
- telemetria de understeer/oversteer, carga dos eixos e slip
- FOV da câmera varia suavemente com a velocidade
- asfalto, meio-fio e grama com comportamento diferente
- colisão com prédios, props e tráfego
- quatro perfis de carro jogável

## Tráfego e mundo vivo

- grafo viário + A*
- lane model, car-following, semáforos e lane connectors em interseções
- troca de faixa, yielding e densidade por horário/setor
- oito silhuetas fictícias de veículos inspiradas em categorias comuns no Brasil
- pedestres leves, barcos e carros estacionados

## Renderer

- OpenGL nativo com renderer GLSL 1.20 e fallback fixed-function
- materiais procedurais e iluminação fisicamente inspirada compatível
- wetness persistente, chuva, névoa, vento e relâmpagos
- sombras projetadas, reflexos estilizados de pista molhada e pós leve
- glTF/GLB compilado para runtime PAK
- cache de meshes por display lists
- LOD/frame budget adaptativo para preservar física e navegação antes de cortar detalhes cosméticos

## Áudio

- áudio procedural nativo via WinMM
- motor ligado a RPM/carga
- ruído de rodagem
- chuva e ambiente costeiro
- fallback silencioso se o dispositivo de áudio não estiver disponível

## Navegação e UI

- GPS A* com reroute
- minimapa em tempo real com ruas, rota, tráfego e jogador
- Photo Mode: **P** congela a simulação; **H** esconde a HUD
- tour opcional por landmarks: **F2** escolhe o próximo destino conectado
- tela de ajuda/self-check: **F1**
- `Tab` mostra também a telemetria da camada Driving Feel 2.0

## Controles

| Controle | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | direção |
| Espaço | freio de mão |
| Mouse | orbitar / olhar ao redor |
| C | trocar câmera |
| E | trocar carro |
| R | reset |
| M | destino GPS aleatório |
| F2 | próximo ponto do tour opcional |
| P | Photo Mode |
| H | esconder HUD no Photo Mode |
| F10 | editor de mundo |
| 1–8 no editor | tipo de prop |
| Enter no editor | colocar prop |
| Delete no editor | remover prop próximo |
| [ / ] no editor | girar prop |
| Tab | debug / telemetria |
| T | avançar horário |
| Y | trocar clima |
| U | limpar cache do mapa |
| F1 | ajuda / self-check |
| Esc | sair / comportamento de menu conforme contexto |

## Arquivos locais importantes

- `itajai_osm_cache_v10.json` — resposta OSM reutilizável
- `itajai_world_v10.bin` — cache binário do mundo
- `itajai_settings.ini` — configurações persistentes
- `itajai_landmarks_v19.bin` — edição/landmarks persistentes
- `itajai_assets_v08.pak` — asset pack compilado

## Build e releases

O workflow `.github/workflows/release.yml` compila os executáveis em Windows x64, gera as carrocerias fictícias em glTF, compila todos os assets para o PAK, gera manifesto + hashes SHA-256, cria a tag e publica a GitHub Release. O updater valida tamanho e hash antes de substituir arquivos gerenciados; caches, configurações e landmarks permanecem preservados.

## Evolução principal

- 0.5 — collision foundation + spatial grid
- 0.6/0.7 — física, lanes, câmera, GPS e streaming
- 0.8 — glTF/GLB asset pipeline
- 0.9/0.14 — renderer programável e efeitos
- 0.10–0.12 — expansão urbana, terreno e footprints OSM
- 0.15–0.18 — rodas, drivetrain, frota, tráfego e áudio
- 0.19/0.20 — editor e otimização/LOD
- 0.30 — Beira-Rio Quality District
- 0.40 — frota ambiente expandida
- 0.50 — mundo vivo
- 0.60 — clima dinâmico
- 0.70 — minimapa, UI e Photo Mode
- 0.80 — performance polish
- 0.90 — conteúdo final de distritos
- 1.0 — vertical slice integrado
- **1.1 — Driving Feel 2.0**

Dados de mapa: © OpenStreetMap contributors.
