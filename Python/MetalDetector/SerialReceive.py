import serial
import time
import struct
import numpy as np
def bytes_to_int(bytes):
    result = 0

    for b in bytes:
        result = result * 256 + int(b)

    return result
try:
        ser=serial.Serial('/dev/ttyACM0')
        end=0
        start=0
        last_time=start
        start=time.time()
        count=0
        faf={}
        while(count<300):
                line=ser.read()
                
                if(bytes_to_int(line)>15):
                        faf[count]=bytes_to_int(line)
                        count=count+1
        
        i=0
        while i<len(faf):
                print(faf[i])
                i+=1
        ser.close()
except KeyboardInterrupt:
        print("W: interrupt received, stopping…")
finally:
        ser.close()


