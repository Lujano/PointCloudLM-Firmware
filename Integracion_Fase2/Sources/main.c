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
#include "TI1.h"
#include "AS2.h"
#include "Bit1.h"
#include "Bit2.h"
#include "Cap1.h"
#include "Bit3.h"
#include "AD1.h"
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
unsigned char estado = MOTOR ;

// Variables Echo
unsigned char estado_trigger = TRIGGER_TERMINADO;
unsigned char estado_echo = ECHO_TERMINADO;
unsigned int medicion = 0;

// Variables COMM
unsigned char CodError;
unsigned int Enviados = 11;		// Esta variable no aporta nada más sino el número de elementos del arreglo a enviar.
unsigned int error;
bool primero = FALSE;


unsigned char Trama_PC[11]={0xf5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};; 	// Esta es una primera trama que yo hice de ejemplo.
// Variable ADC
unsigned int ADC16;

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
unsigned char theta_90 = 131;
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
  			if (dir == 0){
  				
  				if(step1 > phi_180){
  				  	step1 = step1-4; // Comienza en phi_0 (mayor) y disminuye hasta phi_180
  				 }
				if (step1 <= phi_180){ // Dio la vuelta
					dir = 1; // Cambiar de direccion
					step1 = phi_180;
					band = 1;
				}
  			}
  			else{
  				 if(step1 < phi_0){
					step1 = step1+4; // Comienza en phi_180 (menor) y aumenta hasta phi_0
				 }
				if (step1 >= phi_0){ // Dio la vuelta
					step1 = phi_0;
					dir = 0; // Cambiar de direccion
					step1 = phi_0;
					band = 1;
				}
  			}
  			
  			
  			servo_send(1, step1);
  			delay_ms(250);
  			
  			if(reset_band == 1){
  			  				estado = ESPERAR;
  			 }
  			else {
  				           estado = MEDIR;
  			}
  			
  			if(band == 1){
				if (step2 < theta_0){ // Comienza en theta_90(menor)
					step2 = step2+4;
				}
				band = 0; // Reiniciar
				if(step2 >= theta_0){
					step2 = theta_0;
					reset_band = 1;
				}
				servo_send(2, step2);
				delay_ms(100);
			}
  			
  			
  			
  			break;
  			
  			
  		case MEDIR:
  			estado_echo = ECHO_TRIGGERED;
  			trigger();
  			while(estado_echo!= ECHO_TERMINADO){
  				
  				
  			}
  			CodError = AD1_Enable();
  			// Otras mediciones
  			CodError = AD1_Measure(TRUE);
			CodError = AD1_GetValue16(&ADC16);
			CodError = AD1_Disable();
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
			//Trama_PC[1] = DIG2 & 0xff;//(ADC16 >> 11) & (0x1F);
			//Trama_PC[1] = Trama_PC[1] + ((ADC16 >> 11) & (0x1F));
			Trama_PC[1] =  ((ADC16 >> 11) & (0x1F));
			Trama_PC[2] = (ADC16 >> 4) & (0x7F);
			Trama_PC[3] = (medicion >> 15) & 0x01; // bit mas significativo upper
			Trama_PC[4] = (medicion >> 8) & 0x7F;  // bits restantes upper
			Trama_PC[5] = (medicion >> 7) & 0x01;  // bit mas significativo lower
			Trama_PC[6] = (medicion) & 0x7F;  // bits restantes lower
			Trama_PC[7] = (step1 >> 7) & 0x01;  // bit mas significativo upper
			Trama_PC[8] = (step1) & 0x7F;  // bits restantes
			Trama_PC[9] = (step2 >> 7) & 0x01;  // bit mas significativo upper
			Trama_PC[10] = (step2) & 0x7F;  // bits restantes
			CodError = AS1_SendBlock(Trama_PC,11,&Enviados); //El arreglo con la medición está en iADC.u8 (notar que es un apuntador)
			
			estado = MOTOR;
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
	step1 = phi_0;
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
