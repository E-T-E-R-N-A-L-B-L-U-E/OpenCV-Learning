import cv2
import numpy as np


def BGR2GRAY(img):
    b = img[..., 0]
    g = img[..., 1]
    r = img[..., 2]

    gray = 0.2126 * r + 0.7152 * g + 0.0722 * b
    gray = gray.astype(np.uint8)

    return gray


def laplacian_filter(img):
    if len(img.shape) == 3:
        height, width, channels = img.shape
    else:
        height, width = img.shape

    kernel_size = 3
    kernel = [[0, 1, 0], [1, -4, 1], [0, 1, 0]]

    pad = kernel_size // 2
    src_pad = np.zeros((height + pad * 2, width + pad * 2), np.uint8)
    src_pad[pad: height + pad, pad: width + pad] = img.copy()

    out = np.zeros_like(img, np.float32)

    for i in range(height):
        for j in range(width):
            out[i, j] = np.sum(kernel * src_pad[i: i + kernel_size, j: j + kernel_size])

    out = np.clip(out, 0, 255).astype(np.uint8)

    return out


src = cv2.imread("imori.png")

gray = BGR2GRAY(src)

out = laplacian_filter(gray)

cv2.imshow("result", out)
cv2.imwrite("answer17.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
