# 2.1 — Delivery Polish

A 2.1 fecha a lacuna entre “o jogo atualiza” e “o próprio mecanismo de atualização evolui sem intervenção manual”.

## Updater handoff compatível com instalações antigas

O manifesto passa a gerenciar três arquivos:

1. `ItajaiDriveNative.exe`
2. `itajai_assets_v08.pak`
3. `ItajaiDriveUpdater.next.exe`

`ItajaiDriveUpdater.next.exe` é uma cópia byte-a-byte do updater 1.1 publicado na mesma release. O nome alternativo é deliberado: um updater antigo pode baixá-lo sem tentar sobrescrever o próprio executável enquanto ainda está em execução.

Ao iniciar, o jogo procura `ItajaiDriveUpdater.next.exe` na própria pasta. Se existir, espera brevemente pelo encerramento do updater anterior e tenta promovê-lo para `ItajaiDriveUpdater.exe` usando `MoveFileEx(..., REPLACE_EXISTING | WRITE_THROUGH)`. Se o arquivo antigo ainda estiver bloqueado, o `.next` é mantido; uma abertura posterior do jogo tenta novamente.

Esse desenho é compatível com o updater 1.0: ele não precisa conhecer o significado especial do `.next`; para ele é apenas mais um arquivo do manifesto.

## Gate de CI ampliado

`tools/release_check.py` agora confirma que:

- updater oficial e `.next` existem;
- os dois são byte-idênticos via SHA-256;
- o manifesto contém exatamente EXE + PAK + updater `.next`;
- tamanho/hash dos três arquivos gerenciados correspondem aos artefatos produzidos;
- as verificações anteriores de PAK/frota/versão/canal continuam obrigatórias.

## Resultado esperado para quem já tem o jogo

Abrir o updater existente deve levar o jogo para 2.1 e baixar o updater 1.1 como `.next`. O jogo recém-atualizado então promove o updater novo automaticamente. Não é necessário apagar caches, settings ou landmarks.

## QA ainda manual

O CI não consegue validar o comportamento de locks de executável específico da máquina do usuário nem qualidade subjetiva de imagem/áudio. Se a promoção não ocorrer na primeira abertura, deixar o `.next` no diretório é intencional e a próxima abertura tenta novamente.
