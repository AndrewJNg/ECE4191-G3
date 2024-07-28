import serial
import numpy as np


ser = serial.Serial("COM3", 57600)
start = 2000
stop = 25000
step = 500
readings = 8000
j_counter = 0
step_resp = True

if step_resp == False:
    # Here use 100 readings other wise will take too long.
    data_array = np.array([[0 for _ in range(readings)] for _ in range(start, stop, step)], dtype=np.float64)

    for j in range(start, stop, step):
        for i in range(readings):
            data = ser.readline().decode("utf-8").split("\n")[0]
            data_array[j_counter, i] = float(data)
        j_counter += 1
    print(data_array)
    np.save("motor_velocity_characteristics.npy", data_array)
else:
    data_array = np.zeros((readings))
    for i in range(readings):
        
        data = ser.readline().decode("utf-8").split("\n")[0]
        data_array[i] = data
    np.save("motor_step_characteristics.npy", data_array)