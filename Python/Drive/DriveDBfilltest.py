import sqlite3
import struct
import time
import numpy as np
conn = sqlite3.connect('Drive.db')
c = conn.cursor()
try:
        c.execute('''CREATE TABLE IF NOT EXISTS BeenTo
                     (GPS text)''')
except OperationalError:
        print('database exists')
filepath = 'capture.txt'  
with open(filepath) as fp:  
    for cnt,line in enumerate(fp):
        if '$GPGGA' in line:
            if len(line)<75:
                print(line)
                c.execute("INSERT INTO BeenTo(GPS) VALUES (?)",(line,))
           

conn.commit()
