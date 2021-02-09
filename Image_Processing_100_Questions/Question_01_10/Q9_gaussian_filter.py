import cv2
import numpy as np

src = cv2.imread("imori_noise.jpg")
height, width, channels = src.shape
out1 = np.zeros_like(src, np.float32)
out2 = np.zeros_like(src, np.float32)

kernel_size = 3
sigma = 1.3

pad = kernel_size // 2
src_pad = np.zeros((height + pad * 2, width + pad * 2, channels), np.uint8)
src_pad[pad: height + pad, pad: width + pad] = src.copy()

kernel = np.zeros((kernel_size, kernel_size), np.float32)
for i in range(-pad, pad + 1):
    for j in range(-pad, pad + 1):
        kernel[i + pad, j + pad] = np.exp(-(i * i + j * j) / (2 * sigma * sigma))
kernel /= sigma * np.sqrt(2 * np.pi)
kernel /= kernel.sum()

for i in range(height):
    for j in range(width):
        for k in range(channels):
            val = 0.
            for p in range(kernel_size):
                for q in range(kernel_size):
                    val += src_pad[i + p, j + q, k] * kernel[p, q]
            out1[i, j, k] = val
            out2[i, j, k] = np.sum(kernel * src_pad[i: i + kernel_size, j: j + kernel_size, k])

out1 = out1.astype(np.uint8)
out2 = out2.astype(np.uint8)

cv2.imshow("result1", out1)
cv2.imshow("result2", out2)
cv2.imwrite("answer9.jpg", out1)
cv2.waitKey(0)
cv2.destroyAllWindows()