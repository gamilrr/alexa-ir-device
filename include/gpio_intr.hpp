#ifndef GPIO_INTR_H
#define GPIO_INTR_H

#include "gpio_intr.hpp"
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "esp8266.h"

class gpio_intr{

    public:
    static void set_gpio_inter(const int gpio, void (*isr)(uint8_t )){
        gpio_enable(gpio, GPIO_INPUT);
        gpio_set_interrupt(gpio,  GPIO_INTTYPE_EDGE_ANY, isr);
        printf("Waiting for button press interrupt on gpio %d...\r\n", gpio);
    }
};

#endif