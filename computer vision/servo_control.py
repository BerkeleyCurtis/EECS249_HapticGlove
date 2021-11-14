import serial
import time

# input 1 / 2 to control the arduino servo

arduino = serial.Serial(port='/dev/cu.usbmodem144201', baudrate=115200, timeout=1)


while True:
    i = input("input(1 / 2):").strip()
    if i == 'done':
        print("finish programe")
        break
    arduino.write(i.encode())
    time.sleep(0.5)
    print(arduino.readline().decode('ascii'))

arduino.close()


