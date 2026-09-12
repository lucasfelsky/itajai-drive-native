# Foundation 0.5

A 0.5 transforma o protótipo em uma base física/espacial consistente para as próximas versões da engine.

## Spatial grid

O mundo é dividido em células de 40 metros. Um hash espacial indexa:

- colliders estáticos (construções e postes de semáforo);
- segmentos de rua e sua largura.

Consultas de colisão e superfície passam a visitar apenas as células próximas ao veículo, em vez de percorrer todos os elementos do mapa.

## Collision world

Construções geradas a partir do OpenStreetMap recebem AABBs de colisão compatíveis com o volume atualmente renderizado. Semáforos também entram no collision world como pequenos obstáculos.

A broad phase usa o spatial grid. A narrow phase inicial usa circle-vs-AABB para o veículo do jogador, com resolução iterativa de penetração e remoção da componente de velocidade contra a normal de contato.

O jogador também colide com os carros de tráfego usando circle-vs-circle.

## Physics body

O jogador agora possui `PhysicsBody` com:

- posição e velocidade;
- yaw e velocidade angular;
- massa;
- raio físico;
- restituição;
- aderência;
- superfície atual.

A direção continua propositalmente simcade nesta etapa, mas o estado físico deixa de ser apenas uma posição atualizada diretamente pelo game loop.

## Superfícies

Os segmentos de rua também são indexados no grid. A engine calcula distância à linha central e à borda da via e classifica a superfície em:

- asfalto;
- meio-fio/calçada;
- grama/fora da via.

Potência, resistência ao rolamento, velocidade máxima e aderência variam conforme a superfície. O meio-fio não é uma parede invisível: o free-roam continua permitindo sair da rua.

## Debug

`Tab` exibe dados adicionais da fundação e desenha no mundo:

- grid espacial em azul;
- colliders próximos em laranja;
- número de células/links;
- candidatos da broad phase;
- contatos estáticos e dinâmicos;
- aderência e distância da rua.

## Cache

O cache binário passa para o formato v5 (`itajai_world_v05.bin`). O JSON OSM existente continua reutilizável, então a primeira abertura da 0.5 pode reconstruir o cache v5 sem precisar baixar o mapa novamente.

## Próximos subsistemas que se apoiam nesta fundação

- lane graph e tráfego por faixa;
- streaming de regiões;
- objetos urbanos adicionais;
- modelo veicular com suspensão/rodas;
- loader GLTF e instâncias renderizáveis;
- física de objetos dinâmicos mais geral.
