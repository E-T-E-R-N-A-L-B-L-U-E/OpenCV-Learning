import cv2
import numpy as np

src = cv2.imread("imori.png")
out = np.zeros_like(src)

height, width, channels = src.shape
n_height, n_width = height // 8, width // 8

for i in range(n_height):
    for j in range(n_width):
        for k in range(channels):
            out[i * 8:(i + 1) * 8, j * 8:(j + 1) * 8, k] = np.max(src[i * 8:(i + 1) * 8, j * 8:(j + 1) * 8, k])

out = out.astype(np.uint8)

cv2.imshow("result", out)
cv2.imwrite("answer8.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()