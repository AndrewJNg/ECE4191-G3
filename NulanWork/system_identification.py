import numpy as np
import matplotlib.pyplot as plt

# data = np.load("motor_velocity_characteristics.npy")
data = np.load("motor_step_characteristics.npy")
# print(data)

# data = data[:, 20:]
# print(data.shape)


# avg_data = np.mean(data, axis=1)
# print(avg_data)
# pwm_values = np.arange(2000, 25000, 500)
# print(pwm_values.shape)


plt.plot(data)
# plt.plot(pwm_values, avg_data)
plt.ylabel("Step value")
plt.xlabel("2.856e-6s")
plt.title("Step response full speed")
plt.show()