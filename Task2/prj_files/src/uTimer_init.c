#include "uTimer_init.h" 
#include "MDR32F9Qx_timer.h"
/* m_setUpTimer Description.
	This function set up required time of Timer Handler
	function name: m_setUpTimer
	input: req_Ttime (seconds) - required time (period) of Handler, (MHz) - requred frequency of CPU
	input type of values: uint8_t, uint8_t
	input values: (0;255) seconds,  [8;80] MHz (with increment(step) 8 MHz);
	output: nothing.
*/

void m_setUpTimer(uint8_t req_time, uint8_t req_CPUfreq) //Благодаров 141 стр.
{
	TIMER_CntInitTypeDef TIM_CntInit;
	TIMER_DeInit (TIMER);
	MDR_RST_CLK->TIM_CLOCK = RST_CLK_TIM_CLOCK_TIM1_CLK_EN; //Timer clock enable
	//RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	TIMER_BRGInit (TIMER, TIMER_HCLKdiv1);
	/* About Timer_Prescaler
	We need to set up Timer's counter in ms. To find the  
	prescaler we need solve simply equastion:
	req_CPUfreq (Hz)/TIMER_Prescaler = 1000 ms = 1 s
	Then TIMER_Prescaler = req_CPUfreq[Hz]*1000,
	but TIMER_Prescaler max value 65535 (uint16_t), then 
	we multiply only to 500, and then our maximum is value here is 40000
	then TIMER_Prescaler = req_CPUfreq[Hz]*1000/2
	*/
	TIM_CntInit.TIMER_Prescaler = (req_CPUfreq*500);
	TIM_CntInit.TIMER_Period = req_time*500; // due to input in seconds, but TIMER_Period in ms and devide, because need half of period
	TIMER_CntInit(TIMER, &TIM_CntInit);
	NVIC_EnableIRQ(TIMER_IRQ);
	NVIC_SetPriority(TIMER_IRQ, 0);
	TIMER_ITConfig(TIMER, TIMER_STATUS_CNT_ZERO, ENABLE);
	TIMER_Cmd(TIMER, ENABLE);
}

