/*******************************************************************************
* File Name: ServoBlock1_PM.c
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

#include "ServoBlock1.h"

static ServoBlock1_backupStruct ServoBlock1_backup;


/*******************************************************************************
* Function Name: ServoBlock1_SaveConfig
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
*  ServoBlock1_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void ServoBlock1_SaveConfig(void) 
{

    #if(!ServoBlock1_UsingFixedFunction)
        #if(!ServoBlock1_PWMModeIsCenterAligned)
            ServoBlock1_backup.PWMPeriod = ServoBlock1_ReadPeriod();
        #endif /* (!ServoBlock1_PWMModeIsCenterAligned) */
        ServoBlock1_backup.PWMUdb = ServoBlock1_ReadCounter();
        #if (ServoBlock1_UseStatus)
            ServoBlock1_backup.InterruptMaskValue = ServoBlock1_STATUS_MASK;
        #endif /* (ServoBlock1_UseStatus) */

        #if(ServoBlock1_DeadBandMode == ServoBlock1__B_PWM__DBM_256_CLOCKS || \
            ServoBlock1_DeadBandMode == ServoBlock1__B_PWM__DBM_2_4_CLOCKS)
            ServoBlock1_backup.PWMdeadBandValue = ServoBlock1_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(ServoBlock1_KillModeMinTime)
             ServoBlock1_backup.PWMKillCounterPeriod = ServoBlock1_ReadKillTime();
        #endif /* (ServoBlock1_KillModeMinTime) */

        #if(ServoBlock1_UseControl)
            ServoBlock1_backup.PWMControlRegister = ServoBlock1_ReadControlRegister();
        #endif /* (ServoBlock1_UseControl) */
    #endif  /* (!ServoBlock1_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: ServoBlock1_RestoreConfig
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
*  ServoBlock1_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void ServoBlock1_RestoreConfig(void) 
{
        #if(!ServoBlock1_UsingFixedFunction)
            #if(!ServoBlock1_PWMModeIsCenterAligned)
                ServoBlock1_WritePeriod(ServoBlock1_backup.PWMPeriod);
            #endif /* (!ServoBlock1_PWMModeIsCenterAligned) */

            ServoBlock1_WriteCounter(ServoBlock1_backup.PWMUdb);

            #if (ServoBlock1_UseStatus)
                ServoBlock1_STATUS_MASK = ServoBlock1_backup.InterruptMaskValue;
            #endif /* (ServoBlock1_UseStatus) */

            #if(ServoBlock1_DeadBandMode == ServoBlock1__B_PWM__DBM_256_CLOCKS || \
                ServoBlock1_DeadBandMode == ServoBlock1__B_PWM__DBM_2_4_CLOCKS)
                ServoBlock1_WriteDeadTime(ServoBlock1_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(ServoBlock1_KillModeMinTime)
                ServoBlock1_WriteKillTime(ServoBlock1_backup.PWMKillCounterPeriod);
            #endif /* (ServoBlock1_KillModeMinTime) */

            #if(ServoBlock1_UseControl)
                ServoBlock1_WriteControlRegister(ServoBlock1_backup.PWMControlRegister);
            #endif /* (ServoBlock1_UseControl) */
        #endif  /* (!ServoBlock1_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: ServoBlock1_Sleep
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
*  ServoBlock1_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void ServoBlock1_Sleep(void) 
{
    #if(ServoBlock1_UseControl)
        if(ServoBlock1_CTRL_ENABLE == (ServoBlock1_CONTROL & ServoBlock1_CTRL_ENABLE))
        {
            /*Component is enabled */
            ServoBlock1_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            ServoBlock1_backup.PWMEnableState = 0u;
        }
    #endif /* (ServoBlock1_UseControl) */

    /* Stop component */
    ServoBlock1_Stop();

    /* Save registers configuration */
    ServoBlock1_SaveConfig();
}


/*******************************************************************************
* Function Name: ServoBlock1_Wakeup
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
*  ServoBlock1_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ServoBlock1_Wakeup(void) 
{
     /* Restore registers values */
    ServoBlock1_RestoreConfig();

    if(ServoBlock1_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        ServoBlock1_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
