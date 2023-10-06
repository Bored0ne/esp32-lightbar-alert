#ifndef TOPIC_HANDLER_REGISTRY_H
#define TOPIC_HANDLER_REGISTRY_H

#include "ITopicHandler.h" // Include the header where ITopicHandler is declared
#include <vector>

class TopicHandlerRegistry {
public:
    TopicHandlerRegistry();
    void registerHandler(ITopicHandler* handler);
    void process(char* topic, char* payload);

private:
    std::vector<ITopicHandler*> handlers;
};

#endif // TOPIC_HANDLER_REGISTRY_H
