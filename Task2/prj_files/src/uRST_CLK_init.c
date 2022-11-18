#include "uRST_CLK_init.h"
#include "MDR32F9Qx_rst_clk.h"
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
void m_setUpFreq(uint8_t req_CPUfreq)
{
	//assert_param(req_CPUfreq %8 == 0);
		// Switch on all ports
	MDR_RST_CLK->PER_CLOCK = 0xFFFFFFFF;
	MDR_RST_CLK->HS_CONTROL = RST_CLK_HS_CONTROL_HSE_ON;                  									// HSE_ON = 8MHz now
	while (MDR_RST_CLK->CLOCK_STATUS == 0x00) __NOP();          														// Wait the HSE is ready working
		/* Setting up source and coefficient of multiplier PLL	
		first part of this (before |) is decription of coefficient, that depends of req_freq 
		look register MDR_RST_CLK->PLL_CONTROL. 8 MHz(quartz)*(PLL_CPU_MUL+1) = req_freq */	
	MDR_RST_CLK->PLL_CONTROL |= (((req_CPUfreq/8-1)<<8)|RST_CLK_PLL_CONTROL_PLL_CPU_ON) ; 
	while ( (MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) == 0);           // Wait for start of PLL_CPU
	MDR_EEPROM->CMD = 0x0018;                                     													// Delay 3 cycle
		//RST_CLK->CPU_CLOCK = 0x0006;                           															// HSI
	MDR_RST_CLK->CPU_CLOCK = 2|1<<2|0<<7|1<<8;  // look MDR_RST_CLK->CPU_CLOCK and look at image in project folder ("Diagram of Clock settings")
}

