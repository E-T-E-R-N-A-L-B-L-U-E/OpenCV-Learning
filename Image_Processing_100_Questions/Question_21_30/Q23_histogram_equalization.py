import cv2
import numpy as np
import matplotlib.pyplot as plt


def equalization(src):
    height, width, channels = src.shape
    pix_cnt = height * width * channels
    z_max = 255

    out = np.zeros_like(src, np.float32)

    cnt = 0
    for i in range(256):
        index = np.where(src == i)
        cnt += len(src[index])
        out[index] = z_max / pix_cnt * cnt

    out = out.astype(np.uint8)

    return out


src = cv2.imread("imori.png")

out = equalization(src)

plt.hist(out.ravel(), bins=255, rwidth=0.8, range=(0, 255))
plt.savefig("answer_23_2.png")
plt.show()

cv2.imshow("result", out)
cv2.imwrite("answer_23_1.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()
