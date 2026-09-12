# Itajaí Drive 0.10 — Urban Expansion

A 0.10 move o projeto da pequena vertical slice para um corredor urbano muito maior de Itajaí e costa próxima, preservando a arquitetura de streaming e renderização construída nas versões anteriores.

## Novo mundo OSM

A consulta Overpass passa a usar o bbox aproximado:

```text
south=-26.9580
west=-48.6860
north=-26.8840
east=-48.6250
```

A intenção é cobrir, de forma comprimida e utilizável no sandbox, Porto/Centro/Fazenda-Beira-Rio e o corredor costeiro por Molhes, Atalaia, Cabeçudas e Praia Brava.

A 0.10 usa caches próprios:

```text
itajai_osm_cache_v10.json
itajai_world_v10.bin
```

Por isso a primeira abertura da 0.10 pode demorar mais: ela precisa obter/reprocessar o mundo uma vez. As aberturas seguintes voltam a usar cache.

## Capacidade

O World foi redimensionado para até:

- 8.000 road ways
- 60.000 road segments
- 24.000 buildings
- 2.048 traffic signals
- 40.000 graph nodes
- 120.000 directed graph edges
- 48 traffic vehicles

O spatial grid também foi ampliado para acompanhar o mundo maior: 65.536 células, até ~26 mil colliders estáticos e centenas de milhares de links de broad phase.

## Setores de direção de arte

A engine possui sete setores aproximados:

- Centro
- Beira-Rio / Fazenda
- Porto
- Molhes
- Atalaia
- Cabeçudas
- Praia Brava

Eles não representam limites administrativos oficiais. São regiões de direção de arte escolhidas por proximidade, usadas para mudar densidade e mistura de vegetação, objetos costeiros, objetos portuários e mobiliário urbano.

## Prédios

O parser agora classifica `building=*` em arquétipos simples:

- casa/residencial baixa
- apartamentos/torres
- comercial
- industrial/galpão
- cívico
- genérico

`height` e `building:levels` continuam tendo prioridade. Quando não existem, a altura procedural usa faixas diferentes por arquétipo.

O renderer acrescenta detalhes procedurais simples por tipo: coroamento/roof cap, podium de torre, storefront comercial, detalhe de cobertura industrial e variações de material.

> Limitação atual: a geometria renderizada do prédio continua sendo uma aproximação retangular baseada no bounding box do footprint OSM. A 0.10 melhora classificação e leitura urbana, mas ainda não extruda o polígono exato do footprint.

## Ruas

Sobre o renderer 0.9 entram:

- calçadas procedurais nas vias urbanas compatíveis;
- canteiro central simples em avenidas largas de mão dupla;
- faixas e asfalto continuam usando o lane model existente.

## Asset pack urbano

O mesmo formato de PAK criado na 0.8 continua válido. O pacote agora compila oito meshes glTF:

- `urban_tree`
- `street_lamp`
- `port_container`
- `coastal_palm`
- `urban_bench`
- `bus_shelter`
- `road_guardrail`
- `road_sign`

A distribuição é procedural e depende do setor e do tipo/largura da via.

## Tráfego e mundo maior

O grafo completo permanece residente para A* e roteamento. Veículos de tráfego que ficam muito longe do jogador podem ser reciclados e receber uma nova rota próxima, evitando que uma quantidade fixa de carros se espalhe por quilômetros de mapa e deixe a área atual vazia.

## Streaming

O formato regional da 0.8 não mudou, mas os limites foram ampliados:

- até 1.024 regiões;
- working set 5×5;
- até 30.000 road segments e 12.000 buildings no conjunto ativo;
- até 5.000 props regionais.

O arquivo continua sendo `itajai_regions_v08.bin`; ele é reconstruído automaticamente quando a contagem do mundo não corresponde ao cache existente.

## Compatibilidade

A 0.10 preserva:

- configurações locais em `itajai_settings.ini`;
- asset pack gerenciado pelo updater;
- renderer GLSL 1.20 + fallback;
- simulação/física 0.6;
- lane connectors/GPS 0.7;
- regional streaming 0.8;
- atmosfera 0.9.
