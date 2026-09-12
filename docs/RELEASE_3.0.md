# Itajaí Drive 3.0 — Open World Vertical Slice

A 3.0 consolida a sequência de fundação, direção, renderer, tráfego, mundo vivo, áudio, UI e conteúdo urbano em uma build nativa Windows pronta para o próximo ciclo de QA visual/interativo.

## O que fecha nesta versão

### Direção e veículos

- física simcade com quatro rodas, slip, transferência de carga, FWD/RWD/AWD, ABS/TCS aproximados e superfícies;
- casco físico dimensionado por veículo, sweep contra estáticos e substeps adaptativos;
- ré com direção intuitiva e câmera chase que gira para a direção do movimento, respeita mouse-look e retorna automaticamente;
- oito veículos brasileiros originais no PAK; quatro slots jogáveis;
- apresentação por partes de carroceria, vidro e trim, cockpit e áudio procedural por perfil.

### Cidade e fidelidade

- OpenStreetMap/Overpass como base para ruas, prédios, semáforos, grafo e navegação;
- setores art-directed: Centro, Beira-Rio/Fazenda, Porto, Molhes, Atalaia, Cabeçudas e Praia Brava;
- pass de fidelidade para Centro/Beira-Rio/Porto;
- Navegantes Gateway: margem/corredor já presente no bbox OSM recebe linguagem própria de logística, rio e aproximação aeroportuária;
- Itajaí-Açu/canal e lâmina d'água da Beira-Rio complementam o oceano/faixa costeira existentes;
- fachadas com janelas iluminadas e reflexos/glints de pista molhada derivados do working set streamado.

### Tráfego e mundo vivo

- grafo viário + A*, lanes, conectores, car-following, sinais e yielding;
- Traffic AI 2.0: perfis cautious/normal/assertive/utility e indicação visual de intenção;
- Traffic 3.0: ritmo adaptado a setor, horário e clima, fila e hazard de utilitários parados;
- pedestres, ciclistas, barcos/rebocadores, carros estacionados e atividade por horário.

### Game feel

- marcas de pneu persistentes durante drift/freio de mão;
- spray em piso molhado e poeira simplificada em superfície solta;
- feedback visual discreto de impacto;
- clima, wetness persistente, vento, lightning e horário dinâmico.

### Performance / streaming

- cache regional `IDREG08` continua compatível;
- builder 2.20 passa a ser hash-indexed/two-pass, evitando revarrer o mundo inteiro para cada região;
- working set 5x5 usa lookup direto das regiões em vez de scan global;
- LOD e budgets de tráfego, prédios, props e mundo vivo continuam ativos.

### Produção / QA

- F1: ajuda + self-check nativo;
- TAB: telemetria detalhada;
- 2.90 adiciona health score de release candidate e alerta de pressão nos budgets de streaming;
- F3: garagem nativa;
- Photo Mode, minimapa/GPS, tour e editor de landmarks permanecem integrados;
- updater nativo usa manifesto, SHA-256, retry, rollback e handoff do próprio updater;
- GitHub Actions compila Windows x64, gera PAK, valida release candidate e só depois cria tag/release.

## Controles principais

- `W/S`: acelerar, frear e ré
- `A/D`: direção
- `Space`: freio de mão
- `Mouse`: olhar/orbitar
- `C`: câmera
- `E`: trocar carro
- `F3`: garagem
- `M`: GPS aleatório
- `P`: Photo Mode
- `H`: esconder HUD no Photo Mode
- `F10`: editor
- `TAB`: debug/telemetria/RC health
- `T`: horário
- `Y`: clima
- `F1`: ajuda/self-check

## Validação desta entrega

O gate automatizado verifica compilação do jogo/updater, geração do PAK, contrato da frota, manifesto, hashes, updater handoff e publicação. Os checkpoints 2.50, 2.60, 2.70, 2.80 e 2.90 foram levados pelo build gate antes do corte da 3.0.

A validação automatizada não substitui QA interativo no PC real. Mudanças recentes de atmosfera, rio/costa, Traffic 3.0 e game feel ainda devem ser avaliadas visualmente na máquina do jogador após a release 3.0.

## Limites conhecidos

- os setores, rio, Navegantes Gateway, fachadas e detalhes são direção de arte; não são levantamento cadastral, fotogrametria ou réplica centimétrica;
- o tráfego é simcade e não um simulador rígido multiagente completo;
- sinalização e mobiliário urbano ainda usam uma combinação de OSM e geração procedural, sem reprodução semântica completa de cada placa brasileira;
- o renderer mantém compatibilidade OpenGL/GLSL 1.20 e fallback fixed-function;
- áudio é procedural via WinMM;
- o objetivo da 3.0 é um vertical slice dirigível e expansível, não uma reconstrução integral de Itajaí/Navegantes.

Dados de mapa: © OpenStreetMap contributors.
