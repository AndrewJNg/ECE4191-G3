/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>

int32 current_counter = 0;

int32 millis()
{
    return (GlobalClock_ReadPeriod() - GlobalClock_ReadCounter());   
}

int main(void)
{
    GlobalClock_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    char data[50];
    for(;;)
    {
        sprintf(data, "timer:%ld\n", millis());
        UART_1_PutString(data);
        CyDelay(100);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
