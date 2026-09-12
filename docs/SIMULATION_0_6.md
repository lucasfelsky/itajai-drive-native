# Simulation 0.6

A 0.6 transforma a fundação física da 0.5 em uma camada de simulação viária/veicular mais estruturada.

## Lane model

As faixas são derivadas da largura já existente em `RoadWay`.

- vias bidirecionais dividem a largura entre os dois sentidos;
- vias `oneway` podem ocupar toda a largura com múltiplas faixas;
- cada `Edge` do grafo tem centros de faixa calculados em runtime;
- a IA trafega deslocada do centro geométrico da rua para o centro da faixa do seu sentido;
- as marcações visuais usam o mesmo modelo de largura/faixas.

A 0.6 não adiciona ultrapassagem completa nem troca inteligente de faixa. A estrutura necessária para isso passa a existir.

## Traffic following

O tráfego agora considera:

- limite de velocidade do `Edge`;
- fator individual de velocidade de cruzeiro;
- distância para veículo à frente na mesma faixa;
- distância segura proporcional à velocidade;
- frenagem progressiva para semáforo vermelho;
- jogador à frente da IA;
- aceleração e desaceleração limitadas em vez de interpolação instantânea;
- luz de freio visual.

## Vehicle model

Cada carro possui um `VehicleProfile` com:

- massa;
- wheelbase;
- track width;
- aceleração;
- velocidade máxima;
- ângulo máximo de esterço;
- capacidade de frenagem;
- aderência base;
- arrasto aerodinâmico;
- resistência ao rolamento.

O jogador usa um bicycle model simplificado. A velocidade é separada em componentes longitudinal e lateral. O yaw rate depende de velocidade, wheelbase, esterço e grip.

## Four-wheel surface sampling

As quatro posições de roda consultam o spatial grid da 0.5 de forma independente. Isso permite:

- rodas em superfícies diferentes;
- grip médio por veículo;
- transição asfalto / meio-fio / grama menos binária;
- base para suspensão e terreno futuros.

A carroceria possui roll/pitch visual derivados de esterço, aceleração/frenagem e diferenças de superfície entre as rodas.

## Collision

A colisão estática continua usando broad phase pelo spatial grid e narrow phase circle/AABB. A colisão jogador-tráfego agora usa massa relativa e impulso aproximado na normal do contato.

## Cache

O formato do `World` não foi alterado. Por isso a 0.6 reutiliza `itajai_world_v05.bin`: atualizar a simulação não invalida dados de mapa desnecessariamente.

## Próximos passos naturais

- troca de faixa/ultrapassagem;
- curvas de interseção suavizadas por lane connectors;
- GPS com reroute dinâmico;
- streaming de regiões;
- modelo de suspensão/altura de terreno mais real;
- loader glTF e renderer moderno.
