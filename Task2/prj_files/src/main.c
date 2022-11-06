/* Program information.
 Header: Homework 2. 
 1) Set up UART and timer handler.
 2) Set up LCD and timer handler.
 File Name: main.c. Main file of project  
 Author: Zharkikh Sergei
 Date: 05.11.2022 
*/
#include "uLCD_func.h"
#include "uUART_func.h"
#include "uRST_CLK_init.h"
#include "uTimer_init.h"
#include "uLED_fun.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_uart.h"
#include "string.h"
//#include "stdio.h"
//#include "retarget_uart.h" // tried to debug with UART

//----------------- INPUT parameters -----------------
#define UART_SPEED		115200 			 // bps
#define FREQ_CPU			  80    			 // MHz
#define TIMER_PERIOD		5			  	 // S
unsigned char Data_receive[4] = {0,0,0,0}; // package for receiving data from UART 
uint32_t counter_handler = 0;

int main()
{
//	while(UART_GetFlagStatus(UART,UART_FLAG_RXFE)==0)
//	{
//		Data_receive[3] = (unsigned char)UART_ReceiveData(UART);
//	}
	char adcString [15];
	m_setUpFreq(FREQ_CPU);
	m_initLEDs();
	m_MLT_Init();
	m_setUpTimer(TIMER_PERIOD, FREQ_CPU);
	m_UART_Init(UART_SPEED);
	
	
	m_MLT_Put_String ("Жарких Сергей",0);
	m_MLT_Put_String ("Андреевич",1);
	m_MLT_Put_String ("РЛ1-113",2);
	sprintf(adcString,"CPU Freq %d MHz",FREQ_CPU); // convert number to string 
	m_MLT_Put_String(adcString,4);
	sprintf(adcString,"UART %d bps",UART_SPEED); // convert number to string 
	m_MLT_Put_String(adcString,6);

	while(1)
	{
		sprintf(adcString,"Handler %d",counter_handler);
		m_MLT_Put_String(adcString,5);
	}
}
/* TIMER1_IRQHandler Description.
	Timer Handler
	function name: TIMER1_IRQHandler
	input: nothing
	output: nothing
	When Timer 1 is overflow and reset to zero value, we start this function,
	toggle LED (on/off).
*/
void TIMER1_IRQHandler (void)
{
	if (TIMER_GetITStatus(MDR_TIMER1, TIMER_STATUS_CNT_ZERO))
	{
		m_LED_toggle(PORT_Pin_15);
		counter_handler+=1;
		TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ZERO);
		//UART_SendData(UART, (uint8_t)FREQ_CPU);
		//printf("MHz");
	}
}

//// Try to do change speed by UART
//void UART_Handler_RX_TX(void)
//{
//}
//void UART3_IRQHandler (void)
//{
//	UART_Handler_RX_TX();
//}