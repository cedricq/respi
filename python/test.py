import serial
import serial.tools.list_ports

ser = serial.Serial()
ser.baudrate = 230400
ser.port = 'COM9'
ser.timeout = 1
ser.write_timeout = 10
ser._xonxoff = 0
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.open()
ser.flushOutput()

str_out = '6:666' + '\r\n'
ser.write(str_out.encode())
ser.flushOutput()

while(True):
    print(".", end='')

ser.close()