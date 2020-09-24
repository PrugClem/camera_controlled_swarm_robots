import socket
import time
import numpy as np
s = socket.socket()

IP = '127.0.0.1'
PORT = 10000

s.connect((IP,PORT))
while True:
    data = input()
    s.send(bytes(data,'utf-8'))
