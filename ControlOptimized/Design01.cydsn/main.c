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
#include <ctype.h>

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

typedef struct motionParams{
    float ta;
    float tc;
    float tcf;
    float T;
    float Vm;
    float s_req;
    int control_period;
    float velocity;
    float acceleration;
    float time_step;
    int32 previous_timestep;
} motionParams;


int32 millis()
{
    return (GlobalClock_ReadPeriod() - GlobalClock_ReadCounter());   
}


void setupFeedForwardController(motionParams* motion)
{
    // Call this before every major action of the controller, since it will 
    // calculate the controller parameters.
    motion->Vm = sqrt(motion->s_req * accel_max);
    if (motion->Vm <= velocity_max){
        motion->ta = motion->Vm / accel_max;
        motion->T = 2 * motion->ta;
        motion->tc = 0;
        motion->tcf = 0;
    }else{
        motion->ta = velocity_max / accel_max;
        motion->tc = motion->s_req / velocity_max - motion->ta;
        motion->T = 2 * motion->ta + motion->tc;
        motion->tcf = motion->T - motion->ta;
    }
}

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
    
void stopMotion(FeedForward* ff1, FeedForward* ff2, PID* pid1, PID* pid2)
{
    ff1->control_signal = 0;
    ff2->control_signal = 0;
    pid1->control_signal = 0;
    pid2->control_signal = 0;
    writePWM(ff1, pid1, ff2, pid2);
}

void feedForwardTrajectory(motionParams* motion, FeedForward* ff1, FeedForward* ff2, PID* pid1, PID* pid2)
{
    if ( (millis() - motion->previous_timestep) >= motion->control_period ){
        motion->time_step += control_period;
        motion->previous_timestep = millis();
        pid1->current_encoder_val = QuadDec_1_GetCounter();
        pid2->current_encoder_val = QuadDec_2_GetCounter();

        if (motion->time_step < motion->ta){
            motion->velocity = accel_max * motion->time_step;
            motion->acceleration = accel_max;
        } else if (motion->Vm <= velocity_max){
            motion->velocity = motion->Vm - accel_max * (motion->time_step - motion->ta);
            motion->acceleration = -accel_max;
        } else if ((motion->time_step >= motion->ta) && (motion->time_step <= motion->tcf)){
            motion->velocity = velocity_max;
            motion->acceleration = 0;
        } else if (motion->time_step > motion->tcf){
            motion->velocity = velocity_max - (motion->time_step - motion->tcf) * accel_max;
            motion->acceleration = -accel_max;
        } else {
            // To reach here we have passed the full duration of the feed forward action, we can stop the motor now.
            stopMotion(ff1, ff2, pid1, pid2);
        }
        // Update the control blocks with new control signals
        pidControl(pid1, motion->velocity, (pid1->current_encoder_val - pid1->prev_encoder_val) * pid1->control_freq);
        pidControl(pid2, motion->velocity, (pid2->current_encoder_val - pid2->prev_encoder_val) * pid2->control_freq); 
        feedForwardControl(ff1, motion->velocity, motion->acceleration);
        feedForwardControl(ff2, motion->velocity, motion->acceleration);
        // Write the new signals
        writePWM(ff1, pid1, ff2, pid2);
        // Update the previous encoder readings
        pid1->prev_encoder_val = pid1->current_encoder_val;
        pid2->prev_encoder_val = pid2->current_encoder_val;
    }
    
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

void getData(char* data)
{
    size_t data_size = UART_2_GetRxBufferSize();
    uint i;
    for (i = 0 ; i < data_size ; i++){
        data[i] = UART_2_GetByte();
    }
    data[i] = '\0';
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    UART_2_Start();
    GlobalClock_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PID pid1, pid2;
    FeedForward ff1, ff2;
    motionParams motion_parameters;

    startMotorSystem(&pid1, &pid2, &ff1, &ff2);
    
    int32 counter1 = QuadDec_1_GetCounter();
    int32 counter2 = QuadDec_2_GetCounter();
    char transmit_str[50];
    char rx_data[50];
    int i;
    char data_string[20];
    for(;;)
    {
        i = UART_2_ReadRxStatus();
        size_t data_size = (size_t)UART_2_GetRxBufferSize();
        if (UART_2_GetRxBufferSize() > 0)
        {
            getData(rx_data);
            sprintf(transmit_str, "Data Recieved: %s\n", rx_data);
            UART_2_PutString(transmit_str);
            for (int i = 0 ; rx_data[i] != '\0' ; i++){
                data_string[i] = rx_data[i + 1];
            }
            sprintf(transmit_str, "Value Recieved: %.2f\n", atof(data_string));
            UART_2_PutString(transmit_str);
            trajectory_plan(atof(data_string) * 4, atof(data_string) * 4, &ff1, &ff2, &pid1, &pid2);
            setupFeedForwardController(&motion_parameters);        
            // For testing only, reset the quad decoders
        }
        feedForwardTrajectory(&motion_parameters, &ff1, &ff2, &pid1, &pid2);
        /* Place your application code here. */
        CyDelay(100);
    }

}

/* [] END OF FILE */
