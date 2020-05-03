#ifndef NEC_TX_H
#define NEC_TX_H
#include <driver/ir_tx.h>
#include <freertos/FreeRTOS.h>

#include <vector>
namespace alexa_ir { namespace nec_protocol { namespace nec_tx {
constexpr auto kDefaultCarrierFreq{38000};
constexpr auto kDefaultGpio{14};
class NecTx {
   private:
    uint32_t m_carrier_freq{kDefaultCarrierFreq};
    uint32_t m_gpio{kDefaultGpio};

   public:
    NecTx();
    NecTx(uint32_t a_gpio, uint32_t a_carrier_freq);

    NecTx(const NecTx&) = delete;
    NecTx& operator=(const NecTx&) = delete;
    NecTx(NecTx&&) = delete;
    NecTx& operator=(NecTx&&) = delete;

    int SendCmd(std::vector<ir_tx_nec_data_t> a_cmds, uint32_t a_timeout_ms = portMAX_DELAY);
    uint32_t GetGpio() const;
    uint32_t GetCarrierFreq() const;
    ~NecTx();
};
}}}  // namespace alexa_ir::nec_protocol::nec_tx

#endif /* NEC_TX_H */
