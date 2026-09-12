# Itajaí Drive Native

Mini engine 3D nativa para Windows, feita do zero em C/Win32 + OpenGL — sem Unity, Unreal, Electron ou navegador embutido.

O projeto começou como um protótipo de direção livre inspirado em Itajaí/SC e evoluiu para uma engine própria com OpenStreetMap, tráfego, grafo viário, A*, collision world, spatial grid, simulação de faixas, física veicular simcade, câmera livre, GPS dinâmico, configurações persistentes e updater nativo.

## Estado atual

- Jogo: **0.7.1 — Settings & Camera QoL**
- Updater: **1.0.0**
- Plataforma: Windows x64
- Canal de releases: **GitHub Releases público**

### Implementado

- Janela Win32 nativa e renderização OpenGL 1.1
- Quatro perfis de veículo e quatro câmeras
- Ruas e construções via OpenStreetMap / Overpass
- Cache local do mundo (`itajai_world_v05.bin`)
- Grafo viário direcionado, mão única e `oneway=-1`
- Spatial grid de 40 m e collision world
- Colliders de prédios/semáforos e colisão jogador × tráfego
- Superfícies, grip, bicycle model simcade, slip, roll/pitch e quatro rodas amostradas individualmente
- Modelo de faixas derivado da largura da via
- Car-following, frenagem progressiva e semáforos
- Câmera controlável pelo mouse com captura por clique e suavização
- Câmera externa orbitável e câmera interna com free-look
- **Sensibilidade padrão da câmera reduzida em relação à 0.7.0**
- **Menu de configurações/pausa**
- **Sensibilidade da câmera ajustável em tempo real**
- **Opção de inverter eixo Y**
- **Preferências persistidas em `itajai_settings.ini` e preservadas pelo updater**
- Lane connectors Bézier nas interseções
- Seleção de faixa de saída conforme conversão esquerda/direita/reto
- Redução de velocidade antes de curvas mais fechadas
- GPS A* com destino persistente e reroute automático ao sair da rota
- Working set de células ativas para ruas/prédios próximos
- Renderer de ruas, faixas e prédios preparado para streaming regional futuro
- Debug visual do spatial grid/colliders e telemetria física/streaming via Tab
- Updater com staging + tamanho + SHA-256
- Build/release automático via GitHub Actions

Documentação técnica:

- `docs/FOUNDATION_0.5.md`
- `docs/SIMULATION_0_6.md`
- `docs/SIMULATION_0_7.md`
- `docs/SETTINGS_0.7.1.md`

## Próximas etapas naturais

A 0.7 cria o primeiro working set espacial real, mas o mapa completo ainda permanece em RAM. Os próximos blocos naturais são streaming regional de verdade, objetos urbanos adicionais, loader glTF e a evolução gradual do renderer para materiais/texturas/iluminação modernos.

## Controles

| Controle | Ação |
|---|---|
| W / S | acelerar / frear / ré |
| A / D | esterço |
| Espaço | freio de mão / redução de grip |
| Clique esquerdo | capturar mouse para controlar a câmera |
| Mouse | orbitar / olhar ao redor |
| Esc | soltar mouse; com mouse livre, abrir configurações |
| W/S ou setas no menu | navegar |
| A/D ou esquerda/direita no menu | ajustar opção |
| Enter no menu | selecionar |
| C | trocar câmera |
| E | trocar carro |
| R | reset |
| M | criar destino GPS / A* |
| Tab | debug + grid/colliders/telemetria |
| T | horário |
| Y | clima |
| U | limpar cache do mapa |

## Configurações locais

A 0.7.1 cria `itajai_settings.ini` ao lado do executável. Esse arquivo é local e não faz parte do manifesto de atualização, então preferências de câmera não são apagadas por novas versões.

## Build e releases

O workflow `.github/workflows/release.yml` compila `ItajaiDriveNative.exe` e `ItajaiDriveUpdater.exe` em um runner Windows. Alterar o arquivo `VERSION` na branch `main` dispara uma build de release, gera o manifesto com SHA-256, cria a tag correspondente e publica os binários no GitHub Releases.

## Atualizador

O updater consulta automaticamente o `manifest.txt` da release mais recente, baixa primeiro para `.update/`, valida tamanho e SHA-256 e só então substitui os arquivos instalados. Se a rede falhar, a instalação atual permanece intacta e o jogo abre normalmente.

A 0.7.1 mantém o formato de mundo da 0.5/0.6/0.7 e reutiliza `itajai_world_v05.bin`.

O canal de releases é público, então o updater não precisa de login nem token GitHub.

Dados de mapa: © OpenStreetMap contributors.
