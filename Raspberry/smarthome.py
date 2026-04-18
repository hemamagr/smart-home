import serial

a = serial.Serial("/dev/ttyACM0", 9600)

while True : 
    d = a.readline()
    print(d)
