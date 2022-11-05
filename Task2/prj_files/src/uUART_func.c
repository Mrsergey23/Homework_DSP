#include "uUART_func.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_uart.h"
void m_UART_Init (uint32_t UART_Speed)
{
	// ��������� ��� ������������� ����� �����-������
  PORT_InitTypeDef PortInitStructure; 
	// ��������� ��� ������������� UART
  UART_InitTypeDef UARTInitStructure; 
	
  // ��������� ������������ UART3 � PORTF
  //RST_CLK_PCLKcmd (RST_CLK_PCLK_UART3 | RST_CLK_PCLK_PORTF, ENABLE);
	
  // ������������ ������� UART3
	PORT_StructInit (&PortInitStructure);
  PortInitStructure.PORT_PULL_UP 		= PORT_PULL_UP_OFF;
  PortInitStructure.PORT_PULL_DOWN 	= PORT_PULL_DOWN_OFF;
  PortInitStructure.PORT_PD_SHM 		= PORT_PD_SHM_OFF;
  PortInitStructure.PORT_PD 				= PORT_PD_DRIVER;
  PortInitStructure.PORT_GFEN 			= PORT_GFEN_OFF;
  PortInitStructure.PORT_FUNC 			= PORT_FUNC_ALTER;
  PortInitStructure.PORT_SPEED 			= PORT_SPEED_MAXFAST;
  PortInitStructure.PORT_MODE 			= PORT_MODE_DIGITAL;

  // PF0 (UART3_RX) - ����
  PortInitStructure.PORT_OE 				= PORT_OE_IN;
  PortInitStructure.PORT_Pin 				= PORT_Pin_0;
  PORT_Init (MDR_PORTF, &PortInitStructure);
  
  // PF1 (UART3_TX) - �����
  PortInitStructure.PORT_OE 				= PORT_OE_OUT;
  PortInitStructure.PORT_Pin 				= PORT_Pin_1;
  PORT_Init (MDR_PORTF, &PortInitStructure);	

  // ������ ����������� ������� ������� ��� UART2 
  UART_BRGInit (UART, UART_HCLKdiv1);

  // ��������������� UART3
  UART_DeInit (UART);

  // ������������ UART 
  UARTInitStructure.UART_BaudRate             = UART_Speed;              // �������� ������: 115200 ���/�
  UARTInitStructure.UART_WordLength           = UART_WordLength8b;   // ���������� ��� � �������: 8
  UARTInitStructure.UART_StopBits             = UART_StopBits2;      // ���������� �������� ���: 2
  UARTInitStructure.UART_Parity               = UART_Parity_No;      // �������� ��������: ����������� 
  UARTInitStructure.UART_FIFOMode             = UART_FIFO_OFF;       // ����� FIFO ��������
  UARTInitStructure.UART_HardwareFlowControl  = UART_HardwareFlowControl_RXE | // ���������� �������� ��������: �������� �������� � ���������� 
	                                              UART_HardwareFlowControl_TXE;    
  UART_Init(UART, &UARTInitStructure);

  // ��������� ���������� �� ������ �������
  UART_ITConfig (UART, UART_IT_RX, ENABLE);
  
  // ������ ��������� ����, ��� � ������������ ����� RTX
  NVIC_SetPriority (UART_IRQn, 0x01); 	  

  // ��������� ���������� �� UART
  NVIC_EnableIRQ (UART_IRQn); 
  // ��������� ������ UART
  UART_Cmd (UART, ENABLE);		

}