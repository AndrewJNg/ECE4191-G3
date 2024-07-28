/*******************************************************************************
* File Name: Pulse_counter_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Pulse_counter.h"

static Pulse_counter_backupStruct Pulse_counter_backup;


/*******************************************************************************
* Function Name: Pulse_counter_SaveConfig
********************************************************************************
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
*  Pulse_counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Pulse_counter_SaveConfig(void) 
{
    #if (!Pulse_counter_UsingFixedFunction)

        Pulse_counter_backup.CounterUdb = Pulse_counter_ReadCounter();

        #if(!Pulse_counter_ControlRegRemoved)
            Pulse_counter_backup.CounterControlRegister = Pulse_counter_ReadControlRegister();
        #endif /* (!Pulse_counter_ControlRegRemoved) */

    #endif /* (!Pulse_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pulse_counter_RestoreConfig
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
*  Pulse_counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Pulse_counter_RestoreConfig(void) 
{      
    #if (!Pulse_counter_UsingFixedFunction)

       Pulse_counter_WriteCounter(Pulse_counter_backup.CounterUdb);

        #if(!Pulse_counter_ControlRegRemoved)
            Pulse_counter_WriteControlRegister(Pulse_counter_backup.CounterControlRegister);
        #endif /* (!Pulse_counter_ControlRegRemoved) */

    #endif /* (!Pulse_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pulse_counter_Sleep
********************************************************************************
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
*  Pulse_counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Pulse_counter_Sleep(void) 
{
    #if(!Pulse_counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Pulse_counter_CTRL_ENABLE == (Pulse_counter_CONTROL & Pulse_counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Pulse_counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Pulse_counter_backup.CounterEnableState = 0u;
        }
    #else
        Pulse_counter_backup.CounterEnableState = 1u;
        if(Pulse_counter_backup.CounterEnableState != 0u)
        {
            Pulse_counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!Pulse_counter_ControlRegRemoved) */
    
    Pulse_counter_Stop();
    Pulse_counter_SaveConfig();
}


/*******************************************************************************
* Function Name: Pulse_counter_Wakeup
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
*  Pulse_counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Pulse_counter_Wakeup(void) 
{
    Pulse_counter_RestoreConfig();
    #if(!Pulse_counter_ControlRegRemoved)
        if(Pulse_counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Pulse_counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Pulse_counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
