#ifndef __U_LED_FUN_H
#define __U_LED_FUN_H
#include "MDR32F9Qx_port.h"
#define PIN_LEDs (PORT_Pin_15); // here you can add number of LEDs. Use | for connect two names of pins 
#define PORT_LEDs MDR_PORTB 		// select the port to which the LEDs are connected
void m_initLEDs (void);
void m_LED_off (uint32_t Pins);
void m_LED_on (uint32_t Pins);
void m_LED_toggle (uint32_t Pins);
#endif /*__U_LED_FUN_H */