import socket
import sys
import cv2
import numpy as np
import time
import math

class tracker:
  def __init__(self):
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.ppp = int(sys.argv[1])
    self.server_address = ('192.168.50.1', self.ppp)
    print >>sys.stderr, 'starting up on %s port %s' % self.server_address
    self.bind = self.sock.bind(self.server_address)
    self.cap = cv2.VideoCapture(0)
    self.listen = self.sock.listen(1)
    self.whiteBall = (0, 0)
    self.kernel = np.ones((5,5),np.uint8)
    self.lower_red = np.array([170,100,100])
    self.upper_red = np.array([180,255,255])
    self.lower_red1 = np.array([170,200,20])
    self.upper_red1 = np.array([180,255,150])
    self.lower_red2 = np.array([0,200,20])
    self.upper_red2 = np.array([10,255,150])
    self.win_left_x = 0
    self.win_right_x = 0
    self.win_y = 0
    self.pre_time = int(round(time.time() * 1000))
    self.factor = 0
    self.start_listen()
    
    

  def start_listen(self):
    while True:
    #try to find the blue hat
      '''
      loc = (np.array([]), np.array([]))
      lower_blue = np.array([100,100,50])
      upper_blue = np.array([130,255,255])
      kernel = np.ones((8,8),np.uint8)
      while loc[0].size == 0:
        _, frame = self.cap.read()
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lower_blue, upper_blue)
        mask = cv2.erode(mask, kernel, iterations = 1)
        loc = np.nonzero(mask)
      max_y = np.amax(loc[0])
      min_y = np.amin(loc[0])
      max_x = np.amax(loc[1])
      min_x = np.amin(loc[1])
      self.win_left_x = int(min_x)
      self.win_right_x = int(max_x)
      self.win_y = int((max_y + min_y) / 2) - 10
      #self.factor = 480 / (self.win_right_x - self.win_left_x)
      self.factor = 1
      print('hehe: ' + str(self.win_right_x) + ' ' + str(self.win_left_x))
      '''
      print >>sys.stderr, 'waiting for a connection'
      connection, client_address = self.sock.accept()
      print >>sys.stderr, 'connection from', client_address
      data = connection.recv(16)
      print >>sys.stderr, 'received "%s"' % data
      self.updateWhite(data)
      if data:
        pre_x = 0
        pre_y = 0
        while True:
          _, frame = self.cap.read()
          #print(frame.shape)
          hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
          #mask = cv2.inRange(hsv, self.lower_red, self.upper_red)
          mask1 = cv2.inRange(hsv, self.lower_red1, self.upper_red1)
          mask2 = cv2.inRange(hsv, self.lower_red2, self.upper_red2)
          mask = mask1 + mask2
          mask = cv2.erode(mask,self.kernel,iterations = 1)
          loc = np.nonzero(mask)
          if loc[0].size != 0:
            max_y = np.amax(loc[0])
            min_y = np.amin(loc[0])
            max_x = np.amax(loc[1])
            min_x = np.amin(loc[1])
            cur_x = int((max_x + min_x) / 2)
            cur_y = int((max_y + min_y) / 2)
            period = int(round(time.time() * 1000)) - self.pre_time
            cv2.rectangle(frame, (cur_x, cur_y), (cur_x + 3,cur_y + 3), (0,255,0),3)
            #pos_x = (cur_x - self.win_left_x) / self.factor
            #pos_y = (cur_y - self.win_y) / self.factor
            pos_x = cur_x
            pos_y = cur_y
            #vx = round((cur_x - pre_x) * 100 / period) + 20
            #vy = round((cur_y - pre_y) * 100 / period) + 20
            vx = 30 if cur_x - pre_x > 0 else 10
            vy = 30 if cur_y - pre_y > 0 else 10
            pre_x = cur_x
            pre_y = cur_y
            if math.sqrt(((self.whiteBall[0] - pos_x) ** 2) + ((self.whiteBall[1] - pos_y) ** 2)) > 20:
              vx = 20
              vy = 20
            #else:
             # vx = 40
             # vy = 20
            else:
              print("v" + str(vx) +' ' + str(vy) + '!!!!!!!!!!!!!!!!!!!!')
            #vx = 0 if vx < 0 else vx
            #vx = 40 if vx > 40 else vx 
            #vy = 0 if vx < 0 else vy
            #vy = 40 if vy > 40 else vy 
            string = str(pos_x) + ' ' + str(pos_y) + ' ' + str(vx) + ' ' + str(vy)
            connection.sendall(string)
            if vx != 20 or vy != 20:
              break
            print string
          k = cv2.waitKey(1)
          cv2.imshow('frame',frame)
          #cv2.imshow('mask',mask)
          #cv2.imshow('res',res)
        connection.close()

  def updateWhite(self, data):
    tmp = data.split(' ')
    print("white data: " + tmp[0] + tmp[1])
    self.whiteBall = (int(tmp[0]), int(tmp[1]))

if __name__ == '__main__':
    track = tracker()
