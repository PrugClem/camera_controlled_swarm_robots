import numpy as np
import cv2

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

cap = cv2.VideoCapture(0)
while (True):
    imthresh, frame = cap.read()
    # Detect blobs.
    #keypoints = detector.detect(imthresh)

    # Draw detected blobs as red circles.
    # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
    # the size of the circle corresponds to the size of blob
    #im_with_keypoints = cv2.drawKeypoints(imthresh, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    cv2.imshow('Webcam',imthresh)
    cv2.resizeWindow('Webcam', 1280,720)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()