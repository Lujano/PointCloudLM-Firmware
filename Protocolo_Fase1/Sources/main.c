/* ###################################################################
**     Filename    : main.c
**     Project     : Mata
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-01-17, 15:01, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AS1.h"
#include "Bit1.h"
#include "TI1.h"
#include "Bit2.h"
#include "AD1.h"
#include "Bit3.h"
#include "Bit4.h"
#include "Bit5.h"
#include "Bit6.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

unsigned char estado = ESPERAR;
unsigned char CodError;
unsigned int Enviados = 3;		// Esta variable no aporta nada más sino el número de elementos del arreglo a enviar.
unsigned int error;
bool primero = FALSE;
unsigned int periodo;

unsigned int ADC16;
unsigned int DIG1;
unsigned int DIG2;
unsigned int DIG3;
unsigned int DIG4;



unsigned char cTrama[5]={0xF2,0x00,0x00,0x00,0x00}; 	// Esta es una primera trama que yo hice de ejemplo.

void main(void)
{
  /* Write your local variable definition here */

	
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
    
  /* For example: for(;;) { } */

  for(;;){

  	switch (estado){
  		
  		case ESPERAR:
  			break;
  			
  		case MEDIR:
  			CodError = AD1_Measure(TRUE);
  			CodError = AD1_GetValue16(&ADC16);
  			DIG1 = Bit3_GetVal();
  			DIG1 = DIG1 >> 2;
  			DIG2 = Bit4_GetVal();
  			DIG2 = DIG2 >> 3;
  			
  			DIG3 = Bit5_GetVal();
  			DIG3 = DIG3 >> 4;
  			DIG4 = Bit6_GetVal();
  			DIG4 = DIG4 >> 5;
  			estado = ENVIAR;
  			break;
  	
  		case ENVIAR:
  			
	
			//PROTOCOLO DE COMUNICACION 0,D1,D2,A12,A11,A10,A9,A8 0,A7,A6,A5,A4,A3,A2,A1
  			//CANAL 1
  			DIG2 = (DIG1 + (DIG2<<1)) << 5;
  			cTrama[1] = DIG2 & 0xff;//(ADC16 >> 11) & (0x1F);
  			cTrama[1] = cTrama[1] + ((ADC16 >> 11) & (0x1F));
  			cTrama[2] = (ADC16 >> 4) & (0x7F);
  			CodError = AS1_SendBlock(cTrama,3,&Enviados); //El arreglo con la medición está en iADC.u8 (notar que es un apuntador)
  			//CANAL 2
  			DIG4 = (DIG3 + (DIG4<<1)) << 5;
   			cTrama[3] = DIG2 & 0xff;//(ADC16 >> 11) & (0x1F);
   			cTrama[3] = cTrama[3] + ((ADC16 >> 11) & (0x1F));
   			cTrama[4] = (ADC16 >> 4) & (0x7F);
   			CodError = AS1_SendBlock(cTrama,5,&Enviados); //El arreglo con la medición está en iADC.u8 (notar que es un apuntador)
  			estado = ESPERAR;
  			
  			break;
  			
  		default:
  			break;
  	
  	}
  }
  
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.08]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
