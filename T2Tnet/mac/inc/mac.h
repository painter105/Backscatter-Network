#include "buffers.h"
#include "frameValidation.h"
#include "tx.h"
#include "rx.h"

#ifndef MAC_H_
#define MAC_H_

/**
 *  MAC related timing
 */
#define MAC_PREAMBLE_SAMPLING_INTERVAL   1000                     // Radio Settling time = 7.5 ms, so minimum ACLK cycles (32kHz) = ~250 cycles => 500 cycles = ~15 ms, 650 cycles = ~20 ms
#define MAC_RX_TIMEOUT_ACLK				SLEEP_PERIOD_ACLK/2	    // Ticks with 32.768 kHz CLK
#define MAC_CSMA_PERIOD_ACLK			    3000						// Radio Settling time = 7.5 ms, so minimum ACLK cycles (32kHz) = ~250 cycles => 500 cycles = ~15 ms, 650 cycles = ~20 ms
#define MAC_EXTRA_CSMA_MAX_ACLK			500						// Maximum extra ticks to do CSMA with 32.768 kHz CLK (i.e. max 10 bytes CSMA)

void mac_init();
void* preamble_sampling();
func_ptr mac_func;

#endif  // end of MAC_H_
