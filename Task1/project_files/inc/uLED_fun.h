#ifndef __U_LED_FUN_H
#define __U_LED_FUN_H
#include "MDR32F9Qx_port.h"
#define PIN_LEDs (PORT_Pin_15);
#define PORT_LEDs MDR_PORTB
void m_initLEDs (void);
void m_LED_off (uint32_t Pins);
void m_LED_on (uint32_t Pins);
void m_LED_toggle (uint32_t Pins);
static PORT_InitTypeDef PortInitStructure;
#endif /*__U_LED_FUN_H */