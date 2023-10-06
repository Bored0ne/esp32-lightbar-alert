//
// Created by stephen on 9/29/2023.
//

#ifndef LIGHT_AND_SOUND_MQTT_ITOPICHANDLER_H
#define LIGHT_AND_SOUND_MQTT_ITOPICHANDLER_H


class ITopicHandler {
public:
    ITopicHandler();
    virtual void handle(char * topic, char * payload);
};


#endif //LIGHT_AND_SOUND_MQTT_ITOPICHANDLER_H
