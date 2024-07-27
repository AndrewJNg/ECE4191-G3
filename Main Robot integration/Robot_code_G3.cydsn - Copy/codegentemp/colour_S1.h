/*******************************************************************************
* File Name: colour_S1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_colour_S1_H) /* Pins colour_S1_H */
#define CY_PINS_colour_S1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "colour_S1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 colour_S1__PORT == 15 && ((colour_S1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    colour_S1_Write(uint8 value);
void    colour_S1_SetDriveMode(uint8 mode);
uint8   colour_S1_ReadDataReg(void);
uint8   colour_S1_Read(void);
void    colour_S1_SetInterruptMode(uint16 position, uint16 mode);
uint8   colour_S1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the colour_S1_SetDriveMode() function.
     *  @{
     */
        #define colour_S1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define colour_S1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define colour_S1_DM_RES_UP          PIN_DM_RES_UP
        #define colour_S1_DM_RES_DWN         PIN_DM_RES_DWN
        #define colour_S1_DM_OD_LO           PIN_DM_OD_LO
        #define colour_S1_DM_OD_HI           PIN_DM_OD_HI
        #define colour_S1_DM_STRONG          PIN_DM_STRONG
        #define colour_S1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define colour_S1_MASK               colour_S1__MASK
#define colour_S1_SHIFT              colour_S1__SHIFT
#define colour_S1_WIDTH              1u

/* Interrupt constants */
#if defined(colour_S1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in colour_S1_SetInterruptMode() function.
     *  @{
     */
        #define colour_S1_INTR_NONE      (uint16)(0x0000u)
        #define colour_S1_INTR_RISING    (uint16)(0x0001u)
        #define colour_S1_INTR_FALLING   (uint16)(0x0002u)
        #define colour_S1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define colour_S1_INTR_MASK      (0x01u) 
#endif /* (colour_S1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define colour_S1_PS                     (* (reg8 *) colour_S1__PS)
/* Data Register */
#define colour_S1_DR                     (* (reg8 *) colour_S1__DR)
/* Port Number */
#define colour_S1_PRT_NUM                (* (reg8 *) colour_S1__PRT) 
/* Connect to Analog Globals */                                                  
#define colour_S1_AG                     (* (reg8 *) colour_S1__AG)                       
/* Analog MUX bux enable */
#define colour_S1_AMUX                   (* (reg8 *) colour_S1__AMUX) 
/* Bidirectional Enable */                                                        
#define colour_S1_BIE                    (* (reg8 *) colour_S1__BIE)
/* Bit-mask for Aliased Register Access */
#define colour_S1_BIT_MASK               (* (reg8 *) colour_S1__BIT_MASK)
/* Bypass Enable */
#define colour_S1_BYP                    (* (reg8 *) colour_S1__BYP)
/* Port wide control signals */                                                   
#define colour_S1_CTL                    (* (reg8 *) colour_S1__CTL)
/* Drive Modes */
#define colour_S1_DM0                    (* (reg8 *) colour_S1__DM0) 
#define colour_S1_DM1                    (* (reg8 *) colour_S1__DM1)
#define colour_S1_DM2                    (* (reg8 *) colour_S1__DM2) 
/* Input Buffer Disable Override */
#define colour_S1_INP_DIS                (* (reg8 *) colour_S1__INP_DIS)
/* LCD Common or Segment Drive */
#define colour_S1_LCD_COM_SEG            (* (reg8 *) colour_S1__LCD_COM_SEG)
/* Enable Segment LCD */
#define colour_S1_LCD_EN                 (* (reg8 *) colour_S1__LCD_EN)
/* Slew Rate Control */
#define colour_S1_SLW                    (* (reg8 *) colour_S1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define colour_S1_PRTDSI__CAPS_SEL       (* (reg8 *) colour_S1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define colour_S1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) colour_S1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define colour_S1_PRTDSI__OE_SEL0        (* (reg8 *) colour_S1__PRTDSI__OE_SEL0) 
#define colour_S1_PRTDSI__OE_SEL1        (* (reg8 *) colour_S1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define colour_S1_PRTDSI__OUT_SEL0       (* (reg8 *) colour_S1__PRTDSI__OUT_SEL0) 
#define colour_S1_PRTDSI__OUT_SEL1       (* (reg8 *) colour_S1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define colour_S1_PRTDSI__SYNC_OUT       (* (reg8 *) colour_S1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(colour_S1__SIO_CFG)
    #define colour_S1_SIO_HYST_EN        (* (reg8 *) colour_S1__SIO_HYST_EN)
    #define colour_S1_SIO_REG_HIFREQ     (* (reg8 *) colour_S1__SIO_REG_HIFREQ)
    #define colour_S1_SIO_CFG            (* (reg8 *) colour_S1__SIO_CFG)
    #define colour_S1_SIO_DIFF           (* (reg8 *) colour_S1__SIO_DIFF)
#endif /* (colour_S1__SIO_CFG) */

/* Interrupt Registers */
#if defined(colour_S1__INTSTAT)
    #define colour_S1_INTSTAT            (* (reg8 *) colour_S1__INTSTAT)
    #define colour_S1_SNAP               (* (reg8 *) colour_S1__SNAP)
    
	#define colour_S1_0_INTTYPE_REG 		(* (reg8 *) colour_S1__0__INTTYPE)
#endif /* (colour_S1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_colour_S1_H */


/* [] END OF FILE */
