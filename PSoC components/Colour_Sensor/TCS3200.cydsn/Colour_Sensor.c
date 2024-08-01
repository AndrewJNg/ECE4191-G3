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
#include "project.h" // PSoC Creator generated header for project-specific definitions
#include <stdbool.h>
#include <stdio.h>

#include "Colour_Sensor.h"



uint32 map(uint32 x, uint32 in_min, uint32 in_max, uint32 out_min, uint32 out_max) {
    // limit output to 0 or 255 when it is out of desired range
    if (x <=in_min) return out_min;
    else if (x >=in_max) return out_max;
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Colour sensor functions
void Colour_Sensor_Setup()
{
    // Colour sensor setup
    Pulse_counter_Start();
    Count7_10ms_Start();
    freq_isr_ClearPending(); 
    freq_isr_StartEx(colour_sensor_isr);
}

void colourSensor_Full_Scan()
{
    // Calibration values
    uint32 red   = map(colourSensor_Single_Scan(0,0)  ,120,200   ,0,100);  // min=120 max=260
    uint32 blue  = map(colourSensor_Single_Scan(0,1)  ,120,200   ,0,100);  // min=100 max=275
    uint32 clear =  map(colourSensor_Single_Scan(1,0) ,100,1500  ,0,100);  // min=100 max=275
    uint32 green = map(colourSensor_Single_Scan(1,1)  ,80 ,180   ,0,100);  // min=80  max=170
    
    
    ////////////////////////////////////////////
    // Printing 
    char str[50];
    sprintf(str, "red: %lu     \t green: %lu\t blue: %lu  \t clear: %lu \t", red, green, blue, clear);
    UART_1_PutString(str);
    
    ////////////////////////////////////////////
    if((red > blue) && (red>green))
        sprintf(str, "red \n");
    else if((blue > red) && (blue>green))
        sprintf(str, "blue \n");
    else if((green> red) && (green>blue))
        sprintf(str, "green \n");
    else
        sprintf(str, "no answer \n");
    
    UART_1_PutString(str);
}


uint32 value = 0;
bool colour_sensor_ready = 0;
CY_ISR(colour_sensor_isr){
    value= Pulse_counter_ReadCounter();
    colour_sensor_ready =1;
    
}

uint32 colourSensor_Single_Scan(int S2,int S3)
{
    Control_Reg_1_Write(1);
    
    ColourSensor_S2_Write(S2);
    ColourSensor_S3_Write(S3);
    colour_sensor_ready =0;
    
    CyDelay(5);
    Control_Reg_1_Write(0);
    
    while(colour_sensor_ready==0)
    {
        // there is an expected 10ms dead zone here
    }
    CyDelay(10);
    return value;
}
/* [] END OF FILE */
