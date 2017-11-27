from tkinter import *
import serial
import struct
import time
import numpy as np
import _thread
from serial.tools import list_ports
import sqlite3


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
def Serialreader(threadName,value):
    conn = sqlite3.connect('/media/chiungachanga/SanDisk/example6.db')
    c = conn.cursor()
    try:
            c.execute('''CREATE TABLE IF NOT EXISTS GPS
                         (pidar text)''')
    except OperationalError:
            print('database exists')
    
    ser=serial.Serial(value,921600)
    h=0
    while(1):
        h=h+1
        line=ser.readline()
        print(line)
        c.execute("INSERT INTO GPS VALUES (?)",(line,))
        if(h==1):
            start=time.time()
            conn.commit()
            print(time.time()-start)
            h=0
if __name__ == '__main__':
    import random
    root = Tk()
    
    returnValue = True
    list = [port.device  for port in list_ports.comports()]
    while returnValue:
        returnValue = ListBoxChoice(root, "Drive Test", "Pick one of these crazy random numbers", list).returnValue()
        _thread.start_new_thread( Serialreader, ("Thread-1", returnValue, ) )

       
