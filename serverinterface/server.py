import tkinter
import socket
from _thread import *
import threading
import os
from time import sleep

ServerSocket = socket.socket()
host = '0.0.0.0'
port = 10000
clients = []

"""
Binding the address and port
If an errors occours, it will be displayed and the program exits
"""
def initialise():
    try:
        ServerSocket.bind((host, port))
    except socket.error as e:
        print(str(e))
        exit()

"""
Allows all connections
"""
def allowconnections():
    print('Warten auf Verbindungen'+"\r\n")
    ServerSocket.listen()

"""
Transforms rawdata into an client address and message
rawdata:
    b'IP from destination "," message\r\n'
"""
def transmitter(rawdata):
    destination,message  = rawdata[2:-5].split(",")
    for client in clients:
        if destination in str(client):
            client.send(bytes(message,"UTF-8"))

"""
Client Handler
"""
def threaded_client(connection):
    connection.send(str.encode('Verbunden\r\n'))
    while True:
        data = connection.recv(1024)
        if not data:
            break
        transmitter(str(data))
    connection.close()

"""
GUI
"""
def GUI():
    pass



def main():
    ThreadCount = 0
    initialise()
    allowconnections()
    while True:
        Client, address = ServerSocket.accept()
        clients.append(Client)
        print("Different Clients: {}\r\n", clients)
        print('Verbunden mit: {} \r\n' + address[0] + ':' + str(address[1]))
        start_new_thread(threaded_client, (Client, ))
        ThreadCount += 1
        print('Thread: {}\r\n' + str(ThreadCount))
    ServerSocket.close()

if __name__ == '__main__': 
    main() 