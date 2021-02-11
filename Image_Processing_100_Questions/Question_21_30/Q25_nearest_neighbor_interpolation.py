import cv2
import numpy as np


def enlarge(src, rate=1.5):
    height, width, channels = src.shape
    after_height = int(height * rate)
    after_width = int(width * rate)

    out = np.zeros((after_height, after_width, channels), np.uint8)

    index_y = np.arange(after_height).repeat(after_width).reshape(after_width, -1)
    index_x = np.tile(np.arange(after_width), (after_height, 1))
    index_y = (index_y / rate).astype(np.uint8)
    index_x = (index_x / rate).astype(np.uint8)

    out = src[index_y, index_x]

    return out


src = cv2.imread("imori.png")

out = enlarge(src)

cv2.imshow("result", out)
cv2.imwrite("answer_25.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
