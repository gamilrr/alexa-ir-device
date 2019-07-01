/* 
 * The MIT License (MIT)
 * 
 * ESP8266 FreeRTOS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * https://github.com/SuperHouse/esp-open-rtos
 * 
 */

#include <task.hpp>
#include <queue.h>
#include <espressif/esp_common.h>
#include <esp/uart.h>
#include "nec_protocol_task.hpp"
#include "gpio_intr.hpp"
#include "logger.hpp"



static QueueHandle_t nec_queue;

void gpio_intr_handler(uint8_t gpio_num)
{
    static volatile int32_t before = 0;
    uint32_t now = sdk_system_get_time();
    uint32_t interval = now - before;
    xQueueSendToBackFromISR(nec_queue, &interval, NULL);
    before = now; 
}


nec_protocol_task nec_task;
/**
 * 
 */
extern "C" void user_init(void)
{
    uart_set_baud(0, 115200);
    sdk_system_update_cpu_freq(SYS_CPU_160MHZ); //Set CPU frequency to 160MHZ


    nec_task.task_create("nec");
    nec_queue = nec_task.get_queue();
    gpio_intr::set_gpio_inter(14, gpio_intr_handler);

}
