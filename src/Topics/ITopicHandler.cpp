//
// Created by stephen on 9/29/2023.
//

#include "Topics/ITopicHandler.h"
#include <Arduino.h>

ITopicHandler::ITopicHandler() {}

void ITopicHandler::handle(char* topic, char* payload){
    Serial.println("Handle not implemented yet");
};

