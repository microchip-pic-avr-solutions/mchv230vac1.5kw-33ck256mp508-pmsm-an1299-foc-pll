// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file userparms.h
 *
 * @brief This file has definitions to be configured by the user for spinning
 * motor using field oriented control.
 *
 * Board : MCHV-230VAC-1.5kW + dsPIC33CK256MP508 MC DIM
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
#ifndef USERPARMS_H
#define USERPARMS_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES "> 
    
#include <stdint.h>
#include <xc.h>
    
#include "general.h"

// </editor-fold>    
    
// <editor-fold defaultstate="expanded" desc="DEFINITIONS/MACROS "> 
        

/* Definition for tuning - if active the speed reference is a ramp with a 
constant slope. The slope is determined by TUNING_DELAY_RAMPUP constant.
 the software ramp implementing the speed increase has a constant slope, 
 adjusted by the delay TUNING_DELAY_RAMPUP when the speed is incremented.
 The potentiometer speed reference is overwritten. The speed is          
 increased from 0 up to the END_SPEED_RPM in open loop  with the speed  
 increase typical to open loop, the transition to closed loop is done    
 and the software speed ramp reference is continued up to MAXIMUM_SPEED_RPM. */
#undef TUNING

/* if TUNING was define, then the ramp speed is needed: */
#ifdef TUNING
    /* the smaller the value, the quicker the ramp */
    #define TUNING_DELAY_RAMPUP   0xF      
#endif


/* open loop continuous functioning */
/* closed loop transition disabled  */
#undef OPEN_LOOP_FUNCTIONING

/* Definition for torque mode - for a separate tuning of the current PI
controllers, tuning mode will disable the speed PI controller */
#undef TORQUE_MODE
    
/* undef to work with dual Shunt  */    
#define SINGLE_SHUNT     

/* undef to work with External Op-Amp*/
#define INTERNAL_OPAMP_CONFIG    

// </editor-fold>
  
// <editor-fold defaultstate="expanded" desc="DEFINITIONS/CONSTANTS ">
    
/****************************** Motor Parameters ******************************/
/********************  support xls file definitions begin *********************/
/* The following values are given in the xls attached file */    
    
/* Motor's number of pole pairs */
#define POLE_PAIRS 5
/* Nominal speed of the motor in RPM */
#define NOMINAL_SPEED_RPM    3000 
/* Maximum speed of the motor in RPM - given by the motor's manufacturer */
#define MAXIMUM_SPEED_RPM    5000 
/* Motor rated peak current in amps*/
#define MOTOR_RATED_PEAK_CURRENT     3

/* The following values are given in the xls attached file */
#define NORM_CURRENT_CONST     0.000671
/* normalized ls/dt value */
#define NORM_LSDTBASE 4551
#define NORM_LSDTBASE_SCALE 5    /* 2^NORM_LSDTBASE_SCALE is the scaling */
#define NORM_LSDTBASE_SCALE_SHIFT        (15- NORM_LSDTBASE_SCALE)
#define NORM_LSDTBASE_FILT_SCALE_SHIFT   (15- NORM_LSDTBASE_SCALE + 3)   
/* normalized rs value */
#define NORM_RS  4007
#define NORM_RS_SCALE       0   /* 2^NORM_RS_SCALE is the scaling */ 
#define NORM_RS_SCALE_SHIFT         (15 - NORM_RS_SCALE) 
/* the calculation of Rs gives a value exceeding the Q15 range so,
 the normalized value is further divided by 2 to fit the 32768 limit
 this is taken care in the estim.c where the value is implied
 normalized inv kfi at base speed */
#define NORM_INVKFIBASE  6460
#define NORM_INVKFIBASE_SCALE	3   /* 2^NORM_INVKFIBASE_SCALE is the scaling */ 
/* the calculation of InvKfi gives a value which not exceed the Q15 limit
   to assure that an increase of the term with 5 is possible in the lookup table
   for high flux weakening the normalized is initially divided by 2
   this is taken care in the estim.c where the value is implied
   normalized dt value */
#define NORM_DELTAT  1790

/* Limitation constants */
/* di = i(t1)-i(t2) limitation
 high speed limitation, for dt 50us 
 the value can be taken from attached xls file */
#define D_ILIMIT_HS 1365
/* low speed limitation, for dt 8*50us */
#define D_ILIMIT_LS 6554
    
/**********************  support xls file definitions end *********************/


/* Filters constants definitions  */
/* BEMF filter for d-q components @ low speeds */
#define KFILTER_ESDQ 1200
/* BEMF filter for d-q components @ high speed - Flux Weakening case */
#define KFILTER_ESDQ_FW 164
/* Estimated speed filter constant */
#define KFILTER_VELESTIM 2*374


/* initial offset added to estimated value, 
 when transitioning from open loop to closed loop 
 the value represents 45deg and should satisfy both 
 open loop and closed loop functioning 
 normally this value should not be modified, but in 
 case of fine tuning of the transition, depending on 
 the load or the rotor moment of inertia */
#define INITOFFSET_TRANS_OPEN_CLSD     0

/* current transformation macro, used below */
#define NORM_CURRENT(current_real) (Q15(current_real/NORM_CURRENT_CONST/32768))

/* Open loop startup constants */

/* The following values depends on the PWM frequency,
 lock time is the time needed for motor's poles alignment 
before the open loop speed ramp up */
/* This number is: 20,000 is 1 second. */
#define LOCK_TIME 8000 
/* Open loop speed ramp up end value Value in RPM*/
#define END_SPEED_RPM 700 
/* Open loop acceleration */
#define OPENLOOP_RAMPSPEED_INCREASERATE 10
/* Open loop q current setup - */
#define Q_CURRENT_REF_OPENLOOP NORM_CURRENT(0.5)

/* Specify Over Current Limit - DC BUS */
#define Q15_OVER_CURRENT_THRESHOLD NORM_CURRENT(3.5)

/* Maximum motor speed converted into electrical speed */
#define MAXIMUMSPEED_ELECTR MAXIMUM_SPEED_RPM*POLE_PAIRS
/* Nominal motor speed converted into electrical speed */
#define NOMINALSPEED_ELECTR NOMINAL_SPEED_RPM*POLE_PAIRS

/* End speed converted to fit the startup ramp */
#define END_SPEED (END_SPEED_RPM * POLE_PAIRS * LOOPTIME_SEC * 65536 / 60.0)*1024
/* End speed of open loop ramp up converted into electrical speed */
#define ENDSPEED_ELECTR END_SPEED_RPM*POLE_PAIRS
    
/* In case of the potentiometer speed reference, a reference ramp
is needed for assuring the motor can follow the reference imposed /
minimum value accepted */
#define SPEEDREFRAMP   Q15(0.00003)  
/* The Speed Control Loop Executes every  SPEEDREFRAMP_COUNT */
#define SPEEDREFRAMP_COUNT   2  
    
/* Maximum Speed PI controller output*/
#define SPEED_PI_OUT_MAX    (MOTOR_RATED_PEAK_CURRENT/NORM_CURRENT_CONST)
/* PI controllers tuning values - */     
/* D Control Loop Coefficients */
#define D_CURRCNTR_PTERM       Q15(0.05)
#define D_CURRCNTR_ITERM       Q15(0.003)
#define D_CURRCNTR_CTERM       Q15(0.999)
#define D_CURRCNTR_OUTMAX      0x7FFF

/* Q Control Loop Coefficients */
#define Q_CURRCNTR_PTERM       Q15(0.05)
#define Q_CURRCNTR_ITERM       Q15(0.003)
#define Q_CURRCNTR_CTERM       Q15(0.999)
#define Q_CURRCNTR_OUTMAX      0x7FFF

/* Velocity Control Loop Coefficients */
#define SPEEDCNTR_PTERM        Q15(0.05)
#define SPEEDCNTR_ITERM        Q15(0.001)
#define SPEEDCNTR_CTERM        Q15(0.999)
#define SPEEDCNTR_OUTMAX       SPEED_PI_OUT_MAX
 
/******************************** Field Weakening *****************************/
/* Field Weakening constant for constant torque range 
   Flux reference value */
#define IDREF_BASESPEED         NORM_CURRENT(0.0)   

/*-------------------------------------------------------------
   IMPORTANT:--------------------------------------------------
  -------------------------------------------------------------
   In flux weakening of the surface mounted permanent magnets
   PMSMs the mechanical damage of the rotor and the
   demagnetization of the permanent magnets is possible if
   cautions measures are not taken or the motor’s producer
   specifications are not respected.
  -------------------------------------------------------------
   IMPORTANT:--------------------------------------------------
  -------------------------------------------------------------
   In flux weakening regime implementation, if the FOC is lost
   at high speed above the nominal value, the possibility of
   damaging the inverter is eminent. The reason is that the
   BEMF will have a greater value than the one that would be
   obtained for the nominal speed exceeding the DC bus voltage
   value and though the inverter’s power semiconductors and DC
   link capacitors would have to support it. Since the tuning
   proposed implies iterative coefficient corrections until
   the optimum functioning is achieved, the protection of the
   inverter with corresponding circuitry should be assured in
   case of stalling at high speeds.                            */

    /*-------------------------------------------------------------
   IMPORTANT:--------------------------------------------------
  -------------------------------------------------------------
     The field weakening is not implemented in this algorithm with Leadshine 400W 
     motor using look-up tables. The below table values must be re-calculated for
     the specific motor. The maximum speed of 5000rpm is achieved from the 310VDC
     bus voltage* */
/* speed index is increase */
#define SPEED_INDEX_CONST 10                

/* the following values indicate the d-current variation with speed 
 please consult app note for details on tuning */
#define	IDREF_SPEED0	NORM_CURRENT(0)     /* up to 2800 RPM */
#define	IDREF_SPEED1	NORM_CURRENT(-0.7)  /* ~2950 RPM */
#define	IDREF_SPEED2	NORM_CURRENT(-0.9)  /* ~3110 RPM */
#define	IDREF_SPEED3	NORM_CURRENT(-1.0)  /* ~3270 RPM */
#define	IDREF_SPEED4	NORM_CURRENT(-1.4)  /* ~3430 RPM */
#define	IDREF_SPEED5	NORM_CURRENT(-1.7)  /* ~3600 RPM */
#define	IDREF_SPEED6	NORM_CURRENT(-2.0)  /* ~3750 RPM */
#define	IDREF_SPEED7	NORM_CURRENT(-2.1)  /* ~3910 RPM */
#define	IDREF_SPEED8	NORM_CURRENT(-2.2)  /* ~4070 RPM */
#define	IDREF_SPEED9	NORM_CURRENT(-2.25) /* ~4230 RPM */
#define	IDREF_SPEED10	NORM_CURRENT(-2.3)  /* ~4380 RPM */
#define	IDREF_SPEED11	NORM_CURRENT(-2.35) /* ~4550 RPM */
#define	IDREF_SPEED12	NORM_CURRENT(-2.4)  /* ~4700 RPM */
#define	IDREF_SPEED13	NORM_CURRENT(-2.45) /* ~4860 RPM */
#define	IDREF_SPEED14	NORM_CURRENT(-2.5)  /* ~5020 RPM */
#define	IDREF_SPEED15	NORM_CURRENT(-2.5)  /* ~5180 RPM */
#define	IDREF_SPEED16	NORM_CURRENT(-2.5)  /* ~5340 RPM */
#define	IDREF_SPEED17	NORM_CURRENT(-2.5)  /* ~5500 RPM */

/* the following values indicate the invKfi variation with speed 
 please consult app note for details on tuning */
#define	INVKFI_SPEED0           NORM_INVKFIBASE     /* up to 2800 RPM */
#define	INVKFI_SPEED1           8674        /* ~2950 RPM */
#define	INVKFI_SPEED2           9156        /* ~3110 RPM */
#define	INVKFI_SPEED3           9638        /* ~3270 RPM */
#define	INVKFI_SPEED4           10120       /* ~3430 RPM */
#define	INVKFI_SPEED5           10602       /* ~3600 RPM */
#define	INVKFI_SPEED6           11084       /* ~3750 RPM */
#define	INVKFI_SPEED7           11566       /* ~3910 RPM */
#define	INVKFI_SPEED8           12048       /* ~4070 RPM */
#define	INVKFI_SPEED9           12530       /* ~4230 RPM */
#define	INVKFI_SPEED10          13012       /* ~4380 RPM */
#define	INVKFI_SPEED11          13494       /* ~4550 RPM */
#define	INVKFI_SPEED12          13976       /* ~4700 RPM */
#define	INVKFI_SPEED13          14458       /* ~4860 RPM */
#define	INVKFI_SPEED14          14940       /* ~5020 RPM */
#define	INVKFI_SPEED15          15422       /* ~5180 RPM */
#define	INVKFI_SPEED16          15904       /* ~5340 RPM */
#define	INVKFI_SPEED17          16387       /* ~5500 RPM */



/* the following values indicate the Ls variation with speed 
 please consult app note for details on tuning */
#define LS_OVER2LS0_SPEED0      Q15(0.5)    /* up to 2800 RPM */
#define LS_OVER2LS0_SPEED1      Q15(0.45)   /* ~2950 RPM */
#define LS_OVER2LS0_SPEED2      Q15(0.4)    /* ~3110 RPM */
#define LS_OVER2LS0_SPEED3      Q15(0.4)    /* ~3270 RPM */
#define LS_OVER2LS0_SPEED4      Q15(0.35)   /* ~3430 RPM */
#define LS_OVER2LS0_SPEED5      Q15(0.35)   /* ~3600 RPM */
#define LS_OVER2LS0_SPEED6      Q15(0.34)   /* ~3750 RPM */
#define LS_OVER2LS0_SPEED7      Q15(0.34)   /* ~3910 RPM */
#define LS_OVER2LS0_SPEED8      Q15(0.33)   /* ~4070 RPM */
#define LS_OVER2LS0_SPEED9      Q15(0.33)   /* ~4230 RPM */
#define LS_OVER2LS0_SPEED10     Q15(0.32)   /* ~4380 RPM */
#define LS_OVER2LS0_SPEED11     Q15(0.32)   /* ~4550 RPM */
#define LS_OVER2LS0_SPEED12     Q15(0.31)   /* ~4700 RPM */
#define LS_OVER2LS0_SPEED13     Q15(0.30)   /* ~4860 RPM */
#define LS_OVER2LS0_SPEED14     Q15(0.29)   /* ~5020 RPM */
#define LS_OVER2LS0_SPEED15     Q15(0.28)   /* ~5180 RPM */
#define LS_OVER2LS0_SPEED16     Q15(0.27)   /* ~5340 RPM */
#define LS_OVER2LS0_SPEED17     Q15(0.26)   /* ~5500 RPM */

// </editor-fold>
    
#ifdef __cplusplus
}
#endif

#endif /* __USERPARMS_H */
