/* File decription 
	***********************************************************************************
// CPU: Milandr 1901ВЦ1Т
// Device: Development board Milandr 1901ВЦ1Т
// File: uLCD_func.c 
// Function/Option:  Set up for LCD screen MT–12864J 
//              due to driver "Ангстрем" К145ВГ10 (analog Samsung KS0108)  
  *********************************************************************************** */

#include "uLCD_func.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
// Строка, выводимая на индикатор при инициализации
static const char* init_string = "   \xC7\xC0\xCE\"\xCC\xE8\xEB\xE0\xED\xE4\xF0\"";  // ЗАО "Миландр"

// Изображение, выводимое на индикатор при инициализации (Эмблема "Миландра")
static const uint8_t init_image[32] = 
{
	254, 254, 254, 254, 254, 126,  30,   6,
		6,  30, 126, 254, 254, 254, 254, 254,
	127,  95,  71,  65, 124, 120, 112,  96,
	 96, 112, 120, 124,  65,  71,  127,  127
};


// Инициализация ЖКИ
void m_MLT_Init (void)
{
  // Разрешить тактирование требуемых портов
  RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTC, ENABLE);	
	
	// Инициализировать выводы МК для работы с ЖКИ
	m_MLT_Pin_Cfg ();
	
	// Инициализировать драйвер ЖКИ 
	m_MLT_LCD_Init();
	

	// Дождаться завершения операции с дисплеем 1
	while (m_MLT_Read_Status(1) & 0x80);
	// Включить дисплей 1  
	m_MLT_Disp_On (1);
	
	// Дожаться завершения операции с дисплеем 2
	while (m_MLT_Read_Status(2) & 0x80);
	// Включить дисплей 2 
	m_MLT_Disp_On (2);

	// Очистить дисплей 1 
	m_MLT_Clear_Chip (1);
	// Очистить дисплей 2
	m_MLT_Clear_Chip (2);	
	
	// Вывести начальное сообщение и рисунок
  m_MLT_Put_String (init_string, 1);	
	m_MLT_Put_Image (init_image, 0, 0, 1, 1);

}


// Инициализация выводов МК для работы с ЖКИ
void m_MLT_Pin_Cfg (void)
{
	// Структура данных для инициализации портов
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


// Инициализация драйвера ЖКИ
void m_MLT_LCD_Init(void)
{
	uint8_t s1,s2;
	// Сформировать сигнал RESET
	m_MLT_Clr_Res_Pin;
	m_MLT_Delay(200);
	
	m_MLT_Set_Res_Pin;
	m_MLT_Delay(5);

	// Дождаться окончания инициализации
	do
	{
		s1 = m_MLT_Read_Status(1) & 0x90;
		s2 = m_MLT_Read_Status(2) & 0x90;
	}
	while (s1 != 0x00 && s2 != 0x00);
}

// Выставить данные на шину данных
void m_MLT_Set_Data_Bits (uint8_t value)
{
	MDR_PORTA->RXTX &= 0xFF00;
	MDR_PORTA->RXTX |= value; 
}

// Задержка 
void m_MLT_Delay (uint32_t value)
{
	volatile uint32_t x = value;
	
	while ((x--) != 0);
}

// Получить статус операции
uint8_t m_MLT_Read_Status (uint8_t Chip)
{
  uint8_t data;

	// Настроить шину данных на прием данных
	m_MLT_Data_Dir_Input_PA;
	
	
	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	// Сбросить вывод А0
	m_MLT_Clr_A0_Pin;

	// Установить вывод RW (прием данных)
  m_MLT_Set_RW_Pin;
	m_MLT_Delay(5);
	
	// Установить вывод E
	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);
	
	// Прочитать данные
	data = m_MLT_Output_Data;
	m_MLT_Delay(5);
	
	// Сбросить вывод E
	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);
	
	// Сбросить вывод A0
	m_MLT_Clr_A0_Pin;

	// Сбросить вывод RW
	m_MLT_Clr_RW_Pin;

	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }

	return data;
}

// Включить дисплей
void m_MLT_Disp_On (uint8_t Chip)
{
	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
	else 
  {
    m_MLT_Set_E2_Pin;
  }

	// Сбросить вывод А0
	m_MLT_Clr_A0_Pin;

	// Сбросить вывод RW (передача данных)
	m_MLT_Clr_RW_Pin;
	m_MLT_Delay(5);
	
	m_MLT_Data_Dir_Output_PA;
	
	
	// Выдать данные на шину
	m_MLT_Set_Data_Bits (0x3F);
	m_MLT_Delay(5);

	m_MLT_Set_Stb_Pin;
	m_MLT_Delay(5);

	m_MLT_Clr_Stb_Pin;
	m_MLT_Delay(5);

  m_MLT_Clr_A0_Pin;

	m_MLT_Clr_RW_Pin;


	m_MLT_Data_Dir_Input_PA;
	 

	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Включить и выключить дисплей
void m_MLT_Disp_Off (uint8_t Chip)
{
	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
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

	
	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Задать текущую страницу
void m_MLT_Set_Page (uint8_t Chip, uint8_t page)
{
	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
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

	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Задать текущий адрес
void m_MLT_Set_Address (uint8_t Chip, uint8_t address)
{
	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
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

	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Записать данные в ЖКИ
void m_MLT_Write_Data (uint8_t Chip, uint8_t data)
{
	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
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

	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }
}

// Прочитать данные с ЖКИ
uint8_t m_MLT_Read_Data (uint8_t Chip)
{
  uint8_t data = 0;

	// Если выбран дисплей 1, то установить вывод Е1 (дисплей 1 выбран)
	if (Chip == 1) 
	{
    m_MLT_Set_E1_Pin;
  }
	// Если выбран дисплей 2, то установить вывод Е2 (дисплей 2 выбран)
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

	// Если выбран дисплей 1, то сбросить вывод Е1 (дисплей 1 НЕ выбран)
	if (Chip == 1) 
	{
    m_MLT_Clr_E1_Pin;
  }
	// Если выбран дисплей 2, то сбросить вывод Е2 (дисплей 1 НЕ выбран)
	else 
  {
    m_MLT_Clr_E2_Pin;
  }

	return (data);
}

// Очистить страницу
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

// Очистить дисплей
void m_MLT_Clear_Chip (uint8_t Chip)
{
  int32_t i;

	for (i = 0; i < 8; i++)
		m_MLT_Clear_Page (Chip, i);
}

// Вывести символ
void m_MLT_Put_Char (uint8_t symbol, int32_t X, int32_t Y)
{
  int32_t chp, page, adr;
  int32_t i;
	int32_t symbol_image_index;  // Индекс элемента в массиве шрифта, с которого начинается изображение данного символа.

	chp  = (int32_t)(X / 8) + 1;
	page = Y;
	adr  = (int32_t)((X % 8) * 8);	

	m_MLT_Set_Page (chp, page);
	m_MLT_Set_Address (chp, adr);
	
	// Проверить, чтобы символ не выходил за диапазон, определенный в шрифте.
	if (symbol >= MLT_FONT_MIN_CODE && symbol <= MLT_FONT_MAX_CODE)
	{
	  symbol_image_index = (((uint32_t) symbol) - MLT_FONT_MIN_CODE) * 8;
	}
	// Символ вне диапазона
	else
	{
	  symbol_image_index = 0;  // Выводим самый первый символ, как правило,- пробел.
  }
	
	for (i = symbol_image_index; i < symbol_image_index + 8; i++)
	{	
		while (m_MLT_Read_Status (chp) & 0x80);
		
		m_MLT_Write_Data (chp, MLT_Font[i]);
	}

}

// Вывести строку
void m_MLT_Put_String (const char* str, int32_t Y)
{
  int32_t i;

	for (i = 0; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
	{
	  // Достигнут конец строки
		if (!str[i])
      break;

    m_MLT_Put_Char (str[i], i, Y); // Вывести символ
  }
	
	for (; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
	{
 	  m_MLT_Put_Char (32, i, Y);  // Вывести пробел
  }
}

// Прокрутить строку на cnt шагов и вывести ее
void m_MLT_Scroll_String (const char* str, int32_t Y, int32_t cnt)
{
  int32_t i, j, L;
	char c;
	char s[m_MLT_SCREEN_WIDTH_SYMBOLS + 1]; // Строка размером с ЖКИ, которую получим в результате прокрутки.
  
	L = strlen(str); // Длина исходной строки
	s[m_MLT_SCREEN_WIDTH_SYMBOLS] = 0;      // Получаемую строку надо закрыть финишным нулем
	
	// Если исходная строка пустая 
	if (!L)
		return; 

  // Если пытаемся несколько раз обойти вокруг Земли :-)	
	cnt %= m_MLT_SCREEN_WIDTH_SYMBOLS;

	// Если исходная строка короче экрана
	if (L < m_MLT_SCREEN_WIDTH_SYMBOLS)
		for (i = 0; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
		{
			j = i + cnt;

			if (j >= m_MLT_SCREEN_WIDTH_SYMBOLS)
				j %= m_MLT_SCREEN_WIDTH_SYMBOLS;
			
			// Если исходная строка еще не кончилась
			if (j < L)  			
				c = str[j];
			// Если исходная строка уже кончилась
			else
				c = 32; // Пробел
			
			s[i] = c;
		}
	// Если исходная строка длиннее экрана
	else
		for (i = 0; i < m_MLT_SCREEN_WIDTH_SYMBOLS; i++)
		{
			j = i + cnt;

			if (j >= L)
				j %= L;
			
      s[i] = str[j];
		}
	
	// Вывести прокрученную строку на ЖКИ	
	m_MLT_Put_String (s, Y);
}


// Вывести изображение
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


