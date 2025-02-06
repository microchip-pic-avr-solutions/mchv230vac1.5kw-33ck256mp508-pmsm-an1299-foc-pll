// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file control.h
 *
 * @brief This module initializes data structure variable type definitions of 
 * motor control algorithm
 * 
 * Component: Control Loop
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

#ifndef __CONTROL_H
#define __CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
    
#include <stdint.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPES ">    
/* Control Parameter data type

  Description:
    This structure will host parameters related to application control
    parameters.
 */
typedef struct
{
    /* Reference velocity */
    int16_t   qVelRef;
    /* Vd flux reference value */
    int16_t   qVdRef;
    /* Vq torque reference value */
    int16_t   qVqRef;
    /* Ramp for speed reference value */
    int16_t   qRefRamp;
    /* Speed of the ramp */
    int16_t   qDiff;
    /* Target Speed*/
    int16_t  targetSpeed;
    /* The Speed Control Loop will be executed only every speedRampCount*/
    int16_t   speedRampCount;  
} CTRL_PARM_T;
/* Motor Parameter data type

  Description:
    This structure will host parameters related to motor parameters.
*/
typedef struct
{
    /* Start up ramp in open loop. */
    uint32_t startupRamp;
    /* counter that is incremented in CalculateParkAngle() up to LOCK_TIME,*/
    uint16_t startupLock;
    /* Start up ramp increment */
    uint16_t tuningAddRampup;	
    uint16_t tuningDelayRampup;
} MOTOR_STARTUP_DATA_T;

/* General system flag data type

  Description:
    This structure will host parameters related to application system flags.
 */
typedef union
{
    struct
    {
        /* Run motor indication */
        unsigned RunMotor:1;
        /* Open loop/closed loop indication */
        unsigned OpenLoop:1;
        /* Mode changed indication - from open to closed loop */
        unsigned ChangeMode:1;
        /* Speed doubled indication */
        unsigned ChangeSpeed:1;
       /* Unused bits */
        unsigned    :12;
    } bits;
    uint16_t Word;
} UGF_T;
// </editor-fold>

// <editor-fold defaultstate="expanded" desc=" ">   
extern CTRL_PARM_T ctrlParm;
extern MOTOR_STARTUP_DATA_T motorStartUpData;
extern MC_ALPHABETA_T valphabeta,ialphabeta;
extern MC_ALPHABETA_T valphabeta,ialphabeta;
extern MC_SINCOS_T sincosTheta;
extern MC_DQ_T vdq,idq;
extern MC_DUTYCYCLEOUT_T pwmDutycycle;
extern MC_ABC_T   vabc,iabc;

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif /* __CONTORL_H */
