# Itajai Drive 1.6 — Real Car Model Pass

## Objetivo

Trocar as silhuetas genéricas do vertical slice por uma frota visualmente reconhecível como carros reais comuns no Brasil, mantendo o projeto leve e compatível com o asset pipeline glTF/PAK existente.

Os meshes são gerados proceduralmente dentro do próprio repositório. Nenhum mesh de fabricante é redistribuído e badges/logotipos não são usados. Os nomes reais são mantidos no protótipo para facilitar calibração e identificação dos perfis.

## Frota 1.6

- Fiat Uno Way 2014
- Volkswagen Gol G6
- Hyundai HB20
- Fiat Strada
- Toyota Corolla
- Jeep Renegade
- Chevrolet Onix
- Chevrolet Celta

Os quatro slots jogáveis usam Uno Way, Gol G6, HB20 e Renegade. O tráfego usa os oito modelos.

## Geometria

`tools/generate_vehicle_gltf.py` deixou de construir carrocerias como caixas empilhadas. Cada preset descreve:

- comprimento/largura/altura aproximados;
- entre-eixos e bitola visual;
- estações longitudinais do corpo;
- perfil de capô/traseira;
- comprimento/posição da cabine;
- rake de para-brisa;
- categoria hatch/box hatch/pickup/sedan/SUV.

As estações são convertidas em um loft facetado e depois compiladas para `itajai_assets_v08.pak` pelo pipeline normal.

## Detalhes nativos

A camada de render adiciona por cima do body mesh:

- vidros laterais;
- para-brisa e vidro traseiro;
- para-choques e lower trim;
- faróis e lanternas;
- detalhes de grade específicos por categoria;
- caçamba visual nas pickups;
- rodas 12-sided com pneu, aro e cubo;
- giro de roda e esterçamento dianteiro;
- suspensão visual ligada à compressão das quatro rodas do player.

## Iluminação

Faróis visuais ficam ativos à noite e em chuva. A 1.7 expande isso para um passe gráfico mais forte, mantendo o fallback do renderer.

## Compatibilidade

O nome do PAK permanece `itajai_assets_v08.pak` para preservar o protocolo do updater. O conteúdo é versionado pela release/manifest e substituído automaticamente quando necessário.
