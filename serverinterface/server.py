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

def initialise():
    try:
        ServerSocket.bind((host, port))
    except socket.error as e:
        print(str(e))
        exit()

def allowconnections(number):
    print('Warten auf Verbindungen')
    ServerSocket.listen(number)

def getconnfromIP(IP):
    print(IP)

def threaded_client(connection):
    connection.send(str.encode('Verbunden'))
    while True:
        data = connection.recv(1024)
        getconnfromIP(str(data)[2:-5])
    connection.close()


def main():
    ThreadCount = 0
    initialise()
    allowconnections(1)
    while True:
        Client, address = ServerSocket.accept()
        clients.append((Client,address))
        print("Different Clients: ", clients)
        print('Verbunden mit: ' + address[0] + ':' + str(address[1]))
        start_new_thread(threaded_client, (Client, ))
        ThreadCount += 1
        print('Thread: ' + str(ThreadCount))
    ServerSocket.close()

if __name__ == '__main__': 
    main() 