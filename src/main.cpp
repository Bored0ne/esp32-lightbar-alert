#include <Arduino.h>
#include <WiFi.h>
#include "blink.h"
#include "PubSubClient.h"
#include "Topics/TopicHandlerRegistry.h"
#include "Topics/D4LightHandler.h"
#include "FastLED.h"
#include "Audio.h"
#include "secrets.h"
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <SD.h>
#include <SD_MMC.h>
#include <SPIFFS.h>
#include <FFat.h>

#define SD_MISO 5
#define SD_MOSI 18
#define SD_SCK 19
#define SD_CS 21

#define BLCK_PIN 25
#define LRC_PIN 26
#define BUILTIN_LED_PIN 2
#define BUTTON_PIN 14
#define AUDIO_OUT_PIN 33

void setClock()
{
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

TopicHandlerRegistry topicHandlerRegistry;
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println(topic);
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    topicHandlerRegistry.process(topic, (char*) payload);
}

WiFiClientSecure wifiClient;
PubSubClient pubSubClient(MQTT_SERVER, 1883, callback, wifiClient);

Blink blinker;
Audio audio;
void setup() {
    Serial.begin(115200);
    blinker.start();
    Serial.println("pin setup");
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    File root = SPIFFS.open("/");

    File file = root.openNextFile();

    while(file){

        Serial.print("FILE: ");
        Serial.println(file.name());

        file = root.openNextFile();
    }

    audio.setPinout(BLCK_PIN, LRC_PIN, AUDIO_OUT_PIN);
    audio.setVolume(21);
    audio.connecttoFS(SPIFFS, "navi_annoying.mp3");

    WiFi.begin(SSID, SSPW);

    Serial.print("Connecting!");

    while (WiFiClass::status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    setClock();
    wifiClient.setInsecure();
    while (pubSubClient.state() != MQTT_CONNECTED) {
        if (pubSubClient.connect("arduinoClient", MQTT_USER, MQTT_PW)) {
            Serial.println("topic subscribing now");
            pubSubClient.subscribe("d4Lights");
        } else {
            Serial.println("Connection failed");
            Serial.print(pubSubClient.state());
            delay(200);
        }
    }

    topicHandlerRegistry.registerHandler(new D4LightHandler());

    blinker.stop();
    Serial.println("good to go!");
}

void loop() {
    pubSubClient.loop();
    audio.loop();
    if (pubSubClient.state() != MQTT_CONNECTED){
        Serial.println(pubSubClient.state());
        if (pubSubClient.connect("arduinoClient", MQTT_USER, MQTT_PW)) {
            Serial.println("topic subscribing now");
            pubSubClient.subscribe("d4Lights");
        } else {
            Serial.println("Connection failed");
            Serial.print(pubSubClient.state());
            delay(200);
        }
    }
}