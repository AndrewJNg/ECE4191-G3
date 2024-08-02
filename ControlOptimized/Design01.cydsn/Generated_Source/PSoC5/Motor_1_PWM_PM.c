/*******************************************************************************
* File Name: Motor_1_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Motor_1_PWM.h"

static Motor_1_PWM_backupStruct Motor_1_PWM_backup;


/*******************************************************************************
* Function Name: Motor_1_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Motor_1_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Motor_1_PWM_SaveConfig(void) 
{

    #if(!Motor_1_PWM_UsingFixedFunction)
        #if(!Motor_1_PWM_PWMModeIsCenterAligned)
            Motor_1_PWM_backup.PWMPeriod = Motor_1_PWM_ReadPeriod();
        #endif /* (!Motor_1_PWM_PWMModeIsCenterAligned) */
        Motor_1_PWM_backup.PWMUdb = Motor_1_PWM_ReadCounter();
        #if (Motor_1_PWM_UseStatus)
            Motor_1_PWM_backup.InterruptMaskValue = Motor_1_PWM_STATUS_MASK;
        #endif /* (Motor_1_PWM_UseStatus) */

        #if(Motor_1_PWM_DeadBandMode == Motor_1_PWM__B_PWM__DBM_256_CLOCKS || \
            Motor_1_PWM_DeadBandMode == Motor_1_PWM__B_PWM__DBM_2_4_CLOCKS)
            Motor_1_PWM_backup.PWMdeadBandValue = Motor_1_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Motor_1_PWM_KillModeMinTime)
             Motor_1_PWM_backup.PWMKillCounterPeriod = Motor_1_PWM_ReadKillTime();
        #endif /* (Motor_1_PWM_KillModeMinTime) */

        #if(Motor_1_PWM_UseControl)
            Motor_1_PWM_backup.PWMControlRegister = Motor_1_PWM_ReadControlRegister();
        #endif /* (Motor_1_PWM_UseControl) */
    #endif  /* (!Motor_1_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Motor_1_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Motor_1_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Motor_1_PWM_RestoreConfig(void) 
{
        #if(!Motor_1_PWM_UsingFixedFunction)
            #if(!Motor_1_PWM_PWMModeIsCenterAligned)
                Motor_1_PWM_WritePeriod(Motor_1_PWM_backup.PWMPeriod);
            #endif /* (!Motor_1_PWM_PWMModeIsCenterAligned) */

            Motor_1_PWM_WriteCounter(Motor_1_PWM_backup.PWMUdb);

            #if (Motor_1_PWM_UseStatus)
                Motor_1_PWM_STATUS_MASK = Motor_1_PWM_backup.InterruptMaskValue;
            #endif /* (Motor_1_PWM_UseStatus) */

            #if(Motor_1_PWM_DeadBandMode == Motor_1_PWM__B_PWM__DBM_256_CLOCKS || \
                Motor_1_PWM_DeadBandMode == Motor_1_PWM__B_PWM__DBM_2_4_CLOCKS)
                Motor_1_PWM_WriteDeadTime(Motor_1_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Motor_1_PWM_KillModeMinTime)
                Motor_1_PWM_WriteKillTime(Motor_1_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Motor_1_PWM_KillModeMinTime) */

            #if(Motor_1_PWM_UseControl)
                Motor_1_PWM_WriteControlRegister(Motor_1_PWM_backup.PWMControlRegister);
            #endif /* (Motor_1_PWM_UseControl) */
        #endif  /* (!Motor_1_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Motor_1_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Motor_1_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Motor_1_PWM_Sleep(void) 
{
    #if(Motor_1_PWM_UseControl)
        if(Motor_1_PWM_CTRL_ENABLE == (Motor_1_PWM_CONTROL & Motor_1_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Motor_1_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Motor_1_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Motor_1_PWM_UseControl) */

    /* Stop component */
    Motor_1_PWM_Stop();

    /* Save registers configuration */
    Motor_1_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Motor_1_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Motor_1_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Motor_1_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Motor_1_PWM_RestoreConfig();

    if(Motor_1_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Motor_1_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
