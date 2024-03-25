# Пример для версии аппаратов MiddleAUV-CM4
# Здесь реализована трансляция видео из скрипта
# Данный пример лучше использовать только для тестирования и отладки

import cv2 as cv
import numpy as np
import pymurapi as mur
from time import sleep

auv = mur.mur_init()
mur_view = auv.get_videoserver()

cap0 = cv.VideoCapture(0)
cap1 = cv.VideoCapture(1)

def find_contours(image, color, approx = cv.CHAIN_APPROX_SIMPLE):
    hsv_image = cv.cvtColor(image, cv.COLOR_BGR2HSV)
    mask = cv.inRange(hsv_image, color[0], color[1])
    contours, _ = cv.findContours(mask, cv.RETR_CCOMP, approx)
    return contours, mask

def img_process(img, num=0):
    font = cv.FONT_HERSHEY_PLAIN
    contours, mask = find_contours(img, ((0, 0, 220), (180, 255, 255)))
    
    if contours:
        
        for contour in contours:
            if cv.contourArea(contour) > 50:
                rectangle = cv.minAreaRect(contour)
                box = np.int0(cv.boxPoints(rectangle))
                cv.drawContours(img,[box],0,(0,0,250),2)

    cv.putText(img,'Camera {}'.format(num),(5,25),font,2,(255,255,255),2,cv.LINE_AA)

    return img, mask

while True:
    ok, frame0 = cap0.read()
#    ok, frame1 = cap1.read()

    frame0 = cv.resize(frame0, (320, 240))
#    frame1 = cv.resize(frame1, (320, 240))
    
    result0, mask0 = img_process(frame0, 0)
#    result1, mask1 = img_process(frame1, 1)
    
    mur_view.show(result0, 0)
    mur_view.show(mask0, 1)
    
mur_view.stop()
print("done")
