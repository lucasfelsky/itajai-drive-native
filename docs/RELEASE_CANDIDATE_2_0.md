# 2.0 — Release Candidate

A 2.0 não é uma reescrita da engine. É o checkpoint de entrega do vertical slice: preservar tudo que já funciona, impedir releases incompletas e tornar falhas visíveis/recuperáveis.

## Gate de release no CI

Antes de criar tag ou publicar GitHub Release, `tools/release_check.py` valida:

- `VERSION` em semver e coerente com o README;
- executável principal e updater presentes e com tamanho mínimo plausível;
- magic/version/count do `itajai_assets_v08.pak`;
- mínimo de 16 assets no PAK;
- presença nominal dos oito veículos de produção;
- `manifest.txt` em protocolo 1;
- versão e `base_url` do manifesto;
- tamanho e SHA-256 do EXE e do PAK;
- `update_config.ini` apontando para o manifesto `latest` e com auto-launch.

Se qualquer uma dessas verificações falhar, o workflow para **antes** de criar a tag/release.

## Self-check em runtime

A tela F1 foi ampliada para 14 pontos. Além dos dez sistemas da 1.0, ela verifica:

- PAK com pelo menos 16 assets;
- frota 8/8 resolvida no PAK;
- renderer inicializado;
- road streaming ativo.

A tela também mostra se GLSL está ativo ou se a engine caiu no fallback compatível.

## Updater 1.1

O updater continua nativo Win32/WinHTTP e sem launcher externo, mas agora:

- repete uma vez a consulta do manifesto se houver falha transitória;
- repete uma vez cada download interrompido;
- continua validando tamanho + SHA-256 antes de aplicar;
- faz backup temporário de cada arquivo gerenciado;
- aplica EXE/PAK de forma transacional;
- restaura os backups se qualquer substituição falhar;
- só grava `VERSION.txt` depois que todos os arquivos foram aplicados;
- preserva caches, settings e landmarks.

O updater não se substitui durante a própria execução; ele continua sendo distribuído em cada release para bootstrap/reinstalação manual quando necessário.

## Render chain

A cadeia de wrappers foi corrigida para 1.9 herdar a apresentação da 1.8, preservando telemetria de cockpit/áudio, e a 2.0 passa a herdar a 1.9.

## Limite de validação automática

O CI consegue provar compilação, estrutura do pacote, assets, hashes e consistência do canal. Ele **não** substitui teste visual/interativo em uma GPU Windows real. Clearcoat, vidro, câmera interna, áudio e sensação de direção ainda precisam de QA humano para aprovação subjetiva final.
