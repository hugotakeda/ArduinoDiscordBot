# Automação da casa inteligente, usando de Arduino junto ao Discord

Este projeto permite controlar dispositivos de uma casa inteligente usando comandos enviados pelo Discord e executados por um Arduino.

## Funcionalidades
- Ligar/desligar luzes.
- Ajustar intensidade das luzes.
- Verificar status dos dispositivos.
- Fácil expansão para novos comandos e dispositivos.

## Pré-requisitos
- Node.js instalado.
- IDE do Arduino instalada.
- Módulo Wi-Fi (ESP8266 ou ESP32).
- Conta no Discord Developer Portal.

## Configuração

### 1. Bot do Discord
1. Crie um bot no [Discord Developer Portal](https://discord.com/developers/applications).
2. Substitua `SEU_TOKEN_DO_BOT` no arquivo `bot.js` pelo token do seu bot.
3. Instale as dependências:
   ```bash
   npm install discord.js axios
