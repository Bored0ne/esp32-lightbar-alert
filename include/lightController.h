//
// Created by stephen on 9/29/2023.
//
#include "lightMode.h"

#ifndef LIGHT_AND_SOUND_MQTT_LIGHTCONTROLLER_H
#define LIGHT_AND_SOUND_MQTT_LIGHTCONTROLLER_H

class LightController {
public :
    LightController();
    void setLeds(int numberOfLeds);
    void setMode(LightMode mode);
};


#endif //LIGHT_AND_SOUND_MQTT_LIGHTCONTROLLER_H
