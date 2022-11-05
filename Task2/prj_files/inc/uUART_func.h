#ifndef __U_UART_FUNC
#define __U_UART_FUNC
#include "stdint.h"
#define 	UART					MDR_UART3
#define   UART_IRQn					UART3_IRQn
void m_UART_Init (uint32_t UART_Speed);
#endif // __U_UART_FUNC