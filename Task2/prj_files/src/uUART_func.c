#include "uUART_func.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_uart.h"
void m_UART_Init (uint32_t UART_Speed)
{
	// Structure for configure input/output port
  PORT_InitTypeDef PortInitStructure; 
	// Structure for configure UART
  UART_InitTypeDef UARTInitStructure; 
	
  // Enable clock to UART3 and corresponding port. Don't need here, because 
	// 		clock all ports in CLK lib (uRST_CLK_init.h)
  //RST_CLK_PCLKcmd (RST_CLK_PCLK_UART3 | RST_CLK_PCLK_PORTF, ENABLE);
	
  // Configure outputs UART3
	PORT_StructInit (&PortInitStructure);
  PortInitStructure.PORT_PULL_UP 		= PORT_PULL_UP_OFF;
  PortInitStructure.PORT_PULL_DOWN 	= PORT_PULL_DOWN_OFF;
  PortInitStructure.PORT_PD_SHM 		= PORT_PD_SHM_OFF;
  PortInitStructure.PORT_PD 				= PORT_PD_DRIVER;
  PortInitStructure.PORT_GFEN 			= PORT_GFEN_OFF;
  PortInitStructure.PORT_FUNC 			= UART_PORT_FuncRX;
  PortInitStructure.PORT_SPEED 			= PORT_SPEED_MAXFAST;
  PortInitStructure.PORT_MODE 			= PORT_MODE_DIGITAL;

  // PF0 (UART3_RX) - input
  PortInitStructure.PORT_OE 				= PORT_OE_IN;
  PortInitStructure.PORT_Pin 				= UART_PORT_PinRX;
  PORT_Init (UART_PORT_TX, &PortInitStructure);
  
  // PF1 (UART3_TX) - output
  PortInitStructure.PORT_OE 				= PORT_OE_OUT;
  PortInitStructure.PORT_Pin 				= UART_PORT_PinTX;
  PORT_Init (UART_PORT_RX, &PortInitStructure);	

  // Divider of frequency 
  UART_BRGInit (UART, UART_HCLKdiv1);

  // Deinint before configure
  UART_DeInit (UART);

  // Configure UART
  UARTInitStructure.UART_BaudRate             = UART_Speed;             // Speed: 
  UARTInitStructure.UART_WordLength           = UART_WordLength8b;   		// Number of bits in data: 8
  UARTInitStructure.UART_StopBits             = UART_StopBits1;      		// Number of stopBits: 2
  UARTInitStructure.UART_Parity               = UART_Parity_No;      		// on/off Parity check
  UARTInitStructure.UART_FIFOMode             = UART_FIFO_OFF;       		// on/off FIFO buffer
  UARTInitStructure.UART_HardwareFlowControl  = UART_HardwareFlowControl_RXE | // Hardware flow control for RX and TX
	                                              UART_HardwareFlowControl_TXE;    
  UART_Init(UART, &UARTInitStructure);

//  // Enable IRQ by receiving symbol
//  UART_ITConfig (UART, UART_IT_RX, ENABLE);
//  
//  // Priority is higher then RX
//  NVIC_SetPriority (UART_IRQn, 0x01); 	  

//  // enable IRQ
//  NVIC_EnableIRQ (UART_IRQn); 
  // Enable working of UART
  UART_Cmd (UART, ENABLE);		
}

void m_UartSetBaud(uint32_t baudRate, uint32_t freqCPU)
{
	uint32_t divider = freqCPU / (baudRate >> 2);
  uint32_t CR_tmp = UART->CR;
  uint32_t LCR_tmp = UART->LCR_H;

  //  Так сделано в Bootloader - сбоит!
  //  while ( !(UART_X->FR & UART_FLAG_TXFE) ); // wait FIFO empty

  //  Так работает!
  while ( (UART->FR & UART_FLAG_BUSY) ); // wait BUSY

  UART->CR = 0;
  UART->IBRD = divider >> 6;
  UART->FBRD = divider & 0x003F;
  UART->LCR_H = LCR_tmp;
  UART->CR = CR_tmp;
}

	