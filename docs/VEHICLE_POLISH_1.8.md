# Itajaí Drive 1.8 — Vehicle Polish

## Cockpit

A câmera interna (`C` até CAM 3 / índice interno 2) passa a usar uma posição de motorista por slot jogável em vez de uma câmera genérica acima do teto.

Offsets são calibrados separadamente para:

- Fiat Uno Way;
- VW Gol G6;
- Hyundai HB20;
- Jeep Renegade.

O mouse-look continua livre dentro do cockpit.

## Interior procedural

No modo cockpit a carroceria externa fechada não é desenhada ao redor da câmera. Em seu lugar entra uma camada interior leve:

- dashboard;
- cluster/instrumentos;
- console central;
- bancos dianteiros;
- pilares A;
- retrovisor interno;
- capô/fenders visíveis pelo para-brisa;
- volante procedural animado com o esterço.

Isso evita back-faces opacas cobrindo a câmera e mantém o custo baixo.

## Exterior

Os oito modelos da 1.6 recebem detalhes próximos adicionais:

- retrovisores externos;
- maçanetas;
- placas claras com faixa azul estilizada;
- trim existente de vidro, grade, para-choques, faróis e lanternas.

As rodas continuam girando/esterçando e a suspensão visual do player continua ligada à compressão das quatro rodas.

## Áudio por carro

O backend WinMM continua procedural e sem arquivos de áudio externos, mas os quatro slots jogáveis agora têm perfis diferentes:

- `UNO 4C SOFT` — grave/combustão mais suave;
- `GOL 4C RASP` — mais conteúdo harmônico/rasp;
- `HB20 3C PULSE` — ordem de combustão procedural diferente e pulsação mais evidente;
- `RENEGADE 4C DEEP` — componente grave/exhaust mais forte.

O som continua seguindo RPM, throttle, marcha, velocidade, chuva e ambiente.

## Debug

`Tab` mostra:

- cockpit ativo/externo;
- perfil de áudio atual ou fallback silencioso;
- nível procedural do motor;
- nível de ruído de rodagem;
- marcha e RPM.
