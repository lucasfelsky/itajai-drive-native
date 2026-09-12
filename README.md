# Itajaí Drive Native

Mini engine 3D nativa para Windows, feita do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido.

O projeto começou como um protótipo de direção livre inspirado em Itajaí/SC e evoluiu para uma engine própria com OpenStreetMap, tráfego, A*, collision world, física veicular simcade, streaming regional, asset pipeline glTF/GLB, renderer programável, geração urbana, configurações persistentes e updater nativo.

## Estado atual

- Jogo: **0.10.0 — Urban Expansion**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Canal de releases: **GitHub Releases público**

## 0.10 — Urban Expansion

A 0.10 amplia o mundo OSM para um corredor muito maior e prepara identidades urbanas diferentes ao longo da cidade/costa.

### Mundo expandido

- Bbox OSM aproximado: `-26.9580,-48.6860,-26.8840,-48.6250`
- Corredor de direção de arte: **Porto, Centro, Beira-Rio/Fazenda, Molhes, Atalaia, Cabeçudas e Praia Brava**
- Novo cache bruto: `itajai_osm_cache_v10.json`
- Novo cache de mundo: `itajai_world_v10.bin`
- Até 8.000 road ways, 60.000 segmentos, 24.000 prédios, 40.000 nós e 120.000 edges
- Spatial grid/collision world redimensionado para o mapa maior

> Os setores são regiões aproximadas de direção de arte, não limites administrativos oficiais.

### Geração urbana

O parser usa tags `building=*`, `height` e `building:levels` para diferenciar arquétipos simples:

- casas/residencial baixo;
- apartamentos/torres;
- comercial;
- industrial/galpões;
- cívico;
- genérico.

O renderer acrescenta variações de cor e silhueta, roof caps, podiums, storefronts e detalhes industriais. Os prédios ainda são aproximações retangulares do bounding box do footprint OSM; extrusão do polígono exato fica para uma evolução posterior.

### Ruas e espaço urbano

- Calçadas procedurais em vias urbanas compatíveis
- Canteiro central simples em avenidas largas de mão dupla
- Faixas continuam usando o lane model da 0.6/0.7
- Props e densidade mudam conforme o setor atual

### Asset pack urbano

O `itajai_assets_v08.pak` continua usando o formato criado na 0.8, mas agora compila oito meshes glTF:

- `urban_tree`
- `street_lamp`
- `port_container`
- `coastal_palm`
- `urban_bench`
- `bus_shelter`
- `road_guardrail`
- `road_sign`

O Porto favorece containers e infraestrutura pesada; setores costeiros favorecem palmeiras/vegetação; áreas urbanas recebem mais iluminação, placas e mobiliário.

### Streaming / tráfego

- Regiões de ~320 m
- Working set 5×5
- Capacidade ampliada para até 1.024 regiões
- Até 30.000 segmentos e 12.000 prédios no conjunto visual ativo
- Até 5.000 props regionais
- Tráfego distante pode ser reciclado e reroteado perto do jogador para o mapa maior não ficar vazio
- O grafo completo continua residente para GPS/A* e tráfego

## Fundação preservada

- Win32 nativo + OpenGL
- Collision world + spatial grid
- Bicycle model simcade, slip, roll/pitch, superfícies e colisões
- Lane model, car-following, semáforos e Bézier lane connectors
- GPS A* com reroute
- Mouse-look estilo GTA/simulador
- Menu de pausa/configurações
- Sensibilidade e invert-Y persistidos em `itajai_settings.ini`
- glTF/GLB -> runtime PAK
- Renderer GLSL 1.20 com fallback fixed-function
- Sol, iluminação, fog, wetness, céu, sombras e chuva

## Primeiro startup da 0.10

A 0.10 usa um cache de mundo novo. Na primeira abertura, o jogo pode ficar mais tempo em **baixando/construindo a cidade** enquanto consulta o OpenStreetMap/Overpass e monta `itajai_world_v10.bin` + cache regional. As próximas aberturas reutilizam esses arquivos.

O `itajai_settings.ini` continua preservado pelo updater.

## Controles

| Controle | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | esterço |
| Espaço | freio de mão |
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
| Tab | debug + telemetria/streaming/renderer/world |
| T | horário |
| Y | clima |
| U | limpar cache do mapa |

## Documentação técnica

- `docs/FOUNDATION_0.5.md`
- `docs/SIMULATION_0.6.md`
- `docs/SIMULATION_0.7.md`
- `docs/SETTINGS_0.7.1.md`
- `docs/ASSETS_STREAMING_0.8.md`
- `docs/RENDERER_0.9.md`
- `docs/URBAN_0.10.md`

## Build / updater

O workflow `.github/workflows/release.yml` compila os executáveis em Windows, compila os assets glTF/GLB para o PAK, gera SHA-256 + manifesto, cria a tag e publica a release.

O updater baixa primeiro para `.update/`, valida tamanho + SHA-256 e só depois substitui os arquivos gerenciados. Caches e configurações locais não são apagados quando permanecem compatíveis.

Dados de mapa: © OpenStreetMap contributors.
