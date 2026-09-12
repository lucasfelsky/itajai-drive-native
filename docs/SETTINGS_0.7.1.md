# Settings 0.7.1

A 0.7.1 adiciona a primeira camada persistente de configuracoes do Itajai Drive Native.

## Arquivo local

As preferencias sao salvas ao lado do executavel em `itajai_settings.ini`.

O arquivo nao faz parte do manifesto do updater e, portanto, nao e substituido durante atualizacoes.

Formato atual:

```ini
camera_sensitivity=0.001700
camera_invert_y=0
```

## Menu

Com o mouse capturado, `Esc` primeiro libera o cursor. Com o cursor livre, `Esc` abre o menu de configuracoes/pausa.

- W/S ou setas: navegar
- A/D ou esquerda/direita: ajustar
- Enter: selecionar
- Esc no menu: voltar ao jogo

Opcoes atuais:

- Sensibilidade da camera
- Inverter eixo Y
- Restaurar padrao
- Voltar ao jogo
- Sair do jogo

A sensibilidade padrao da 0.7.1 e aproximadamente 55% da sensibilidade fixa usada na 0.7.0.
