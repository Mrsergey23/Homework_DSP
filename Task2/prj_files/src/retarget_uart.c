#include "MDR32F9Qx_uart.h"
#include <stdio.h>
#include <rt_misc.h>

#if defined (_USE_DEBUG_UART_)

//void DebugUARTInit()
//{
//	UART_InitTypeDef UART_InitStructure;
//	PORT_InitTypeDef PORT_InitStructure;
//	uint32_t BaudRateStatus;
//	
//#if defined (USE_MDR32F1QI)
//	RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTC | RST_CLK_PCLK_UART1), ENABLE);
//#elif defined (USE_MDR32F9Q2I)
//	RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTF | RST_CLK_PCLK_UART2), ENABLE);
//#elif defined (USE_MDR32FG16S1QI)
//	RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTF | RST_CLK_PCLK_UART3), ENABLE);
//#endif

//	/* Инициализация структуры параметров порта ввода/вывода */
//    PORT_StructInit(&PORT_InitStructure);
//    
//	PORT_InitStructure.PORT_Pin = DEBUG_UART_PINS;
//	PORT_InitStructure.PORT_FUNC = DEBUG_UART_PINS_FUNCTION;
//	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
//	PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;

//	PORT_Init(DEBUG_UART_PORT, &PORT_InitStructure);

//	UART_DeInit(DEBUG_UART);

//	/* Инициализация структуры параметров UART */
//	UART_InitStructure.UART_BaudRate            = DEBUG_BAUD_RATE;
//	UART_InitStructure.UART_WordLength          = UART_WordLength8b;
//	UART_InitStructure.UART_StopBits            = UART_StopBits1;
//	UART_InitStructure.UART_Parity              = UART_Parity_No;
//	UART_InitStructure.UART_FIFOMode            = UART_FIFO_ON;
//	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
//											      
//	/* ----- Инициализация UART ----- */
//	UART_BRGInit(DEBUG_UART, UART_HCLKdiv1);	
//	BaudRateStatus = UART_Init(DEBUG_UART, &UART_InitStructure);		
//	if(BaudRateStatus == SUCCESS){
//		UART_Cmd(DEBUG_UART,ENABLE);
//	}
//	else{
//		while(1);
//	}
//	
//	printf("========System startup========\n\r");
//	printf("Init Debug UART ... Ok\r\n");
//}

int sendchar(int c)
{
	  // Ожидать, если буфер TX заполнен
    while (UART_GetFlagStatus(DEBUG_UART, UART_FLAG_TXFF) == SET);
    UART_SendData(DEBUG_UART, (uint8_t) c);
  
    return (c);
}

int getkey ()
{
	// Ожидать, пока не начнётся передача
	while (UART_GetFlagStatus(DEBUG_UART, UART_FLAG_RXFE) == SET);
	return ( UART_ReceiveData(DEBUG_UART) );
}

#endif /* #if defined _USE_DEBUG_UART_ */
