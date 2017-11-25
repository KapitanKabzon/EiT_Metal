import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5 import QtGui, QtCore
from random import randint, uniform
import pyqtgraph as pg
import numpy as np
import random
import time
values=160


class Example(QtGui.QWidget):

    def __init__(self):
        QtGui.QWidget.__init__(self)
        self.x=pg.PlotWidget(self)
        self.x.setMinimumHeight(600)
        self.x.setMinimumWidth(1000)
        self.x.setYRange(0,4098)
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
try:
    app=QtGui.QApplication(sys.argv)
    ex=Example()
    ex.start()
    sys.exit(app.exec_())
except KeyboardInterrupt:
        print("W: interrupt received, stopping…")
finally:
    ex.stop()
