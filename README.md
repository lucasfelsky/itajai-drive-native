# Itajaí Drive Native

Mini engine 3D nativa para Windows, feita do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido.

O projeto começou como um protótipo de direção livre inspirado em Itajaí/SC e evoluiu para uma engine própria com OpenStreetMap, tráfego, grafo viário, A*, collision world, spatial grid, física veicular simcade, câmera livre, GPS dinâmico, streaming regional, asset pipeline glTF/GLB, renderer programável, configurações persistentes e updater nativo.

## Estado atual

- Jogo: **0.9.0 — Programmable Renderer & Atmosphere**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Canal de releases: **GitHub Releases público**

### Fundação / simulação

- Janela Win32 nativa
- Quatro perfis de veículo e quatro câmeras
- Ruas e construções via OpenStreetMap / Overpass
- Cache do mundo `itajai_world_v05.bin`
- Grafo viário direcionado, mão única e `oneway=-1`
- A* para tráfego e GPS
- Spatial grid de 40 m e collision world
- Colliders de prédios, semáforos, tráfego e props urbanos
- Superfícies, grip, bicycle model simcade, slip, roll/pitch e quatro rodas amostradas individualmente
- Modelo de faixas, car-following, frenagem progressiva e semáforos
- Lane connectors Bézier nas interseções
- GPS com destino persistente e reroute automático

### Câmera / configurações

- Mouse-look com captura por clique
- Câmeras externas orbitáveis e câmera interna free-look
- Sensibilidade ajustável e inverter eixo Y
- Menu de pausa/configurações
- Preferências persistidas em `itajai_settings.ini`, fora do manifesto do updater

### 0.8 — Asset Pipeline & Regional Streaming

- **glTF 2.0 / GLB como formatos-fonte**
- `tools/compile_assets.py` converte assets para `itajai_assets_v08.pak`
- Runtime PAK com vertices `position + normal + UV`, índices, bounds e collider radius
- Assets-semente: árvore, poste e container
- **Streaming regional real por arquivo** em regiões de ~320 m
- Cache local `itajai_regions_v08.bin`
- Working set 5×5 ao redor do jogador carregado por offset de arquivo
- Grafo de rota continua residente para A*/tráfego; geometria visual é streamada
- Props urbanos aparecem/desaparecem com o working set e participam de colisão
- Falha no asset pack/streaming cai para os renderers anteriores em vez de impedir o jogo de abrir

### 0.9 — Renderer programável

- **GLSL 1.20** resolvido dinamicamente por `wglGetProcAddress`
- Fallback automático para fixed-function caso shader não esteja disponível
- Normais e UVs nos primitives da engine
- Materiais/texturas procedurais para grama, asfalto e fachadas
- Luz direcional do sol
- Ambient light dependente de horário e clima
- Fog exponencial
- Wetness/darkening durante chuva
- Céu em gradiente com ciclo dia/noite e sol simples
- Sombras projetadas/contact shadows para veículos, prédios e props próximos
- Chuva leve/forte renderizada com streaks 3D
- Debug HUD mostra se o renderer ativo é GLSL ou fallback

### Infraestrutura

- Updater com staging, validação de tamanho e SHA-256
- `itajai_assets_v08.pak` também é atualizado/verificado automaticamente
- Build de Windows + compilação de assets + release via GitHub Actions
- Caches OSM, mundo, regiões e settings locais são preservados entre updates quando compatíveis

## Documentação técnica

- `docs/FOUNDATION_0.5.md`
- `docs/SIMULATION_0_6.md`
- `docs/SIMULATION_0_7.md`
- `docs/SETTINGS_0.7.1.md`
- `docs/ASSETS_STREAMING_0_8.md`
- `docs/RENDERER_0_9.md`

## Próximo milestone

A arquitetura agora já separa simulação, mundo, streaming, assets e renderização. O próximo passo natural é **0.10 — expansão pesada de Itajaí e geração urbana**: footprints de prédios melhores, calçadas/canteiros, mais objetos urbanos, setores Centro/Beira-Rio/Porto/Atalaia/Molhes/Cabeçudas/Praia Brava e spawn de tráfego integrado ao streaming.

## Controles

| Controle | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | esterço |
| Espaço | freio de mão / redução de grip |
| Clique esquerdo | capturar mouse para controlar a câmera |
| Mouse | orbitar / olhar ao redor |
| Esc | soltar mouse; com mouse livre, abrir configurações |
| W/S ou setas no menu | navegar |
| A/D ou esquerda/direita no menu | ajustar opção |
| Enter no menu | selecionar |
| C | trocar câmera |
| E | trocar carro |
| R | reset |
| M | criar destino GPS / A* |
| Tab | debug + telemetria/streaming/renderer |
| T | horário |
| Y | clima |
| U | limpar cache do mapa |

## Build e releases

O workflow `.github/workflows/release.yml` compila os executáveis no Windows, converte os assets glTF/GLB para o PAK runtime, gera SHA-256/manifesto, cria a tag e publica a release.

## Atualizador

O updater consulta `manifest.txt` da release mais recente, baixa primeiro para `.update/`, valida tamanho + SHA-256 e só depois substitui os arquivos gerenciados. Se a rede ou o download falhar, a instalação atual continua intacta.

Dados de mapa: © OpenStreetMap contributors.
