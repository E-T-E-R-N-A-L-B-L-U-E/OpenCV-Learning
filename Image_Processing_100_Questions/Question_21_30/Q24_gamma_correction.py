import cv2
import numpy as np


def gamma_correction(src, c=1., g=2.2):
    out = src.astype(np.float32) / 255
    out = (out / c) ** (1 / g)
    out *= 255
    out = out.astype(np.uint8)
    return out


src = cv2.imread("imori_gamma.jpg")

out = gamma_correction(src)

cv2.imshow("result", out)
cv2.imwrite("answer_24.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
