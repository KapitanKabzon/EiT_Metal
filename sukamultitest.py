import numpy as np
import matplotlib.pyplot as plt
import random
import serial
import time
f = open('workfile1000', 'w')
import sqlite3
conn = sqlite3.connect('/media/chiungachanga/GRMCULXFREO_EN_DVD/example7.db')
conn2= sqlite3.connect('/media/chiungachanga/GRMCULXFREO_EN_DVD/example4.db')
c = conn.cursor()
c2=conn2.cursor()
try:
        c.execute("CREATE TABLE IF NOT EXISTS pulse (position TEXT NOT NULL,gather TEXT NOT NULL)")
except OperationalError:
        print('database exists')
# Insert a row of data

try:
        start=time.time()
        c.execute("INSERT INTO pulse(position) VALUES (?)",("110",))
        end=time.time()
        conn.commit()
        start=time.time()
        c.execute("SELECT gather FROM pulse")
        end=time.time()
        print(c.fetchall())
        print("aaa %s",str(end-start))
        start=time.time()
        c2.execute("SELECT * FROM pulse ORDER BY pidar DESC LIMIT 1")
        end=time.time()
        print(c2.fetchall())
        print("bbb %s",str(end-start))
        
except KeyboardInterrupt:
        print("W: interrupt received, stopping…")
finally:
        conn.commit()
        conn.close()
        f.close()
