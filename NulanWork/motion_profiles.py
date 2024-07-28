import numpy as np
import matplotlib.pyplot as plt


V_max = 25000
s_req = 10000 # m
# These are either experimental values or can be calculated theoretically.
# For a test, the rated rpm is 19560rpm, rated power is 7.4W and rated torque is 0.00342Nm.
# 2057.74318rad/s and P = T*w
v_max = 4010 #ticks/s
a_max = 33160 #ticks/s^2
min_interval = 0.1 # interval between control actions.

# Vm calculation to determine if it is too short for a constant speed region.
Vm = np.sqrt(s_req * a_max)

if Vm <= v_max:
    # NO constant speed region
    ta = Vm / a_max
    T = ta*2
else:
    ta = v_max / a_max
    tc = s_req / v_max - ta
    T = 2*ta + tc
    t_cf = T - ta

t = np.linspace(0, T, int(T / min_interval))
v = np.zeros(t.shape)
s = np.zeros(t.shape)
a = np.zeros(t.shape)

print(ta, t_cf, T)

for i in range(1, t.shape[0]):
    if t[i] < ta:
        a[i] = a_max
        v[i] = a_max * t[i]
    
    else:
        if Vm <= v_max:
            # No constant speed region
            # And we are pass the first acceleration period, time to start deceleration
            a[i] = -a_max
            v[i] = Vm - a_max * (t[i] - ta)
        else:
            # Got a constant speed region
            if t[i] >= ta and t[i] <= t_cf:
                # within the constant speed region
                v[i] = v_max
            else:
                a[i] = -a_max
                v[i] = v_max - (t[i] - t_cf) * a_max
    
    s[i] = s[i - 1] + v[i] * min_interval + a[i] * min_interval ** 2 / 2

print(f"The final distance movVed: ", s[-1])

## Koffset = bias voltage the motor starts to run
## K_vel = can be found using the velocity vs voltage graph but the motor has reached ss.
## K_acc can also be found using the step response of the motor.
K_offset = 1000
K_velocity = V_max / v_max
K_acceleration = V_max / a_max * 0.005

voltage = (K_acceleration * a + K_velocity * v + K_offset) * 10 /25000

plt.subplot(2, 1, 1)
plt.plot(t, v)
plt.plot(t, s)
plt.plot(t, a)


plt.subplot(2, 1, 2)
plt.plot(t, voltage)

plt.show()