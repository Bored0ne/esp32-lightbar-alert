//
// Created by stephen on 9/29/2023.
//
#include <Arduino.h>
#include "blink.h"

#ifndef BUILTIN_LED_PIN
#define BUILTIN_LED_PIN 2
#endif

void blinkEvery100MS(void *parameter) {
    while (true) {
        digitalWrite(BUILTIN_LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(100));
        digitalWrite(BUILTIN_LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

TaskHandle_t blinkTask = nullptr;

Blink::Blink() {
    pinMode(BUILTIN_LED_PIN, OUTPUT);
    this->start();
}

void Blink::start() {
    if (blinkTask == nullptr) {
        xTaskCreate(blinkEvery100MS, "Blink", 1000, NULL, 1, &blinkTask);
    }
}

void Blink::stop() {
    if (blinkTask != nullptr){
        vTaskDelete(blinkTask);
        blinkTask = nullptr;
        digitalWrite(BUILTIN_LED_PIN, LOW);
    }
}

