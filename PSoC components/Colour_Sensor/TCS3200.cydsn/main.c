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
#include <stdbool.h>
#include <stdio.h>

uint32 value = 0;
CY_ISR(colour_sensor_isr){
    //Control_Reg_1_Write(1);

    //Counter_1_ReadStatusRegister();
    //isr_1_Stop();
    //value= Counter_1_ReadCapture();
    
    value= Counter_1_ReadCounter(); 
    
    //LED_Write(~LED_1_Read());
    
    
    Control_Reg_1_Write(1);
    CyDelay(5);
    Control_Reg_1_Write(0);
    
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    
    PWM_1_Start();
    PWM_2_Start();
    Counter_1_Start();
    
    Count7_1_Start();
    
    isr_1_ClearPending(); 
    isr_1_StartEx(colour_sensor_isr);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        
    //LED_Write(!LED_1_Read());
    //CyDelay(500);
    
      /*  
    Counter_1_Start();
    PWM_1_Start();
    PWM_2_Start();
        
    uint32 red = Counter_1_ReadCapture(); 
    uint32 blue = 0;
    uint32 clear = 0;
    uint32 green = 0;
    
    Control_Reg_1_Write(1);
    CyDelay(1);
    Control_Reg_1_Write(0);
    
    char str[50];
    sprintf(str, "red: %lu\tgreen: %lu\tblue: %lu\tclear: %lu\t \n", red, green, blue, clear);
    UART_1_PutString(str);
    
    
    Control_Reg_1_Write(1);
    PWM_1_Stop();
    PWM_2_Stop();
    Counter_1_Stop();
    
    CyDelay(500);
    */
        
    uint32 red = Counter_1_ReadCounter(); 
    uint32 blue = value;
    uint32 clear = 0;
    uint32 green = 0;
    
    char str[50];
    sprintf(str, "red: %lu\tgreen: %lu\tblue: %lu\tclear: %lu\t \n", red, green, blue, clear);
    UART_1_PutString(str);
        
    CyDelay(1000);
    
        
    }
}



/* [] END OF FILE */
