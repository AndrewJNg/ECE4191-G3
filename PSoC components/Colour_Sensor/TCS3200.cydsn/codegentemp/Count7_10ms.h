/*******************************************************************************
* File Name: Count7_10ms.h
* Version 1.0
*
* Description:
*  This header file contains registers and constants associated with the
*  Count7 component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_COUNT7_Count7_10ms_H)
#define CY_COUNT7_Count7_10ms_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
    #include "CyLib.h"
#else
    #include "syslib/cy_syslib.h"
#endif


/***************************************
* Function Prototypes
***************************************/
void  Count7_10ms_Init(void) ;
void  Count7_10ms_Enable(void) ;
void  Count7_10ms_Start(void) ;
void  Count7_10ms_Stop(void) ;
void  Count7_10ms_WriteCounter(uint8 count) ;
uint8 Count7_10ms_ReadCounter(void) ;
void  Count7_10ms_WritePeriod(uint8 period) ;
uint8 Count7_10ms_ReadPeriod(void) ;
void  Count7_10ms_SaveConfig(void) ;
void  Count7_10ms_RestoreConfig(void) ;
void  Count7_10ms_Sleep(void) ;
void  Count7_10ms_Wakeup(void) ;


/***************************************
*     Data Struct Definitions
***************************************/
/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;
    uint8 count;
} Count7_10ms_BACKUP_STRUCT;


/***************************************
*           Global Variables
***************************************/
extern Count7_10ms_BACKUP_STRUCT Count7_10ms_backup;
extern uint8 Count7_10ms_initVar;


/***************************************
* Initial Parameter
***************************************/
#define Count7_10ms_INITIAL_PERIOD               (99u)


/***************************************
* Registers
***************************************/
#define Count7_10ms_COUNT_REG                    (*(reg8 *) Count7_10ms_Counter7__COUNT_REG)
#define Count7_10ms_COUNT_PTR                    ( (reg8 *) Count7_10ms_Counter7__COUNT_REG)
#define Count7_10ms_PERIOD_REG                   (*(reg8 *) Count7_10ms_Counter7__PERIOD_REG)
#define Count7_10ms_PERIOD_PTR                   ( (reg8 *) Count7_10ms_Counter7__PERIOD_REG)
#define Count7_10ms_AUX_CONTROL_REG              (*(reg8 *) Count7_10ms_Counter7__CONTROL_AUX_CTL_REG)
#define Count7_10ms_AUX_CONTROL_PTR              ( (reg8 *) Count7_10ms_Counter7__CONTROL_AUX_CTL_REG)


/***************************************
* Register Constants
***************************************/
#define Count7_10ms_COUNTER_START                (0x20u)

/* This constant is used to mask the TC bit (bit#7) in the Count register */
#define Count7_10ms_COUNT_7BIT_MASK              (0x7Fu)


#endif /* CY_COUNT7_Count7_10ms_H */


/* [] END OF FILE */
