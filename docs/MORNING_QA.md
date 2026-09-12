# Morning QA — Itajaí Drive 2.1.1

O CI cobre compilação, PAK, oito carros, manifesto, hashes e updater handoff. Este checklist cobre somente o que precisa de olho/ouvido/máquina Windows real.

## 1. Atualização

1. Abra o `ItajaiDriveUpdater.exe` que já está na pasta do jogo.
2. Ele deve encontrar a 2.1.1, baixar EXE + PAK + `Updater.next` e abrir o jogo.
3. Feche o jogo e abra o updater novamente: o título esperado depois do handoff é **ITAJAI DRIVE // UPDATER 1.1**.
4. Se ainda aparecer o updater antigo, abra o jogo uma vez diretamente e tente o updater de novo. O `.next` é preservado justamente para uma segunda tentativa de promoção.

## 2. Self-check

- Pressione **F1** depois que a cidade carregar.
- Ideal: **SELF-CHECK 14/14**.
- Anote se aparece `GLSL ATIVO` ou `FALLBACK`.
- Se algum item ficar em `...`, registrar qual item; não precisa apagar cache automaticamente.

## 3. Frota / câmera

Com **E**, passe por Uno Way, Gol G6, HB20 e Renegade. Verificar:

- carroceria reconhecível e sem peças flutuando;
- quatro rodas presentes, girando e esterçando;
- vidro escuro/refletivo sem ficar completamente opaco;
- pintura sem brilho branco exagerado;
- retrovisores/placas/maçanetas em posição plausível.

Com **C**, entrar no cockpit de cada um:

- olho do motorista abaixo do teto e atrás do painel;
- volante visível e acompanhando a direção;
- capô visível sem ocupar o para-brisa inteiro;
- mouse-look funcionando sem clipping absurdo.

## 4. Mundo / carros estacionados

Passe perto de alguns carros estacionados. Perto da câmera eles devem ter rodas, vidro, trim e materiais completos. Ao longe o LOD pode simplificar para a carroceria, mas não deve haver pop agressivo perceptível durante direção normal.

## 5. Clima / noite

- **T** para avançar horário até dusk/noite.
- **Y** para chuva forte e depois voltar ao tempo seco.
- Conferir faróis no piso, postes/comércio, tachões refletivos, poças e pista ainda úmida por um tempo depois da chuva.
- Em piso molhado, carros próximos podem gerar reflexo local leve; não deve parecer espelho perfeito.

## 6. Direção / áudio

- testar curva rápida, frenagem e handbrake;
- passar com duas rodas fora do asfalto/meio-fio;
- confirmar que o carro não atravessa prédios;
- trocar os quatro carros e ouvir se o timbre do motor realmente muda;
- verificar se chuva/rodagem não abafam completamente o motor.

## 7. Performance

Com **Tab**, dirigir por Centro/Beira-Rio e uma área com prédios/estacionados. Não existe um FPS-alvo rígido ainda; o importante neste QA é identificar quedas grandes, stutter recorrente ou crescimento evidente de custo quando carros estacionados entram no LOD detalhado.

## O que mandar se algo der errado

O mais útil é: versão/título da janela, item do F1 que falhou, `GLSL ATIVO` ou `FALLBACK`, local aproximado no mapa, carro/câmera/clima usados e uma screenshot/foto do problema. Para sensação de direção, descrever `subesterça demais`, `traseira solta demais`, `freio fraco`, etc. é suficiente.
