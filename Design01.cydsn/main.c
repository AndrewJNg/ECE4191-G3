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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// These are constants of the motor.
double V_max = 25000; // The maximum voltage we use.
double v_max = 4010; //ticks per second
double a_max = 30000; // ticks per second^2
double min_interval = 0.01; //seconds
double K_offset = 2000; // 2000 pwm compare
double K_velocity = 6.23441;
double K_acceleration = 0.0037696;
/*
double K_velocity = V_max / v_max; // Vmax / velocity_max
double K_acceleration = V_max / a_max;
*/

int data = 0;
char printstr[50];

void convertToPWM(double velocity, double acceleration)
{
    // Uses the V = K_offs + K_v * velocity + K_a * acceleration to convert the velocity and acceleration requirement
    // to a voltage requirement and transmit to the motor.
    //data = PWM_1_ReadPeriod();
    //sprintf(printstr, "%d\n", data);
    

    int pwm_value = (int) (K_offset + K_velocity * velocity + K_acceleration * acceleration);
    Motor_OUT1_Write((pwm_value > 0) ? 1 : 0);
    Motor_OUT1_Write((pwm_value < 0) ? 1 : 0);
    PWM_1_WriteCompare(abs(pwm_value));
}

void feedForwardTrajectory(double s_req)
{
    double ta = 0;
    double T = 0;
    double tc = 0;
    double t_cf = 0;
    double Vm = sqrt(s_req * a_max); 
    int milli_delay = (int) (min_interval * 1000);
    if (Vm <= v_max){
        ta = Vm / a_max;
        T = 2 * ta;
    }else{
        ta = v_max / a_max;
        tc = s_req / v_max - ta;
        T = 2 * ta + tc;
        t_cf = T - ta;
    }
    double t = 0;
    double i_double = 0.0; // I thinnk this would be better than casting to double each loop
    for ( int i = 0 ; i < (int) (T / min_interval) + 1; i++){
        t = (double) (i * min_interval);
        data = QuadDec_1_GetCounter();
        if ( t < ta ){
            convertToPWM(a_max * t, a_max);
        }
        else if ( Vm <= v_max ){
            // There is no constant speed region
            // So we start decelerating
            convertToPWM(Vm - a_max * (t - ta), -a_max);
        }else if ( (t >= ta) && (t <= t_cf) ){
            // There is constant speed region
            // And we are in the constant speed region
            convertToPWM(v_max, 0);
        }else if (t > t_cf){
            // There is a constant speed region(Vm > Vmax to come here) and we are in the deceleratin region
            convertToPWM(v_max - (t - t_cf) * a_max, -a_max);
        }
        CyDelay(milli_delay); 
    }
    PWM_1_WriteCompare(0);
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    QuadDec_1_Start();
    PWM_1_Start();
    UART_1_Start();
    Motor_OUT1_Write(0);
    Motor_OUT2_Write(1);
    PWM_1_WriteCompare(0);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    feedForwardTrajectory(12560.0);
    
    int counter = QuadDec_1_GetCounter();
    char transmit_str[20];
    sprintf(transmit_str, "\n%d\n", counter);
    UART_1_PutString(transmit_str);
    CyDelay(100);
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
