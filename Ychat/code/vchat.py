from socket import *
import threading
import cv2
import sys
import struct
import pickle
import time
import zlib
import numpy as np

class Video_Server(threading.Thread):
    def __init__(self, port, version) :
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.ADDR = ('', port)
        if version == 4:
            self.sock = socket(AF_INET ,SOCK_STREAM)
        else:
            self.sock = socket(AF_INET6 ,SOCK_STREAM)
    def __del__(self):
        self.sock.close()
        try:
            cv2.destroyAllWindows()
        except:
            pass
    def run(self):
        print("VEDIO server starts... %d"%(self.ADDR[1]))
        self.sock.bind(self.ADDR)
        self.sock.listen(1)
        conn, addr = self.sock.accept()
        print("remote VEDIO client success connected...")
        data = "".encode("utf-8")
        payload_size = struct.calcsize("L")
        cv2.namedWindow('Remote', cv2.WINDOW_NORMAL)
        while True:
            while len(data) < payload_size:
                data += conn.recv(81920)
            packed_size = data[:payload_size]
            data = data[payload_size:]
            msg_size = struct.unpack("L", packed_size)[0]
            while len(data) < msg_size:
                data += conn.recv(81920)
            zframe_data = data[:msg_size]
            data = data[msg_size:]
            frame_data = zlib.decompress(zframe_data)
            frame = pickle.loads(frame_data)
            cv2.imshow('Remote', frame)
            if cv2.waitKey(1) & 0xFF == 27:
                break

class Video_Client(threading.Thread):
    def __init__(self ,ip, port, showme, level, version):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.ADDR = (ip, port)
        self.showme = showme
        if level == 0:
            self.interval = 0
        elif level == 1:
            self.interval = 1
        elif level == 2:
            self.interval = 2
        else:
            self.interval = 3
        self.fx = 1 / (self.interval + 1)
        if self.fx < 0.3:
            self.fx = 0.3
        if version == 4:
            self.sock = socket(AF_INET, SOCK_STREAM)
        else:
            self.sock = socket(AF_INET6, SOCK_STREAM)
        self.cap = cv2.VideoCapture(0)
        print("VEDIO client starts...")
    def __del__(self) :
        self.sock.close()
        self.cap.release()
        if self.showme:
            try:
                cv2.destroyAllWindows()
            except:
                pass
    def run(self):
        while True:
            try:
                self.sock.connect(self.ADDR)
                break
            except:
                time.sleep(3)
                continue
        if self.showme:
            cv2.namedWindow('You', cv2.WINDOW_NORMAL)
        print("VEDIO client connected...")
        while self.cap.isOpened():
            ret, frame = self.cap.read()
            if self.showme:
                cv2.resizeWindow('You',300,220)
                cv2.imshow('You', frame)
                if cv2.waitKey(1) & 0xFF == 27:
                    self.showme = False
                    cv2.destroyWindow('You')
            sframe = cv2.resize(frame, (0,0), fx=self.fx, fy=self.fx)
            data = pickle.dumps(sframe)
            zdata = zlib.compress(data, zlib.Z_BEST_COMPRESSION)
            try:
                self.sock.sendall(struct.pack("L", len(zdata)) + zdata)
            except:
                break
            for i in range(self.interval):
                self.cap.read()