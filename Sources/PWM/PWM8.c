/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : PWM8.c
**     Project   : canSend
**     Processor : MC9S12XS128CAL
**     Component : PWM
**     Version   : Component 02.231, Driver 01.16, CPU db: 3.00.020
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2025-04-16, 20:15
**     Abstract  :
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
**     Settings  :
**         Used output pin             : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       4             |  PP0_KWP0_PWM0_IOC0_RxD1
**             ----------------------------------------------------
**
**         Timer name                  : PWM0 [8-bit]
**         Counter                     : PWMCNT0   [$030C]
**         Mode register               : PWMCTL    [$0305]
**         Run register                : PWME      [$0300]
**         Prescaler                   : PWMPRCLK  [$0303]
**         Compare 1 register          : PWMPER0   [$0314]
**         Compare 2 register          : PWMDTY0   [$031C]
**         Flip-flop 1 register        : PWMPOL    [$0301]
**
**         User handling procedure     : not specified
**
**         Output pin
**
**         Port name                   : P
**         Bit number (in port)        : 0
**         Bit mask of the port        : $0001
**         Port data register          : PTP       [$0258]
**         Port control register       : DDRP      [$025A]
**
**         Runtime setting period      : none
**         Runtime setting ratio       : calculated
**         Initialization:
**              Aligned                : Left
**              Output level           : low
**              Timer                  : Enabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 13333 Hz
**           Initial value of            period        pulse width (ratio 2.745%)
**             Xtal ticks              : 306000        8400
**             microseconds            : 19125         525
**             milliseconds            : 19            1
**             seconds (real)          : 0.019125      0.000525
**
**     Contents  :
**         Enable     - byte PWM8_Enable(void);
**         Disable    - byte PWM8_Disable(void);
**         SetRatio16 - byte PWM8_SetRatio16(word Ratio);
**         SetDutyUS  - byte PWM8_SetDutyUS(word Time);
**         SetDutyMS  - byte PWM8_SetDutyMS(word Time);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE PWM8. */

#include "PWM8.h"

#pragma DATA_SEG PWM8_DATA             /* Select data segment "PWM8_DATA" */

#pragma CODE_SEG PWM8_CODE

static bool EnUser;                    /* Enable/Disable device by user */
static word RatioStore;                /* Ratio of L-level to H-level */


/*
** ===================================================================
**     Method      :  HWEnDi (component PWM)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  if (EnUser) {                        /* Enable device? */
    PWME_PWME0 = 1U;                   /* Run counter */
  } else {                             /* Disable device? */
    PWME_PWME0 = 0U;                   /* Stop counter */
    PWMCNT0 = 0U;                      /* Reset counter */
  }
}

/*
** ===================================================================
**     Method      :  SetRatio (component PWM)
**
**     Description :
**         The method reconfigures the compare and modulo registers of 
**         the peripheral(s) when the speed mode changes. The method is 
**         called automatically as a part of the component 
**         SetHigh/SetLow/SetSlow methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#pragma MESSAGE DISABLE C12056         /* Disable WARNING C12056: SP debug info incorrect because of optimization */
static void SetRatio(void)
{
  PWMDTY0 = (byte)(((0xFFUL * (dword)RatioStore) + 0x8000UL) >> 0x10U); /* Calculate new value according to the given ratio */
}
#pragma MESSAGE DEFAULT C12056         /* Re-enable WARNING C12056 */

/*
** ===================================================================
**     Method      :  PWM8_Enable (component PWM)
**
**     Description :
**         This method enables the component - it starts the signal
**         generation. Events may be generated (<DisableEvent>
**         /<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte PWM8_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    HWEnDi();                          /* Enable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  PWM8_Disable (component PWM)
**
**     Description :
**         This method disables the component - it stops the signal
**         generation and events calling. When the timer is disabled,
**         it is possible to call <ClrValue> and <SetValue> methods.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte PWM8_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    HWEnDi();                          /* Disable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  PWM8_SetRatio16 (component PWM)
**
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as a 16-bit unsigned integer number. Value 0 - 65535 value
**         is proportional to ratio 0 - 100%. 
**         Note: Calculated duty depends on the timer possibilities and
**         on the selected period.
**         The method is available only if method <SetPeriodMode> is
**         not selected.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 65535 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte PWM8_SetRatio16(word Ratio)
{
  RatioStore = Ratio;                  /* Store new value of the ratio */
  SetRatio();                          /* Calculate and set up new appropriate values of the duty and period registers */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  PWM8_SetDutyUS (component PWM)
**
**     Description :
**         This method sets the new duty value of the output signal. The
**         duty is expressed in microseconds as a 16-bit unsigned integer
**         number.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Time       - Duty to set [in microseconds]
**                      (0 to 19125 us in high speed mode)
**     Returns     :
**         ---        - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte PWM8_SetDutyUS(word Time)
{
  dlong rtval;                         /* Result of two 32-bit numbers multiplication */

  if (Time > 0x4AB5U) {                /* Is the given value out of range? */
    return ERR_RANGE;                  /* If yes then error */
  }
  PE_Timer_LngMul((dword)Time, (dword)57490804, &rtval); /* Multiply given value and high speed CPU mode coefficient */
  if (PE_Timer_LngHi3(rtval[0], rtval[1], &RatioStore)) { /* Is the result greater or equal than 65536 ? */
    RatioStore = 0xFFFFU;              /* If yes then use maximal possible value */
  }
  SetRatio();                          /* Calculate and set up new appropriate values of the duty and period registers */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  PWM8_SetDutyMS (component PWM)
**
**     Description :
**         This method sets the new duty value of the output signal. The
**         duty is expressed in milliseconds as a 16-bit unsigned integer
**         number.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Time       - Duty to set [in milliseconds]
**                      (0 to 19 ms in high speed mode)
**     Returns     :
**         ---        - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte PWM8_SetDutyMS(word Time)
{
  dlong rtval;                         /* Result of two 32-bit numbers multiplication */

  if (Time > 0x13U) {                  /* Is the given value out of range? */
    return ERR_RANGE;                  /* If yes then error */
  }
  PE_Timer_LngMul((dword)Time, (dword)224573453, &rtval); /* Multiply given value and high speed CPU mode coefficient */
  if (PE_Timer_LngHi2(rtval[0], rtval[1], &RatioStore)) { /* Is the result greater or equal than 65536 ? */
    RatioStore = 0xFFFFU;              /* If yes then use maximal possible value */
  }
  SetRatio();                          /* Calculate and set up new appropriate values of the duty and period registers */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  PWM8_Init (component PWM)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PWM8_Init(void)
{
  /* PWMCNT0: PWMCNT0=0 */
  setReg8(PWMCNT0, 0x00U);             /* Reset Counter */ 
  /* PWMSDN: PWMIF=1,PWMIE=0,PWMRSTRT=0,PWMLVL=0,??=0,PWM7IN=0,PWM7INL=0,PWM7ENA=0 */
  setReg8(PWMSDN, 0x80U);              /* Emergency shutdown feature settings */ 
  RatioStore = 0x0707U;                /* Store initial value of the ratio */
  EnUser = TRUE;                       /* Enable device */
  /* PWMDTY0: PWMDTY0=7 */
  setReg8(PWMDTY0, 0x07U);             /* Store initial value to the duty-compare register */ 
  /* PWMPER0: PWMPER0=0xFF */
  setReg8(PWMPER0, 0xFFU);             /* and to the period register */ 
  /* PWMPRCLK: ??=0,PCKB2=0,PCKB1=0,PCKB0=0,??=0,PCKA2=0,PCKA1=0,PCKA0=1 */
  setReg8(PWMPRCLK, 0x01U);            /* Set prescaler register */ 
  /* PWMSCLA: BIT7=1,BIT6=0,BIT5=0,BIT4=1,BIT3=0,BIT2=1,BIT1=1,BIT0=0 */
  setReg8(PWMSCLA, 0x96U);             /* Set scale register */ 
  /* PWMCLK: PCLK0=1 */
  setReg8Bits(PWMCLK, 0x01U);          /* Select clock source */ 
  HWEnDi();                            /* Enable/disable device according to status flags */
}

/* END PWM8. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
