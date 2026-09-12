# Itajaí Drive Native

Uma mini-engine 3D nativa para Windows, construída do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido. O objetivo é direção livre em uma versão comprimida e reconhecível de Itajaí/SC, usando OpenStreetMap como base de ruas, prédios e navegação.

## Estado atual

- Jogo: **1.8.0 — Vehicle Polish**
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

> Os setores, relevo, fachadas e detalhes urbanos são art-directed; o objetivo é reconhecimento/atmosfera, não uma reconstrução cadastral ou fotogramétrica.

## Direção e simulação — 1.1+

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

## Renderer — 1.7+

- OpenGL nativo com **GLSL 1.20 PBR-compat** e fallback fixed-function
- microfacet lighting + Fresnel e ACES-like tone mapping
- wetness persistente alimenta roughness e resposta especular
- água com material dedicado, Fresnel aproximado e ondulação procedural
- iluminação/fog respondem a cloud, wetness e lightning
- dusk/amanhecer reforçados, céu estrelado e lua estilizada
- postes e comércio criam pools locais de iluminação
- feixes visuais de farol no piso para player e tráfego próximo
- pista continua úmida enquanto `wetness` seca depois da chuva
- poças determinísticas e reflexos locais
- reflexo vermelho de freio em piso molhado
- vegetação próxima recebe volume adicional de copa

## Roads 2.0 — 1.3

- ruas próximas seguem uma grade vertical derivada do terreno em vez de Y=0 global
- jogador, tráfego e suspensão acompanham a altura da via
- deck de asfalto elevado sobre o terreno
- meio-fio, calçada e sarjeta procedurais
- linhas de bordo, centro amarelo e divisões tracejadas
- remendos de asfalto e grelhas/bueiros determinísticos
- ciclovias em trechos largos de Beira-Rio e setores costeiros
- lombadas procedurais esparsas em vias locais com resposta física leve

## Itajaí Identity Pass — 1.5

- prédios/footprints próximos são ancorados à elevação do terreno
- comércio recebe placas e marquises em português
- casas baixas podem receber muro e portão frontal
- torres costeiras podem receber varandas
- galpões do Porto recebem portas de doca e detalhes logísticos
- alguns comércios recebem estacionamentos e postos procedurais
- marcos para **CENTRO DE ITAJAI, BEIRA-RIO, PORTO DE ITAJAI, MOLHES DA BARRA, ATALAIA, CABECUDAS e PRAIA BRAVA**

## Frota real — 1.6+

`tools/generate_vehicle_gltf.py` gera meshes originais por loft longitudinal usando proporções reconhecíveis de carros reais populares no Brasil. Os modelos são compilados para o PAK da engine.

Frota atual:

- **Fiat Uno Way 2014**
- **Volkswagen Gol G6**
- **Hyundai HB20**
- **Fiat Strada**
- **Toyota Corolla**
- **Jeep Renegade**
- **Chevrolet Onix**
- **Chevrolet Celta**

Os quatro slots jogáveis usam Uno Way, Gol G6, HB20 e Renegade. O tráfego usa os oito modelos. Badges/logotipos não são incluídos nos meshes.

## Vehicle Polish — 1.8

- câmera interna posicionada na região do motorista em vez de acima do teto;
- offsets de cockpit próprios para Uno, Gol, HB20 e Renegade;
- mouse-look continua livre dentro do carro;
- interior procedural leve com dashboard, cluster, console, bancos, pilares A e retrovisor;
- volante acompanha o esterço;
- capô/fenders visíveis pelo para-brisa;
- exterior ganha retrovisores, maçanetas e placas estilizadas;
- rodas continuam girando/esterçando e a suspensão visual acompanha as quatro rodas;
- áudio procedural passa a ter voz diferente para cada carro jogável: `UNO 4C SOFT`, `GOL 4C RASP`, `HB20 3C PULSE` e `RENEGADE 4C DEEP`;
- som continua reagindo a RPM, throttle, marcha, rodagem, chuva e ambiente.

## Tráfego e mundo vivo

- grafo viário + A*
- lane model, car-following, semáforos e lane connectors em interseções
- troca de faixa, yielding e densidade por horário/setor
- oito modelos visuais reconhecíveis de veículos comuns no Brasil
- pedestres leves, barcos e carros estacionados

## Áudio

- áudio procedural nativo via WinMM
- perfis de motor distintos por carro jogável
- motor ligado a RPM/carga/marcha
- ruído de rodagem
- chuva e ambiente costeiro
- fallback silencioso se o dispositivo de áudio não estiver disponível

## Navegação e UI

- GPS A* com reroute
- minimapa em tempo real com ruas, rota, tráfego e jogador
- Photo Mode: **P** congela a simulação; **H** esconde a HUD
- tour opcional por landmarks: **F2** escolhe o próximo destino conectado
- tela de ajuda/self-check: **F1**
- HUD mostra o modelo jogável atual
- `Tab` inclui telemetria de driving, renderer, iluminação, wetness e áudio do veículo

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

O workflow `.github/workflows/release.yml` compila os executáveis em Windows x64, gera os veículos procedurais em glTF, compila todos os assets para o PAK, gera manifesto + hashes SHA-256, cria a tag e publica a GitHub Release. O updater valida tamanho e hash antes de substituir arquivos gerenciados; caches, configurações e landmarks permanecem preservados.

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
- **1.8 — Vehicle Polish**

Dados de mapa: © OpenStreetMap contributors.
