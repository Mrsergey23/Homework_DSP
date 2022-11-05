/* Program information.
 Header: Homework 1. 
 1) Set up required frequency of CPU core.
 2) Set up Timer Handler with required time/frequency.
 File Name: main.c. Main file of project  
 Author: Zharkikh Sergei
 Date: 04.11.2022 
*/
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_rst_clk.h"
#include "uLED_fun.h"
/* m_setUpFreq Description.
This function set up required frequency of CPU
function name: m_setUpFreq;
input: req_CPUfreq (MHz) - requred frequency of CPU
input type of values: uint8_t; 
input values: [8;80] MHz (with increment(step) 8 MHz);
output:nothing.
!! To understanding steps below open datasheet at the chapter "Сигналы тактовой частоты".
And use the block diagram.
*/
void m_setUpFreq(uint8_t req_CPUfreq);
void m_setUpFreq(uint8_t req_CPUfreq)
{
	//assert_param(req_CPUfreq %8 == 0);
		// Switch on all ports
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;
	MDR_RST_CLK->HS_CONTROL = RST_CLK_HS_CONTROL_HSE_ON;                  				// HSE_ON = 8MHz now
	while (MDR_RST_CLK->CLOCK_STATUS == 0x00) __NOP();          									// Wait the HSE is ready working
		/* Setting up source and coefficient of multiplier PLL
		first part of this (before |) is decription of coefficient, that depends of req_freq 
		look register MDR_RST_CLK->PLL_CONTROL. 8 MHz(quartz)*(PLL_CPU_MUL+1) = req_freq */	
	MDR_RST_CLK->PLL_CONTROL |= (((req_CPUfreq/8-1)<<8)|RST_CLK_PLL_CONTROL_PLL_CPU_ON) ; 
	while ( (MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) == 0);           // Wait for start of PLL_CPU
	MDR_EEPROM->CMD = 0x0018;                                     // Delay 3 cycle
		//RST_CLK->CPU_CLOCK = 0x0006;                            // HSI
	MDR_RST_CLK->CPU_CLOCK = 3|1<<2|0<<7|1<<8;                 // look MDR_RST_CLK->CPU_CLOCK and look at image in project folder ("Diagram of Clock settings")
}



/* m_setUpTimer Description.
	This function set up required time of Timer Handler
	function name: m_setUpTimer
	input: req_Ttime (seconds) - required time (period) of Handler, (MHz) - requred frequency of CPU
	input type of values: uint8_t, uint8_t
	input values: (0;255) seconds,  [8;80] MHz (with increment(step) 8 MHz);
	output: nothing.
*/
void m_setUpTimer(uint8_t req_time, uint8_t req_CPUfreq);
void m_setUpTimer(uint8_t req_time, uint8_t req_CPUfreq) //Благодаров 141 стр.
{
	TIMER_CntInitTypeDef TIM_CntInit;
	TIMER_DeInit (MDR_TIMER1);
	MDR_RST_CLK->TIM_CLOCK = RST_CLK_TIM_CLOCK_TIM1_CLK_EN; //Timer clock enable
//	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	TIMER_BRGInit (MDR_TIMER1, TIMER_HCLKdiv2);
	/* About Timer_Prescaler
	We need to set up Timer's counter in ms. To find the  
	prescaler we need solve simply equastion:
	req_CPUfreq (Hz)/TIMER_Prescaler = 1000 ms = 1 s
	Then TIMER_Prescaler = req_CPUfreq[Hz]*1000,
	but TIMER_Prescaler max value 65535 (uint16_t), then 
	we use TIMER_HCLKdiv2, for reduce value TIMER_Prescaler,
	then TIMER_Prescaler = req_CPUfreq[Hz]*1000/2
	*/
	TIM_CntInit.TIMER_Prescaler = (req_CPUfreq*500);
	TIM_CntInit.TIMER_Period = req_time*500; // due to input in seconds, but TIMER_Period in ms
	TIMER_CntInit(MDR_TIMER1, &TIM_CntInit);
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_SetPriority(TIMER1_IRQn, 0);
	TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ZERO, ENABLE);
	TIMER_Cmd(MDR_TIMER1, ENABLE);
}


void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

int main()
{
	uint8_t freqCPU_MHz = 80;
	uint8_t timerPeriod = 7 ;
	m_setUpFreq(freqCPU_MHz);
	m_initLEDs();
	m_setUpTimer(timerPeriod, freqCPU_MHz);

	while(1){}
}

//void assert_failed(uint8_t* file, uint32_t line)
//{
//  printf("Wrong parameters value: file %s on line %d\r\n", file, (int)line);

//}

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
		TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ZERO);
	}
	
}