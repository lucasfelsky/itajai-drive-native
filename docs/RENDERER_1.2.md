# Itajaí Drive 1.2 — Renderer 2.0

A 1.2 evolui o renderer de compatibilidade sem remover o fallback fixed-function.

## Iluminação e materiais

- PBR-compat em GLSL 1.20 preservado e ampliado;
- ACES-like tone mapping no shader;
- wetness usa o estado climático persistente da 0.60;
- material de água dedicado (`material 4`) com normal procedural animada e Fresnel aproximado;
- textura procedural de asfalto ganhou variação de microfissuras;
- roughness do asfalto cai progressivamente quando o piso está molhado.

## Noite

- farol do carro do jogador é calculado em eye space pelo shader;
- cone de luz usa direção, distância e atenuação;
- um pool de luz translúcido no chão permanece visível também no fallback;
- fachadas usam janelas emissivas procedurais à noite;
- exposição noturna foi recalibrada.

## Clima

- nuvens, wetness e lightning alimentam diretamente iluminação/fog;
- relâmpago aumenta temporariamente a luz global;
- chuva mantém materiais molhados mesmo após a troca de preset até o estado climático secar.

## Compatibilidade

Se GLSL 1.20 ou qualquer entry point necessário falhar, a engine continua no caminho fixed-function. O cache de mundo e o asset pack não mudam de formato nesta versão.
