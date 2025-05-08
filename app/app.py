import serial
import time


ser = serial.Serial('/dev/ttyACM1', 2000000, timeout=10)
time.sleep(2) 

def send(i):
    res = bytes([i])
    print(' '.join(f'{byte:02x}' for byte in res))

    ser.write(res)
    # data_bytes = ser.read()
    # print(' '.join(f'{byte:02x}' for byte in data_bytes))
    print()

for i in range(1,50):
    send(0)
    time.sleep(1)
    send(100)
    time.sleep(1)



ser.close()