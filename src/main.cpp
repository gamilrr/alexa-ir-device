#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <iostream>

#include "nec_protocol/nec_rx/nec_rx.hpp"

using alexa_ir::nec_protocol::nec_rx::NecRx;

constexpr auto kGpioNumber = 5;
constexpr auto kBufferSize = 128;
constexpr auto kCmdLen = 1;
constexpr auto kTaskStack = 2048;

void PrintIRCode(void* /*a_param*/) {
    NecRx nec_rx(kGpioNumber, kBufferSize);
    std::cout << "Command received... \n";
    while (true) {
        auto vec = nec_rx.Recv(kCmdLen);
        for (auto& item : vec) {
            ESP_LOGI("main", "addr1: 0x%xh, addr2: 0x%xh, cmd1: 0x%xh, cmd2: 0x%xh\n", item.addr1,
                     item.addr2, item.cmd1, item.cmd2);
        }
    }
    vTaskDelete(nullptr);
}

extern "C" void app_main(void) {
    xTaskCreate(PrintIRCode, "PrintIRCode", kTaskStack, nullptr, 1, nullptr);
}
