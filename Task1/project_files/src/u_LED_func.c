#include "uLED_fun.h"

/* init LED port structure.
	This actions are necessary for working with LED's Port.
	PIN_LEDs, PORT_LEDs initialized in corresponding header.
*/
void m_initLEDs (void)
{
	PORT_InitTypeDef PortInitStructure;
	PORT_StructInit (&PortInitStructure);	
  PortInitStructure.PORT_Pin   = PIN_LEDs;
  PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
  PortInitStructure.PORT_OE    = PORT_OE_OUT;
	PortInitStructure.PORT_SPEED = PORT_SPEED_SLOW;
  PORT_Init (PORT_LEDs, &PortInitStructure);	
}
/* LED or LEDs off. 
Input: you can use chain of numbers pins. 
(Ex. PORT_Pin_15|PORT_Pin_14 etc.). */
void m_LED_off (uint32_t Pins)
{
	PORT_ResetBits (PORT_LEDs, Pins);
}

// LED or LEDs on
void m_LED_on (uint32_t Pins)
{
	PORT_SetBits (PORT_LEDs, Pins);
}

// Change the current state of LED or LEDs
void m_LED_toggle (uint32_t Pins)
{
	uint32_t data;
	data	= PORT_ReadInputData (PORT_LEDs);
	PORT_Write (PORT_LEDs, data ^= Pins);
}