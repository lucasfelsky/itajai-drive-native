# Itajaí Drive 1.3 — Roads 2.0

A 1.3 transforma as vias de uma superfície quase plana em uma camada urbana procedural mais rica e integrada ao relevo.

## Grade vertical

- ruas próximas passam a seguir `terrain11_road_grade()` em vez de serem forçadas a Y=0;
- o terreno próximo da via usa a mesma altura, evitando degraus entre rua e chão;
- o jogador e os carros de tráfego recebem altura compatível com a via;
- a suspensão da 0.15 agora consulta o terreno/grade real sob cada roda;
- a inclinação longitudinal atual aparece na telemetria de debug.

O relevo continua art-directed/analítico; não é um DEM topográfico oficial.

## Geometria e acabamento viário

A camada próxima ao jogador acrescenta:

- deck de asfalto elevado;
- bordas e meio-fio;
- calçada procedural elevada;
- sarjeta;
- linhas de bordo;
- linha central amarela em mão dupla;
- divisões de faixa tracejadas;
- remendos de asfalto determinísticos;
- grelhas/bueiros em ruas locais;
- ciclovia procedural em trechos largos de setores costeiros/Beira-Rio;
- lombadas procedurais esparsas em vias locais.

## Lombadas

As lombadas não são só decoração. Quando o carro cruza uma lombada procedural em velocidade suficiente, a camada 1.3 aplica perda leve de velocidade, pitch e compressão adicional das rodas.

## Compatibilidade

O grafo A*, lane model e cache OSM continuam usando as mesmas coordenadas XZ. A mudança de altura é uma camada vertical de simulação/render e não altera o formato do cache de mundo.
