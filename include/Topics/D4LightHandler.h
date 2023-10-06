//
// Created by stephen on 9/29/2023.
//

#include "ITopicHandler.h"
#include "Audio.h"

#ifndef LIGHT_AND_SOUND_MQTT_D4LIGHTHANDLER_H
#define LIGHT_AND_SOUND_MQTT_D4LIGHTHANDLER_H

class D4LightHandler : public ITopicHandler {
public:
    D4LightHandler();
    void handle(char *topic, char *payload) override;
private:
    int numberOfLeds = 20;
    String effectName = "rainbow";
};
extern Audio audio;

#endif //LIGHT_AND_SOUND_MQTT_D4LIGHTHANDLER_H
