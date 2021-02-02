import cv2
import numpy as np

src = cv2.imread("imori.png")

# use opencv to solve
b, g, r = cv2.split(src)
cv_result = 0.2126 * r + 0.7152 * g + 0.0722 * b
cv_result = cv_result.astype(np.uint8)
cv2.imshow("cv result", cv_result)

# solve without opencv
b = src[:, :, 0].copy()
g = src[:, :, 1].copy()
r = src[:, :, 2].copy()
height, weight, channels = src.shape

# calculate with formula
output = np.zeros((height, weight, 1), np.uint8)
output = 0.2126 * r + 0.7152 * g + 0.0722 * b
output = output.astype(np.uint8)

# show result
cv2.imwrite("answer2.jpg", output)
cv2.imshow("result", output)
cv2.waitKey(0)
cv2.destroyAllWindows()