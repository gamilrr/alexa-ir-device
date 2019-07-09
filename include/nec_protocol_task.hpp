#ifndef NEC_PROTOCOL_TASK_H
#define NEC_PROTOCOL_TASK_H

#include <task.hpp>
#include <queue.h>
#include "message_queue.hpp"


using namespace esp_open_rtos::thread;

class nec_protocol_task : public task_t
{

   
private:

    //control buffer
    struct nec_buffer{
        uint8_t counter;
        uint32_t data;
    };

    //State Machine Control Types
    enum class NEC_STATES{Start, Data, CIdle, DIdle, Zero, One, Repeat}; //NEC Protocol states   
    enum class NEC_EVENTS{E_9ms, E_4_5ms, E_562us, E_2_25ms, E_1_6ms, E_40ms, E_90ms, E_None}; //NEC protocol transicions events
    

    NEC_STATES mState{NEC_STATES::CIdle}; //NEC Protocol state
    message_queue<uint32_t> mInQueue{64}; //Input queue entry to nec protocol task 16 elements
    nec_buffer buffer{0,0}; //Data recieved

    //filter and generate event
    NEC_EVENTS generate_events(const uint32_t burst);

    //init state protocol
    void init_protocol_state();

    //get valid command
    void check_command();

    //states handlers
    void start_handler(NEC_EVENTS event);
    void data_handler(NEC_EVENTS event);
    void data_idle_handler(NEC_EVENTS event); 
    void control_idle_handler(NEC_EVENTS event); 
    void zero_handler(NEC_EVENTS event);
    void one_handler(NEC_EVENTS event);
    void repeat_handler(NEC_EVENTS event);
    
    void task();

public:

    const message_queue<uint32_t>& get_queue() const;

    nec_protocol_task() = default;
    nec_protocol_task(nec_protocol_task&& nec) = delete; //eliminate resources handling

};



#endif