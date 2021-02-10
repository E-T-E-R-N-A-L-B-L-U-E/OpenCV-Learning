import cv2
import numpy as np

src = cv2.imread("imori.png")
height, width, channels = src.shape

b = src[..., 0]
g = src[..., 1]
r = src[..., 2]
gray = 0.2126 * r + 0.7152 * g + 0.0722 * b
gray = gray.astype(np.uint8)

out_v = np.zeros_like(gray, np.uint8)
out_h = np.zeros_like(gray, np.uint8)

kernel_size = 3
kernel_v = [[0, -1, 0], [0, 1, 0], [0, 0, 0]]
kernel_h = [[0, 0, 0], [-1, 1, 0], [0, 0, 0]]

pad = kernel_size // 2
src_pad = np.zeros((height + pad * 2, width + pad * 2), np.float32)
src_pad[pad: height + pad, pad: width + pad] = gray.copy()

for i in range(height):
    for j in range(width):
        out_v[i, j] = np.sum(kernel_v * src_pad[i: i + kernel_size, j: j + kernel_size])
        out_h[i, j] = np.sum(kernel_h * src_pad[i: i + kernel_size, j: j + kernel_size])
out_v = np.clip(out_v, 0, 255)
out_h = np.clip(out_h, 0, 255)

cv2.imshow("result vertical", out_v)
cv2.imshow("result horizon", out_h)
cv2.imwrite("answer14_vertical.jpg", out_v)
cv2.imwrite("answer14_horizon.jpg", out_h)
cv2.waitKey(0)
cv2.destroyAllWindows()
