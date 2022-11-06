


/* File decription 
	***********************************************************************************
// CPU: Milandr 1901ÂÖ1Ò
// Device: Development board Milandr 1901ÂÖ1Ò
// File: uLCD_func.h 
// Function/Option:  Set up for LCD screen MT–12864J 
//              due to driver "Àíãñòðåì" Ê145ÂÃ10 (analog Samsung KS0108)  
  *********************************************************************************** */

#ifndef __U_LCD_FUNC
#define __U_LCD_FUNC

#include "string.h"
#include "mlt_fonts.h"
#include "stdint.h"
// Screen width in symbols
#define	m_MLT_SCREEN_WIDTH_SYMBOLS	16
// Screen heigth in symbols
#define	m_MLT_SCREEN_HEIGHT_SYMBOLS	4

// Set and Reset "reset" pin
#define	m_MLT_Set_Res_Pin		PORT_SetBits   (MDR_PORTD, PORT_Pin_9);
#define	m_MLT_Clr_Res_Pin		PORT_ResetBits (MDR_PORTD, PORT_Pin_9);

// Set and reset pin E1
#define	m_MLT_Set_Stb_Pin		PORT_SetBits   (MDR_PORTC, PORT_Pin_7);
#define	m_MLT_Clr_Stb_Pin		PORT_ResetBits (MDR_PORTC, PORT_Pin_7);

// Set reset RW pin
#define	m_MLT_Set_RW_Pin		PORT_SetBits   (MDR_PORTC, PORT_Pin_2);
#define	m_MLT_Clr_RW_Pin		PORT_ResetBits (MDR_PORTC, PORT_Pin_2);

// Set and reset pin A0
#define	m_MLT_Set_A0_Pin		PORT_SetBits   (MDR_PORTE, PORT_Pin_11);
#define	m_MLT_Clr_A0_Pin		PORT_ResetBits (MDR_PORTE, PORT_Pin_11);

// Set and reset pin RW
#define	m_MLT_Set_E1_Pin		PORT_SetBits   (MDR_PORTE, PORT_Pin_4);
#define	m_MLT_Clr_E1_Pin		PORT_ResetBits (MDR_PORTE, PORT_Pin_4);

// Set and reset pin E2
#define	m_MLT_Set_E2_Pin		PORT_SetBits   (MDR_PORTE, PORT_Pin_5);
#define	m_MLT_Clr_E2_Pin		PORT_ResetBits (MDR_PORTE, PORT_Pin_5);

// Set direction receiving data from Port A
#define m_MLT_Data_Dir_Input_PA  MDR_PORTA->OE &= 0xFF00;
// Set direction transmitting data from Port A
#define m_MLT_Data_Dir_Output_PA MDR_PORTA->OE |= 0x00FF;


// Output Data
#define m_MLT_Output_Data		(uint8_t)MDR_PORTA->RXTX;

// Init Screen
void m_MLT_Init (void);

// Init screen outputs
void m_MLT_Pin_Cfg (void);
// Init screen driver
void m_MLT_LCD_Init(void);

// Set data to data nus
void m_MLT_Set_Data_Bits (uint8_t value);
// Delay
void m_MLT_Delay (uint32_t value);
// Read the status of operation
uint8_t m_MLT_Read_Status (uint8_t Chip);

// Turn on/off display
void m_MLT_Disp_On (uint8_t Chip);
void m_MLT_Disp_Off (uint8_t Chip);


void m_MLT_Set_Page (uint8_t Chip, uint8_t page);

void m_MLT_Set_Address (uint8_t Chip, uint8_t address);

void m_MLT_Write_Data (uint8_t Chip, uint8_t data);

uint8_t m_MLT_Read_Data (uint8_t Chip);


void m_MLT_Clear_Page (uint8_t Chip, uint8_t Page);

void m_MLT_Clear_Chip (uint8_t Chip);

void m_MLT_Put_Char (uint8_t symbol, int32_t X, int32_t Y);

void m_MLT_Put_String (const char* str, int32_t Y);

void m_MLT_Scroll_String (const char* str, int32_t Y, int32_t cnt);

void m_MLT_Put_Image (const uint8_t* image, int32_t top, int32_t left, int32_t bottom, int32_t right);

#endif // __U_LCD_FUNC