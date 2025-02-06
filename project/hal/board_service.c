// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file board_service.c
 *
 * @brief This module implements the board service routines 
 * 
 * Definitions in this file are for dsPIC33CK256MP508
 *
 * Component: BOARD SERVICE
 *
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Disclaimer ">

/*******************************************************************************
* SOFTWARE LICENSE AGREEMENT
* 
* © [2024] Microchip Technology Inc. and its subsidiaries
* 
* Subject to your compliance with these terms, you may use this Microchip 
* software and any derivatives exclusively with Microchip products. 
* You are responsible for complying with third party license terms applicable to
* your use of third party software (including open source software) that may 
* accompany this Microchip software.
* 
* Redistribution of this Microchip software in source or binary form is allowed 
* and must include the above terms of use and the following disclaimer with the
* distribution and accompanying materials.
* 
* SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL 
* MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR 
* CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO
* THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY
* LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL
* NOT EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR THIS
* SOFTWARE
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <stdint.h>
#include <stdbool.h>

#include "board_service.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLES">
BUTTON_T buttonStartStop;
BUTTON_T buttonSpeedHalfDouble;

uint16_t boardServiceISRCounter = 0;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="STATIC FUNCTIONS ">
static void ButtonGroupInitialize(void);
static void ButtonScan(BUTTON_T * ,bool);

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
bool IsPressed_Button1(void)
{
    if (buttonStartStop.status)
    {
        buttonStartStop.status = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool IsPressed_Button2(void)
{
    if (buttonSpeedHalfDouble.status)
    {
        buttonSpeedHalfDouble.status = false;
        return true;
    }
    else
    {
        return false;
    }
}

void BoardServiceStepIsr(void)
{
    if (boardServiceISRCounter <  BOARD_SERVICE_TICK_COUNT)
    {
        boardServiceISRCounter += 1;
    }
}
void BoardService(void)
{
    if (boardServiceISRCounter ==  BOARD_SERVICE_TICK_COUNT)
    {
        /* Button scanning loop for Button 1 to start Motor A */
        ButtonScan(&buttonStartStop,BUTTON_START_STOP);

        /* Button scanning loop for SW2 to enter into filed
            weakening mode */
        ButtonScan(&buttonSpeedHalfDouble,BUTTON_SPEED_HALF_DOUBLE);

        boardServiceISRCounter = 0;
    }
}
void BoardServiceInit(void)
{
    ButtonGroupInitialize();
    boardServiceISRCounter = BOARD_SERVICE_TICK_COUNT;
}

void ButtonScan(BUTTON_T *pButton,bool button) 
{
    if (button == true) 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->debounceCount--;
            pButton->state = BUTTON_DEBOUNCE;
        }
    } 
    else 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->state = BUTTON_NOT_PRESSED;
        } 
        else 
        {
            pButton->state = BUTTON_PRESSED;
            pButton->status = true;
        }
        pButton->debounceCount = 0;
    }
}
void ButtonGroupInitialize(void)
{
    buttonStartStop.state = BUTTON_NOT_PRESSED;
    buttonStartStop.debounceCount = 0;
    buttonStartStop.state = false;

    buttonSpeedHalfDouble.state = BUTTON_NOT_PRESSED;
    buttonSpeedHalfDouble.debounceCount = 0;
    buttonSpeedHalfDouble.state = false;

}
// *****************************************************************************
/* Function:
    Init_Peripherals()

  Summary:
    Routine initializes controller peripherals

  Description:
    Routine to initialize Peripherals used for Inverter Control

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void InitPeripherals(void)
{        
    uint16_t cmpReference = 0;
    CMP_Initialize();
    CMP1_ModuleEnable(true);
    cmpReference = (uint16_t)(__builtin_mulss(Q15_OVER_CURRENT_THRESHOLD,2047)>>15);
    cmpReference = cmpReference + 2048; 
    CMP1_ReferenceSet(cmpReference);
    InitializeADCs();
    /*400ms POR delay for IBUS_EXT signal coming from MCP651S in Dev Board*/
    __delay_ms(400);
    InitPWMGenerators();
    
    /* Make sure ADC does not generate interrupt while initializing parameters*/
    DisableADCInterrupt();
}
/**
 * Disable the PWM channels assigned by overriding them to low state.
 * @example
 * <code>
 * DisablePWMOutputs();
 * </code>
 */
void DisablePWMOutputs(void)
{
    /** Set Override Data on all PWM outputs */
    // 0b00 = State for PWM3H,L, if Override is Enabled
    PG3IOCONLbits.OVRDAT = 0;
    // 0b00 = State for PWM2H,L, if Override is Enabled
    PG2IOCONLbits.OVRDAT = 0; 
    // 0b00 = State for PWM1H,L, if Override is Enabled
    PG1IOCONLbits.OVRDAT = 0;  
    
    // 1 = OVRDAT<1> provides data for output on PWM3H
    PG3IOCONLbits.OVRENH = 1; 
    // 1 = OVRDAT<0> provides data for output on PWM3L
    PG3IOCONLbits.OVRENL = 1; 
    
    // 1 = OVRDAT<1> provides data for output on PWM2H
    PG2IOCONLbits.OVRENH = 1;
    // 1 = OVRDAT<0> provides data for output on PWM2L
    PG2IOCONLbits.OVRENL = 1; 
    
    // 1 = OVRDAT<1> provides data for output on PWM1H
    PG1IOCONLbits.OVRENH = 1;  
    // 1 = OVRDAT<0> provides data for output on PWM1L
    PG1IOCONLbits.OVRENL = 1;     
}

/**
 * Enable the PWM channels assigned by removing Override.
 * @example
 * <code>
 * EnablePWMOutputs();
 * </code>
 */
void EnablePWMOutputs(void)
{    
    // 0 = PWM Generator provides data for the PWM3H pin
    PG3IOCONLbits.OVRENH = 0; 
    // 0 = PWM Generator provides data for the PWM3L pin
    PG3IOCONLbits.OVRENL = 0; 
    
    // 0 = PWM Generator provides data for the PWM2H pin
    PG2IOCONLbits.OVRENH = 0;
    // 0 = PWM Generator provides data for the PWM2L pin
    PG2IOCONLbits.OVRENL = 0; 
    
    // 0 = PWM Generator provides data for the PWM1H pin
    PG1IOCONLbits.OVRENH = 0;  
    // 0 = PWM Generator provides data for the PWM1L pin
    PG1IOCONLbits.OVRENL = 0;     
}

void ClearPWMPCIFault(void)
{
    
    PG1FPCILbits.SWTERM = 1;
    PG2FPCILbits.SWTERM = 1;
    PG3FPCILbits.SWTERM = 1;
}

void PWMDutyCycleSet(MC_DUTYCYCLEOUT_T *pPwmDutycycle)
{
    pwmDutyCycleLimitCheck(pPwmDutycycle,(DEADTIME>>1),(LOOPTIME_TCY - (DEADTIME>>1)));  
    PWM_PDC3 = pPwmDutycycle->dutycycle3;
    PWM_PDC2 = pPwmDutycycle->dutycycle2;
    PWM_PDC1 = pPwmDutycycle->dutycycle1;
}
void PWMDutyCycleSetDualEdge(MC_DUTYCYCLEOUT_T *pPwmDutycycle1,MC_DUTYCYCLEOUT_T *pPwmDutycycle2)
{
    pwmDutyCycleLimitCheck(pPwmDutycycle1,(DEADTIME>>1),(LOOPTIME_TCY - (DEADTIME>>1)));
    
    PWM_PHASE3 = pPwmDutycycle1->dutycycle3 + (DEADTIME>>1);
    PWM_PHASE2 = pPwmDutycycle1->dutycycle2 + (DEADTIME>>1);
    PWM_PHASE1 = pPwmDutycycle1->dutycycle1 + (DEADTIME>>1);
    
    pwmDutyCycleLimitCheck(pPwmDutycycle2,(DEADTIME>>1),(LOOPTIME_TCY - (DEADTIME>>1)));
    
    PWM_PDC3 = pPwmDutycycle2->dutycycle3 - (DEADTIME>>1);
    PWM_PDC2 = pPwmDutycycle2->dutycycle2 - (DEADTIME>>1);
    PWM_PDC1 = pPwmDutycycle2->dutycycle1 - (DEADTIME>>1);
}
void pwmDutyCycleLimitCheck (MC_DUTYCYCLEOUT_T *pPwmDutycycle,uint16_t min,uint16_t max)
{
    if (pPwmDutycycle->dutycycle1 < min)
    {
        pPwmDutycycle->dutycycle1 = min;
    }
    else if (pPwmDutycycle->dutycycle1 > max)
    {
        pPwmDutycycle->dutycycle1 = max;
    }
    
    if (pPwmDutycycle->dutycycle2 < min)
    {
        pPwmDutycycle->dutycycle2 = min;
    }
    else if (pPwmDutycycle->dutycycle2 > max)
    {
        pPwmDutycycle->dutycycle2 = max;
    }
    
    if (pPwmDutycycle->dutycycle3 < min)
    {
        pPwmDutycycle->dutycycle3 = min;
    }
    else if (pPwmDutycycle->dutycycle3 > max)
    {
        pPwmDutycycle->dutycycle3 = max;
    }
}

/**
* <B> Function: void CompensateDCBusVoltage(MC_ABC_T *,MCAPP_MEASURE_T *)  </B>
*
* @brief Computes the reciprocal of the DC bus voltage.
*
* @param DC bus voltage
* @return none.
* @example
* <CODE> CompensateDCBusVoltage(&vabc,&measureInputs); </CODE>
*
*/
void CompensateDCBusVoltage(MC_ABC_T *pvabc, MCAPP_MEASURE_T *pmeasureInputs)
{
    int16_t rvdc = 0;
    
    /* Calculating the reciprocal of DC Bus voltage*/
    if (pmeasureInputs->dcBusVoltage <= MC_RVDC_MIN_VDC)
    {
        rvdc =  MC_RVDC_MIN;
    }
    else
    {
        rvdc =  __builtin_divf(MC_RVDC_MIN_VDC, pmeasureInputs->dcBusVoltage);
    }
    
    /*DC Bus Voltage Compensation*/
    pvabc->a = (int16_t)(__builtin_mulss(pvabc->a, rvdc)>> MC_RVDC_Q); 
                                                                
    pvabc->b = (int16_t)(__builtin_mulss(pvabc->b, rvdc)>> MC_RVDC_Q); 
                                                                
    pvabc->c = (int16_t)(__builtin_mulss(pvabc->c, rvdc)>> MC_RVDC_Q); 
                                                                
}
/**
* <B> Function: void SaturateAndScalePOTvalue(MC_ABC_T *,MCAPP_MEASURE_T *)  </B>
*
* @brief Computes the scaled value of Potentiometer input.
*
* @param Potentiometer
* @return none.
* @example
* <CODE> SaturateAndScalePOTvalue(&measureInputs); </CODE>
*
*/
void SaturateAndScalePOTvalue(MCAPP_MEASURE_T *pmeasureInputs)
{

    if(pmeasureInputs->potValue >= POT_COUNTS_SATURATE)
    {
        pmeasureInputs->potValue = POT_COUNTS_SATURATE;
    }

    pmeasureInputs->potValueScaled = pmeasureInputs->potValue + 
                                                (pmeasureInputs->potValue>>1);

}

// </editor-fold>