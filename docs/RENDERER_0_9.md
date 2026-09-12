# 0.9 — Programmable Renderer

## Objetivo

A 0.9 inicia a migração do renderer fixed-function para uma pipeline programável sem abandonar compatibilidade com a engine existente.

## GLSL 1.20

As funções OpenGL 2.x são resolvidas por `wglGetProcAddress` somente depois de existir um contexto válido. O shader usa os built-ins do compatibility profile (`gl_Vertex`, `gl_Normal`, `gl_Color`, `gl_MultiTexCoord0`) para permitir uma migração gradual do renderer immediate-mode.

Se uma função obrigatória não existir, shader compilation falhar ou program linking falhar, `g_renderer09_shader` permanece falso e a engine continua pelo caminho fixed-function.

## Materiais

A 0.9 introduz três materiais procedurais básicos:

- grama / terreno
- asfalto
- fachada / janelas

As texturas são geradas deterministicamente na inicialização, sem dependência de decoder externo. O asset PAK da 0.8 já carrega UVs e normais, então assets importados podem participar da iluminação imediatamente.

## Iluminação e atmosfera

O shader aplica:

- luz direcional de sol
- ambient light dependente de hora e clima
- fog exponencial
- wetness/darkening em chuva
- cor de fog coerente com dia/noite/clima

A tela de céu ganhou gradiente dependente de horário, sol simples durante o dia e tons específicos de noite/chuva.

## Sombras

A 0.9 usa projected/contact shadows baratos para veículos, prédios próximos e objetos urbanos. Ainda não são shadow maps; o objetivo é adicionar contato visual com custo baixo enquanto a arquitetura de framebuffer/shadow maps não existe.

## Chuva

Chuva leve e forte agora possuem streaks 3D ao redor do player, com densidade e velocidade diferentes.

## Compatibilidade

A 0.9 mantém:

- mundo/cache v05
- cache regional v08
- asset pack v08
- settings locais 0.7.1
- gameplay/física 0.8

Isso permite evoluir renderização sem invalidar mapa, rota, caches ou preferências do usuário.
