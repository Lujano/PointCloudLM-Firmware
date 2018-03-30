/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : PE_Timer.c
**     Project     : Integracion_Fase3
**     Processor   : MC9S08QE128CLK
**     Component   : PE_Timer
**     Version     : Driver 01.04
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-03-29, 23:43, # CodeGen: 1
**     Abstract    :
**         This module "PE_Timer" implements internal methods and definitions
**         used by components working with timers.
**     Settings    :
**     Contents    :
**         No public methods
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PE_Timer.c
** @version 01.04
** @brief
**         This module "PE_Timer" implements internal methods and definitions
**         used by components working with timers.
*/         
/*!
**  @addtogroup PE_Timer_module PE_Timer module documentation
**  @{
*/         


/* MODULE PE_Timer. */

#include "PE_Timer.h"


typedef unsigned long UINT32;

/*lint -save  -esym(960,18.4) Disable MISRA rule (18.4) checking. */
typedef union {
  uint32_t val;
  struct {
    uint16_t hi16,lo16;
  } s;
} OP_UINT32;

typedef struct {
  uint16_t dummy;
  volatile uint32_t mid;
} M_UINT32;

typedef struct {
  volatile uint32_t hi32, lo32;
} UINT64;

typedef union {
  UINT64 val;
  M_UINT32 m;
  dlong DL;
} OP_UINT64;
/*lint -restore  +esym(960,18.4) Enable MISRA rule (18.4) checking. */

/*
** ===================================================================
**     Method      :  PE_Timer_LngMul (component PE_Timer)
**
**     Description :
**         The method multiplies two 32 bit values and returns pointer to 
**         64 bit result.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_Timer_LngMul(dword va1, dword va2, dlong *var)
{
  OP_UINT64 Result;

  Result.val.hi32 = 0UL;
  Result.val.lo32 = (va1 & 0xFFFFU) * (va2 & 0xFFFFU);
  {
    OP_UINT32 tmp;

    tmp.val = (va1 & 0xFFFFU) * ((va2 >> 16U) & 0xFFFFU);
    Result.m.mid += (UINT32)tmp.s.lo16;
    Result.val.hi32 += (UINT32)tmp.s.hi16;
  }
  {
    OP_UINT32 tmp;

    tmp.val = ((va1 >> 16U) & 0xFFFFU) * (va2 & 0xFFFFU);
    Result.m.mid += (UINT32)tmp.s.lo16;
    Result.val.hi32 += (UINT32)tmp.s.hi16;
  }
  Result.val.hi32 += ((va1 >> 16U) & 0xFFFFU) * ((va2 >> 16U) & 0xFFFFU);
  /* *** */
  (*var)[0] = Result.DL[0];
  (*var)[1] = Result.DL[1];
}

/*
** ===================================================================
**     Method      :  PE_Timer_LngHi2 (component PE_Timer)
**
**     Description :
**         The method transfers 64 bit result to 16 bit ratio value and 
**         returns overflow flag.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
bool PE_Timer_LngHi2(dword High, dword Low, word *Out)
{
  if (High == 0x00UL) {
    if ((Low & 0x8000UL) != 0x00U) {
      if ((Low >> 0x10UL) < 0xFFFFUL) {
        *Out = (word)(((word)(Low >> 0x10UL)) + 1U);
        return FALSE;
      }
    } else {
      *Out = (word)(Low >> 0x10UL);
      return FALSE;
    }
  }
  *Out = (word)(Low >> 0x10UL);
  return TRUE;
}

/*
** ===================================================================
**     Method      :  PE_Timer_LngHi4 (component PE_Timer)
**
**     Description :
**         The method transfers 64 bit result to 16 bit ratio value and 
**         returns overflow flag.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
bool PE_Timer_LngHi4(dword High, dword Low, word *Out)
{
  if ((High >> 0x10UL) == 0x00U) {
    if ((Low & 0x80000000UL) != 0x00UL) {
      if (High < 0xFFFFUL) {
        *Out = (word)(((word)High) + 1U);
        return FALSE;
      }
    } else {
      *Out = (word)High;
      return FALSE;
    }
  }
  *Out = (word)High;
  return TRUE;
}



/* END PE_Timer. */

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