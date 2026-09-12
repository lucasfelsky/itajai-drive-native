# Itajaí Drive Native

Mini-engine 3D nativa para Windows, construída do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido. O objetivo é direção livre em uma versão comprimida e reconhecível de Itajaí/SC, usando OpenStreetMap como base de ruas, prédios e navegação.

## Estado atual

- Jogo: **2.1.0 — Delivery Polish**
- Updater: **1.1.0 — retry + rollback transacional + handoff automático**
- Plataforma: Windows x64
- Distribuição: GitHub Releases público + updater nativo
- Build: GitHub Actions / Windows / clang-cl + lld-link
- Release gate: `tools/release_check.py` valida EXE, PAK, frota, updater handoff, manifesto, hashes e canal antes da tag

## Mundo

- OpenStreetMap / Overpass para ruas, prédios e semáforos
- corredor urbano de direção de arte: **Porto, Centro, Beira-Rio/Fazenda, Molhes, Atalaia, Cabeçudas e Praia Brava**
- footprints OSM extrudados para prédios próximos, com LOD volumétrico à distância
- relevo procedural, faixa costeira, praia e oceano
- spatial grid, collision world e streaming regional
- quality layer da Beira-Rio e conteúdo curado para Porto, Molhes e setores costeiros
- editor persistente de landmarks via `itajai_landmarks_v19.bin`

> Os setores, relevo, fachadas e detalhes urbanos são art-directed; o objetivo é reconhecimento/atmosfera, não reconstrução cadastral ou fotogramétrica.

## Direção e simulação

- quatro rodas com estado próprio de superfície/suspensão/carga
- slip angle dianteiro/traseiro e saturação progressiva de pneu
- transferência longitudinal de carga
- comportamento distinto **FWD / RWD / AWD**
- TCS/ABS aproximados, freio-motor, handbrake e perda de grip sob potência
- FOV dinâmico e câmeras externas/cockpit

## Renderer — 1.9+

- OpenGL nativo com **GLSL 1.20 PBR-compat** e fallback fixed-function
- microfacet lighting + Fresnel e ACES-like tone mapping
- wetness persistente, água dedicada, fog/clima e lightning
- céu de dusk/noite com estrelas e lua estilizada
- postes/comércio com pools locais de luz e feixes de farol no piso
- poças, pista úmida persistente e reflexos locais de freio
- materiais automotivos dedicados:
  - `CAR_PAINT` com clearcoat/Fresnel e reflexão hemisférica aproximada
  - `GLASS` tintado/refletivo
  - `RUBBER` para pneus/plásticos
  - `CHROME` para aro/trim/grade
- reflexos locais da cor dos veículos sobre pista molhada
- tachões refletivos procedurais em vias largas à noite

## Roads 2.0

- ruas acompanham a grade vertical do terreno
- jogador, tráfego e suspensão acompanham a altura da via
- deck de asfalto, meio-fio, calçada e sarjeta procedurais
- linhas de bordo, centro amarelo, divisões tracejadas, remendos e bueiros
- ciclovias em vias largas e lombadas procedurais

## Identidade de Itajaí

- comércio com placas e marquises em português
- casas baixas com muro/portão, torres costeiras com varandas e galpões portuários
- estacionamentos, postos e detalhes logísticos
- marcos para **CENTRO DE ITAJAI, BEIRA-RIO, PORTO DE ITAJAI, MOLHES DA BARRA, ATALAIA, CABECUDAS e PRAIA BRAVA**

## Frota brasileira

`tools/generate_vehicle_gltf.py` gera meshes originais por loft longitudinal usando proporções reconhecíveis de carros reais populares no Brasil. Badges/logotipos não são incluídos.

- **Fiat Uno Way 2014**
- **Volkswagen Gol G6**
- **Hyundai HB20**
- **Fiat Strada**
- **Toyota Corolla**
- **Jeep Renegade**
- **Chevrolet Onix**
- **Chevrolet Celta**

Os quatro slots jogáveis usam Uno Way, Gol G6, HB20 e Renegade. O tráfego usa os oito modelos.

## Vehicle Polish

- câmera interna na posição do motorista com offsets por carro
- mouse-look livre
- dashboard, cluster, console, bancos, pilares A, retrovisor e capô visível
- volante acompanha o esterço
- exterior com retrovisores, maçanetas e placas estilizadas
- rodas giram/esterçam e suspensão visual acompanha as quatro rodas
- áudio procedural por carro: `UNO 4C SOFT`, `GOL 4C RASP`, `HB20 3C PULSE`, `RENEGADE 4C DEEP`

## Tráfego e mundo vivo

- grafo viário + A*
- lane model, car-following, semáforos e lane connectors
- troca de faixa, yielding e densidade por horário/setor
- pedestres leves, barcos e carros estacionados

## Navegação e UI

- GPS A* com reroute
- minimapa em tempo real
- Photo Mode: **P** congela a simulação; **H** esconde a HUD
- tour opcional por landmarks com **F2**
- **F1** abre ajuda e **self-check 14/14**
- o self-check mostra PAK/frota/renderer/streaming e se GLSL está ativo ou em fallback
- `Tab` exibe telemetria de driving, renderer, wetness, iluminação, materiais e áudio

## Updater 1.1 + handoff 2.1

O updater continua sendo um executável Win32/WinHTTP independente, sem navegador ou runtime externo. Ele consulta o manifesto `latest`, valida tamanho + SHA-256 de cada arquivo, repete uma vez falhas transitórias e usa backup/rollback antes de substituir os arquivos gerenciados.

A 2.1 adiciona atualização automática do **próprio updater** sem exigir que um executável sobrescreva a si mesmo. A release publica `ItajaiDriveUpdater.next.exe`, uma cópia byte-idêntica do updater 1.1. Updaters antigos tratam esse arquivo normalmente e o baixam junto com o jogo; na abertura seguinte, o jogo tenta promovê-lo para `ItajaiDriveUpdater.exe` após o processo anterior encerrar. Se o Windows ainda mantiver o executável antigo bloqueado, o `.next` permanece e uma abertura posterior tenta novamente.

O manifesto gerencia o executável do jogo, o PAK e o updater `.next`. Cache OSM, world cache, configurações e landmarks ficam fora do update e são preservados.

## Gate de produção

Antes de publicar uma release, o CI exige:

- semver e README coerentes;
- EXE principal e updater presentes;
- PAK válido com pelo menos 16 assets;
- os oito carros de produção presentes nominalmente no PAK;
- updater oficial e `.next` byte-idênticos por SHA-256;
- manifesto contendo exatamente EXE + PAK + updater `.next`;
- `base_url` da tag correta e tamanho/SHA-256 dos arquivos gerenciados;
- `update_config.ini` válido.

Se uma dessas verificações falhar, não há tag nem GitHub Release.

## Controles

| Controle | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | direção |
| Espaço | freio de mão |
| Mouse | orbitar / olhar ao redor |
| C | trocar câmera, incluindo cockpit |
| E | trocar carro |
| R | reset |
| M | destino GPS aleatório |
| F2 | próximo ponto do tour |
| P | Photo Mode |
| H | esconder HUD no Photo Mode |
| F10 | editor de mundo |
| 1–8 no editor | tipo de prop |
| Enter / Delete | colocar / remover prop |
| [ / ] | girar prop |
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

O workflow `.github/workflows/release.yml` compila os executáveis Windows x64, gera os veículos procedurais em glTF, compila o PAK, cria o updater handoff, manifesto/hashes, executa `tools/release_check.py` e só então cria tag/GitHub Release.

## Evolução principal

- 0.5 — collision foundation + spatial grid
- 0.6/0.7 — física, lanes, câmera, GPS e streaming
- 0.8 — glTF/GLB asset pipeline
- 0.9/0.14 — renderer programável e efeitos
- 0.10–0.12 — expansão urbana, terreno e footprints OSM
- 0.15–0.18 — rodas, drivetrain, frota, tráfego e áudio
- 0.19/0.20 — editor e otimização/LOD
- 0.30–0.90 — quality district, mundo vivo, clima, UI, performance e conteúdo
- 1.0 — vertical slice integrado
- 1.1 — Driving Feel 2.0
- 1.2 — Renderer 2.0
- 1.3 — Roads 2.0
- 1.5 — Itajaí Identity Pass
- 1.6 — Real Car Model Pass
- 1.7 — Graphics Uplift
- 1.8 — Vehicle Polish
- 1.9 — Production Presentation
- 2.0 — Release Candidate / hardening
- **2.1 — Delivery Polish / updater handoff**

Dados de mapa: © OpenStreetMap contributors.
