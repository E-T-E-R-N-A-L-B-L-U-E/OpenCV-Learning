import cv2
import numpy as np

src = cv2.imread("imori.png")

[height, weight, channels] = src.shape
b = src[:, :, 0].copy()
g = src[:, :, 1].copy()
r = src[:, :, 2].copy()

gray_img = 0.2126 * r + 0.7152 * g + 0.0722 * b
gray_img = gray_img.astype(np.uint8)

threshold = 0
max_variance = 0
for i in range(1, 255):
    v0 = gray_img[np.where(gray_img < i)]
    m0 = np.mean(v0) if len(v0) > 0 else 0
    w0 = len(v0) / (height * weight)
    v1 = gray_img[np.where(gray_img >= i)]
    m1 = np.mean(v1) if len(v1) > 0 else 0
    w1 = len(v1) / (height * weight)
    variance = w0 * w1 * ((m0 - m1) * (m0 - m1))
    if variance > max_variance:
        max_variance = variance
        threshold = i

print("threshold: ", threshold)
gray_img[gray_img < threshold] = 0
gray_img[gray_img >= threshold] = 255

cv2.imshow("result", gray_img)
cv2.imwrite("answer4.jpg", gray_img)
cv2.waitKey(0)
cv2.destroyAllWindows()