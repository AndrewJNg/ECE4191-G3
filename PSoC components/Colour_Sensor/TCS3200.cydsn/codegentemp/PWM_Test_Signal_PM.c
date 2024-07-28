/*******************************************************************************
* File Name: PWM_Test_Signal_PM.c
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

#include "PWM_Test_Signal.h"

static PWM_Test_Signal_backupStruct PWM_Test_Signal_backup;


/*******************************************************************************
* Function Name: PWM_Test_Signal_SaveConfig
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
*  PWM_Test_Signal_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Test_Signal_SaveConfig(void) 
{

    #if(!PWM_Test_Signal_UsingFixedFunction)
        #if(!PWM_Test_Signal_PWMModeIsCenterAligned)
            PWM_Test_Signal_backup.PWMPeriod = PWM_Test_Signal_ReadPeriod();
        #endif /* (!PWM_Test_Signal_PWMModeIsCenterAligned) */
        PWM_Test_Signal_backup.PWMUdb = PWM_Test_Signal_ReadCounter();
        #if (PWM_Test_Signal_UseStatus)
            PWM_Test_Signal_backup.InterruptMaskValue = PWM_Test_Signal_STATUS_MASK;
        #endif /* (PWM_Test_Signal_UseStatus) */

        #if(PWM_Test_Signal_DeadBandMode == PWM_Test_Signal__B_PWM__DBM_256_CLOCKS || \
            PWM_Test_Signal_DeadBandMode == PWM_Test_Signal__B_PWM__DBM_2_4_CLOCKS)
            PWM_Test_Signal_backup.PWMdeadBandValue = PWM_Test_Signal_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Test_Signal_KillModeMinTime)
             PWM_Test_Signal_backup.PWMKillCounterPeriod = PWM_Test_Signal_ReadKillTime();
        #endif /* (PWM_Test_Signal_KillModeMinTime) */

        #if(PWM_Test_Signal_UseControl)
            PWM_Test_Signal_backup.PWMControlRegister = PWM_Test_Signal_ReadControlRegister();
        #endif /* (PWM_Test_Signal_UseControl) */
    #endif  /* (!PWM_Test_Signal_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Test_Signal_RestoreConfig
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
*  PWM_Test_Signal_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Test_Signal_RestoreConfig(void) 
{
        #if(!PWM_Test_Signal_UsingFixedFunction)
            #if(!PWM_Test_Signal_PWMModeIsCenterAligned)
                PWM_Test_Signal_WritePeriod(PWM_Test_Signal_backup.PWMPeriod);
            #endif /* (!PWM_Test_Signal_PWMModeIsCenterAligned) */

            PWM_Test_Signal_WriteCounter(PWM_Test_Signal_backup.PWMUdb);

            #if (PWM_Test_Signal_UseStatus)
                PWM_Test_Signal_STATUS_MASK = PWM_Test_Signal_backup.InterruptMaskValue;
            #endif /* (PWM_Test_Signal_UseStatus) */

            #if(PWM_Test_Signal_DeadBandMode == PWM_Test_Signal__B_PWM__DBM_256_CLOCKS || \
                PWM_Test_Signal_DeadBandMode == PWM_Test_Signal__B_PWM__DBM_2_4_CLOCKS)
                PWM_Test_Signal_WriteDeadTime(PWM_Test_Signal_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Test_Signal_KillModeMinTime)
                PWM_Test_Signal_WriteKillTime(PWM_Test_Signal_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Test_Signal_KillModeMinTime) */

            #if(PWM_Test_Signal_UseControl)
                PWM_Test_Signal_WriteControlRegister(PWM_Test_Signal_backup.PWMControlRegister);
            #endif /* (PWM_Test_Signal_UseControl) */
        #endif  /* (!PWM_Test_Signal_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Test_Signal_Sleep
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
*  PWM_Test_Signal_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Test_Signal_Sleep(void) 
{
    #if(PWM_Test_Signal_UseControl)
        if(PWM_Test_Signal_CTRL_ENABLE == (PWM_Test_Signal_CONTROL & PWM_Test_Signal_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Test_Signal_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Test_Signal_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Test_Signal_UseControl) */

    /* Stop component */
    PWM_Test_Signal_Stop();

    /* Save registers configuration */
    PWM_Test_Signal_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Test_Signal_Wakeup
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
*  PWM_Test_Signal_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Test_Signal_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Test_Signal_RestoreConfig();

    if(PWM_Test_Signal_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Test_Signal_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
