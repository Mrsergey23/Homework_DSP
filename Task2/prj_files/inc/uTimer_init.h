#ifndef __UTIMER_INIT
#define __UTIMER_INIT
#include "stdint.h"
#define TIMER				MDR_TIMER1
#define TIMER_IRQ		TIMER1_IRQn
void m_setUpTimer(uint8_t req_time, uint8_t req_CPUfreq);
#endif // __UTIMER_INIT