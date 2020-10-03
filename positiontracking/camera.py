import cv2 
import numpy as np
# Setup SimpleBlobDetector parameters.
params = cv2.SimpleBlobDetector_Params()

# Filter by Area.
params.filterByArea = True
params.minArea = 100
params.maxArea =100000

# Don't filter by Circularity
params.filterByCircularity = False

# Don't filter by Convexity
params.filterByConvexity = False

# Don't filter by Inertia
params.filterByInertia = False


# Create a detector with the parameters
detector = cv2.SimpleBlobDetector_create(params)

# Open the device at the ID 0
cap = cv2.VideoCapture(0)
#Check whether user selected camera is opened successfully.
if not (cap.isOpened()):
    print("Could not open video device")

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    grayImage = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # Display the resulting frame
    keypoints = detector.detect(frame)
    im_with_keypoints = cv2.drawKeypoints(grayImage, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    cv2.imshow('preview',frame)
    cv2.imshow('points',im_with_keypoints)
    #Waits for a user input to quit the application
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()