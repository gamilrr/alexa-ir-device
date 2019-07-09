#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "queue.h"
#include <cassert>
#include "logger.hpp"
#include <utility>

template<typename T>
class message_queue
{

private:
    QueueHandle_t mQueue{nullptr};
    
public:

    message_queue() = default;

    explicit  message_queue(uint16_t pSize):mQueue{xQueueCreate(pSize, sizeof(T))}{
       assert(mQueue != nullptr); //assertion in invalid constructor
    }

    message_queue(message_queue&& other): mQueue{other.mQueue}{
        other.mQueue = nullptr;
    }
    

    message_queue& operator=(message_queue&& other) noexcept{
        
       if(this != &other) return *this;

       vQueueDelete(mQueue);

       mQueue = other.mQueue;
       other.mQueue = nullptr;

       return *this;
    }


    bool post(const T& data, const TickType_t& msTimeout) const{

        TickType_t timeout = msTimeout/portTICK_PERIOD_MS;

        if(xQueueSendToBack(mQueue, &data, timeout) == pdPASS)
            return true;

        return false;

    }

    bool postFormISR(const T& data) const{

        if(xQueueSendToBackFromISR(mQueue, &data, nullptr) == pdPASS)
            return true;
        
        return false;
    }

    bool receive(T& data, uint32_t msTimeout) const{

        TickType_t timeout = msTimeout/portTICK_PERIOD_MS;

        if(xQueueReceive(mQueue, &data, timeout) == pdPASS)
            return true;

        return false;

    }

    bool receiveAndBlock(T& data) const {
        
        if(xQueueReceive(mQueue, &data, portMAX_DELAY) == pdPASS)
            return true;

        return false;
    }
    

    ~message_queue(){
        vQueueDelete(mQueue);
    }


};

#endif 