/***************************************************************************************************************************
* This file is subject to the terms and conditions defined in file 'LICENSE.txt', 
* which is part of this source code package. 
*
* Project									: Setting the HSE as the system clock - switching clock sources
* Program Name						: STM32F407_HSE_as_SYSCLK
* Author									: vkaiser
* Date Created						: Oct-09-2019
* 
* Purpose									: Learning about the clocks in the Discovery board
*
*														
* MCU											: STM32F407VGT6
* Language								: C
* Hardware Modifications	: N/A
* Debugger								: ST-Link Debugger (on-board)
*
* Repo / Revision History	: https://github.com/vlkaiser/STM32F407_Disc_Test_1
*
* - Special Setup -
* Keil Pack Installer 		: Device Specific DFP
*													:
*		
* Revision History				:
* 	Date				Author			Notes
* 						vkaiser			- Initial commit
*
***************************************************************************************************************************/
#include <stdint.h>	
#include <stm32f407xx.h>		//Device-specific
/**************************************************************************************************************************/
/* Functions */

/**************************************************************************************************************************/
	

/* Main */
int main(void)
{
	//Initialize the RCC and GPIO
	RCC_TypeDef *pRCC;
	GPIO_TypeDef *pGPIO;
	
	pRCC = RCC;
	pGPIO = GPIOC;
	
	//Turn on the HSE
	pRCC->CR |= (1 << 16);
	
	//Wait for the HSE to stabilize by polling HSE READY
	while( !(pRCC->CR & (1 << 17) ) );
	
	//Set the HSE as the System Clock
	pRCC->CFGR &= ~(0x3 << 0);		//Clear the HSE System Clock ON registers
	pRCC->CFGR |= (0x01 << 0);		//Set the HSE as the system clock
	
	//Set MCO2 as the clock output
	//pRCC->CFGR |= (1 << 27);			//Set MCO2 clock output Prescaling
	//pRCC->CFGR |= (1 << 28);
	pRCC->CFGR |= ~(1 << 29);
	
	pRCC->CFGR &= ~(1U << 30);		//Clear MCO2 clock output registers
	pRCC->CFGR &= ~(1U << 31);
	pRCC->CFGR |= (1U << 31);			//Set MCO2 clock output to HSE (10)
	
	pRCC->AHB1ENR = pRCC->AHB1ENR | (1 << 2);		//Enable GPIOC clock - AHB1
	
	pGPIO->MODER &= ~(1 << 18);		//Clear GPIOC (PC9) Alternate Function Registers
	pGPIO->MODER &= ~(1 << 19);
	pGPIO->MODER |= (1 << 19);		//Set GPIOC Alternate Function Register
	
	//Remember to 'reset' the board after loading the code
	//Remember to set the Target Option Debug to ST_Link Debugger
	
	return 0;
}
