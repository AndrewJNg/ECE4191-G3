/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARECount7_1ms
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

uint32 colourSensor_Scan_freq(int S2,int S3);
CY_ISR(colour_sensor_isr);


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    
    // Test Signal setup
    PWM_Test_Signal_Start();
    
    // Colour sensor setup
    Pulse_counter_Start();
    Count7_1ms_Start();
    freq_isr_ClearPending(); 
    freq_isr_StartEx(colour_sensor_isr);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    for(;;)
    {
        
    //ColourSensor_S2_Write(1);
    //ColourSensor_S3_Write(1);
    //uint32 red = Pulse_counter_ReadCounter(); 
    //uint32 blue = value;
    //uint32 clear = 0;
    //uint32 green = 0;
    
    uint32 red   = colourSensor_Scan_freq(0,0);
    uint32 blue  = colourSensor_Scan_freq(0,1);
    uint32 clear = colourSensor_Scan_freq(1,0);
    uint32 green = colourSensor_Scan_freq(1,1);
    
    
    char str[50];
    sprintf(str, "red: %lu\tgreen: %lu\tblue: %lu\tclear: %lu\t", red, green, blue, clear);
    UART_1_PutString(str);
    
    if((red > blue) && (red>green))
        sprintf(str, "red \n");
    else if((blue > red) && (blue>green))
        sprintf(str, "blue \n");
    else if((green> red) && (green>blue))
        sprintf(str, "green \n");
    else
        sprintf(str, "no answer \n");
    
    
    UART_1_PutString(str);
    
        
    //CyDelay(1000);
    
        
    }
}



bool colour_sensor_ready = 0;
CY_ISR(colour_sensor_isr){
    value= Pulse_counter_ReadCounter();
    colour_sensor_ready =1;
    
}

uint32 colourSensor_Scan_freq(int S2,int S3)
{
    Control_Reg_1_Write(1);
    
    ColourSensor_S2_Write(S2);
    ColourSensor_S3_Write(S3);
    colour_sensor_ready =0;
    
    CyDelay(5);
    Control_Reg_1_Write(0);
    
    while(colour_sensor_ready==0)
    {
        
    }
    CyDelay(10);
    
    
    return value;
}


/* [] END OF FILE */
