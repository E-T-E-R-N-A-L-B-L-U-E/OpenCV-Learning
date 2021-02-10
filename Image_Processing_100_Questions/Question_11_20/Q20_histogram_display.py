import cv2
import numpy as np
import matplotlib.pyplot as plt

src = cv2.imread("imori_dark.jpg").astype(np.float32)

plt.hist(src.ravel(), bins=255, rwidth=0.5, range=(0, 255))
plt.savefig("out.png")
plt.show()
