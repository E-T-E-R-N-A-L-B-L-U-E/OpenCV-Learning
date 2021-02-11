import cv2
import numpy as np
import matplotlib.pyplot as plt


def operation(src):
    m0 = 128
    s0 = 52

    m = np.mean(src)
    s = np.std(src)

    out = src.copy().astype(np.float32)
    out = s0 / s * (out - m) + m0
    out = np.clip(out, 0, 255).astype(np.uint8)

    return out


src = cv2.imread("imori.png")

out = operation(src)

plt.hist(out.ravel(), bins=255, rwidth=0.8, range=(0, 255))
plt.savefig("answer_22_2.png")
plt.show()

cv2.imshow("result", out)
cv2.imwrite("answer_22_1.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
