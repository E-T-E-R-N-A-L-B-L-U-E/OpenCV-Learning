import cv2
import numpy as np

src = cv2.imread("imori.png")
out = np.zeros_like(src)

[height, width, channels] = src.shape
h_num = height // 8
w_num = width // 8
for i in range(0, h_num):
    for j in range(0, w_num):
        for k in range(0, channels):
            out[i * 8: (i + 1) * 8, j * 8: (j + 1) * 8, k] = np.mean(src[i * 8: (i + 1) * 8, j * 8: (j + 1) * 8, k]).astype(np.uint8)

out = out.astype(np.uint8)

cv2.imshow("result", out)
cv2.imwrite("answer7.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()