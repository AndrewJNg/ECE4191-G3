/*******************************************************************************
* File Name: Counter_pulse_counter_PM.c  
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

#include "Counter_pulse_counter.h"

static Counter_pulse_counter_backupStruct Counter_pulse_counter_backup;


/*******************************************************************************
* Function Name: Counter_pulse_counter_SaveConfig
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
*  Counter_pulse_counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_pulse_counter_SaveConfig(void) 
{
    #if (!Counter_pulse_counter_UsingFixedFunction)

        Counter_pulse_counter_backup.CounterUdb = Counter_pulse_counter_ReadCounter();

        #if(!Counter_pulse_counter_ControlRegRemoved)
            Counter_pulse_counter_backup.CounterControlRegister = Counter_pulse_counter_ReadControlRegister();
        #endif /* (!Counter_pulse_counter_ControlRegRemoved) */

    #endif /* (!Counter_pulse_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_pulse_counter_RestoreConfig
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
*  Counter_pulse_counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_pulse_counter_RestoreConfig(void) 
{      
    #if (!Counter_pulse_counter_UsingFixedFunction)

       Counter_pulse_counter_WriteCounter(Counter_pulse_counter_backup.CounterUdb);

        #if(!Counter_pulse_counter_ControlRegRemoved)
            Counter_pulse_counter_WriteControlRegister(Counter_pulse_counter_backup.CounterControlRegister);
        #endif /* (!Counter_pulse_counter_ControlRegRemoved) */

    #endif /* (!Counter_pulse_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_pulse_counter_Sleep
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
*  Counter_pulse_counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_pulse_counter_Sleep(void) 
{
    #if(!Counter_pulse_counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_pulse_counter_CTRL_ENABLE == (Counter_pulse_counter_CONTROL & Counter_pulse_counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_pulse_counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_pulse_counter_backup.CounterEnableState = 0u;
        }
    #else
        Counter_pulse_counter_backup.CounterEnableState = 1u;
        if(Counter_pulse_counter_backup.CounterEnableState != 0u)
        {
            Counter_pulse_counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_pulse_counter_ControlRegRemoved) */
    
    Counter_pulse_counter_Stop();
    Counter_pulse_counter_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_pulse_counter_Wakeup
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
*  Counter_pulse_counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_pulse_counter_Wakeup(void) 
{
    Counter_pulse_counter_RestoreConfig();
    #if(!Counter_pulse_counter_ControlRegRemoved)
        if(Counter_pulse_counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_pulse_counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_pulse_counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
