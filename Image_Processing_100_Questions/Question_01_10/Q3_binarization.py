import cv2
import numpy as np

src = cv2.imread("imori.png")

# bgr to gray
[b, g, r] = cv2.split(src)
gray = 0.2126 * r + 0.7152 * g + 0.0722 * b
gray = gray.astype(np.uint8)

# solve with opencv
threshold = np.zeros(gray.shape, np.uint8)
cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY, threshold)
cv2.imshow("opencv result", threshold)

# binarization
gray[gray < 128] = 0
gray[gray >= 128] = 255

cv2.imwrite("answer3.jpg", gray)
cv2.imshow("binarization", gray)
cv2.waitKey(0)
cv2.destroyAllWindows()
