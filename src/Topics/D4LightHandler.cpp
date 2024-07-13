//
// Created by stephen on 9/29/2023.
//

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include "Topics/D4LightHandler.h"

#define LED_PIN 4


struct LedEffectParameters {
    int numberOfLeds;
    String effectName;
    CRGB *leds;
};

TaskHandle_t effectTask = nullptr;
LedEffectParameters lep;
CRGB leds[30];
SemaphoreHandle_t lightLockout;

void rainbowTask(void *parameter) {
    Serial.println("Starting rainbows");
    LedEffectParameters *lep = reinterpret_cast<LedEffectParameters *>(parameter);
    Serial.println(lep->numberOfLeds);
    bool isBrightening = true;
    int intensity = 0;
    int rainbowCyclePos = 0;
    int intensityUpperBound = 255;
    int intensityLowerBound = 0;
    int sleepSpeed = 1;
    audio.connecttoFS(SPIFFS, "navi_float.mp3");
    while (true) {
        xSemaphoreTake(lightLockout, portMAX_DELAY);
        fill_solid(leds, 30, CRGB::Black);
        int iv;

        if (isBrightening) {
            iv = intensity++;
            if (intensity > intensityUpperBound) {
                isBrightening = false;
            }
        } else {
            iv = intensity--;
            if (intensity < intensityLowerBound) {
                isBrightening = true;
            }
        }
        FastLED.setBrightness(iv);

        if (rainbowCyclePos == 255) {
            rainbowCyclePos = 0;
        }
        fill_rainbow_circular(leds, lep->numberOfLeds, rainbowCyclePos++);
        FastLED.show();
        xSemaphoreGive(lightLockout);
        vTaskDelay(pdMS_TO_TICKS(sleepSpeed));
    }
}

D4LightHandler::D4LightHandler() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 30);
    lightLockout = xSemaphoreCreateMutex();
}

void D4LightHandler::handle(char *topic, char *payload) {
    xSemaphoreTake(lightLockout, portMAX_DELAY);
    Serial.println(topic);
    if (strcmp(topic, "d4Lights") != 0) {
        return;
    }

    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.println("Failed to parse JSON");
        return;
    }

    JsonObject jsonObject = doc.as<JsonObject>();

    int newNumberOfLeds = jsonObject["numberOfLeds"];
    String newEffectName = jsonObject["effect"];
    Serial.println("nullptr check");
    if (effectTask != nullptr) {
        vTaskDelete(effectTask);
        effectTask = nullptr;
    }

    if (effectTask == nullptr){
        Serial.println("effectTask is a nullptr");
    }

    numberOfLeds = newNumberOfLeds;
    effectName = newEffectName;
    if (effectName == "rainbows" || effectName == "rainbow") {
        lep.numberOfLeds = numberOfLeds;
        lep.effectName = effectName;
        lep.leds = leds;
        xTaskCreate(rainbowTask, "rainbowTask", 4096, &lep, 1, &effectTask);
    }
    xSemaphoreGive(lightLockout);
}
