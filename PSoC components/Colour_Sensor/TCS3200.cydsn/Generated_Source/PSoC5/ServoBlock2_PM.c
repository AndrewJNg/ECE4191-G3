/*******************************************************************************
* File Name: ServoBlock2_PM.c
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

#include "ServoBlock2.h"

static ServoBlock2_backupStruct ServoBlock2_backup;


/*******************************************************************************
* Function Name: ServoBlock2_SaveConfig
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
*  ServoBlock2_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void ServoBlock2_SaveConfig(void) 
{

    #if(!ServoBlock2_UsingFixedFunction)
        #if(!ServoBlock2_PWMModeIsCenterAligned)
            ServoBlock2_backup.PWMPeriod = ServoBlock2_ReadPeriod();
        #endif /* (!ServoBlock2_PWMModeIsCenterAligned) */
        ServoBlock2_backup.PWMUdb = ServoBlock2_ReadCounter();
        #if (ServoBlock2_UseStatus)
            ServoBlock2_backup.InterruptMaskValue = ServoBlock2_STATUS_MASK;
        #endif /* (ServoBlock2_UseStatus) */

        #if(ServoBlock2_DeadBandMode == ServoBlock2__B_PWM__DBM_256_CLOCKS || \
            ServoBlock2_DeadBandMode == ServoBlock2__B_PWM__DBM_2_4_CLOCKS)
            ServoBlock2_backup.PWMdeadBandValue = ServoBlock2_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(ServoBlock2_KillModeMinTime)
             ServoBlock2_backup.PWMKillCounterPeriod = ServoBlock2_ReadKillTime();
        #endif /* (ServoBlock2_KillModeMinTime) */

        #if(ServoBlock2_UseControl)
            ServoBlock2_backup.PWMControlRegister = ServoBlock2_ReadControlRegister();
        #endif /* (ServoBlock2_UseControl) */
    #endif  /* (!ServoBlock2_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: ServoBlock2_RestoreConfig
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
*  ServoBlock2_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void ServoBlock2_RestoreConfig(void) 
{
        #if(!ServoBlock2_UsingFixedFunction)
            #if(!ServoBlock2_PWMModeIsCenterAligned)
                ServoBlock2_WritePeriod(ServoBlock2_backup.PWMPeriod);
            #endif /* (!ServoBlock2_PWMModeIsCenterAligned) */

            ServoBlock2_WriteCounter(ServoBlock2_backup.PWMUdb);

            #if (ServoBlock2_UseStatus)
                ServoBlock2_STATUS_MASK = ServoBlock2_backup.InterruptMaskValue;
            #endif /* (ServoBlock2_UseStatus) */

            #if(ServoBlock2_DeadBandMode == ServoBlock2__B_PWM__DBM_256_CLOCKS || \
                ServoBlock2_DeadBandMode == ServoBlock2__B_PWM__DBM_2_4_CLOCKS)
                ServoBlock2_WriteDeadTime(ServoBlock2_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(ServoBlock2_KillModeMinTime)
                ServoBlock2_WriteKillTime(ServoBlock2_backup.PWMKillCounterPeriod);
            #endif /* (ServoBlock2_KillModeMinTime) */

            #if(ServoBlock2_UseControl)
                ServoBlock2_WriteControlRegister(ServoBlock2_backup.PWMControlRegister);
            #endif /* (ServoBlock2_UseControl) */
        #endif  /* (!ServoBlock2_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: ServoBlock2_Sleep
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
*  ServoBlock2_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void ServoBlock2_Sleep(void) 
{
    #if(ServoBlock2_UseControl)
        if(ServoBlock2_CTRL_ENABLE == (ServoBlock2_CONTROL & ServoBlock2_CTRL_ENABLE))
        {
            /*Component is enabled */
            ServoBlock2_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            ServoBlock2_backup.PWMEnableState = 0u;
        }
    #endif /* (ServoBlock2_UseControl) */

    /* Stop component */
    ServoBlock2_Stop();

    /* Save registers configuration */
    ServoBlock2_SaveConfig();
}


/*******************************************************************************
* Function Name: ServoBlock2_Wakeup
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
*  ServoBlock2_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ServoBlock2_Wakeup(void) 
{
     /* Restore registers values */
    ServoBlock2_RestoreConfig();

    if(ServoBlock2_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        ServoBlock2_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
