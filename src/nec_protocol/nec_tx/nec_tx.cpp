#include "nec_tx.hpp"

#include <driver/ir_tx.h>
#include <esp_log.h>

#include <cstdlib>
using alexa_ir::nec_protocol::nec_tx::NecTx;

NecTx::NecTx() {
    ir_tx_config_t ir_rx_config = {
        .io_num = m_gpio, .freq = m_carrier_freq, .timer = IR_TX_WDEV_TIMER};

    if (ir_tx_init(&ir_rx_config) != ESP_OK) {
        ESP_LOGE("Nec Tx", "Error initiating ir tx driver");
        ::exit(-1);
    }
}

NecTx::NecTx(uint32_t a_gpio, uint32_t a_carrier_freq)
    : m_carrier_freq{a_carrier_freq}, m_gpio{a_gpio} {
    ir_tx_config_t ir_rx_config = {
        .io_num = m_gpio, .freq = m_carrier_freq, .timer = IR_TX_WDEV_TIMER};

    if (ir_tx_init(&ir_rx_config) != ESP_OK) {
        ESP_LOGE("Nec Tx", "Error initiating ir tx driver");
        ::exit(-1);
    }
}

int NecTx::SendCmd(std::vector<ir_tx_nec_data_t> a_cmds, uint32_t a_timeout_ticks) {
    return ir_tx_send_data(a_cmds.data(), a_cmds.size(), a_timeout_ticks);
}
uint32_t NecTx::GetGpio() const { return m_gpio; }
uint32_t NecTx::GetCarrierFreq() const { return m_carrier_freq; }
NecTx::~NecTx() { ir_tx_deinit(); }
