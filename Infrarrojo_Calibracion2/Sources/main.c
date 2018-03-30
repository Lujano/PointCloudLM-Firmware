/* ###################################################################
**     Filename    : main.c
**     Project     : Integracion_Fase1
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-02-03, 08:14, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
** LUJANO-MATA
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module con tains user's application code.
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
#include "TI1.h"
#include "AS2.h"
#include "Bit1.h"
#include "Bit2.h"
#include "Cap1.h"
#include "Bit3.h"
#include "AD1.h"
#include "TI2.h"
#include "Bit4.h"
#include "Bit5.h"
#include "Bit6.h"
#include "Bit7.h"
#include "FC161.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

// Variables Maquina de estados
unsigned char estado = ESPERAR ;

// Variables Echo
unsigned char estado_trigger = TRIGGER_TERMINADO;
unsigned char estado_echo = ECHO_TERMINADO;
unsigned int medicion = 0;

// Variables COMM
unsigned char CodError;
unsigned int Enviados = 5;		// Esta variable no aporta nada más sino el número de elementos del arreglo a enviar.
unsigned int error;
bool primero = FALSE;


unsigned char Trama_PC[5]={0xf2, 0x00, 0x00, 0x00, 0x00}; // Esta es una primera trama 
// Variable ADC
unsigned int ADC16;
unsigned int MADC16;
unsigned int i = 0;
//Variables Bits Digitales
unsigned int DIG1;
unsigned int DIG2;
unsigned int DIG3;
unsigned int DIG4;

// Variables Motores
unsigned char step1 = 0;
unsigned char step2 = 0;
unsigned char phi_0 = 228;
unsigned char phi_180 = 36;
unsigned char theta_0 = 245;
unsigned char theta_90 = 133;
unsigned char theta_min = 100;  // minimo angulo sin que el motor choque con la base
unsigned char dir = 0; // Direccion en aumento de angulo phi (0)
unsigned char mTrama[3]={0xFF,0x00,0x00};
unsigned int EnviadosM = 3;	
unsigned char band = 0; // bandera de direccion cambiada
unsigned char reset_band = 0;


void trigger(void);
void init(void);
void servo_send (unsigned char motor, unsigned char posicion);
void delay_ms (unsigned int time_delay  );
/* User includes (#include below this line is not maintained by Processor Expert) */


void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  init();
  /* Write your code here */
    
  /* For example: for(;;) { } */

  for(;;){

  	switch (estado){
  		
  		case ESPERAR:
  			//servo_send(1, step1 );
  			//servo_send(2, step2);
  			break;
  				
  		case MOTOR:
  			
  			break;
  			
  			
  		case MEDIR:
  			
  			CodError = AD1_Enable();
  			// Otras mediciones
  			ADC16 = 0;
  			MADC16 = 0;
  			estado = MEDIR_ADC;
			
			
			break;
	  			
  		case MEDIR_ADC:
  			for (i = 0; i<16; i++){
  				CodError = AD1_Measure(TRUE);
  				CodError = AD1_GetValue16(&ADC16);
  				MADC16= MADC16+(ADC16>>4);
  				delay_ms(4);
  			}
  			CodError = AD1_Disable();
  			estado = ENVIAR;
  			
  		case ENVIAR:
  			
  			//PROTOCOLO DE COMUNICACION 0,D1,D2,A12,A11,A10,A9,A8 0,A7,A6,A5,A4,A3,A2,A1
			//CANAL 1
  			Trama_PC[1] = (MADC16 >> 15) & 0x01; // bit mas significativo upper
			Trama_PC[2] = (MADC16 >> 8) & 0x7F;  // bits restantes upper
			Trama_PC[3] = (MADC16 >> 7) & 0x01;  // bit mas significativo lower
			Trama_PC[4] = (MADC16) & 0x7F;  // bits restantes lower

			CodError = AS1_SendBlock(Trama_PC,5,&Enviados); 
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



void trigger(void){
	estado_trigger = TRIGGER_BAJO;
	CodError = TI1_Enable();
	while (estado_trigger != TRIGGER_TERMINADO){
		}
	CodError = TI1_Disable();	
}

void init(void){
	Bit1_ClrVal();
	Bit7_ClrVal();
	step1 = phi_0-97;
	step2 = theta_90;
	servo_send(1,step1);
	delay_ms(400);
	servo_send(2, step2);
	dir = 0;
	
};

void servo_send (unsigned char motor, unsigned char posicion){
	mTrama[1] = motor; // Direccion del motor
	mTrama[2] = posicion;	// Posicion a colocar del servo
	CodError=AS2_SendBlock(mTrama, 3, &EnviadosM);
}

void delay_ms (unsigned int time_delay){
	
	unsigned int time;
	CodError = FC161_Reset(); // Resetear contador
	CodError = FC161_GetTimeMS(&time);
	while(time_delay> time){
		CodError = FC161_GetTimeMS(&time);
	}
	
	
}
/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
