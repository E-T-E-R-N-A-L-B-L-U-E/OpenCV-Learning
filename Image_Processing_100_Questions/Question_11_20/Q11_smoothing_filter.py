import cv2
import numpy as np

src = cv2.imread("imori.png")
height, width, channels = src.shape
out = np.zeros_like(src, np.float32)

kernel_size = 3
kernel = np.ones((kernel_size, kernel_size), np.float32) / (kernel_size * kernel_size)

pad = kernel_size // 2
src_pad = np.zeros((height + pad * 2, width + pad * 2, channels))
src_pad[pad: height + pad, pad: width + pad] = src.copy()

for i in range(height):
    for j in range(width):
        for k in range(channels):
            out[i, j, k] = np.sum(kernel * src_pad[i: i + kernel_size, j: j + kernel_size, k])

out = out.astype(np.uint8)

cv2.imshow('result', out)
cv2.imwrite("answer11.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
