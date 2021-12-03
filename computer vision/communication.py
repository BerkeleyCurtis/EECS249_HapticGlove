import serial
import time

arduino = serial.Serial(port='/dev/cu.usbmodem144101', baudrate=115200, timeout=.1)


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    
    #time.sleep(0.05)
    data = arduino.readline()
    return data



back = None
num = str(0)+ "\n"
back =  write_read(num)
time.sleep(1)

# while True:
#     num = input("Enter a number: ")
#     #num = num.encode()
#     value = write_read(num)

#     print(value)



for value in range(0,180, 1):
    if back is not None:
        back = None
        num = str(value) + "\n"
        back = write_read(num)
        #time.sleep(0.05)




# value = write_read(num)
# print(value)



