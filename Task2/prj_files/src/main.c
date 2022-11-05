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
int main()
{
	m_UART_Init(115200);
	m_MLT_Init();
	while(1)
	{
		
	}
}