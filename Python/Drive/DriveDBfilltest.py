import sqlite3
import struct
import time
import numpy as np
conn = sqlite3.connect('Drive2.db')
c = conn.cursor()
try:
        c.execute('''CREATE TABLE IF NOT EXISTS DriveTo(pos text)''')
        c.execute('''CREATE TABLE IF NOT EXISTS shaitan(che text)''')
except OperationalError:
        print('database exists')
filepath = 'snake.txt'  
with open(filepath) as fp:  
    for cnt,line in enumerate(fp):
        print(line)
        c.execute("INSERT INTO DriveTo(pos) VALUES (?)",(line,))

c.execute("INSERT INTO shaitan(che) VALUES (?)",("0",))
conn.commit()
