#ifndef NEC_RX_H
#define NEC_RX_H
#include <driver/ir_rx.h>
#include <freertos/FreeRTOS.h>

#include <vector>

namespace alexa_ir { namespace nec_protocol { namespace nec_rx {
constexpr auto kDefaulGpio{5};
constexpr auto kDefaultBuffLen{128};
class NecRx {
   private:
    uint32_t m_gpio{kDefaulGpio};
    uint32_t m_buffer_len{kDefaultBuffLen};

   public:
    NecRx();
    NecRx(uint32_t a_gpio, uint32_t a_buf_len);
    NecRx(const NecRx& a_nec_rx) = delete;
    NecRx& operator=(const NecRx& a_nec_rx) = delete;
    NecRx(NecRx&& a_nec_rx) = delete;
    NecRx& operator=(NecRx&& a_nec_rx) = delete;
    void Disable();
    void Enable();
    std::vector<ir_rx_nec_data_t> Recv(std::size_t a_cmd_len,
                                       uint32_t a_timeout_ticks = portMAX_DELAY);

    uint32_t GetGpio() const;
    uint32_t GetBufferLen() const;
    ~NecRx();
};
}}}  // namespace alexa_ir::nec_protocol::nec_rx

#endif /* NEC_RX_H */
