const { Client, Intents } = require('discord.js');
const axios = require('axios');

const client = new Client({ intents: [Intents.FLAGS.GUILDS, Intents.FLAGS.GUILD_MESSAGES] });
const ARDUINO_IP = 'SEU_IP_DO_ARDUINO'; // Substitua pelo IP do Arduino
const ARDUINO_PORT = 80; // Porta que o Arduino está escutando

client.once('ready', () => {
    console.log(`Bot conectado como ${client.user.tag}`);
});

client.on('messageCreate', async (message) => {
    if (message.author.bot || !message.content.startsWith('@arduino')) return;

    const [command, ...args] = message.content.slice(8).trim().split(' ');
    const action = args.join(' ');

    try {
        const response = await sendCommandToArduino(command, action);
        message.reply(response);
    } catch (error) {
        console.error('Erro ao enviar comando:', error);
        message.reply('Erro ao processar o comando.');
    }
});

async function sendCommandToArduino(command, action) {
    const url = `http://${ARDUINO_IP}:${ARDUINO_PORT}/comando`;
    const { data } = await axios.post(url, { command, action });
    return data;
}

client.login('SEU_TOKEN_DO_BOT'); // Substitua pelo token do seu bot
