// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file adc.h
 *
 * @brief This header file lists the functions and definitions - to configure 
 * and enable ADC Module and its features 
 * 
 * Definitions in this file are for dsPIC33CK256MP508
 *
 * Component: ADC
 *
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Disclaimer ">

/*******************************************************************************
* SOFTWARE LICENSE AGREEMENT
* 
* � [2024] Microchip Technology Inc. and its subsidiaries
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
#ifndef _ADC_H
#define _ADC_H

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <xc.h>
#include <stdint.h>
        
#include "userparms.h"

// </editor-fold>
        
// <editor-fold defaultstate="expanded" desc="DEFINITIONS/MACROS ">
// ADC MODULE Related Definitions
#define ADCBUF_INV_A_IPHASE1    -ADCBUF1
#define ADCBUF_INV_A_IPHASE2    -ADCBUF4
#define ADCBUF_INV_A_IBUS       ADCBUF0
        
#define ADCBUF_SPEED_REF_A      ADCBUF17
#define ADCBUF_VBUS_A           ADCBUF15
#define ADCBUF_MOSFET_TEMP_A    ADCBUF18

#ifdef SINGLE_SHUNT   /* IBUS used for single shunt ADC interrupt */    
    #define EnableADCInterrupt()   _ADCAN0IE = 1
    #define DisableADCInterrupt()  _ADCAN0IE = 0
    #define ClearADCIF()           _ADCAN0IF = 0
    #define ClearADCIF_ReadADCBUF() ADCBUF0       
    #define _ADCInterrupt _ADCAN0Interrupt  
#else               /* POT used for dual shunt ADC interrupt */
    #define EnableADCInterrupt()   _ADCAN17IE = 1
    #define DisableADCInterrupt()  _ADCAN17IE = 0
    #define ClearADCIF()           _ADCAN17IF = 0
    #define ClearADCIF_ReadADCBUF() ADCBUF17      
    #define _ADCInterrupt _ADCAN17Interrupt  
#endif

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS">
void InitializeADCs(void);

// </editor-fold>
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of ADC_H

