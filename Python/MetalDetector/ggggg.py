from tkinter import *
import serial
import struct
import time
import numpy as np
import matplotlib.pyplot as plt
import _thread
from serial.tools import list_ports
values=50
class ListBoxChoice(object):
    def __init__(self, master=None, title=None, message=None, list=[]):
        self.master = master
        self.value = None
        self.list = list[:]
        
        self.modalPane = Toplevel(self.master)

        self.modalPane.transient(self.master)
        self.modalPane.grab_set()

        self.modalPane.bind("<Return>", self._choose)
        self.modalPane.bind("<Escape>", self._cancel)

        if title:
            self.modalPane.title(title)

        if message:
            Label(self.modalPane, text=message).pack(padx=5, pady=5)

        listFrame = Frame(self.modalPane)
        listFrame.pack(side=TOP, padx=5, pady=5)
        
        scrollBar = Scrollbar(listFrame)
        scrollBar.pack(side=RIGHT, fill=Y)
        self.listBox = Listbox(listFrame, selectmode=SINGLE)
        self.listBox.pack(side=LEFT, fill=Y)
        scrollBar.config(command=self.listBox.yview)
        self.listBox.config(yscrollcommand=scrollBar.set)
        self.list.sort()
        for item in self.list:
            self.listBox.insert(END, item)

        buttonFrame = Frame(self.modalPane)
        buttonFrame.pack(side=BOTTOM)

        chooseButton = Button(buttonFrame, text="Choose", command=self._choose)
        chooseButton.pack()

        cancelButton = Button(buttonFrame, text="Cancel", command=self._cancel)
        cancelButton.pack(side=RIGHT)

    def _choose(self, event=None):
        try:
            firstIndex = self.listBox.curselection()[0]
            self.value = self.list[int(firstIndex)]
        except IndexError:
            self.value = None
        self.modalPane.destroy()

    def _cancel(self, event=None):
        self.modalPane.destroy()
        
    def returnValue(self):
        self.master.wait_window(self.modalPane)
        return self.value
def plotter(threadName,value):
    ser=serial.Serial(value)
    plt.show()
    while(1):
        b=0
        y_pos=np.array([])
        while (b < values):
            faf=random.randint(0,4092)
            line=ser.readline()
            try:
                faf=int(float(line))
            except(ValueError):
                faf=0
            y_pos=np.append(y_pos,faf)
            b=b+1
            #print(faf)
        
        #print(line)
        start=time.time()
        plt.cla()
        plt.xlim(0,values)
        plt.ylim(0,1024)
        plt.xlabel('time (s)')
        plt.ylabel('voltage (mV)')
        plt.title('About as simple as it gets, folks')
        plt.grid(True)
        plt.plot(y_pos)
        plt.draw()
        plt.pause(0.001)
        print(time.time()-start)
if __name__ == '__main__':
    import random
    root = Tk()
    
    returnValue = True
    list = [port.device  for port in list_ports.comports()]
    while returnValue:
        returnValue = ListBoxChoice(root, "Number Picking", "Pick one of these crazy random numbers", list).returnValue()
        _thread.start_new_thread( plotter, ("Thread-1", returnValue, ) )
