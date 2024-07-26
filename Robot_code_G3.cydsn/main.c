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
// test andrew branch
#include "project.h"
void print_uart(int num);

CY_ISR(ReadTimer);

// Interrupt routine for displaying each sections
int time =0;
CY_ISR(ReadTimer) 
{
    time = (int)(Timer_1_ReadPeriod()-Timer_1_ReadCounter());
    
    print_uart(time);
    CyDelay(5);
    UART_1_PutString("\n");
    CyDelay(5);
    Timer_1_Start();
    
    Control_Reg_1_Write(1); // Reset counter to do more pulses
    CyDelay(5);
    Control_Reg_1_Write(0); // Reset counter to do more pulses
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    PWM_1_Start();
    
    Control_Reg_1_Write(0); // Reset counter to do more pulses
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    // Setup Colour sensor
    colour_S0_Write(1);
    colour_S1_Write(1);
    
    colour_S2_Write(1);
    colour_S3_Write(1);
    
    
    Timer_1_SetInterruptMode(3);
    Timer_1_Start();
    
    // Setup UART
    UART_1_Start();
    UART_1_PutString("Start UART");
    CyDelay(5);
    UART_1_WriteTxData(0x20);   
    CyDelay(5);
    
    isr_1_ClearPending(); 
    isr_1_StartEx(ReadTimer);  
    
    
    for(;;)
    {
        /*for(int i=0;i<=255;i++)
        {
        PWM_1_WriteCompare(i);
        CyDelay(100);
        
        }*/
        //print_uart(15);
       // time = Timer_1_ReadCounter();
       // print_uart(time);
    //CyDelay(5);
    //UART_1_WriteTxData(0x20);  
    //UART_1_PutString("\n");
    //CyDelay(5);
        /* Place your application code here. */
        //LED_Write(0);
        //CyDelay(500);
        //LED_Write(1);
       // CyDelay(500);
    }
}


uint8 value_text[16];
void print_uart(int num)
{ 
    int num_digits = 0;
    int temp = num;
    
    // Count the number of digits (optional for pre-allocation)
    while (temp != 0) {
        temp /= 10;
        num_digits++;
    }

    for (int i = num_digits-1; i >= 0; --i) {
        value_text[i] = num % 10;
        num /= 10;
    }
    
    for (int i = 0; i < num_digits; ++i) {
        UART_1_WriteTxData(value_text[i]+0x30); //display number
        CyDelay(5);
    }
    
}
/* [] END OF FILE */
