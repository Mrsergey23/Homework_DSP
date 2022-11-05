#include "uLED_fun.h"

void m_initLEDs (void)
{
	PORT_StructInit (&PortInitStructure);	
  PortInitStructure.PORT_Pin   = PIN_LEDs;
  PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
  PortInitStructure.PORT_OE    = PORT_OE_OUT;
	PortInitStructure.PORT_SPEED = PORT_SPEED_SLOW;
  PORT_Init (PORT_LEDs, &PortInitStructure);	
}
// Потушить указанные светодиоды
void m_LED_off (uint32_t Pins)
{
	PORT_ResetBits (PORT_LEDs, Pins);
}

// Зажечь указанные светодиоды
void m_LED_on (uint32_t Pins)
{
	PORT_SetBits (PORT_LEDs, Pins);
}

// Переключить указанные светодиоды
void m_LED_toggle (uint32_t Pins)
{
	uint32_t data;
	data	= PORT_ReadInputData (PORT_LEDs);
	PORT_Write (PORT_LEDs, data ^= Pins);
}