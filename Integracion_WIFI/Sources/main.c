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
unsigned int Enviados = 13;		// Esta variable no aporta nada m�s sino el n�mero de elementos del arreglo a enviar.
unsigned int error;
bool primero = FALSE;
unsigned char anuncio;
unsigned char anuncio2;
unsigned char found_band;
unsigned char n_canales;
unsigned char command; // Comando enviado desde pc para cambiar estado del sistema

unsigned char Trama_PC[13]={0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};; 	// Esta es una primera trama que yo hice de ejemplo.


// Variables estado FREERUN

// Variable estado POINTCLOUD

unsigned char Buffer[4] = {0x00, 0x00, 0x00, 0x00};
unsigned int Lectura_Buffer = 2;
unsigned char Trama_end[3]={0xf1,0x00,0x00}; // Trama enviada cuando se termina el proceso de PointCloud
unsigned int env_end = 3;

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
unsigned char step1 ;
unsigned char step2 ;
unsigned char last_step1;
unsigned char last_step2;

const unsigned char phi_0 = 228;
const unsigned char phi_180 = 36;
const unsigned char theta_0 = 245;
const unsigned char theta_90 = 131;
const unsigned char theta_min = 100;  // minimo angulo sin que el motor choque con la base
unsigned char dir = 0; // Direccion en aumento de angulo phi (0)
unsigned char mTrama[3]={0xFF,0x00,0x00};
unsigned int EnviadosM = 3;	
unsigned char band = 0; // bandera de direccion cambiada
unsigned char reset_band = 0;

// Control de motores
unsigned char phi_start;
unsigned char phi_end ;
unsigned char theta_start ;
unsigned char theta_end ;




void trigger(void);
void reset(void);
void servo_send (unsigned char motor, unsigned char posicion);
void delay_ms (unsigned int time_delay  );
/* User includes (#include below this line is not maintained by Processor Expert) */


void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  reset();
  /* Write your code here */
    
  /* For example: for(;;) { } */

  for(;;){

  	switch (estado){
  		
  		case ESPERAR:
  			//servo_send(1, step1 );
  			//servo_send(2, step2);
  			break;
  			
  		case FREERUN:
  			Lectura_Buffer = 2;
  			CodError = AS2_RecvBlock(Buffer, 2, & Lectura_Buffer );
  			step1 =  Buffer[0];
  			step2 =  Buffer[1];
  			//CodError = AS1_ClearRxBuf(); // Limpiar Buffer Rx de forma opcional
  			if (last_step1 != step1){
  				servo_send(1, step1);
  			}
  			 // Mandar angulo phi al motor
  			if(last_step2 != step2){
  				delay_ms(20);
  				servo_send(2, step2); 
  			}
  			// Mandar angulo theta al motor
  			last_step1 = step1;
  			last_step2 = step2;
  			estado = ESPERAR;
  			break;
  		
  		case POINTCLOUD_START:
  			Lectura_Buffer = 4;
			CodError = AS2_RecvBlock(Buffer, 4, & Lectura_Buffer );
			
		    phi_start = Buffer[0]; 
			phi_end = Buffer[1]; 
			theta_start = Buffer[2]; 
			theta_end = Buffer[3];
			
			//CodError = AS1_ClearRxBuf(); // Limpiar Buffer Rx de forma opcional
			
			step1 = phi_start;
			step2 = theta_start;
			servo_send(1,step1);
			delay_ms(400);
			servo_send(2, step2);
			dir = 0;
			if (estado != FREERUN ){
				estado = MOTOR;
			}
  			break;
  			
  		case POINTCLOUD_END:
  			CodError = AS2_SendBlock(Trama_end,3,&env_end);
  			dir = 0;
  			estado = ESPERAR;
  		  	
  		  	break;
  			
  		case MOTOR:
  			if (dir == 0){ // direccion del crecimiento del angulo phi
  				
  				if(step1 > phi_end){
  				  	step1 = step1-1; // Comienza en phi_start (mayor) y disminuye hasta phi_180
  				 }
  				else {
  					if (step1 <= phi_end){ // Dio la vuelta
  				
					dir = 1; // Cambiar de direccion
					step1 = phi_end;
					band = 1;
				}
  				}
  			}
  			else{
  				 if(step1 < phi_start){
					step1 = step1+1; // Comienza en phi_180 (menor) y aumenta hasta phi_0
				 }
  				 else {
  					if (step1 >= phi_start){ // Dio la vuelta
					step1 = phi_start;
					dir = 0; // Cambiar de direccion
					step1 = phi_start;
					band = 1;
				}
  					 }
  			}
  			
  			
  			servo_send(1, step1); // Mandar angulo phi al motor
  			delay_ms(150);
  			
  			
  			if(reset_band == 1){
  			  				estado = POINTCLOUD_END;
  			 }
  			else {
  				if (estado != FREERUN ){
  					 	 estado = MEDIR;
  							}
  				          
  			}
  			
  			if(band == 1){
				if (step2 > theta_end){ // Comienza en theta_0(menor)
					step2 = step2-1;
					band = 0; // Reiniciar
				}
				
		    else { 
					if (step2 <= theta_end) {
					step2 = theta_end;
					reset_band = 1;
				}
				}
				servo_send(2, step2);
				delay_ms(150);
			}
  			
  			
  			
  			break;
  			
  			
  		case MEDIR:
  			Bit2_NegVal();
			 estado_echo = ECHO_TRIGGERED;
			 trigger();
			 
			 CodError = AD1_Enable();
			 ADC16 = 0;
			 MADC16 = 0;
			 for (i = 0; i<16; i++){
				 CodError = AD1_Measure(TRUE);
				 CodError = AD1_GetValue16(&ADC16);
				 MADC16= MADC16+(ADC16>>4);
				 delay_ms(4);
			   			}
			 CodError = AD1_Disable();
			 estado = ENVIAR;
			 if(estado_echo!= ECHO_TERMINADO){
			 estado_echo= ECHO_TERMINADO;
			 medicion = 0;
			 }
  			
			 if (estado != FREERUN ){
				 	 	 	 estado = ENVIAR;
			  }
			
			break;
	  			
  		case ENVIAR:
  			
  			//PROTOCOLO DE COMUNICACION 0,D1,D2,A12,A11,A10,A9,A8 0,A7,A6,A5,A4,A3,A2,A1
			//CANAL 1
			DIG2 = (DIG1 + (DIG2<<1)) << 5;
			//Trama_PC[1] = DIG2 & 0xff;//(ADC16 >> 11) & (0x1F);
			//Trama_PC[1] = Trama_PC[1] + ((ADC16 >> 11) & (0x1F));
			Trama_PC[1] = (MADC16 >> 15) & 0x01; // bit mas significativo upper
			Trama_PC[2] = (MADC16 >> 8) & 0x7F;  // bits restantes upper
			Trama_PC[3] = (MADC16 >> 7) & 0x01;  // bit mas significativo lower
			Trama_PC[4] = (MADC16) & 0x7F;  // bits restantes lower
			Trama_PC[5] = (medicion >> 15) & 0x01; // bit mas significativo upper
			Trama_PC[6] = (medicion >> 8) & 0x7F;  // bits restantes upper
			Trama_PC[7] = (medicion >> 7) & 0x01;  // bit mas significativo lower
			Trama_PC[8] = (medicion) & 0x7F;  // bits restantes lower
			Trama_PC[9] = (step1 >> 7) & 0x01;  // bit mas significativo upper
			Trama_PC[10] = (step1) & 0x7F;  // bits restantes
			Trama_PC[11] = (step2 >> 7) & 0x01;  // bit mas significativo upper
			Trama_PC[12] = (step2) & 0x7F;  // bits restantes
			//CodError = AS1_SendBlock(Trama_PC,13,&Enviados); //El arreglo con la medici�n est� en iADC.u8 (notar que es un apuntador)
			CodError = AS2_SendBlock(Trama_PC,13,&Enviados); //El arreglo con la medici�n est� en iADC.u8 (notar que es un apuntador)
			 if (estado != FREERUN ){
				 estado = ESPERAR;
						  }
			
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

void reset( void){
	Bit1_ClrVal();
	Bit7_ClrVal();
	found_band = 0; // Por default, para detectar el anuncio
    phi_start = phi_0-75; // 90 grados
	phi_end = phi_0-118;  // 110 grados
	theta_start = theta_0; // 45 grados
	theta_end = theta_0-40; //
	
	step1 = phi_0-97 ;
	step2 =theta_90+57;
	last_step1 = step1;
	last_step2 = step2;
	servo_send(1,step1);
	delay_ms(400);
	servo_send(2, step2);
	dir = 0;
	estado = ESPERAR;
	
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
