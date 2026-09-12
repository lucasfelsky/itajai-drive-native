# Itajaí Drive Native

Mini-engine 3D nativa para Windows, construída do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido. O projeto é um sandbox de direção livre em uma versão comprimida e reconhecível de Itajaí/SC, com OpenStreetMap como base aberta para ruas, prédios, grafo e navegação.

## Estado atual

- Jogo: **3.0.0 — Open World Vertical Slice**
- Updater: **1.1.0 — retry + rollback transacional + handoff automático**
- Plataforma: Windows x64
- Distribuição: GitHub Releases público + updater nativo
- Build: GitHub Actions / Windows / clang-cl + lld-link
- Release gate: `tools/release_check.py` valida EXE, PAK, frota, updater handoff, manifesto, hashes e canal antes da tag

> A 3.0 fecha o primeiro grande vertical slice integrado. O gate automatizado valida build/distribuição; QA visual e de dirigibilidade das mudanças mais recentes continua sendo feito no PC real.

## 3.0 — o que está integrado

### Mundo e identidade local

- OpenStreetMap / Overpass para ruas, prédios e semáforos;
- corredor urbano art-directed: **Porto, Centro, Beira-Rio/Fazenda, Molhes, Atalaia, Cabeçudas e Praia Brava**;
- **Navegantes Gateway** aproveitando a margem/corredor já presente no bbox OSM, com linguagem logística, margem fluvial e aproximação aeroportuária procedural;
- footprints OSM no narrow phase de colisão quando disponíveis, com fallback AABB;
- relevo procedural, praia e oceano;
- Itajaí-Açu/canal e lâmina d'água da Beira-Rio complementando a costa;
- pass de fidelidade para Centro, Beira-Rio e Porto;
- casas, comércio, torres residenciais e galpões com identidade visual distinta;
- janelas acesas à noite e glints/reflexos simplificados em pista molhada;
- editor persistente de landmarks via `itajai_landmarks_v19.bin`.

Os setores, relevo, rio, fachadas e detalhes urbanos são **direção de arte**: o objetivo é reconhecimento, atmosfera e boa dirigibilidade, não reconstrução cadastral/fotogramétrica.

### Direção e física

- quatro rodas com estado próprio de superfície, suspensão e carga;
- slip angle dianteiro/traseiro e saturação progressiva de pneu;
- transferência longitudinal de carga;
- comportamento distinto FWD / RWD / AWD;
- TCS/ABS aproximados, freio-motor e freio de mão;
- casco físico por veículo com três círculos longitudinais;
- continuous sweep contra colisores estáticos;
- substeps adaptativos em frames lentos;
- ré com direção intuitiva;
- câmera chase gira para a direção de movimento em ré, respeita mouse-look e retorna automaticamente ao chase normal.

### Frota brasileira

`tools/generate_vehicle_gltf.py` gera meshes originais procedurais usando proporções inspiradas em carros populares no Brasil. Badges/logotipos não são incluídos.

- Fiat Uno Way 2014
- Volkswagen Gol G6
- Hyundai HB20
- Fiat Strada
- Toyota Corolla
- Jeep Renegade
- Chevrolet Onix
- Chevrolet Celta

Os quatro slots jogáveis usam Uno Way, Gol G6, HB20 e Renegade. O tráfego usa os oito modelos. O PAK de produção inclui partes separadas de body, glass e trim para a frota.

### Vehicle Polish / Game Feel

- cockpit na posição do motorista com offsets por carro;
- dashboard, cluster, console, bancos, pilares A, retrovisor e capô visível;
- volante acompanha o esterço;
- rodas giram/esterçam e acompanham suspensão visual;
- FOV/câmeras dinâmicos e mouse-look;
- marcas de pneu persistentes durante drift/freio de mão;
- spray em piso molhado e poeira simplificada em superfície solta;
- feedback visual discreto de impacto;
- áudio procedural distinto para os quatro carros jogáveis, mais pneu, vento, piso, troca de marcha e impactos.

### Tráfego 3.0 e mundo vivo

- grafo viário + A*;
- lane model, car-following, semáforos, conectores e yielding;
- perfis estáveis de motorista: cautious, normal, assertive e utility;
- setas para intenção de curva/troca de faixa;
- ritmo do tráfego responde a setor, horário e clima;
- fila e hazard visual para utilitários parados por mais tempo;
- reciclagem de tráfego distante mantém atividade perto do jogador;
- pedestres, ciclistas, barcos/rebocadores e carros estacionados;
- atividade do mundo vivo varia por horário.

### Renderer / atmosfera

- OpenGL nativo com GLSL 1.20 e fallback fixed-function;
- iluminação microfacet/Fresnel e tone mapping aproximado;
- materiais dedicados para pintura, vidro, borracha, chrome, água, asfalto e edifícios;
- céu, dusk/noite, estrelas/lua estilizada, fog e lightning;
- wetness persistente e pista molhada;
- iluminação local, faróis e reflexos simplificados;
- rio/canal + costa/oceano com movimento de água leve;
- espuma costeira procedural;
- janelas iluminadas determinísticas em prédios streamados;
- budgets e LOD para manter o passe atmosférico barato.

### Roads / cidade

- ruas acompanham a grade vertical do terreno;
- jogador, tráfego e suspensão acompanham a altura da via;
- deck de asfalto, meio-fio, calçada e sarjeta procedurais;
- centro amarelo, divisões tracejadas, linhas de bordo e tachões;
- remendos, bueiros, ciclovias e lombadas procedurais;
- mobiliário e props por setor: árvores, palmeiras, iluminação, containers, bancos, abrigo, guardrail e sinalização genérica.

### Streaming e performance

- spatial grid e collision world;
- cache regional `IDREG08` compatível;
- working set 5x5;
- builder regional hash-indexed/two-pass, evitando revarrer todos os objetos para cada região;
- lookup direto das regiões carregadas em vez de scan global;
- LOD de prédios, frota, tráfego, props e mundo vivo;
- telemetria de capacidade mostra pressão sobre budgets do stream antes de saturar arrays.

### Navegação, UI e QA

- GPS A* com reroute;
- minimapa em tempo real;
- Photo Mode;
- tour opcional por landmarks;
- garagem nativa em F3;
- F1 abre ajuda e self-check;
- TAB mostra telemetria dos sistemas e, no RC 2.90, health score + pressão de streaming;
- editor de mundo/landmarks em F10.

## Updater 1.1 + handoff

O updater é um executável Win32/WinHTTP independente. Ele consulta o manifesto `latest`, valida tamanho + SHA-256, repete uma vez falhas transitórias e usa backup/rollback antes de substituir arquivos gerenciados.

A própria atualização do updater usa `ItajaiDriveUpdater.next.exe`: a release publica uma cópia byte-idêntica e o jogo promove o `.next` quando o updater antigo já não está bloqueado pelo Windows. Cache OSM, world cache, configurações e landmarks ficam fora do manifesto e são preservados.

## Gate de produção

Antes de publicar uma release, o CI exige:

- semver e identidade de release coerentes;
- EXE principal e updater presentes;
- PAK válido e contrato da frota presente;
- oito carros de produção no PAK;
- parts de body/glass/trim quando o PAK usa o contrato de 32 assets;
- updater oficial e `.next` byte-idênticos por SHA-256;
- manifesto com o conjunto exato de arquivos gerenciados;
- `base_url` da tag correta, tamanho e SHA-256 consistentes;
- `update_config.ini` válido.

Se o gate falhar, não há tag nem GitHub Release.

## Controles

| Controle | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | direção, incluindo comportamento intuitivo em ré |
| Espaço | freio de mão |
| Mouse | orbitar / olhar ao redor |
| C | trocar câmera, incluindo cockpit |
| E | trocar carro |
| R | reset |
| M | destino GPS aleatório |
| F2 | próximo ponto do tour |
| F3 | garagem / seletor de veículo |
| P | Photo Mode |
| H | esconder HUD no Photo Mode |
| F10 | editor de mundo |
| 1–8 no editor | tipo de prop |
| Enter / Delete | colocar / remover prop |
| [ / ] | girar prop |
| Tab | debug / telemetria / RC health |
| T | avançar horário |
| Y | trocar clima |
| U | limpar cache do mapa |
| F1 | ajuda / self-check |
| Esc | menu / liberar mouse conforme contexto |

## Arquivos locais importantes

- `itajai_osm_cache_v10.json` — resposta OSM reutilizável
- `itajai_world_v10.bin` — cache binário do mundo
- `itajai_regions_v08.bin` — cache regional de render/stream
- `itajai_settings.ini` — configurações persistentes
- `itajai_landmarks_v19.bin` — landmarks persistentes
- `itajai_assets_v08.pak` — asset pack compilado

## Build e releases

`.github/workflows/release.yml` compila os executáveis Windows x64, gera/compila os assets glTF, cria updater handoff, manifesto e hashes, executa `tools/release_check.py` e só então cria a tag/GitHub Release.

## Evolução principal

- 0.5–1.0 — fundação, física, lanes, streaming, renderer, cidade e vertical slice inicial
- 1.1 — Driving Feel 2.0
- 1.2 — Renderer 2.0
- 1.3 — Roads 2.0
- 1.5 — Itajaí Identity Pass
- 1.6 — Real Car Model Pass
- 1.7 — Graphics Uplift
- 1.8 — Vehicle Polish
- 1.9 — Production Presentation
- 2.0 — Release Candidate / hardening
- 2.1–2.3 — delivery, QA de footprints e foundation physics
- 2.11 — chase camera/reverse driving pass
- 2.12 — Traffic AI 2.0
- 2.13 — Living City 2.0
- 2.14 — Audio 2.0
- 2.15 — Production UI / garagem nativa
- 2.20 — Performance & Streaming 3.0
- 2.30 — Itajaí Fidelity Pass
- 2.40 — Navegantes Gateway
- 2.50 — Game Feel + City Fidelity
- 2.60 — Traffic 3.0
- 2.70 — Coast & River 3.0
- 2.80 — Night & Wet City 3.0
- 2.90 — Release Candidate health/telemetry
- **3.0 — Open World Vertical Slice**

Detalhes do corte em `docs/RELEASE_3.0.md`.

Dados de mapa: © OpenStreetMap contributors.
