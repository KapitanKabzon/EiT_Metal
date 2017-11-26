from tkinter import *
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5 import QtGui, QtCore
from random import randint, uniform
import pyqtgraph as pg
import numpy as np
import random
import time
from threading import Thread
values=160

class App:

    def __init__(self, master):

        frame = Frame(master)
        frame.pack()

        self.button = Button(
            frame, text="QUIT", fg="red",bg="green", command=frame.quit
            )
        self.button.pack(side=LEFT)

        self.hi_there = Button(frame, text="Start grpah", command=self.say_hi)
        self.hi_there.pack(side=LEFT)
        self.fukoff=Button(frame, text="End graph",command=self.fukoff)
        self.fukoff.pack(side=LEFT)

    def say_hi(self):
        print ("hi there, everyone!")
        app=QtGui.QApplication(sys.argv)
        ex=Example()
        thread=Thread(target=ex.start)
        thread.start()
        thread.join()
        sys.exit(app.exec_())
    def fukoff(self):
        print("modafuka")
        thread.stop()

class Example(QtGui.QWidget):

    def __init__(self):
        QtGui.QWidget.__init__(self)
        self.x=pg.PlotWidget(self)
        self.x.setMinimumHeight(600)
        self.x.setMinimumWidth(1000)
        self.x.setYRange(0,4092)
        self.switch = True
        self.show()

    def start(self):
        
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.run)
        self.timer.start(0.5)

    def run(self):
        self.x.clear()
        b=0
        y_pos=np.array([])
        while (b < values):
                    faf=random.randint(0,4092)
                    y_pos=np.append(y_pos,faf)
                    b=b+1
        start=time.time()
        self.x.plot(y_pos)
        print(time.time()-start)
root = Tk()

app = App(root)

root.mainloop()
root.destroy() # optional; see description below
