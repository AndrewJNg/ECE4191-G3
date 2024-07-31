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
double v_max = 3900 * 4; //ticks per second
double a_max = 30000 * 4; // ticks per second^2
double min_interval = 0.004; //seconds
double K_offset = 2000; // 2000 pwm compare
double K_velocity = 6.23441 / 4;
double K_acceleration = 0.0037696 / 4;
double Kp = 0.0565969969;
double Kd = 0.000390726;
double Ki = 1.43001626;
int pwm_value = 0;
/*
double K_velocity = V_max / v_max; // Vmax / velocity_max
double K_acceleration = V_max / a_max;
*/

double measured_speed = 0.0;
char printstr[50];

int feedForward(double velocity, double acceleration)
{
    
    return (int) (K_offset + K_velocity * velocity + K_acceleration * acceleration);
}


int error=0;
int error_old=0;
int cummulative_error=0;

int PidControl(double current_speed, double target_speed)
{
    error = target_speed - current_speed;
    int pid_pwm_value = Kp*error + Ki*(cummulative_error) + Kd*(error-error_old) ;
    error_old = error;
    cummulative_error += error;
    
    return pid_pwm_value;
}

void WritePWM(double velocity, double acceleration)
{
    // Uses the V = K_offs + K_v * velocity + K_a * acceleration to convert the velocity and acceleration requirement
    // to a voltage requirement and transmit to the motor.
    //data = PWM_1_ReadPeriod();
    //sprintf(printstr, "%d\n", data);
    // This one combines both the feed forward and the feedback control part.
    // u = velocity, error = 
    
    pwm_value = feedForward(velocity, acceleration) + PidControl(measured_speed, velocity);
    //pwm_value = PidControl(measured_speed, velocity);
    //pwm_value = feedForward(velocity, acceleration);
    
    //pwm_value = (int) (K_offset + K_velocity * velocity + K_acceleration * acceleration); 
    //sprintf(printstr, "%d\n", pwm_value);
    //UART_1_PutString(printstr);
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
    //sprintf(printstr, "%.2f\n", T / min_interval);
    //UART_1_PutString(printstr);
    double t = 0;
    double i_double = 0.0; // I thinnk this would be better than casting to double each loop
    int32 prev_val = 0;
    int32 counter = 0;
    //double data_to_trans[(int) (T / min_interval)][2];
    double v = 0.0;
    for ( int i = 0 ; i < (int) (T / min_interval) + 1; i++){
        t = (double) (i * min_interval); //optimize this
        counter = QuadDec_1_GetCounter();
        measured_speed = (double) (counter - prev_val) / min_interval; //optimize this
        //data_to_trans[i][0] = measured_speed;
        
        if ( t < ta ){
            //v = a_max * t;
            WritePWM(a_max * t, a_max);
        }
        else if ( Vm <= v_max ){
            // There is no constant speed region
            // So we start decelerating
            //v = Vm - a_max * (t - ta);
            WritePWM(Vm - a_max * (t - ta), -a_max);
        }else if ( (t >= ta) && (t <= t_cf) ){
            // There is constant speed region
            // And we are in the constant speed region
            //v = v_max;
            WritePWM(v_max, 0);
        }else if (t > t_cf){
            // There is a constant speed region(Vm > Vmax to come here) and we are in the deceleratin region
            //v = v_max - (t - t_cf) * a_max;
            WritePWM(v_max - (t - t_cf) * a_max, -a_max);
        }
        //data_to_trans[i][1] = v;
        prev_val = counter;
        CyDelay(milli_delay); 
    }
    PWM_1_WriteCompare(0);
/*    
    for (int i = 0 ; i < (int) (T / min_interval) + 1; i++){
        //sprintf(printstr, "%.2f,%.2f\n", data_to_trans[i][0], data_to_trans[i][1]);
        //UART_1_PutString(printstr);
        //CyDelay(1);
    }
*/  
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
    QuadDec_1_SetCounter(0);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //////////////////////////////////////////////////////////////////////
    // Change the Kd to include the timestep;
    Kd = Kd * 1 / min_interval;
    //////////////////////////////////////////////////////////////////////
    feedForwardTrajectory(1222.4 * 10 * 4);
    
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
