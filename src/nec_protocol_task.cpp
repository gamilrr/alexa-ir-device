#include "nec_protocol_task.hpp"
#include "logger.hpp"
#include <FreeRTOS.h>

const message_queue<uint32_t>& nec_protocol_task::get_queue() const{
    return mInQueue;
}

void nec_protocol_task::task(){

    NEC_EVENTS event = NEC_EVENTS::E_None;

    while(true){
        uint32_t burst = 0;
        
        if(!mInQueue.receiveAndBlock(burst)){
            logger::LOG("ERROR: Error receiving burst data in nec_protocol_task\n");
            continue;
        } 
        
        event = generate_events(burst);

        if(event == NEC_EVENTS::E_None){ //restart if unknown event is tiggered
            init_protocol_state();
            continue;
        } 

        switch(mState){

            case NEC_STATES::Start:
                start_handler(event);
                break;
           
            case NEC_STATES::Data:
                data_handler(event);
                break;
           
            case NEC_STATES::CIdle:
                control_idle_handler(event);
                break;

            case NEC_STATES::DIdle:
                data_idle_handler(event);
                break;
           
            case NEC_STATES::Zero:
                zero_handler(event);
                break;
           
            case NEC_STATES::One:
                one_handler(event);
                break;
           
            case NEC_STATES::Repeat:
                repeat_handler(event);
                break;
           
            default:
                break;
        }



        check_command();
       

    }
}

void nec_protocol_task::init_protocol_state(){
    buffer = {0, 0};
    mState = NEC_STATES::CIdle;
}

void nec_protocol_task::check_command(){
    
    if(buffer.counter == 32){
            uint8_t hex = (0xFF000000 & buffer.data) >> 24; 
            logger::LOG("%02X", hex);
            hex = (0x00FF0000 & buffer.data) >> 16; 
            logger::LOG("%02X", hex);
            hex = (0x0000FF00 & buffer.data) >> 8; 
            logger::LOG("%02X", hex);
            hex = (0x000000FF & buffer.data); 
            logger::LOG("%02X\n", hex);

            init_protocol_state();
    }

}

nec_protocol_task::NEC_EVENTS nec_protocol_task::generate_events(const uint32_t burst){

    if(burst > 8500 && burst < 9500){
        return NEC_EVENTS::E_9ms;
    }

    if(burst > 4000 && burst < 5000){
        return NEC_EVENTS::E_4_5ms;
    }

    if(burst >= 450 && burst <= 800){
        return NEC_EVENTS::E_562us;
    }

    if(burst > 2000 && burst < 3000){
        return NEC_EVENTS::E_2_25ms;
    }

    if(burst > 1000 && burst <= 2000){
        return  NEC_EVENTS::E_1_6ms;
    }

    if(burst > 38000 && burst < 42000){
        return NEC_EVENTS::E_40ms;
    }

    if(burst > 90000 && burst < 110000){
        return NEC_EVENTS::E_90ms;
    }

    return NEC_EVENTS::E_None;

}


/// State Pending Processing
void nec_protocol_task::control_idle_handler(NEC_EVENTS event){

    switch(event){

        case NEC_EVENTS::E_9ms:
            mState = NEC_STATES::Start;
            break;

        default:
            break;
    }

}

/// State Pending Processing
void nec_protocol_task::data_idle_handler(NEC_EVENTS event){

    switch(event){

        case NEC_EVENTS::E_562us:
            mState = NEC_STATES::Zero;
            break;

        case NEC_EVENTS::E_1_6ms:
            mState = NEC_STATES::One;
            break;

        default:
            break;
    }

}

/// State Start Processing
void nec_protocol_task::start_handler(NEC_EVENTS event){
    
    switch(event){

        case NEC_EVENTS::E_4_5ms:
            mState = NEC_STATES::Data;
            break;

        case NEC_EVENTS::E_2_25ms: 
            mState = NEC_STATES::Repeat;
            break;

        default:
            break;
    }

}

/// State Data Processing
void nec_protocol_task::data_handler(NEC_EVENTS event){

    
    switch(event){

        case NEC_EVENTS::E_562us:
            mState = NEC_STATES::DIdle;
            break;

        default:
            break;
    }

}

/// State Zero Processing
void nec_protocol_task::zero_handler(NEC_EVENTS event){

    buffer.data <<= 1;
    buffer.counter++;

    switch(event){

        case NEC_EVENTS::E_562us:
            mState = NEC_STATES::DIdle;
            break;

        default:
            break;
    }

}

/// State One Processing
void nec_protocol_task::one_handler(NEC_EVENTS event){

    buffer.data <<= 1; 
    buffer.data |= 1;
    buffer.counter++;

    switch(event){

        case NEC_EVENTS::E_562us:
            mState = NEC_STATES::DIdle;
            break;

        default:
            break;
    }

}

/// State Repeat Processing
void nec_protocol_task::repeat_handler(NEC_EVENTS event){
    
    logger::LOG("FFFFFFFF\n");
    
    switch(event){

        case NEC_EVENTS::E_562us:
            mState = NEC_STATES::CIdle;
            break;

        default:
            break;
    }

}