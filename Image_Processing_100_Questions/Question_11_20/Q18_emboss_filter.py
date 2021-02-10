import cv2
import numpy as np


def BGR2GRAY(src):
    b = src[..., 0]
    g = src[..., 1]
    r = src[..., 2]

    gray = 0.2126 * r + 0.7152 * g + 0.0722 * b

    gray = gray.astype(np.uint8)

    return gray


def emboss_filter(src):
    if len(src.shape) == 3:
        height, width, channels = src.shape
    else:
        height, width = src.shape

    kernel_size = 3
    kernel = [[-2, -1, 0], [-1, 1, 1], [0, 1, 2]]

    pad = kernel_size // 2
    src_pad = np.zeros((height + pad * 2, width + pad * 2), np.uint8)
    src_pad[pad: pad + height, pad: pad + width] = src.copy()

    out = np.zeros_like(src)

    for i in range(height):
        for j in range(width):
            out[i, j] = np.sum(kernel * src_pad[i: i + kernel_size, j: j + kernel_size])

    out = np.clip(out, 0, 255).astype(np.uint8)

    return out


src = cv2.imread("imori.png")

gray = BGR2GRAY(src)

out = emboss_filter(gray)

cv2.imshow("result", out)
cv2.imwrite("answer18.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
