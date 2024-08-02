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

// Constants
float Voltage_max = 25000;
float velocity_max = 3900 * 4;
float accel_max = 30000 * 4;
float bias_voltage = 2000;
float K_velocity = 6.23441 / 4;
float K_acceleration = 0.0037696 / 4;
float Kp = 0.0565969969;
float Kd = 0.000390726;
float Ki = 1.43001626;
float control_period = 0.002;

typedef struct PID{
    float Kp;
    float Ki;
    float Kd;
    float intergral_error;
    float prev_error;
    float err;
    int control_signal;
    float control_freq;
    int multiplier;
    int32 current_encoder_val;
    int32 prev_encoder_val;
} PID;

typedef struct FeedForward{
    float multiplier;
    float control_signal;
    float K_offset;
    float K_velocity;
    float K_acceleration;
    
} FeedForward;

void pidControl(PID* pid, float target_speed, float measured_speed)
{
    pid->err = target_speed - measured_speed;
    pid->control_signal = pid->multiplier * (pid->Kp * pid->err + pid->Ki * pid->intergral_error + pid->Kd * (pid->err - pid->prev_error));
    pid->prev_error = pid->err;
    pid->intergral_error += pid->err;
}

void feedForwardControl(FeedForward* ff, float velocity, float acceleration){
       ff->control_signal = (ff->multiplier) * (ff->K_offset + ff->K_velocity * velocity + ff->K_acceleration * acceleration);
}

void writePWM(FeedForward* ff1, PID* pid1, FeedForward* ff2, PID* pid2)
{
    int pwm_value = ff1->control_signal + pid1->control_signal;
    Motor_1_IN1_Write((pwm_value > 0) ? 1 : 0);
    Motor_1_IN2_Write((pwm_value < 0) ? 1 : 0);
    Motor_1_PWM_WriteCompare(abs(pwm_value));
    
    pwm_value = ff2->control_signal + pid2->control_signal;
    Motor_2_IN1_Write((pwm_value > 0) ? 1 : 0);
    Motor_2_IN2_Write((pwm_value < 0) ? 1 : 0);
    Motor_2_PWM_WriteCompare(abs(pwm_value));
}
    
void feedForwardTrajectory(float s_req, FeedForward* ff1, FeedForward* ff2, PID* pid1, PID* pid2)
{
    float ta = 0;
    float T = 0;
    float tc = 0;
    float t_cf = 0;
    float Vm = sqrt(s_req * accel_max);
    int milliseconds_delay = control_period * 1000;
    if (Vm <= velocity_max){
        ta = Vm / accel_max;
        T = 2 * ta;
    } else {
        ta = velocity_max / accel_max;
        tc = s_req / velocity_max - ta;
        T = 2 * ta + tc;
        t_cf = T - ta;
    }
    
    double time_step = 0;
    double i_counter_double = 0.0;
    double ff_velocity = 0;
    double ff_accel = 0;
    int max_counts = (int) (T / control_period) + 1;
    for (int i = 0; i < max_counts + 1; i++){
        time_step = i_counter_double * control_period;
        pid1->current_encoder_val = QuadDec_1_GetCounter();
        pid2->current_encoder_val = QuadDec_2_GetCounter();
        
        if (time_step < ta){
            ff_velocity = accel_max * time_step;
            ff_accel = accel_max;
        } else if (Vm <= velocity_max){
            ff_velocity = Vm - accel_max * (time_step - ta);
            ff_accel = -accel_max;
        } else if ((time_step >= ta) && (time_step <= t_cf)){
            ff_velocity = velocity_max;
            ff_accel = 0;
        } else if (time_step > t_cf){
            ff_velocity = velocity_max - (time_step - t_cf) * accel_max;
            ff_accel = -accel_max;
        }
        // Update the control blocks with new control signals
        pidControl(pid1, ff_velocity, (pid1->current_encoder_val - pid1->prev_encoder_val) * pid1->control_freq);
        pidControl(pid2, ff_velocity, (pid2->current_encoder_val - pid2->prev_encoder_val) * pid2->control_freq); 
        feedForwardControl(ff1, ff_velocity, ff_accel);
        feedForwardControl(ff2, ff_velocity, ff_accel);
        // Write the new signals
        writePWM(ff1, pid1, ff2, pid2);
        // Update the previous encoder readings
        pid1->prev_encoder_val = pid1->current_encoder_val;
        pid2->prev_encoder_val = pid2->current_encoder_val;
        i_counter_double += 1;
        CyDelay(milliseconds_delay);
    }
    ff1->control_signal = 0;
    ff2->control_signal = 0;
    pid1->control_signal = 0;
    pid2->control_signal = 0;
    writePWM(ff1, pid1, ff2, pid2);
}

void trajectory_plan(float ticks_left, float ticks_right, FeedForward* ff1, FeedForward* ff2, PID* pid1, PID* pid2)
{
    pid1->multiplier = (ticks_left) > 0 ? 1 : -1;
    pid2->multiplier = (ticks_right) > 0 ? 1 : -1;
    ff1->multiplier = pid1->multiplier;
    ff2->multiplier = pid2->multiplier;
    pid1->current_encoder_val = 0;
    pid1->prev_encoder_val = 0;
    pid2->current_encoder_val = 0;
    pid2->prev_encoder_val = 0;
    
    
    feedForwardTrajectory(ticks_left, ff1, ff2, pid1, pid2);
}

void init_pid(PID* pid)
{
    pid->control_freq = 1 / control_period;
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd * pid->control_freq;
    pid->intergral_error = 0;
    pid->multiplier = 1;
    
    pid->prev_encoder_val = 0;
    pid->prev_error = 0;
}

void init_ff(FeedForward* ff)
{
    ff->K_offset = bias_voltage;
    ff->K_velocity = K_velocity;
    ff->K_acceleration = K_acceleration;
}

void startMotorSystem(PID* pid1, PID* pid2, FeedForward* ff1, FeedForward* ff2)
{
    QuadDec_1_Start();
    QuadDec_2_Start();
    Motor_1_PWM_Start();
    Motor_2_PWM_Start();
    
    init_pid(pid1);
    init_pid(pid2);
    init_ff(ff1);
    init_ff(ff2);
    QuadDec_1_SetCounter(0);
    QuadDec_2_SetCounter(0);
}   

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PID pid1, pid2;
    FeedForward ff1, ff2;
    UART_1_PutString("\nStarted\n");
    startMotorSystem(&pid1, &pid2, &ff1, &ff2);
    trajectory_plan(1222.4 * 10 * 4, 0, &ff1, &ff2, &pid1, &pid2);
    int counter = QuadDec_1_GetCounter();
    char transmit_str[20];
    sprintf(transmit_str, "\nCounter1:%d\n", counter);
    UART_1_PutString(transmit_str);
    counter = QuadDec_2_GetCounter();
    sprintf(transmit_str, "\nCounter2:%d\n", counter);
    UART_1_PutString(transmit_str);
    CyDelay(100);
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
