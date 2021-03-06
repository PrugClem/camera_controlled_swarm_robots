import cv2
import numpy as np
import parserblob as pb
import importlib
import socket
"""
Disclaimer this code only works for one car on the screen.
The position is calculated by taking in any and all points that are detected
and getting the center of all of them
"""

HOST = '10.212.134.6'
"""
IP Address still not static,
might be a good idea to change that in the future
"""
PORT = 10005
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
"""
Authentification for the server's matching algorithm
More security features would be a good idea in the near future
"""
s.sendall(b'Mario')
data = s.recv(1024)
print(data)

"""
Start of the definition of parameters for the SimpleBlob detector build into opencv
parameters may need to be changed depending on the camera used
"""
params = cv2.SimpleBlobDetector_Params()
     
# Change thresholds
params.minThreshold = 0
params.maxThreshold = 256
    
# Filter by Area.
params.filterByArea = True
params.minArea = 30
    
# Filter by Circularity
params.filterByCircularity = True
params.minCircularity = 0.1
    
# Filter by Convexity
params.filterByConvexity = True
params.minConvexity = 0.5
    
# Filter by Inertia
params.filterByInertia = True
params.minInertiaRatio = 0.5
    
detector = cv2.SimpleBlobDetector_create(params)
"""
End of the definition and implementation of the SimpleBlob detector
"""

"""
Definition of lower and upper values for the detectable color
Format:  [R,G,B]
sensitivity may need adjustments depending on the camera used
"""
sensitivity = 11
lower_white = np.array([0,0,255-sensitivity])
upper_white = np.array([255,sensitivity,255])


"""
Camera Selection
0: Default Camera/Webcam
If the system has a built in webcam change the value from 0 to 1 if its desired
to use the external one
"""
cap = cv2.VideoCapture('vid2.mp4')   
 
while cap.isOpened():
    ret,frame = cap.read()                                          #grabing the current frame
    if ret == True:
        width  = cap.get(3)  # float `width`
        height = cap.get(4)  # float `height`
        xrel = 0
        yrel = 0
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)            #converting the frame to black/white not necessary right now
        blurred = cv2.GaussianBlur(frame, (11, 11), 0)                  #blurring the image helps with noise reduction
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)                    #converting the 
        mask = cv2.inRange(hsv, lower_white, upper_white)               #declaring mask with the lower and upper boundaries set for the color
        mask = cv2.dilate(mask, None, iterations=1)
        reversemask=255-mask                                            #reversing the mask so we can find the objects
        keypoints = detector.detect(reversemask)                        #detection the keypoints in the picture
        pts = cv2.KeyPoint_convert(keypoints)                           #converting the keypoints into proper x,y coordinates in the picture
        xrel,yrel = pb.findcar(pts,width,height)                                                 #detecting all cars [work in progress]
        pb.sendto(s,xrel,yrel)                                                #sending all keypoints of the car 
        importlib.reload(pb)                                            #reloading the python module 
        resImage = frame.shape[1::-1]                                   #resolution is needed for simulation purposes
        im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
        cv2.imshow('Picture with Points', im_with_keypoints)
        cv2.imshow('Masked Picture',mask)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release()
cv2.destroyAllWindows()
