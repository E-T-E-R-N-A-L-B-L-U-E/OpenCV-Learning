import cv2
import numpy as np

src = cv2.imread("imori.png")
out = src.copy()

out[np.where((0 <= src) & (src < 63))] = 32
out[np.where((63 <= src) & (src < 127))] = 96
out[np.where((127 <= src) & (src < 191))] = 160
out[np.where((191 <= src) & (src < 256))] = 224

cv2.imshow("result", out)
cv2.imwrite("answer6.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()