#include "nec_rx.hpp"

#include <driver/gpio.h>
#include <driver/ir_rx.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "esp_log.h"

using alexa_ir::nec_protocol::nec_rx::NecRx;
NecRx::NecRx() {
    ir_rx_config_t m_config = {.io_num = m_gpio, .buf_len = m_buffer_len};
    if (ir_rx_init(&m_config) != ESP_OK) {
        ESP_LOGE("NEC_RX", "Error Initiating IR RX drvier");
        ::exit(-1);
    }
}
NecRx::NecRx(uint32_t a_gpio, uint32_t a_buf_len) : m_gpio{a_gpio}, m_buffer_len{a_buf_len} {
    ir_rx_config_t m_config = {.io_num = a_gpio, .buf_len = a_buf_len};

    if (ir_rx_init(&m_config) != ESP_OK) {
        ESP_LOGE("NEC_RX", "Error Initiating IR RX drvier");
        ::exit(-1);
    }
}
std::vector<ir_rx_nec_data_t> NecRx::Recv(std::size_t a_len, uint32_t a_timeout_ticks) {
    std::vector<ir_rx_nec_data_t> nec_cmds;
    nec_cmds.resize(a_len);
    if (ir_rx_recv_data(nec_cmds.data(), nec_cmds.size(), a_timeout_ticks) == -1) {
        return {};
    }
    return nec_cmds;
}
void NecRx::Disable() { ir_rx_disable(); }
void NecRx::Enable() { ir_rx_enable(); }
uint32_t NecRx::GetGpio() const { return m_gpio; }
uint32_t NecRx::GetBufferLen() const { return m_buffer_len; }
NecRx::~NecRx() {
    ir_rx_disable();
    ir_rx_deinit();
    gpio_uninstall_isr_service();
}
