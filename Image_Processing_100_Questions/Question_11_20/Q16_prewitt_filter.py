import cv2
import numpy as np


def BGR2GRAY(img):
    b = img[..., 0]
    g = img[..., 1]
    r = img[..., 2]

    gray = 0.2126 * r + 0.7152 * g +0.0722 * b
    gray = gray.astype(np.uint8)

    return gray


def prewitt_filter(src, kernel_size = 3):
    if len(src.shape) == 3:
        height, width, channels = src.shape
    else:
        height, width = src.shape

    pad = kernel_size // 2
    src_pad = np.zeros((height + pad * 2, width + pad * 2), np.uint8)
    src_pad[pad: height + pad, pad: width + pad] = src.copy()

    kernel_v = [[-1, -1, -1], [0, 0, 0], [1, 1, 1]]
    kernel_h = [[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]]

    out_v = np.zeros_like(src, np.float32)
    out_h = np.zeros_like(src, np.float32)

    for i in range(height):
        for j in range(width):
            out_v[i, j] = np.sum(kernel_v * src_pad[i: i + kernel_size, j: j + kernel_size])
            out_h[i, j] = np.sum(kernel_h * src_pad[i: i + kernel_size, j: j + kernel_size])
    out_v = np.clip(out_v, 0, 255).astype(np.uint8)
    out_h = np.clip(out_h, 0, 255).astype(np.uint8)

    return out_v, out_h


src = cv2.imread("imori.png")

gray = BGR2GRAY(src)

out_v, out_h = prewitt_filter(gray, 3)

cv2.imshow("result vertical", out_v)
cv2.imwrite("answer16_vertical.jpg", out_v)
cv2.imshow("result horizon", out_h)
cv2.imwrite("answer16_horizon.jpg", out_h)
cv2.waitKey(0)
cv2.destroyAllWindows()
