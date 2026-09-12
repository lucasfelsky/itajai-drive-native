# Itajaí Drive Native

Mini engine 3D nativa para Windows, feita do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido.

O projeto começou como um protótipo de direção livre inspirado em Itajaí/SC e evoluiu para uma engine própria com OpenStreetMap, tráfego, grafo viário, A*, collision world, spatial grid, clima, horário, cache binário do mundo e updater nativo.

## Estado atual

- Jogo: **0.5.0 — Foundation**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Canal de releases: **GitHub Releases público**

### Implementado

- Janela Win32 nativa
- Renderização OpenGL 1.1
- Quatro perfis de veículo e quatro câmeras
- Ruas e construções via OpenStreetMap / Overpass
- Cache local do mundo (`itajai_world_v05.bin`)
- Grafo viário direcionado, mão única e `oneway=-1`
- A* para tráfego e GPS
- Tráfego básico e semáforos
- Horário e clima
- **Spatial grid de 40 m com hash espacial**
- **Indexação espacial de colliders e segmentos de rua**
- **Colliders para construções e postes de semáforo**
- **Broad phase via spatial grid + narrow phase circle-vs-AABB**
- **Colisão jogador × cenário com resolução de penetração**
- **Colisão jogador × tráfego**
- **PhysicsBody com posição, velocidade, yaw, velocidade angular, massa, restituição e aderência**
- **Superfícies: asfalto, meio-fio/calçada e grama/fora da via**
- **Aderência, arrasto, potência e velocidade máxima dependentes da superfície**
- **Debug visual do spatial grid e colliders via Tab**
- Updater com staging + tamanho + SHA-256
- Build/release automático via GitHub Actions

A arquitetura da 0.5 está documentada em `docs/FOUNDATION_0.5.md`.

## Próxima etapa

Com a fundação física/espacial pronta, as próximas versões podem crescer em cima dela sem voltar a percorrer o mapa inteiro para cada sistema. Os próximos candidatos naturais são lane graph/tráfego por faixa, objetos urbanos adicionais, streaming de regiões, física veicular por rodas/suspensão e loader GLTF.

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
| Tab | debug + grid/colliders |
| T | horário |
| Y | clima |
| U | limpar cache do mapa |
| Esc | sair |

## Build e releases

O workflow `.github/workflows/release.yml` compila `ItajaiDriveNative.exe` e `ItajaiDriveUpdater.exe` em um runner Windows. Alterar o arquivo `VERSION` na branch `main` dispara uma build de release, gera o manifesto com SHA-256, cria a tag correspondente e publica os binários no GitHub Releases.

## Atualizador

O updater consulta automaticamente o `manifest.txt` da release mais recente, baixa primeiro para `.update/`, valida tamanho e SHA-256 e só então substitui os arquivos instalados. Se a rede falhar, a instalação atual permanece intacta e o jogo abre normalmente.

`itajai_osm_cache.json` não é gerenciado pelo updater. A 0.5 usa um novo cache binário `itajai_world_v05.bin`; se o JSON da 0.4 já existir, ele pode ser reutilizado para gerar o novo cache sem baixar novamente o mapa.

O canal de releases é público, então o updater não precisa de login nem token GitHub. Nenhum token pessoal é embutido no executável.

Dados de mapa: © OpenStreetMap contributors.
