import cv2
import numpy as np
cap = cv2.VideoCapture(0)
while(1):
    # Take each frame
    _, frame = cap.read()
    # Convert BGR to HSV
    img_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    #frame = cv2.GaussianBlur(frame, (10,), 0)
    #lower_blue = np.array([100,100,50])
    #upper_blue = np.array([130,255,255])
    #mask1 = cv2.inRange(img_hsv, lower_blue, upper_blue)



    lower_red1 = np.array([170,200,20])
    upper_red1 = np.array([180,255,150])
    mask1 = cv2.inRange(img_hsv, lower_red1, upper_red1)
    lower_red2 = np.array([0,200,20])
    upper_red2 = np.array([10,255,150])
    mask2 = cv2.inRange(img_hsv, lower_red2, upper_red2)
    
    mask = mask1 + mask2
    
    
    
    
    #cv2.imwrite("/home/pi/frame.jpg", frame)
    
    
    kernel = np.ones((3,3),np.uint8)
    mask = cv2.erode(mask1,kernel,iterations = 1)
    # Threshold the HSV image to get only blue colors
    #mask = cv2.inRange(hsv, lower_blue, upper_blue)
    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)
    cv2.imshow('frame',frame)
    cv2.imshow('mask',mask)
    #cv2.imshow('res',res)
    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break
cv2.destroyAllWindows()
