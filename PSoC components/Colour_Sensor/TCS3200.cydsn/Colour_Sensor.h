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


// Colour sensor functions 
uint32 colourSensor_Single_Scan(int S2,int S3);
CY_ISR(colour_sensor_isr);
void colourSensor_Full_Scan();
uint32 map(uint32 x, uint32 in_min, uint32 in_max, uint32 out_min, uint32 out_max);
    
    
/* [] END OF FILE */
