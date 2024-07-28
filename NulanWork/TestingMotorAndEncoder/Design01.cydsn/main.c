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
#include <math.h>
#include <stdlib.h>

#define MIN_PWM 3750

int counter = 0;
char transmit_str[20];

/* Testing config for the motor:
    OUT1 -> PIN5 on ribbon
    OUT2 -> PIN6 on ribbon
    IN1 -> 1.5 on PSOC --> MOTOR_IN_1
    IN2 -> 1.4 on PSOC --> MOTOR_IN_2
    PIN1 on ribbon -> 0.6 on PSOC --> PHASE_A
    PIN2 on ribbon -> 0.7 on PSOC --> PHASE_B
    
    IF MOTOR_IN_1 == 1 & MOTOR_IN_2 == 0 ---> Quadrature Counter is negative(CCW)
    IF MOTOR_IN_1 == 1 & MOTOR_IN_2 == 0 ---> Quadrature Counter is positive(CW)

*/


void motorWrite(int speed)
{
    // For reference (condition) ? value if condition is true : value if condition is false
    Motor_IN_1_Write( (speed < 0) ? 1 : 0); // if speed is negative ie we need to make the counter negative enable in1
    Motor_IN_2_Write( (speed > 0) ? 1 : 0); // if speed is positive ie we need to make the counter negative enable in2
    speed = abs(speed);
    
    if (speed > PWM_1_ReadPeriod())
        speed =  PWM_1_ReadPeriod();
    if (speed < MIN_PWM)
        speed = MIN_PWM;
    
    PWM_1_WriteCompare(speed);

}

void reachCount(int count)
{
    // Given a certain tick count, it reaches this count.
    
    int currentCount = QuadDec_1_GetCounter();
    sprintf(transmit_str, "--%d\n", currentCount);
    UART_1_PutString(transmit_str);
    motorWrite(0); // Stop the motor
    while (currentCount != count){
        motorWrite( count - currentCount );
        currentCount = QuadDec_1_GetCounter();
        
    }
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_1_Start();
    UART_1_Start();
    QuadDec_1_Start();
    PWM_1_WriteCompare(0);
    Motor_IN_1_Write(0);
    Motor_IN_2_Write(1);
    int run = 1;
    double speed = 0;
    int prevcounter = 0;
    int readings = 8000;
    double velocities[readings];
    int i = 0;
    int temp = 0;
    int delay = 2;
    CyDelay(1000);
    
    for (int j = 2000 ; j <= 25000 ; j = j + 500)
    {
        speed = 0;
        PWM_1_WriteCompare(j);
	    for (int i = 0 ; i < readings ; i++){
		    counter = QuadDec_1_GetCounter();
            velocities[i] = (double) (counter - prevcounter);
		    CyDelay(delay); // Comment this for the step response.
		    prevcounter = counter;
	    }
        for (int i = 0 ; i < readings ; i++){
            sprintf(transmit_str, "%.2f\n",  velocities[i] / ((double) delay / 1000));
            UART_1_PutString(transmit_str);
            CyDelay(1);
        }

    }
    PWM_1_WriteCompare(0);
    for(;;)
    {
        /* Place your application code here. */
    }
}




/* [] END OF FILE */
