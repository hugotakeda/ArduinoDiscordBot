#include <ESP8266WiFi.h> // Ou #include <WiFi.h> para ESP32
#include <ESP8266WebServer.h> // Ou #include <WebServer.h> para ESP32
#include <ArduinoJson.h>

// Configurações de rede
const char* ssid = "SUA_REDE_WIFI";
const char* password = "SENHA_DA_REDE";

// Defina os pinos para cada dispositivo
const int PIN_LUZ_SALA = D1;
const int PIN_LUZ_QUARTO = D2;
const int PIN_AR_CONDICIONADO = D3;
const int PIN_VENTILADOR = D4;
const int PIN_TV = D5;
const int PIN_SISTEMA_SEGURANCA = D6;
const int PIN_CAMERA = D7;
const int PIN_CORTINA_SALA = D8;
const int PIN_CORTINA_QUARTO = D9;
const int PIN_IRRIGACAO = D10;
const int PIN_PORTAO_GARAGEM = D11;
const int PIN_PORTA_FRENTE = D12;

ESP8266WebServer server(80); // Ou WebServer server(80); para ESP32

void setup() {
    Serial.begin(115200);
    connectToWiFi();

    // Configura os pinos como saída
    pinMode(PIN_LUZ_SALA, OUTPUT);
    pinMode(PIN_LUZ_QUARTO, OUTPUT);
    pinMode(PIN_AR_CONDICIONADO, OUTPUT);
    pinMode(PIN_VENTILADOR, OUTPUT);
    pinMode(PIN_TV, OUTPUT);
    pinMode(PIN_SISTEMA_SEGURANCA, OUTPUT);
    pinMode(PIN_CAMERA, OUTPUT);
    pinMode(PIN_CORTINA_SALA, OUTPUT);
    pinMode(PIN_CORTINA_QUARTO, OUTPUT);
    pinMode(PIN_IRRIGACAO, OUTPUT);
    pinMode(PIN_PORTAO_GARAGEM, OUTPUT);
    pinMode(PIN_PORTA_FRENTE, OUTPUT);

    server.on("/comando", HTTP_POST, handleCommand);
    server.begin();
}

void loop() {
    server.handleClient();
}

// Conecta ao Wi-Fi
void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConectado ao WiFi");
    Serial.println("IP: " + WiFi.localIP());
}

// Processa os comandos recebidos
void handleCommand() {
    if (!server.hasArg("plain")) {
        server.send(400, "text/plain", "Corpo da requisição inválido");
        return;
    }

    String body = server.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);

    String command = doc["command"];
    String action = doc["action"];
    String response = processCommand(command, action);

    server.send(200, "text/plain", response);
}

// Função principal para processar comandos
String processCommand(const String& command, const String& action) {
    if (command == "ligar" || command == "desligar") {
        return controlDevice(command, action);
    } else if (command == "ajustar") {
        return adjustDevice(action);
    } else if (command == "status") {
        return getStatus(action);
    } else if (command == "ativar" || command == "desativar") {
        return toggleFeature(command, action);
    } else if (command == "abrir" || command == "fechar") {
        return controlCortinas(command, action);
    } else if (command == "trancar" || command == "destrancar") {
        return controlPorta(command, action);
    } else if (command == "adicionar" || command == "remover" || command == "listar") {
        return manageCommands(command, action);
    }

    return "Comando não reconhecido.";
}

// Controla dispositivos (ligar/desligar)
String controlDevice(const String& command, const String& action) {
    int pin = getPinByAction(action);
    if (pin == -1) return "Dispositivo não encontrado.";

    bool state = (command == "ligar") ? HIGH : LOW;
    digitalWrite(pin, state);

    return String(command == "ligar" ? "Ligado" : "Desligado") + ": " + action;
}

// Ajusta dispositivos (intensidade, temperatura, etc.)
String adjustDevice(const String& action) {
    if (action.startsWith("luz da sala para")) {
        int intensity = action.substring(17).toInt();
        analogWrite(PIN_LUZ_SALA, intensity);
        return "Intensidade da luz da sala ajustada para " + String(intensity) + "%.";
    } else if (action.startsWith("temperatura para")) {
        int temperature = action.substring(17).toInt();
        // Código para ajustar a temperatura
        return "Temperatura ajustada para " + String(temperature) + "°C.";
    } else if (action.startsWith("tempo de irrigação para")) {
        int duration = action.substring(23).toInt();
        // Código para ajustar o tempo de irrigação
        return "Tempo de irrigação ajustado para " + String(duration) + " minutos.";
    }

    return "Ajuste não reconhecido.";
}

// Obtém o status dos dispositivos
String getStatus(const String& action) {
    if (action == "luzes") {
        return "Status das luzes:\n" +
               "Luz da sala: " + getState(PIN_LUZ_SALA) + "\n" +
               "Luz do quarto: " + getState(PIN_LUZ_QUARTO);
    } else if (action == "eletrodomésticos") {
        return "Status dos eletrodomésticos:\n" +
               "Ar condicionado: " + getState(PIN_AR_CONDICIONADO) + "\n" +
               "Ventilador: " + getState(PIN_VENTILADOR) + "\n" +
               "Televisão: " + getState(PIN_TV);
    } else if (action == "segurança") {
        return "Status da segurança:\n" +
               "Sistema de segurança: " + getState(PIN_SISTEMA_SEGURANCA) + "\n" +
               "Câmera de segurança: " + getState(PIN_CAMERA);
    } else if (action == "irrigação") {
        return "Status da irrigação: " + getState(PIN_IRRIGACAO);
    }

    return "Status não reconhecido.";
}

// Ativa/desativa recursos (notificações, modos, etc.)
String toggleFeature(const String& command, const String& action) {
    if (action == "notificações de movimento") {
        // Código para ativar/desativar notificações de movimento
        return "Notificações de movimento " + (command == "ativar" ? "ativadas." : "desativadas.");
    } else if (action == "notificações de porta aberta") {
        // Código para ativar/desativar notificações de porta aberta
        return "Notificações de porta aberta " + (command == "ativar" ? "ativadas." : "desativadas.");
    }

    return "Recurso não reconhecido.";
}

// Controla cortinas
String controlCortinas(const String& command, const String& action) {
    int pin = (action == "cortinas da sala") ? PIN_CORTINA_SALA : PIN_CORTINA_QUARTO;
    bool state = (command == "abrir") ? HIGH : LOW;
    digitalWrite(pin, state);

    return String(command == "abrir" ? "Abertas" : "Fechadas") + ": " + action;
}

// Controla portas/portões
String controlPorta(const String& command, const String& action) {
    int pin = (action == "porta da frente") ? PIN_PORTA_FRENTE : PIN_PORTAO_GARAGEM;
    bool state = (command == "destrancar") ? HIGH : LOW;
    digitalWrite(pin, state);

    return String(command == "destrancar" ? "Destrancada" : "Trancada") + ": " + action;
}

// Gerenciamento de comandos personalizados
String manageCommands(const String& command, const String& action) {
    if (command == "adicionar") {
        // Código para adicionar comando
        return "Comando adicionado: " + action.substring(9);
    } else if (command == "remover") {
        // Código para remover comando
        return "Comando removido: " + action.substring(9);
    } else if (command == "listar") {
        // Código para listar comandos
        return "Lista de comandos personalizados: ...";
    }

    return "Gerenciamento de comandos não reconhecido.";
}

// Retorna o pino correspondente à ação
int getPinByAction(const String& action) {
    if (action == "luz da sala") return PIN_LUZ_SALA;
    if (action == "luz do quarto") return PIN_LUZ_QUARTO;
    if (action == "ar condicionado") return PIN_AR_CONDICIONADO;
    if (action == "ventilador") return PIN_VENTILADOR;
    if (action == "televisão") return PIN_TV;
    if (action == "sistema de segurança") return PIN_SISTEMA_SEGURANCA;
    if (action == "câmera de segurança") return PIN_CAMERA;
    if (action == "irrigação") return PIN_IRRIGACAO;
    if (action == "portão da garagem") return PIN_PORTAO_GARAGEM;
    if (action == "porta da frente") return PIN_PORTA_FRENTE;
    return -1; // Pino não encontrado
}

// Retorna o estado atual de um pino (Ligado/Desligado)
String getState(int pin) {
    return digitalRead(pin) ? "Ligado" : "Desligado";
}
