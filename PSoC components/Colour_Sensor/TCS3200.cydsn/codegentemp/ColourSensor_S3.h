/*******************************************************************************
* File Name: ColourSensor_S3.h  
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

#if !defined(CY_PINS_ColourSensor_S3_H) /* Pins ColourSensor_S3_H */
#define CY_PINS_ColourSensor_S3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ColourSensor_S3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ColourSensor_S3__PORT == 15 && ((ColourSensor_S3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ColourSensor_S3_Write(uint8 value);
void    ColourSensor_S3_SetDriveMode(uint8 mode);
uint8   ColourSensor_S3_ReadDataReg(void);
uint8   ColourSensor_S3_Read(void);
void    ColourSensor_S3_SetInterruptMode(uint16 position, uint16 mode);
uint8   ColourSensor_S3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ColourSensor_S3_SetDriveMode() function.
     *  @{
     */
        #define ColourSensor_S3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ColourSensor_S3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ColourSensor_S3_DM_RES_UP          PIN_DM_RES_UP
        #define ColourSensor_S3_DM_RES_DWN         PIN_DM_RES_DWN
        #define ColourSensor_S3_DM_OD_LO           PIN_DM_OD_LO
        #define ColourSensor_S3_DM_OD_HI           PIN_DM_OD_HI
        #define ColourSensor_S3_DM_STRONG          PIN_DM_STRONG
        #define ColourSensor_S3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ColourSensor_S3_MASK               ColourSensor_S3__MASK
#define ColourSensor_S3_SHIFT              ColourSensor_S3__SHIFT
#define ColourSensor_S3_WIDTH              1u

/* Interrupt constants */
#if defined(ColourSensor_S3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ColourSensor_S3_SetInterruptMode() function.
     *  @{
     */
        #define ColourSensor_S3_INTR_NONE      (uint16)(0x0000u)
        #define ColourSensor_S3_INTR_RISING    (uint16)(0x0001u)
        #define ColourSensor_S3_INTR_FALLING   (uint16)(0x0002u)
        #define ColourSensor_S3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ColourSensor_S3_INTR_MASK      (0x01u) 
#endif /* (ColourSensor_S3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ColourSensor_S3_PS                     (* (reg8 *) ColourSensor_S3__PS)
/* Data Register */
#define ColourSensor_S3_DR                     (* (reg8 *) ColourSensor_S3__DR)
/* Port Number */
#define ColourSensor_S3_PRT_NUM                (* (reg8 *) ColourSensor_S3__PRT) 
/* Connect to Analog Globals */                                                  
#define ColourSensor_S3_AG                     (* (reg8 *) ColourSensor_S3__AG)                       
/* Analog MUX bux enable */
#define ColourSensor_S3_AMUX                   (* (reg8 *) ColourSensor_S3__AMUX) 
/* Bidirectional Enable */                                                        
#define ColourSensor_S3_BIE                    (* (reg8 *) ColourSensor_S3__BIE)
/* Bit-mask for Aliased Register Access */
#define ColourSensor_S3_BIT_MASK               (* (reg8 *) ColourSensor_S3__BIT_MASK)
/* Bypass Enable */
#define ColourSensor_S3_BYP                    (* (reg8 *) ColourSensor_S3__BYP)
/* Port wide control signals */                                                   
#define ColourSensor_S3_CTL                    (* (reg8 *) ColourSensor_S3__CTL)
/* Drive Modes */
#define ColourSensor_S3_DM0                    (* (reg8 *) ColourSensor_S3__DM0) 
#define ColourSensor_S3_DM1                    (* (reg8 *) ColourSensor_S3__DM1)
#define ColourSensor_S3_DM2                    (* (reg8 *) ColourSensor_S3__DM2) 
/* Input Buffer Disable Override */
#define ColourSensor_S3_INP_DIS                (* (reg8 *) ColourSensor_S3__INP_DIS)
/* LCD Common or Segment Drive */
#define ColourSensor_S3_LCD_COM_SEG            (* (reg8 *) ColourSensor_S3__LCD_COM_SEG)
/* Enable Segment LCD */
#define ColourSensor_S3_LCD_EN                 (* (reg8 *) ColourSensor_S3__LCD_EN)
/* Slew Rate Control */
#define ColourSensor_S3_SLW                    (* (reg8 *) ColourSensor_S3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ColourSensor_S3_PRTDSI__CAPS_SEL       (* (reg8 *) ColourSensor_S3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ColourSensor_S3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ColourSensor_S3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ColourSensor_S3_PRTDSI__OE_SEL0        (* (reg8 *) ColourSensor_S3__PRTDSI__OE_SEL0) 
#define ColourSensor_S3_PRTDSI__OE_SEL1        (* (reg8 *) ColourSensor_S3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ColourSensor_S3_PRTDSI__OUT_SEL0       (* (reg8 *) ColourSensor_S3__PRTDSI__OUT_SEL0) 
#define ColourSensor_S3_PRTDSI__OUT_SEL1       (* (reg8 *) ColourSensor_S3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ColourSensor_S3_PRTDSI__SYNC_OUT       (* (reg8 *) ColourSensor_S3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ColourSensor_S3__SIO_CFG)
    #define ColourSensor_S3_SIO_HYST_EN        (* (reg8 *) ColourSensor_S3__SIO_HYST_EN)
    #define ColourSensor_S3_SIO_REG_HIFREQ     (* (reg8 *) ColourSensor_S3__SIO_REG_HIFREQ)
    #define ColourSensor_S3_SIO_CFG            (* (reg8 *) ColourSensor_S3__SIO_CFG)
    #define ColourSensor_S3_SIO_DIFF           (* (reg8 *) ColourSensor_S3__SIO_DIFF)
#endif /* (ColourSensor_S3__SIO_CFG) */

/* Interrupt Registers */
#if defined(ColourSensor_S3__INTSTAT)
    #define ColourSensor_S3_INTSTAT            (* (reg8 *) ColourSensor_S3__INTSTAT)
    #define ColourSensor_S3_SNAP               (* (reg8 *) ColourSensor_S3__SNAP)
    
	#define ColourSensor_S3_0_INTTYPE_REG 		(* (reg8 *) ColourSensor_S3__0__INTTYPE)
#endif /* (ColourSensor_S3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ColourSensor_S3_H */


/* [] END OF FILE */
