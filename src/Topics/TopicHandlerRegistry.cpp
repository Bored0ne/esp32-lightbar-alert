//
// Created by stephen on 9/29/2023.
//

#include "Topics/TopicHandlerRegistry.h"

TopicHandlerRegistry::TopicHandlerRegistry() {
    // Constructor initialization if needed
}

void TopicHandlerRegistry::registerHandler(ITopicHandler* handler) {
    // Store the handler in the vector
    handlers.push_back(handler);
}

void TopicHandlerRegistry::process(char* topic, char* payload) {
    // Iterate through all registered handlers and call their handle functions
    for (ITopicHandler* handler : handlers) {
        handler->handle(topic, payload);
    }
}