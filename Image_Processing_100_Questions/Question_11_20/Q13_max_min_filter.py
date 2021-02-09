import cv2
import numpy as np

src = cv2.imread("imori.png")
height, width, channels = src.shape
out = np.zeros_like(src, np.uint8)

b = src[..., 0]
g = src[..., 1]
r = src[..., 2]

gray = 0.2126 * r + 0.7152 * g + 0.0722 * b
gray = gray.astype(np.uint8)

kernel_size = 3

pad = kernel_size // 2
gray_pad = np.zeros((height + pad * 2, width + pad * 2), np.uint8)
gray_pad[pad: height + pad, pad: width + pad] = gray.copy()

for i in range(height):
    for j in range(width):
        out[i, j] = np.max(gray_pad[i: i + kernel_size, j: j + kernel_size]) - np.min(gray_pad[i: i + kernel_size, j: j + kernel_size])

cv2.imshow("result", out)
cv2.imwrite("answer13.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
