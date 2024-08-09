/*******************************************************************************
* File Name: GlobalClock.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_GlobalClock_H)
#define CY_TIMER_GlobalClock_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 GlobalClock_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define GlobalClock_Resolution                 32u
#define GlobalClock_UsingFixedFunction         0u
#define GlobalClock_UsingHWCaptureCounter      0u
#define GlobalClock_SoftwareCaptureMode        0u
#define GlobalClock_SoftwareTriggerMode        0u
#define GlobalClock_UsingHWEnable              0u
#define GlobalClock_EnableTriggerMode          0u
#define GlobalClock_InterruptOnCaptureCount    0u
#define GlobalClock_RunModeUsed                0u
#define GlobalClock_ControlRegRemoved          0u

#if defined(GlobalClock_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define GlobalClock_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (GlobalClock_UsingFixedFunction)
    #define GlobalClock_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define GlobalClock_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End GlobalClock_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!GlobalClock_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (GlobalClock_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!GlobalClock_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}GlobalClock_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    GlobalClock_Start(void) ;
void    GlobalClock_Stop(void) ;

void    GlobalClock_SetInterruptMode(uint8 interruptMode) ;
uint8   GlobalClock_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define GlobalClock_GetInterruptSource() GlobalClock_ReadStatusRegister()

#if(!GlobalClock_UDB_CONTROL_REG_REMOVED)
    uint8   GlobalClock_ReadControlRegister(void) ;
    void    GlobalClock_WriteControlRegister(uint8 control) ;
#endif /* (!GlobalClock_UDB_CONTROL_REG_REMOVED) */

uint32  GlobalClock_ReadPeriod(void) ;
void    GlobalClock_WritePeriod(uint32 period) ;
uint32  GlobalClock_ReadCounter(void) ;
void    GlobalClock_WriteCounter(uint32 counter) ;
uint32  GlobalClock_ReadCapture(void) ;
void    GlobalClock_SoftwareCapture(void) ;

#if(!GlobalClock_UsingFixedFunction) /* UDB Prototypes */
    #if (GlobalClock_SoftwareCaptureMode)
        void    GlobalClock_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!GlobalClock_UsingFixedFunction) */

    #if (GlobalClock_SoftwareTriggerMode)
        void    GlobalClock_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (GlobalClock_SoftwareTriggerMode) */

    #if (GlobalClock_EnableTriggerMode)
        void    GlobalClock_EnableTrigger(void) ;
        void    GlobalClock_DisableTrigger(void) ;
    #endif /* (GlobalClock_EnableTriggerMode) */


    #if(GlobalClock_InterruptOnCaptureCount)
        void    GlobalClock_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (GlobalClock_InterruptOnCaptureCount) */

    #if (GlobalClock_UsingHWCaptureCounter)
        void    GlobalClock_SetCaptureCount(uint8 captureCount) ;
        uint8   GlobalClock_ReadCaptureCount(void) ;
    #endif /* (GlobalClock_UsingHWCaptureCounter) */

    void GlobalClock_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void GlobalClock_Init(void)          ;
void GlobalClock_Enable(void)        ;
void GlobalClock_SaveConfig(void)    ;
void GlobalClock_RestoreConfig(void) ;
void GlobalClock_Sleep(void)         ;
void GlobalClock_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define GlobalClock__B_TIMER__CM_NONE 0
#define GlobalClock__B_TIMER__CM_RISINGEDGE 1
#define GlobalClock__B_TIMER__CM_FALLINGEDGE 2
#define GlobalClock__B_TIMER__CM_EITHEREDGE 3
#define GlobalClock__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define GlobalClock__B_TIMER__TM_NONE 0x00u
#define GlobalClock__B_TIMER__TM_RISINGEDGE 0x04u
#define GlobalClock__B_TIMER__TM_FALLINGEDGE 0x08u
#define GlobalClock__B_TIMER__TM_EITHEREDGE 0x0Cu
#define GlobalClock__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define GlobalClock_INIT_PERIOD             4294967295u
#define GlobalClock_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << GlobalClock_CTRL_CAP_MODE_SHIFT))
#define GlobalClock_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << GlobalClock_CTRL_TRIG_MODE_SHIFT))
#if (GlobalClock_UsingFixedFunction)
    #define GlobalClock_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << GlobalClock_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << GlobalClock_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define GlobalClock_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << GlobalClock_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << GlobalClock_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << GlobalClock_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (GlobalClock_UsingFixedFunction) */
#define GlobalClock_INIT_CAPTURE_COUNT      (2u)
#define GlobalClock_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << GlobalClock_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (GlobalClock_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define GlobalClock_STATUS         (*(reg8 *) GlobalClock_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define GlobalClock_STATUS_MASK    (*(reg8 *) GlobalClock_TimerHW__SR0 )
    #define GlobalClock_CONTROL        (*(reg8 *) GlobalClock_TimerHW__CFG0)
    #define GlobalClock_CONTROL2       (*(reg8 *) GlobalClock_TimerHW__CFG1)
    #define GlobalClock_CONTROL2_PTR   ( (reg8 *) GlobalClock_TimerHW__CFG1)
    #define GlobalClock_RT1            (*(reg8 *) GlobalClock_TimerHW__RT1)
    #define GlobalClock_RT1_PTR        ( (reg8 *) GlobalClock_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define GlobalClock_CONTROL3       (*(reg8 *) GlobalClock_TimerHW__CFG2)
        #define GlobalClock_CONTROL3_PTR   ( (reg8 *) GlobalClock_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define GlobalClock_GLOBAL_ENABLE  (*(reg8 *) GlobalClock_TimerHW__PM_ACT_CFG)
    #define GlobalClock_GLOBAL_STBY_ENABLE  (*(reg8 *) GlobalClock_TimerHW__PM_STBY_CFG)

    #define GlobalClock_CAPTURE_LSB         (* (reg16 *) GlobalClock_TimerHW__CAP0 )
    #define GlobalClock_CAPTURE_LSB_PTR       ((reg16 *) GlobalClock_TimerHW__CAP0 )
    #define GlobalClock_PERIOD_LSB          (* (reg16 *) GlobalClock_TimerHW__PER0 )
    #define GlobalClock_PERIOD_LSB_PTR        ((reg16 *) GlobalClock_TimerHW__PER0 )
    #define GlobalClock_COUNTER_LSB         (* (reg16 *) GlobalClock_TimerHW__CNT_CMP0 )
    #define GlobalClock_COUNTER_LSB_PTR       ((reg16 *) GlobalClock_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define GlobalClock_BLOCK_EN_MASK                     GlobalClock_TimerHW__PM_ACT_MSK
    #define GlobalClock_BLOCK_STBY_EN_MASK                GlobalClock_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define GlobalClock_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define GlobalClock_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define GlobalClock_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define GlobalClock_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define GlobalClock_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define GlobalClock_CTRL_ENABLE                        ((uint8)((uint8)0x01u << GlobalClock_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define GlobalClock_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define GlobalClock_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << GlobalClock_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define GlobalClock_CTRL_MODE_SHIFT                 0x01u
        #define GlobalClock_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << GlobalClock_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define GlobalClock_CTRL_RCOD_SHIFT        0x02u
        #define GlobalClock_CTRL_ENBL_SHIFT        0x00u
        #define GlobalClock_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define GlobalClock_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << GlobalClock_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define GlobalClock_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << GlobalClock_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define GlobalClock_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << GlobalClock_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define GlobalClock_CTRL_RCOD       ((uint8)((uint8)0x03u << GlobalClock_CTRL_RCOD_SHIFT))
        #define GlobalClock_CTRL_ENBL       ((uint8)((uint8)0x80u << GlobalClock_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define GlobalClock_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define GlobalClock_RT1_MASK                        ((uint8)((uint8)0x03u << GlobalClock_RT1_SHIFT))
    #define GlobalClock_SYNC                            ((uint8)((uint8)0x03u << GlobalClock_RT1_SHIFT))
    #define GlobalClock_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define GlobalClock_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << GlobalClock_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define GlobalClock_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << GlobalClock_SYNCDSI_SHIFT))

    #define GlobalClock_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << GlobalClock_CTRL_MODE_SHIFT))
    #define GlobalClock_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << GlobalClock_CTRL_MODE_SHIFT))
    #define GlobalClock_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << GlobalClock_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define GlobalClock_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define GlobalClock_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define GlobalClock_STATUS_TC_INT_MASK_SHIFT        (GlobalClock_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define GlobalClock_STATUS_CAPTURE_INT_MASK_SHIFT   (GlobalClock_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define GlobalClock_STATUS_TC                       ((uint8)((uint8)0x01u << GlobalClock_STATUS_TC_SHIFT))
    #define GlobalClock_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << GlobalClock_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define GlobalClock_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << GlobalClock_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define GlobalClock_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << GlobalClock_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define GlobalClock_STATUS              (* (reg8 *) GlobalClock_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define GlobalClock_STATUS_MASK         (* (reg8 *) GlobalClock_TimerUDB_rstSts_stsreg__MASK_REG)
    #define GlobalClock_STATUS_AUX_CTRL     (* (reg8 *) GlobalClock_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define GlobalClock_CONTROL             (* (reg8 *) GlobalClock_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(GlobalClock_Resolution <= 8u) /* 8-bit Timer */
        #define GlobalClock_CAPTURE_LSB         (* (reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define GlobalClock_CAPTURE_LSB_PTR       ((reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define GlobalClock_PERIOD_LSB          (* (reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define GlobalClock_PERIOD_LSB_PTR        ((reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define GlobalClock_COUNTER_LSB         (* (reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define GlobalClock_COUNTER_LSB_PTR       ((reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(GlobalClock_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define GlobalClock_CAPTURE_LSB         (* (reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define GlobalClock_CAPTURE_LSB_PTR       ((reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define GlobalClock_PERIOD_LSB          (* (reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define GlobalClock_PERIOD_LSB_PTR        ((reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define GlobalClock_COUNTER_LSB         (* (reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define GlobalClock_COUNTER_LSB_PTR       ((reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define GlobalClock_CAPTURE_LSB         (* (reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define GlobalClock_CAPTURE_LSB_PTR       ((reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define GlobalClock_PERIOD_LSB          (* (reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define GlobalClock_PERIOD_LSB_PTR        ((reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define GlobalClock_COUNTER_LSB         (* (reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define GlobalClock_COUNTER_LSB_PTR       ((reg16 *) GlobalClock_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(GlobalClock_Resolution <= 24u)/* 24-bit Timer */
        #define GlobalClock_CAPTURE_LSB         (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define GlobalClock_CAPTURE_LSB_PTR       ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define GlobalClock_PERIOD_LSB          (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define GlobalClock_PERIOD_LSB_PTR        ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define GlobalClock_COUNTER_LSB         (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define GlobalClock_COUNTER_LSB_PTR       ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define GlobalClock_CAPTURE_LSB         (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define GlobalClock_CAPTURE_LSB_PTR       ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define GlobalClock_PERIOD_LSB          (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define GlobalClock_PERIOD_LSB_PTR        ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define GlobalClock_COUNTER_LSB         (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define GlobalClock_COUNTER_LSB_PTR       ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define GlobalClock_CAPTURE_LSB         (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define GlobalClock_CAPTURE_LSB_PTR       ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define GlobalClock_PERIOD_LSB          (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define GlobalClock_PERIOD_LSB_PTR        ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define GlobalClock_COUNTER_LSB         (* (reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define GlobalClock_COUNTER_LSB_PTR       ((reg32 *) GlobalClock_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define GlobalClock_COUNTER_LSB_PTR_8BIT       ((reg8 *) GlobalClock_TimerUDB_sT32_timerdp_u0__A0_REG )
    
    #if (GlobalClock_UsingHWCaptureCounter)
        #define GlobalClock_CAP_COUNT              (*(reg8 *) GlobalClock_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define GlobalClock_CAP_COUNT_PTR          ( (reg8 *) GlobalClock_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define GlobalClock_CAPTURE_COUNT_CTRL     (*(reg8 *) GlobalClock_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define GlobalClock_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) GlobalClock_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (GlobalClock_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define GlobalClock_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define GlobalClock_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define GlobalClock_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define GlobalClock_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define GlobalClock_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define GlobalClock_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << GlobalClock_CTRL_INTCNT_SHIFT))
    #define GlobalClock_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << GlobalClock_CTRL_TRIG_MODE_SHIFT))
    #define GlobalClock_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << GlobalClock_CTRL_TRIG_EN_SHIFT))
    #define GlobalClock_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << GlobalClock_CTRL_CAP_MODE_SHIFT))
    #define GlobalClock_CTRL_ENABLE                    ((uint8)((uint8)0x01u << GlobalClock_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define GlobalClock_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define GlobalClock_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define GlobalClock_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define GlobalClock_STATUS_TC_INT_MASK_SHIFT       GlobalClock_STATUS_TC_SHIFT
    #define GlobalClock_STATUS_CAPTURE_INT_MASK_SHIFT  GlobalClock_STATUS_CAPTURE_SHIFT
    #define GlobalClock_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define GlobalClock_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define GlobalClock_STATUS_FIFOFULL_INT_MASK_SHIFT GlobalClock_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define GlobalClock_STATUS_TC                      ((uint8)((uint8)0x01u << GlobalClock_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define GlobalClock_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << GlobalClock_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define GlobalClock_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << GlobalClock_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define GlobalClock_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << GlobalClock_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define GlobalClock_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << GlobalClock_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define GlobalClock_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << GlobalClock_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define GlobalClock_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << GlobalClock_STATUS_FIFOFULL_SHIFT))

    #define GlobalClock_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define GlobalClock_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define GlobalClock_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define GlobalClock_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define GlobalClock_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define GlobalClock_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_GlobalClock_H */


/* [] END OF FILE */
