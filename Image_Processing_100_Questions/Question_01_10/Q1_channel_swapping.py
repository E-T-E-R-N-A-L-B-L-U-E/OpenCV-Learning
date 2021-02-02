import cv2
import numpy as np

src = cv2.imread("imori.png")


# split channel:b, g, r
b = src[:, :, 0].copy()
g = src[:, :, 1].copy()
r = src[:, :, 2].copy()

# collect info
height, weight, channel = src.shape
result = np.ones((height, weight, channel), np.uint8)

# reconstruct
result[:, :, 0] = r
result[:, :, 1] = g
result[:, :, 2] = b

# save & output
cv2.imwrite("answer1.jpg", result)
cv2.imshow("result", result)
cv2.waitKey(0)
cv2.destroyAllWindows()
