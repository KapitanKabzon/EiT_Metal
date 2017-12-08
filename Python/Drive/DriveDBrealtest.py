from tkinter import *
import serial
import struct
import time
import numpy as np
import _thread
from serial.tools import list_ports
import sqlite3

def Serialreader(threadName,value):
    conn = sqlite3.connect('Drive2.db')
    c = conn.cursor()
    try:
            c.execute('''CREATE TABLE IF NOT EXISTS GPS
                         (pidar text)''')
    except OperationalError:
            print('database exists')
    ser=serial.Serial(value,921600) #921600 is the baudrate
    h=0
    meme=0
    kekistan=1
    while(1):
        
        line=ser.readline()
        #line_as_bytes=line.decode()
        #type(line_as_bytes)
        print(line)
        if b'$GPGGA' in line: #checking if the gotten line is an GPGGA data,
            c.execute("INSERT INTO GPS VALUES (?)",(line,))
            h=h+1
        if b'NEW' in line:
            g=5
            #c.execute("
        if(h==1): # is buffer time is needed the h should be increased
            start=time.time()
            conn.commit()
            print(time.time()-start)
            h=0
        if meme>=9:
            c.execute("SELECT pos FROM DriveTo")
            heh=c.fetchall()
            print("     ",heh[1])
            kekistan=kekistan+1
            meme=0
        meme=meme+1
def serial_ports():

    # produce a list of all serial ports. The list contains a tuple with the port number, 
    # description and hardware address
    #
    ports = list(serial.tools.list_ports.comports())  

    # return the port if 'USB' is in the description 
    for port_no, description, address in ports:
        if("ttyACM" in description):
            print(port_no,description)
            return port_no
if __name__ == '__main__':
    huj=serial_ports()
    print(huj)
    _thread.start_new_thread( Serialreader, ("Thread-1", huj, ) )
