import sys
from random import randint, uniform
import matplotlib.pyplot as plt
import numpy as np
import random
import time
import threading
import serial
from serial.tools import list_ports
from tkinter import *
values=160
#ser=serial.Serial('/dev/ttyACM0')

class App:
    def __init__(self, master):

        frame = Frame(master)
        frame.pack()
        
        self.button = Button(
            frame, text="QUIT", fg="red",bg="green", command=frame.quit
            )
        self.button.pack(side=LEFT)

        self.hi_there = Button(frame, text="init Graph", command=self.say_hi)
        self.hi_there.pack(side=LEFT)
        self.fukoff=Button(frame, text="update graph",command=self.fukoff)
        self.fukoff.pack(side=LEFT)
        self.fukoff2=Button(frame, text="Kill graph",command=self.fukoff2)
        self.fukoff2.pack(side=LEFT)
        self.list = Listbox(self, selectmode=EXTENDED)
        self.list.pack(fill=BOTH, expand=1)

    def say_hi(self):
        print ("initializing graph")
        fig, ax = plt.subplots()
        line, = ax.plot(np.random.randn(100))
        plt.xlabel('time (s)')
        plt.ylabel('voltage (mV)')
        plt.title('About as simple as it gets, folks')
        plt.grid(True)
        plt.show()
    def fukoff(self):
        print("Updating graph")
        
        starter=time.time()
        
        b=0
        y_pos=np.array([])
        while (b < values):
            faf=random.randint(0,4092)
            y_pos=np.append(y_pos,faf)
            b=b+1
        plt.cla()
        plt.xlim(0,160)
        plt.ylim(0,4092)
        plt.xlabel('time (s)')
        plt.ylabel('voltage (mV)')
        plt.title('About as simple as it gets, folks')
        plt.grid(True)
        plt.plot(y_pos)
        plt.draw()
        print(1/(time.time()-starter))
        
    def fukoff2(self):
        print("killing graph")
        plt.close()
    
root = Tk()

app = App(root)
print(
    "\n".join(
        [
            port.device + ': ' + port.description
            for port in list_ports.comports()
        ]))
root.mainloop()
root.destroy() # optional; see description below

