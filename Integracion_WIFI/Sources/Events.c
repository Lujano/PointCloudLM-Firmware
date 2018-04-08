/* ###################################################################
**     Filename    : Events.c
**     Project     : Integracion_Fase1
**     Processor   : MC9S08QE128CLK
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-02-03, 08:14, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

void TI1_OnInterrupt(void)
{
  /* Write your code here ... */
		
	if (estado_trigger == TRIGGER_TERMINADO){
	}
	else if (estado_trigger == TRIGGER_BAJO){
		Bit1_SetVal();
		estado_trigger = TRIGGER_ALTO;
		}
	else if (estado_trigger == TRIGGER_ALTO){
		Bit1_ClrVal();
		estado_trigger = TRIGGER_TERMINADO;
		}
}



/*
** ===================================================================
**     Event       :  AS2_OnError (module Events)
**
**     Component   :  AS2 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS2_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS2_OnRxChar (module Events)
**
**     Component   :  AS2 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS2_OnRxChar(void)
{
	 if (found_band == 0){
		 CodError =  AS2_RecvChar( & anuncio ) ;
		 if (( anuncio & 0xf0) == 0xf0 ) {
			 found_band = 1;
			 
		 }
	 }
	 
	 else if (found_band == 1){
		 CodError =  AS2_RecvChar( & anuncio2 ) ;
		 if (anuncio2 == 0){
			 found_band = found_band+1 ; // es un commando y se lee el siguiente byte
		 }
		 else{
			 found_band = 0 ; // No es un commando
		 }
		
	 }
	 else if (found_band == 2){
		 CodError =  AS2_RecvChar( & command ) ;
		 if (command == 2){
			 n_canales = anuncio & 0x0f ; // Numero de canales a leer
			 found_band = found_band+1; // es un commando y se lee el siguiente byte
		 }
		 else if (command == 1){
			 n_canales = anuncio & 0x0f ; // Numero de canales a leer
			 found_band = found_band+1 ; // es un commando y se lee el siguiente byte
		 }
		 else if (command == 3){
		     estado = MOTOR;
		     found_band = 0;
		     command = 0;
		     anuncio = 0;
		     anuncio2 = 0;
		 }
		 else{
			 found_band = 0; // No es un commando
			 command = 0;
			 anuncio = 0;
			 anuncio2 = 0;
		 }
		 	
	  }
	 else {
		 if (found_band == (n_canales+n_canales+2)){ // Se lee hasta que se alcance el numero de bytes de trama
			 found_band = 0; // Se termino la lectura del Bloque
			 if (command == 2){
				 estado = POINTCLOUD_START;
			 }
			 else if (command == 1){
				 estado = FREERUN;
				
			 }
			 command = 0;
			 anuncio = 0;
			 anuncio2 = 0;
			 n_canales = 0;
			 
		 }
		 else {found_band = found_band+1;} // se lee el siguiente byte
		 
	 }

	 
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS2_OnTxChar (module Events)
**
**     Component   :  AS2 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS2_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS2_OnFullRxBuf (module Events)
**
**     Component   :  AS2 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS2_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS2_OnFreeTxBuf (module Events)
**
**     Component   :  AS2 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS2_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnError (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS1_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnRxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS1_OnRxChar(void)
{
	 

		 
}

/*
** ===================================================================
**     Event       :  AS1_OnTxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS1_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnFullRxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS1_OnFullRxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnFreeTxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void  AS1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  Cap1_OnCapture (module Events)
**
**     Component   :  Cap1 [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Cap1_OnCapture(void)
{
	/* Write your code here ... */
			
		if (estado_echo == ECHO_TRIGGERED){
			Bit7_SetVal();
			Cap1_Reset(); // Se detecta el Rising edge, resetear registro de captura
			estado_echo = ECHO_MEDIR;
		}
		else if (estado_echo == ECHO_MEDIR){
			Cap1_GetCaptureValue(&medicion); // Se guarda el valor medido en el Falling edge
			estado_echo= ECHO_TERMINADO;
			}
		else if (estado_echo == ECHO_TERMINADO){

			}
  /* Write your code here ... */
}

/* END Events */

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
