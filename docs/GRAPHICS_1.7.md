# Itajaí Drive 1.7 — Graphics Uplift

## Objetivo

A 1.7 melhora a leitura visual da cidade sem abandonar a compatibilidade da engine atual. Em vez de trocar todo o renderer por uma pipeline deferred/FBO, ela combina o GLSL 1.20 PBR-compat já existente com passes locais baratos e previsíveis.

## Atmosfera

- overlay de dusk/amanhecer no horizonte;
- céu noturno com estrelas determinísticas;
- lua estilizada;
- iluminação/fog existentes continuam respondendo a cloud, wetness e lightning.

## Iluminação urbana

- postes de rua geram pools de luz quentes no solo;
- fachadas comerciais podem iluminar a calçada à noite;
- o carro do jogador projeta um feixe de farol sobre a via;
- tráfego próximo também projeta feixes mais curtos;
- lanternas de freio criam pequenos reflexos vermelhos no piso molhado.

## Chuva e piso molhado

A 0.14 desenhava reflexos apenas enquanto o preset de chuva estava ativo. A 1.7 usa `g_wetness60`, portanto a pista continua visualmente úmida enquanto seca depois da chuva.

- sheen longitudinal em trechos próximos;
- poças determinísticas esparsas;
- intensidade aumenta à noite e com wetness;
- nenhum buffer extra ou render-to-texture é obrigatório.

## Vegetação

Árvores e palmeiras próximas recebem uma camada adicional de volume de copa. O asset principal continua vindo do PAK; o passe extra reduz a aparência de silhueta simples em distâncias curtas.

## Compatibilidade

Todos os novos efeitos usam o caminho OpenGL de compatibilidade e convivem com o renderer GLSL atual. Se o shader programável falhar, os overlays locais ainda podem ser desenhados pelo caminho fixed-function.

## Debug

`Tab` mostra:

- quantidade de postes/pools de luz;
- quantidade de headlight beams;
- wetness atual;
- poças desenhadas;
- fator de noite.
