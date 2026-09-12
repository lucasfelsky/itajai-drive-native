# Itajaí Drive Native

Mini engine 3D nativa para Windows, feita do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido.

O projeto começou como um protótipo de direção livre inspirado em Itajaí/SC e evoluiu para uma engine própria com OpenStreetMap, tráfego, grafo viário, A*, clima, horário, cache binário do mundo e updater nativo.

## Estado atual

- Jogo: **0.4.2**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Canal de releases: **GitHub Releases público**

### Implementado

- Janela Win32 nativa
- Renderização OpenGL 1.1
- Física simcade básica
- Quatro perfis de veículo
- Quatro câmeras
- Ruas e construções via OpenStreetMap / Overpass
- Cache local do mundo
- Grafo viário direcionado
- Mão única e `oneway=-1`
- A* para tráfego e GPS
- Tráfego básico
- Semáforos
- Horário e clima
- Updater com staging + tamanho + SHA-256
- Build/release automático via GitHub Actions

## Roadmap imediato — Foundation 0.5

1. Spatial grid
2. Building colliders
3. Road / curb colliders
4. Vehicle collision
5. Melhorias na física do carro
6. Base para loader de assets 3D

## Controles

| Tecla | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | esterço |
| Espaço | freio de mão |
| C | câmera |
| E | trocar carro |
| R | reset |
| M | GPS / A* |
| Tab | debug |
| T | horário |
| Y | clima |
| U | limpar cache do mapa |
| Esc | sair |

## Build e releases

O workflow `.github/workflows/release.yml` compila `ItajaiDriveNative.exe` e `ItajaiDriveUpdater.exe` em um runner Windows. Alterar o arquivo `VERSION` na branch `main` dispara uma build de release, gera o manifesto com SHA-256, cria a tag correspondente e publica os binários no GitHub Releases.

A release inicial `v0.4.2` foi compilada e publicada com sucesso pelo GitHub Actions.

## Atualizador

O updater consulta automaticamente:

`https://github.com/lucasfelsky/itajai-drive-native/releases/latest/download/manifest.txt`

Ele baixa tudo primeiro para `.update/`, valida tamanho e SHA-256 e só então substitui os arquivos instalados. Se a rede falhar, a instalação atual permanece intacta e o jogo abre normalmente.

Arquivos locais como `itajai_osm_cache.json` e `itajai_world_v04.bin` não são gerenciados pelo updater e são preservados.

O canal de releases é público, então o updater não precisa de login nem token GitHub. Nenhum token pessoal é embutido no executável.

Dados de mapa: © OpenStreetMap contributors.
