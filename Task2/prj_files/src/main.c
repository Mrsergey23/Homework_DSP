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
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_uart.h"
#include "string.h"
//#include "stdio.h"
//#include "retarget_uart.h" // tried to debug with UART

//----------------- INPUT parameters -----------------
#define UART_SPEED		57600 			 // bps
#define FREQ_CPU			  32    		 // MHz
#define TIMER_PERIOD		0.5			  	 // S
unsigned char Data_receive[4] = {0,0,0,0}; // package for receiving data from UART 
uint32_t counter_handler = 0;
char adcString [15];
volatile char speed_char[10];
volatile uint32_t speed_uart = UART_SPEED;
volatile int i = 0;
//uint32_t cpuclock_check;
int main()
{

	m_setUpFreq(FREQ_CPU);
	m_initLEDs();
	m_MLT_Init();
	m_setUpTimer(TIMER_PERIOD, FREQ_CPU);
	m_UART_Init(UART_SPEED);
// Using this structure we can check current CPU_freq. Uncomment for checking
//	RST_CLK_FreqTypeDef RST_CLK_Clocks;
//	RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
//	cpuclock_check = RST_CLK_Clocks.CPU_CLK_Frequency;
	m_MLT_Put_String ("?????? ??????",0);
	m_MLT_Put_String ("?????????",1);
	m_MLT_Put_String ("??1-113",2);
	sprintf(adcString,"CPU Freq %d MHz",FREQ_CPU); // convert number to string 
	m_MLT_Put_String(adcString,4);
	sprintf(adcString,"UART %d bps",UART_SPEED); // convert number to string 
	m_MLT_Put_String(adcString,7);
//	sprintf(speed_char, "%d", UART_SPEED);
//	size_speed_char = strlen(speed_char);
	NVIC_EnableIRQ(TIMER_IRQ);
	while(1)
	{
		sprintf(adcString,"Handler %2d",counter_handler);
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
		//sprintf(adcString,"%d",FREQ_CPU); // convert number to string 
		UART_SendData(UART, ((UART_SPEED>>16)&0xFF));
		UART_SendData(UART, ((UART_SPEED>>8)&0xFF));
		UART_SendData(UART, (UART_SPEED&0xFF));
	}
}

