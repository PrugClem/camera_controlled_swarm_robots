import serial
import serial.tools.list_ports as list_ports
import socket
import threading
import pty
import os

#defines
IP = '0.0.0.0';
PORT = 10001;
COMPORT = '/dev/ttyS3';
BAUDRATE = 115200;

#running variable
running = True;

def initialisierung():
    try:
        serielle = serial.Serial(COMPORT, BAUDRATE, timeout = 0);
    except serial.SerialException as exception:
        print("========== Opening Physical Serial Port failed ==========");
        print(type(exception));
        print(exception.args[0]);
        print("Avaliable Ports:");
        print(list(list_ports.comports()));
        exit(-1);
        pass
    print("Opening COM Port");
    if not serielle.isOpen():
        serielle.open();
    s = socket.socket();
    s.bind((IP,PORT));
    print("Waiting for connection on %s %d" % (IP, PORT) );
    s.listen();
    conn,addr = s.accept();
    print("Verbunden mit ",addr);
    return serielle,conn;

def tcptoserial(conn,serielle):
    global running;
    while running:
        data = conn.recv(1024);
        if data:
            print("Data from tcp to serial = ",data);
            serielle.write(data);
        else:
            # tcp socket was closed, shutting program down
            running = False;

def serialtotcp(conn,serielle):
    global running;
    global v_com;
    while running:
        data = serielle.readline(1024);
        if data:
            print("Data from serial to tcp = ",data);
            conn.send(data);
            v_com.write(data);

def main():
    global running;
    master, slave = pty.openpty();
    v_com_name = os.ttyname(slave);
    v_com = serial.Serial(v_com_name);
    print("Virtual com port opened:",v_com.name);

    serielle,conn = initialisierung();

    tcptos = threading.Thread(target = tcptoserial, args=(conn,serielle));
    stotcp = threading.Thread(target = serialtotcp, args=(conn,serielle));
    tcptos.start();
    stotcp.start();
    while running:
        pass
    stotcp.join();
    tcptos.join();
    
main();