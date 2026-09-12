# Itajaí Drive Native — Simulation 0.7

A 0.7 evolui a fundação 0.5 e a simulação viária 0.6 em quatro frentes: câmera controlável pelo mouse, transições de faixa em interseções, GPS dinâmico e primeiro working set espacial para streaming.

## Mouse camera

- Clique esquerdo dentro da janela captura o mouse.
- Movimento horizontal orbita a câmera ao redor do veículo.
- Movimento vertical controla a elevação/olhar, com limites para evitar inversão.
- Câmeras externas orbitam o carro; a câmera interna permite olhar para os lados e para cima/baixo.
- A câmera possui suavização independente do input bruto.
- `Esc` solta o mouse quando capturado; com o mouse livre, `Esc` fecha o jogo.
- Perda de foco/Alt+Tab libera automaticamente o cursor.

## Lane connectors

A 0.6 seguia o centro de cada faixa, mas a troca entre dois edges ainda acontecia diretamente no nó. A 0.7 mantém o edge graph para roteamento e adiciona uma curva de transição local:

- o veículo preserva a faixa anterior;
- escolhe a faixa de saída conforme o tipo de conversão;
- conversão à direita tende à faixa da direita;
- conversão à esquerda tende à faixa da esquerda;
- movimento reto preserva a faixa quando possível;
- os primeiros 22% do novo edge podem ser amostrados por uma Bézier quadrática `faixa anterior -> nó -> nova faixa`;
- velocidade alvo é reduzida antes de conversões fechadas.

Isso suaviza posição e orientação sem alterar o formato persistido do mapa.

## GPS dinâmico

O destino aleatório continua sendo escolhido com `M`, porém agora é persistido durante a navegação.

Periodicamente o jogo mede a distância do jogador aos edges da rota. Se o carro estiver mais de aproximadamente 22 m fora dela, A* é executado novamente do nó viário mais próximo até o destino original. Ao chegar perto do destino a rota é encerrada.

A telemetria de debug mostra distância fora da rota e quantidade de recálculos.

## Active-cell streaming layer

Esta versão ainda mantém o mundo completo em RAM. Portanto, não é streaming de disco/regiões completo.

A novidade é um working set derivado do spatial grid 0.5:

- células próximas ao jogador são consideradas ativas;
- segmentos viários ativos são coletados sem duplicatas;
- prédios ativos são coletados a partir dos colliders indexados;
- renderer de ruas, faixas e prédios itera esse conjunto, em vez de percorrer o mundo inteiro em cada frame;
- o conjunto é atualizado ao mudar de célula ou em intervalos curtos.

Essa camada é a base para uma versão futura descarregar/carregar regiões do disco sem reescrever renderer e sistemas espaciais.

## Compatibilidade

O formato do mundo não mudou. A 0.7 continua usando `itajai_world_v05.bin`, evitando download ou reconstrução desnecessária do mapa.
