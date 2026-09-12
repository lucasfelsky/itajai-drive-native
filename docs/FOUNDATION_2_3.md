# 2.3 — Foundation Pass

A 2.3 prioriza a fundação física antes de investir em fidelidade visual dos carros.

## Vehicle-sized collision hull

O jogador deixa de depender apenas de um círculo central de aproximadamente um metro. Cada um dos quatro slots jogáveis recebe dimensões físicas próprias e um casco formado por três círculos sobrepostos ao longo do eixo longitudinal:

- Uno Way: 1,64 × 3,81 m;
- Gol G6: 1,66 × 3,90 m;
- HB20: 1,72 × 4,02 m;
- Renegade: 1,81 × 4,23 m.

O círculo central legado continua existindo como núcleo de compatibilidade e broad phase. O casco 2.3 executa o narrow phase real para nariz, centro e traseira.

## Colisão com prédios

A fundação 2.3 reutiliza a correção da 2.2: AABB é apenas broad phase e, quando disponível, o footprint OSM é a geometria final de colisão. Os três círculos do veículo são testados contra esse polígono. Isso evita tanto os antigos cantos invisíveis do AABB quanto a situação inversa em que o nariz do carro entrava no prédio antes de o centro tocar a parede.

## Continuous sweep

Entre a posição anterior e a nova posição são criadas amostras adaptativas quando o deslocamento de um substep passa de 28 cm. O primeiro contato encontrado interrompe o avanço e resolve a penetração antes que o carro consiga atravessar uma fachada fina ou um sinal em um frame lento.

Teleportes deliberados maiores que 6 m, como reset, não são varridos para evitar testar todo o caminho entre duas posições desconectadas.

## Adaptive physics substeps

O game loop continua integrado ao código legado, mas frames acima de 18 ms são subdivididos automaticamente, até quatro substeps. Assim, um frame de 33 ms roda aproximadamente como duas etapas de 16,5 ms e o limite de 80 ms do bootstrap vira quatro etapas de 20 ms.

Isso reduz dependência do FPS sem exigir ainda a migração completa para um relógio de física desacoplado do renderer.

## Tráfego

A colisão dinâmica central legada continua operando. A 2.3 adiciona detecção complementar no círculo dianteiro e traseiro do jogador, evitando atravessar parcialmente outro carro quando apenas o nariz ou a traseira encostam.

## Renegade

O quarto perfil físico foi alinhado ao veículo que efetivamente ocupa o quarto slot jogável: wheelbase 2,57 m, track 1,55 m e massa aproximada de 1.470 kg. Antes, o perfil ainda carregava dimensões herdadas de uma configuração anterior maior.

## Debug

Com `Tab`, a 2.3 mostra:

- quantidade de substeps do frame;
- contatos do casco;
- ativações do sweep;
- contatos complementares com tráfego;
- raio e espaçamento do casco atual.

Além disso, três círculos ciano são desenhados sobre o carro para tornar o collider visível durante QA.

A meta desta etapa é tornar a física confiável para receber modelos de carro muito melhores depois, sem precisar refazer colisões quando a arte evoluir.
