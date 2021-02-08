import cv2
import numpy as np

img = cv2.imread("imori.png")
src = img.copy() / 255
[height, width, channels] = src.shape

R = src[:, :, 2].copy()
G = src[:, :, 1].copy()
B = src[:, :, 0].copy()

Max = np.max(src, axis=2).copy()
Min = np.min(src, axis=2).copy()
min_arg = np.argmin(src, axis=2)

H = np.zeros_like(Max)

H[np.where(Max == Min)] = 0
ind = np.where(min_arg == 0)
H[ind] = 60 * (G[ind] - R[ind]) / (Max[ind] - Min[ind]) + 60
ind = np.where(min_arg == 2)
H[ind] = 60 * (B[ind] - G[ind]) / (Max[ind] - Min[ind]) + 180
ind = np.where(min_arg == 1)
H[ind] = 60 * (R[ind] - B[ind]) / (Max[ind] - Min[ind]) + 300

V = Max
S = Max - Min

H = (H + 180) % 360

H_1 = H // 60
X = S * (1 - abs(H_1 % 2 - 1))
Z = np.zeros_like(H)
vals = [[Z, X, S], [Z, S, X], [X, S, Z], [S, X, Z], [S, Z, X], [X, Z, S]]

out = np.zeros_like(src)
for i in range(6):
    ind = np.where((i <= H_1) & (H_1 < i + 1))
    out[..., 0][ind] = (V - S)[ind] + vals[i][0][ind]
    out[..., 1][ind] = (V - S)[ind] + vals[i][1][ind]
    out[..., 2][ind] = (V - S)[ind] + vals[i][2][ind]
out[np.where(Max == Min)] = 0

out = (out * 255).astype(np.uint8)

cv2.imshow("src", src)
cv2.imshow("result", out)
cv2.imwrite("answer5.jpg", out)
cv2.waitKey(0)
cv2.destroyAllWindows()