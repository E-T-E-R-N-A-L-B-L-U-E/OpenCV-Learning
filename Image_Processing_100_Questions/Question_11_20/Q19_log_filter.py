import cv2
import numpy as np


def BGR2GRAY(src):
    b = src[..., 0].copy()
    g = src[..., 1].copy()
    r = src[..., 2].copy()

    gray = r * 0.2126 + 0.7152 * g + 0.0722 * b
    gray = gray.astype(np.uint8)

    return gray


def log_filter(src, kernel_size=5, sigma=3):
    if len(src.shape) == 3:
        height, width, channels = src.shape
    else:
        height, width = src.shape

    pad = kernel_size // 2
    src_pad = np.zeros((height + pad * 2, width + pad * 2), np.uint8)
    src_pad[pad: pad + height, pad: pad + width] = src.copy()

    kernel = np.zeros((kernel_size, kernel_size), np.float32)
    for i in range(-pad, kernel_size - pad):
        for j in range(-pad, kernel_size - pad):
            kernel[i + pad, j + pad] = (i * i + j * j - sigma * sigma) * np.exp(-(i * i + j * j) / (2 * sigma * sigma))
    kernel /= 2 * np.pi * (sigma ** 6)
    kernel /= np.sum(kernel)

    out = np.zeros_like(src, np.float32)

    for i in range(height):
        for j in range(width):
            out[i, j] = np.sum(kernel * src_pad[i: i + kernel_size, j: j + kernel_size])
    out = np.clip(out, 0, 255).astype(np.uint8)

    return out


src = cv2.imread("imori_noise.jpg")

gray = BGR2GRAY(src)

out = log_filter(gray, 5, 3)

cv2.imshow("result", out)
cv2.imwrite("answer19.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
