/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdbool.h>
#include <stdio.h>

#include "Colour_Sensor.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions listing

    // Global functions
    //uint32 map(uint32 x, uint32 in_min, uint32 in_max, uint32 out_min, uint32 out_max);

    //// Colour sensor functions 
    //uint32 colourSensor_Single_Scan(int S2,int S3);
    //CY_ISR(colour_sensor_isr);
    //void colourSensor_Full_Scan();

    // Servo functions
    void Servo_Setup();
    void servoDegree(float degrees_1,float degrees_2);
    void Colour_Sensor_Setup();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
int main(void)
{
    // Global setup
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    
    
    // Sensor setup
    Colour_Sensor_Setup();
    Servo_Setup();
    char str[50];
    sprintf(str, "Hello \n");
    UART_1_PutString(str);
    
    for(;;)
    {
    colourSensor_Full_Scan();
        
    /*
    servoDegree(0,180);
    CyDelay(1000);
    servoDegree(90,90);
    CyDelay(1000);
    servoDegree(180,0);
    CyDelay(1000);
    */
        
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global function
/*
uint32 map(uint32 x, uint32 in_min, uint32 in_max, uint32 out_min, uint32 out_max) {
    // limit output to 0 or 255 when it is out of desired range
    if (x <=in_min) return out_min;
    else if (x >=in_max) return out_max;
    
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Servo functions
void Servo_Setup()
{
    // Servo setup
    ServoBlock1_Start();
    ServoBlock2_Start();
}

void servoDegree(float degrees_1,float degrees_2)
{
    double pulse_1 = (degrees_1/180.0)*4000 +1000; 
    double pulse_2 = (degrees_2/180.0)*4000 +1000; 
    
    ServoBlock1_WriteCompare(pulse_1);
    ServoBlock2_WriteCompare(pulse_2);
}

