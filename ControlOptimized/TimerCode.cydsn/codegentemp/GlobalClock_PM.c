/*******************************************************************************
* File Name: GlobalClock_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "GlobalClock.h"

static GlobalClock_backupStruct GlobalClock_backup;


/*******************************************************************************
* Function Name: GlobalClock_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GlobalClock_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void GlobalClock_SaveConfig(void) 
{
    #if (!GlobalClock_UsingFixedFunction)
        GlobalClock_backup.TimerUdb = GlobalClock_ReadCounter();
        GlobalClock_backup.InterruptMaskValue = GlobalClock_STATUS_MASK;
        #if (GlobalClock_UsingHWCaptureCounter)
            GlobalClock_backup.TimerCaptureCounter = GlobalClock_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!GlobalClock_UDB_CONTROL_REG_REMOVED)
            GlobalClock_backup.TimerControlRegister = GlobalClock_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: GlobalClock_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GlobalClock_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void GlobalClock_RestoreConfig(void) 
{   
    #if (!GlobalClock_UsingFixedFunction)

        GlobalClock_WriteCounter(GlobalClock_backup.TimerUdb);
        GlobalClock_STATUS_MASK =GlobalClock_backup.InterruptMaskValue;
        #if (GlobalClock_UsingHWCaptureCounter)
            GlobalClock_SetCaptureCount(GlobalClock_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!GlobalClock_UDB_CONTROL_REG_REMOVED)
            GlobalClock_WriteControlRegister(GlobalClock_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: GlobalClock_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GlobalClock_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void GlobalClock_Sleep(void) 
{
    #if(!GlobalClock_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(GlobalClock_CTRL_ENABLE == (GlobalClock_CONTROL & GlobalClock_CTRL_ENABLE))
        {
            /* Timer is enabled */
            GlobalClock_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            GlobalClock_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    GlobalClock_Stop();
    GlobalClock_SaveConfig();
}


/*******************************************************************************
* Function Name: GlobalClock_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  GlobalClock_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void GlobalClock_Wakeup(void) 
{
    GlobalClock_RestoreConfig();
    #if(!GlobalClock_UDB_CONTROL_REG_REMOVED)
        if(GlobalClock_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                GlobalClock_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
