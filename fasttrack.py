import cv2
import numpy as np
import time

cap = cv2.VideoCapture(0)

lower_blue = np.array([0,100,100])
upper_blue = np.array([10,255,255])
pre_x = 0
pre_y = 0
pre_time = int(round(time.time() * 1000))
while 1:
    # Take each frame
    _, frame = cap.read()
    # Convert BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # define range of blue color in HSV
    # Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    loc = np.nonzero(mask)
    if loc[0].size != 0:
        max_x = np.amax(loc[0])
        min_x = np.amin(loc[0])
        max_y = np.amax(loc[1])
        min_y = np.amin(loc[1])
        cur_x = int((max_x + min_x) / 2)
        cur_y = int((max_y + min_y) / 2)
        period = int(round(time.time() * 1000)) - pre_time
        vx = round((cur_x - pre_x) * 50 / period)
        vy = round((cur_y - pre_y) * 50 / period)
        pre_x = cur_x
        pre_y = cur_y
        cv2.rectangle(frame, (cur_y, cur_x), (cur_y + 3,cur_x + 3), (0,255,0),3)
        print(str(vx) + " " +str(vy))
    # Bitwise-AND mask and original image
    #res = cv2.bitwise_and(frame,frame, mask= mask)
    cv2.imshow('frame',frame)
    #cv2.imshow('mask',mask)
    #cv2.imshow('res',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
