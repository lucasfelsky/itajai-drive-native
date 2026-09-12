# Itajaí Drive 1.5 — Itajaí Identity Pass

A 1.5 muda o foco de infraestrutura de engine para leitura visual do lugar. O objetivo não é reproduzir cada fachada real, e sim fazer o mundo parecer inequivocamente uma cidade costeira brasileira inspirada em Itajaí.

## Arquitetura e fachadas

- prédios OSM, inclusive footprints exatos, passam a ser ancorados à elevação do terreno/rua;
- comércio recebe placas, marquises e nomes em português gerados deterministicamente;
- exemplos de categorias: mercado, farmácia, padaria, autopeças, café, restaurante, materiais e conveniência;
- residências baixas podem receber muro frontal e portão;
- edifícios altos de Atalaia/Cabeçudas/Praia Brava podem receber varandas procedurais;
- galpões do setor Porto recebem portas de doca e detalhes logísticos;
- alguns comércios recebem pequenos bolsões de estacionamento.

## Serviços de rua

Vias largas em setores urbanos podem gerar postos de combustível estilizados, com cobertura, pilares, totem e placa `POSTO`. A distribuição é determinística a partir dos segmentos viários.

## Marcos de setor

A versão adiciona marcos de direção de arte com nomes legíveis:

- CENTRO DE ITAJAI
- BEIRA-RIO
- PORTO DE ITAJAI
- MOLHES DA BARRA
- ATALAIA
- CABECUDAS
- PRAIA BRAVA

Esses marcos são elementos do jogo, não cópias exatas de placas ou estruturas reais.

## Princípio de fidelidade

O mapa continua usando OpenStreetMap para a estrutura real de ruas/prédios disponível, mas detalhes arquitetônicos, sinalização, relevo e ambientação são procedurais/art-directed. A intenção é reconhecimento e atmosfera, não reconstrução cadastral ou fotogramétrica.

## Compatibilidade

A 1.5 não altera o formato dos caches do mapa, do PAK ou das configurações. Ela é majoritariamente uma camada de render/conteúdo sobre os sistemas 1.1–1.3.
