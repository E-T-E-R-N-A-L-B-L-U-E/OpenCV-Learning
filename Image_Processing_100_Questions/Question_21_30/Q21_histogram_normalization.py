import cv2
import numpy as np
import matplotlib.pyplot as plt


def normalization(src):
    a, b = 0., 255.
    min_val = np.min(src)
    max_val = np.max(src)

    out = src.copy().astype(np.float32)

    out[out < a] = a
    out[out > b] = b
    out = (b - a) / (max_val - min_val) * (out - min_val) + a

    out = out.astype(np.uint8)

    return out


src = cv2.imread("imori_dark.jpg")

out = normalization(src)

plt.hist(out.ravel(), bins=255, rwidth=0.8, range=(0, 255))
plt.savefig("answer_21_2.png")
plt.show()

cv2.imshow("result", out)
cv2.imwrite("answer_21_1.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
