# 0.8 — Asset Pipeline & Regional Streaming

## Objetivo

A 0.8 separa o formato usado por artistas/ferramentas do formato usado pelo runtime e introduz streaming regional realmente respaldado por arquivo.

## Asset pipeline

Assets fonte ficam em `assets/source/` como glTF 2.0 ou GLB. O script `tools/compile_assets.py` aceita ambos os formatos, lê POSITION + índices da primeira primitive TRIANGLES, calcula normais/UVs e produz `itajai_assets_v08.pak`.

O PAK contém uma tabela de meshes e dados intercalados `position3 + normal3 + uv2`, além de índices 32-bit, bounds e collider radius. A release publica um único PAK, evitando dezenas de arquivos pequenos no updater.

Assets-semente da 0.8:

- `urban_tree`
- `street_lamp`
- `port_container`

O asset pack é gerenciado pelo updater e validado pelo mesmo SHA-256 do executável.

## Streaming regional

O grafo viário completo continua residente porque A*, GPS e tráfego dependem dele. Geometria de renderização, por outro lado, é compilada localmente em `itajai_regions_v08.bin`, dividido em regiões de 320 m.

O runtime mantém um quadrado de 5×5 regiões ao redor do jogador. Ao cruzar uma fronteira regional, estradas e prédios do working set são lidos do cache por offset com `SetFilePointerEx + ReadFile`.

Isso é diferente da 0.7, que apenas filtrava arrays já residentes.

## Objetos urbanos

Instâncias de árvores, postes e containers são derivadas deterministicamente das vias ativas. Elas só existem no working set regional e são recriadas quando a região entra em streaming.

Os meshes carregam `colliderRadius`; o player resolve colisão contra as instâncias urbanas ativas.

## Fallback

Se o PAK não estiver disponível, o jogo continua funcionando sem props 3D. Se o cache regional falhar, o renderer volta ao working set da 0.7. O objetivo é que falhas de asset/streaming nunca impeçam o sandbox de abrir.
