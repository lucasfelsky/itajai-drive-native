# 1.9 — Production Presentation

O objetivo deste milestone é reduzir a aparência de protótipo sem abandonar a compatibilidade do renderer nativo.

## Materiais automotivos

O renderer GLSL 1.20 agora possui materiais dedicados para:

- `MAT09_CAR_PAINT` — pintura com roughness baixo, Fresnel, clearcoat especular e reflexão hemisférica aproximada;
- `MAT09_GLASS` — vidro azulado/tintado com resposta de Fresnel e highlight próprio;
- `MAT09_RUBBER` — pneus, borrachas e plásticos foscos;
- `MAT09_CHROME` — aro, acabamento metálico e detalhes de grade.

Esses materiais continuam com fallback fixed-function. Em hardware sem shader compatível o jogo continua desenhando a mesma geometria, apenas sem a resposta PBR aproximada.

## Veículos

A frota 1.6/1.8 foi reatribuída aos novos materiais. Carroceria usa paint, janelas e lentes usam glass, pneus/plásticos usam rubber e aros/trim usam chrome. O cockpit também separa dashboard, cluster, acabamento e capô por material.

## Estrada e noite

- reflexos locais e baratos das cores dos veículos sobre pista molhada;
- tachões/refletores procedurais em vias largas durante a noite;
- wetness continua vindo do sistema climático 0.60/1.7;
- todos os novos passes respeitam o streaming atual e possuem raio limitado.

## Apresentação

A tela de loading e o título da janela foram atualizados para o vertical slice atual, removendo textos históricos da versão 0.11.

## Compatibilidade

Nenhuma mudança no formato dos caches de mapa, landmarks ou PAK. O updater existente continua válido. O milestone deve ser validado no CI Windows antes da publicação.
