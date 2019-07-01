#include "logger.hpp"
#include <stdio.h>

void logger::LOG(const char* msg, ...){
        #ifdef LOG_DEBUG   
        va_list args;
        va_start(args,msg);
        vprintf(msg,args);
        va_end(args);
        #endif
}
