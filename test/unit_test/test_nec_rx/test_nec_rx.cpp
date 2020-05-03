
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <nec_rx.hpp>
#include <string>
#include <vector>

#include "unity.h"

using alexa_ir::nec_protocol::nec_rx::NecRx;
constexpr uint32_t kIrGpio = 5;
constexpr size_t kIrBuffLen = 128;

TEST_CASE("Nec RX constructor", "[nec_rx]") {
    // arrange //act
    NecRx nec_rx(kIrGpio, kIrBuffLen);

    // assert
    TEST_ASSERT(nec_rx.GetBufferLen() == kIrBuffLen);
    TEST_ASSERT(nec_rx.GetGpio() == kIrGpio);
}

TEST_CASE("Nec RX default constructor", "[nec_rx]") {
    // arrange //act
    NecRx nec_rx;
    // assert
    TEST_ASSERT(nec_rx.GetBufferLen() == alexa_ir::nec_protocol::nec_rx::kDefaultBuffLen);
    TEST_ASSERT(nec_rx.GetGpio() == alexa_ir::nec_protocol::nec_rx::kDefaulGpio);
}

TEST_CASE("Nec RX recieve data", "[nec_rx]") {
    // arrange
    NecRx nec_rx;

    // act
    std::cout << "Waiting signal.....\n";
    auto cmds = nec_rx.Recv(1);

    // assert
    for (auto item : cmds) {
        std::cout << "command1: " << std::hex << item.cmd1 << " command2: " << std::hex << item.cmd2
                  << " address1: " << std::hex << item.addr1 << " address2: " << std::hex
                  << item.addr2 << '\n';
    }
    TEST_ASSERT(cmds.size() == 1);
}

TEST_CASE("Nec RX recieve data with timeout", "[nec_rx]") {
    // arrange
    NecRx nec_rx;

    // act
    std::cout << "Waiting for timeout.....\n";
    const uint32_t delay_ms = 3000;
    auto cmds = nec_rx.Recv(1, delay_ms / portTICK_RATE_MS);  // 3sec
    TEST_ASSERT(cmds.empty());
}

TEST_CASE("Nec RX receive data with cmd list bigger than buffer size", "[nec_rx]") {
    // arrange
    constexpr auto kNewBufferLen{2};
    constexpr auto kNewCmdListLen{3};

    NecRx nec_rx(kIrGpio, kNewBufferLen);

    // act
    std::cout << "Waiting signal....." << '\n';
    auto cmds = nec_rx.Recv(kNewCmdListLen);

    for (auto& item : cmds) {
        std::cout << "Command: " << std::hex << item.cmd1 << '\n';
    }

    // assert
    TEST_ASSERT(cmds.size() == 3);
}
