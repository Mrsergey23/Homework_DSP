#ifndef __U_UART_FUNC
#define __U_UART_FUNC
#include "stdint.h"
#define 	UART							MDR_UART3
#define   UART_IRQn					UART3_IRQn
	
	#define UART_PORT_TX			MDR_PORTF
	#define UART_PORT_PinTX		PORT_Pin_1
	#define UART_PORT_FuncTX  PORT_FUNC_ALTER
	
	#define UART_PORT_RX			MDR_PORTF	
	#define UART_PORT_PinRX		PORT_Pin_0
	#define UART_PORT_FuncRX  PORT_FUNC_ALTER	
void m_UART_Init (uint32_t UART_Speed);
#endif // __U_UART_FUNC