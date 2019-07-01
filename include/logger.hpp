#ifndef LOGGER_H
#define LOGGER_H


#include <stdarg.h>


struct logger
{
    static void LOG(const char* msg, ...);
};

#endif 