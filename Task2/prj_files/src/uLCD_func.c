/* File decription 
	***********************************************************************************
// CPU: Milandr 1901ÂÖ1Ò
// Device: Development board Milandr 1901ÂÖ1Ò
// File: uLCD_func.c 
// Function/Option:  Set up for LCD screen MT–12864J 
//              due to driver "Angstrem" Ê145ÂÃ10 (analog Samsung KS0108)  
  *********************************************************************************** */
// look datasheet MT–12864J 
#include "uLCD_func.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
// Iniitial string
static const char* init_string = "   \xC7\xC0\xCE\"\xCC\xE8\xEB\xE0\xED\xE4\xF0\"";  // "Milandr" in Russian

// Initial image, like example ("Milandr" logo)
static const uint8_t init_image[32] = 
{
	254, 254, 254, 254, 254, 126,  30,   6,
		6,  30, 126, 254, 254, 254, 254, 254,
	127,  95,  71,  65, 124, 120, 112,  96,
	 96, 112, 120, 124,  65,  71,  127,  127
};

// Init MLT screen for working
void m_MLT_Init (void)
{
  // Enable clock to required Ports, don't need, because init clock all Ports in CLK library
  //RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTC, ENABLE);	
	
	// Init screen pins 
	m_MLT_Pin_Cfg ();
	
	// Init screen driver 
	m_MLT_LCD_Init();
	

	// Delay finishing operations with 1st display
	while (m_MLT_Read_Status(1) & 0x80);
	// Turn on display 1   
	m_MLT_Disp_On (1);
	
	// Delay finishing operations with 2st display
	while (m_MLT_Read_Status(2) & 0x80);
	// Turn on display 2
	m_MLT_Disp_On (2);

	// Clear all display 1 
	m_MLT_Clear_Chip (1);
	// Clear all display 2
	m_MLT_Clear_Chip (2);	
	
	// Here we can show initial image and string
  //m_MLT_Put_String (init_string, 1);	
	//m_MLT_Put_Image (init_image, 0, 0, 1, 1);

}


// Configure screen pins
void m_MLT_Pin_Cfg (void)
{
	// Structure for configure port 
	PORT_InitTypeDef PortInit;
	
		//---------   IN Pins -----------
	/* Fill PortInit structure*/
	PortInit.PORT_PULL_UP 			= PORT_PULL_UP_OFF;
	PortInit.PORT_PULL_DOWN 		= PORT_PULL_DOWN_OFF;
	PortInit.PORT_PD_SHM 				= PORT_PD_SHM_OFF;
	PortInit.PORT_PD 						= PORT_PD_DRIVER;
	PortInit.PORT_GFEN 					= PORT_GFEN_OFF;
	
	PortInit.PORT_FUNC  				= PORT_FUNC_PORT;
	PortInit.PORT_MODE  				= PORT_MODE_DIGITAL;
	PortInit.PORT_SPEED 				= PORT_SPEED_SLOW;
	/* Configure PORTA pins 0..7 for mlt input data  */
	PortInit.PORT_OE    				= PORT_OE_IN;
	PortInit.PORT_Pin   			= (PORT_Pin_0| PORT_Pin_1 | \
												PORT_Pin_2 | PORT_Pin_3 | \
												PORT_Pin_4 | PORT_Pin_5 | \
												PORT_Pin_6 |PORT_Pin_7);
 	PORT_Init(MDR_PORTA, &PortInit);
	
	//---------   OUT Pins -----------
	PortInit.PORT_OE = PORT_OE_OUT;
	
	PortInit.PORT_Pin = (PORT_Pin_4 |PORT_Pin_5 | PORT_Pin_11 );
	PORT_Init(MDR_PORTE, &PortInit);
	
	PortInit.PORT_Pin = (PORT_Pin_9);
	PORT_Init(MDR_PORTD, &PortInit);
	
	PortInit.PORT_Pin = (PORT_Pin_2| PORT_Pin_7 );
	PORT_Init(MDR_PORTC, &PortInit);
}


// Screen driver init
void m_MLT_LCD_Init(void)
{
	uint8_t s1,s2;
	// Reset signal 
	m_MLT_Clr_Res_Pin;
	m_MLT_Delay(200);
	
	m_MLT_Set_Res_Pin;
	m_MLT_Delay(5);

	// Wait finishing init
	do
	{
		s1 = m_MLT_Read_Status(1) & 0x90;
		s2 = m_MLT_Read_Status(2) & 0x90;
	}
	while (s1 != 0x00 && s2 != 0x00);
}

// Put data to data bus 
void m_MLT_Set_Data_Bits (uint8_t value)
{
	MDR_PORTA->RXTX &= 0xFF00;
	MDR_PORTA->RXTX |= value; 
}

// Screen Delay
void m_MLT_Delay (uint32_t value)
{
	volatile uint32_t x = value;
	
	while ((x--) != 0);
}

// Get status of operation
uint8_t m_MLT_Read_Status (uint8_t Chip)
{
  uint8_t data;

	// Configure data bus for receiving 
	m_MLT_Data_Dir_Input_PA;
	
	
	// If we choosed display 1, then set E1
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// If we choosed display 2, then set E2
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	// Reset pin A0
	m_MLT_Clr_A0_Pin;

	// Set OUTPUT RW to transmitting
  m_MLT_Set_RW_Pin;
	m_MLT_Delay(5);
	
	// Set pin E
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	// Read data
	data = m_MLT_Output_Data;
	m_MLT_Delay(5);
	
	// Reset pin E
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	// Reset pin A0
	m_MLT_Clr_A0_Pin;

	// Reset pin RW
	m_MLT_Clr_RW_Pin;

	// If we choosed display 1, then reset E1 
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// If we choosed display 2, then reset E1
	else 
  {
    m_MLT_Clr_E2_Pin;
  }

	return data;
}

// Disp turn on
void m_MLT_Disp_On (uint8_t Chip)
{
	// If we choosed display 1, then set E1
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// If we choosed display 2, then set E2
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	// Reset A0
	m_MLT_Clr_A0_Pin;

	// Reset RW
	m_MLT_Clr_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Output_PA;
	
	
	// Push data to data bus
	m_MLT_Set_Data_Bits (0x3F);
	m_MLT_Delay(5);

	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);

	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);

  m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;


	m_MLT_Data_Dir_Input_PA;
	 
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }

	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Turn off display
void m_MLT_Disp_Off (uint8_t Chip)
{

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }

	else 
  {
    m_MLT_Set_E2_Pin;
  }

	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Output_PA;
	 
	
	m_MLT_Set_Data_Bits (0x3E);
	m_MLT_Delay(5);
	
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;

	m_MLT_Data_Dir_Input_PA;

	
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }

	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Set current Page
void m_MLT_Set_Page (uint8_t Chip, uint8_t page)
{

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }

	else 
  {
    m_MLT_Set_E2_Pin;
  }

	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Output_PA;
	 
	
	m_MLT_Set_Data_Bits (0xB8 | page);
	m_MLT_Delay(5);
	
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Input_PA;

	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;


	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }

	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Set current Adress
void m_MLT_Set_Address (uint8_t Chip, uint8_t address)
{

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Output_PA;
	 
	m_MLT_Set_Data_Bits (0x40 | address);
	m_MLT_Delay(5);
	
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Input_PA;

	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	else 
  {
    m_MLT_Set_E2_Pin;
  }
}

// Write data to Screen
void m_MLT_Write_Data (uint8_t Chip, uint8_t data)
{
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	m_MLT_Set_A0_Pin;

	m_MLT_Clr_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Output_PA;
	 
	
	m_MLT_Set_Data_Bits (data);
	m_MLT_Delay(5);
	
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Input_PA;
	 

	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	else 
  {
    m_MLT_Set_E2_Pin;
  }
}

// Read data from screen
uint8_t m_MLT_Read_Data (uint8_t Chip)
{
  uint8_t data = 0;

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	m_MLT_Set_A0_Pin;

	m_MLT_Set_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Input_PA;
	 
	
	m_MLT_Set_Data_Bits (data);
	m_MLT_Delay(5);
	
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	data = m_MLT_Output_Data;
	m_MLT_Delay(5);
	
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;

	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	return (data);
}

// Clear page
void m_MLT_Clear_Page (uint8_t Chip, uint8_t Page)
{
  int32_t i;

	m_MLT_Set_Page (Chip, Page);

	m_MLT_Set_Address (Chip, 0x00);

	for (i = 0; i < 64; i++)
	{	
		while (m_MLT_Read_Status (Chip) & 0x80);
		
		m_MLT_Write_Data (Chip, 0x00);
	}
}

// Clear display
void m_MLT_Clear_Chip (uint8_t Chip)
{
  int32_t i;

	for (i = 0; i < 8; i++)
		m_MLT_Clear_Page (Chip, i);
}

// Show the Char
void m_MLT_Put_Char (uint8_t symbol, int32_t X, int32_t Y)
{
  int32_t chp, page, adr;
  int32_t i;
	int32_t symbol_image_index;  // index of element from array font, first index of symbols

	chp  = (int32_t)(X / 8) + 1;
	page = Y;
	adr  = (int32_t)((X % 8) * 8);	

	m_MLT_Set_Page (chp, page);
	m_MLT_Set_Address (chp, adr);
	
	// Check, that symbol is not out of boards.
	if (symbol >= MLT_FONT_MIN_CODE && symbol <= MLT_FONT_MAX_CODE)
	{
	  symbol_image_index = (((uint32_t) symbol) - MLT_FONT_MIN_CODE) * 8;
	}
	// If overflow
	else
	{
	  symbol_image_index = 0;  // Show first symbol
  }
	
	for (i = symbol_image_index; i < symbol_image_index + 8; i++)
	{	
		while (m_MLT_Read_Status (chp) & 0x80);
		
		m_MLT_Write_Data (chp, MLT_Font[i]);
	}

}

// Show String
void m_MLT_Put_String (const char* str, int32_t Y)
{
  int32_t i;

	for (i = 0; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
	{
	  // reach the end of string
		if (!str[i])
      break;

    m_MLT_Put_Char (str[i], i, Y); // show char from string
  }
	
	for (; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
	{
 	  m_MLT_Put_Char (32, i, Y);  // put space
  }
}

// Scroll string to cnt steps
void m_MLT_Scroll_String (const char* str, int32_t Y, int32_t cnt)

{
  int32_t i, j, L;
	char c;
	char s[m_MLT_SCREEN_WIDTH_SYMBOLS + 1]; // Size depend on screen size
  
	L = strlen(str); // length of input string
	s[m_MLT_SCREEN_WIDTH_SYMBOLS] = 0;      // Add '\0' character is a string terminating symbol
	
	// If strig is empty
	if (!L)
		return; 

  // if try to do several cycle	
	cnt %= m_MLT_SCREEN_WIDTH_SYMBOLS;

	// If current string is shorter then width screen
	if (L < m_MLT_SCREEN_WIDTH_SYMBOLS)
		for (i = 0; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
		{
			j = i + cnt;

			if (j >= m_MLT_SCREEN_WIDTH_SYMBOLS)
				j %= m_MLT_SCREEN_WIDTH_SYMBOLS;
			
			// If current string doesnt't end
			if (j < L)  			
				c = str[j];
			// If current string have ended yet
			else
				c = 32; // space
			
			s[i] = c;
		}
	// If current string is longer then width screen
	else
		for (i = 0; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
		{
			j = i + cnt;

			if (j >= L)
				j %= L;
			
      s[i] = str[j];
		}
	
	// Show scroll string
	m_MLT_Put_String (s, Y);
}


// Show Image
void m_MLT_Put_Image (const uint8_t* image, int32_t top, int32_t left, int32_t bottom, int32_t right)
{
	int32_t i, j, y;
	int32_t chp, page, adr;
	int32_t cnt = 0;	

	for	(i = top; i <= bottom; i++)
		for (j = left; j <= right; j++)
		{
			chp  = (int32_t)(j / 8) + 1;
			page = i;
			adr  = (int32_t)((j % 8) * 8);

			m_MLT_Set_Page (chp, page);
			m_MLT_Set_Address (chp, adr);
			
			for (y = 0; y < 8; y++)
			{
  		  while (m_MLT_Read_Status (chp) & 0x80);

				m_MLT_Write_Data (chp, image[cnt + y]);
			}
			cnt += 8;
		}	
	
}


