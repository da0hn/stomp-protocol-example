#include <Arduino.h>
#include <WiFi.h>
#include <StompClient.h>


/**
* WiFi settings
**/
const char *wlan_ssid = "---INSIRA O WIFI AQUI---";
const char *wlan_password = "---INSIRA A SENHA AQUI---";

/**
* Stomp server settings
**/
bool useWSS = true;
const char *ws_host = "192.168.100.11";
const int ws_port = 8080;
const char *ws_baseurl = "/gs-guide-websocket/"; // don't forget leading and trailing "/" !!!

bool sample = false;
int blink = 0;
bool blinkOn = false;
long lastBlinked;

WebSocketsClient webSocket;

Stomp::StompClient stomper(webSocket, ws_host, ws_port, ws_baseurl, true);

Stomp::Stomp_Ack_t handleBlinkMessage(const Stomp::StompCommand cmd) {
    Serial.println("Got a message!");
    Serial.println(cmd.body);

    return Stomp::CONTINUE;
}

Stomp::Stomp_Ack_t handleSampleMessage(const Stomp::StompCommand cmd) {
    Serial.println("Got a message!");
    Serial.println(cmd.body);

    sample = true;
    return Stomp::CONTINUE;
}

// Once the Stomp connection has been made, subscribe to a topic
void subscribe(Stomp::StompCommand cmd) {
    Serial.println("Connected to STOMP broker");
    stomper.subscribe("/commands/sample", Stomp::CLIENT, handleSampleMessage);
}

void error(const Stomp::StompCommand cmd) {
    Serial.println("ERROR: " + cmd.body);
}


void takeSample() {
    if (sample) {
        stomper.sendMessage("/esp/sensors",
                            "BATATATATATATATA");
        sample = false;
    }
}


void loop() {
    webSocket.loop();
    takeSample();
}

void setup() {

    // setup serial
    Serial.begin(115200);
    // flush it - ESP Serial seems to start with rubbish
    Serial.println();

    // connect to WiFi
    Serial.println("Logging into WLAN: " + String(wlan_ssid));
    Serial.print(" ...");
    WiFi.begin(wlan_ssid, wlan_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" success.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // Start the StompClient
    stomper.onConnect(subscribe);
    stomper.onError(error);
    stomper.begin();
}