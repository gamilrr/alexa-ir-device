
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <nec_tx.hpp>
#include <string>
#include <vector>

#include "unity.h"

using alexa_ir::nec_protocol::nec_tx::NecTx;
constexpr uint32_t kIrGpio = 14;
constexpr size_t kCarrierFreq = 38000;

TEST_CASE("Nec TX constructor", "[nec_tx]") {
    // arrange //act
    NecTx nec_tx;

    // assert
    TEST_ASSERT(nec_tx.GetGpio() == alexa_ir::nec_protocol::nec_tx::kDefaultGpio);
    TEST_ASSERT(nec_tx.GetCarrierFreq() == alexa_ir::nec_protocol::nec_tx::kDefaultCarrierFreq);
}

TEST_CASE("Nec TX default constructor", "[nec_tx]") {
    // arrange //act
    NecTx nec_tx(kIrGpio, kCarrierFreq);
    // assert
    TEST_ASSERT(nec_tx.GetGpio() == kIrGpio);
    TEST_ASSERT(nec_tx.GetCarrierFreq() == kCarrierFreq)
}

TEST_CASE("Nec TX send data", "[nec_tx]") {
    // arrange
    NecTx nec_tx;
    std::vector<ir_tx_nec_data_t> cmds{};
    ir_tx_nec_data_t data{};
    data.addr1 = 0x8;
    data.addr2 = 0xF5;
    data.cmd1 = 0x2;
    data.cmd2 = 0xFD;

    // act
    constexpr std::size_t kCmdCounter{5};
    auto cmd_counter{0};
    while (cmd_counter < kCmdCounter) {
        cmds.push_back(data);
        ++cmd_counter;
    }

    ESP_LOGI("Nec Tx", "Sending data.....");
    // assert
    TEST_ASSERT(nec_tx.SendCmd(cmds) == cmds.size());
}
