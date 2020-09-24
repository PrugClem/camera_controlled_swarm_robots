import socket


s = socket.socket()

IP = '0.0.0.0'
PORT = 10000

s.bind((IP,PORT))
s.listen()

conn, addr = s.accept()

with conn:
    print("Connected mit: ",addr)
    while True:
        
        data = conn.recv(1024)
        if not data:
            break
        print(data)
        conn.sendall(data)
