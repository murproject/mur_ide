# Данный пример предназначен для симулятора!  

# В данном примере аппарат плывет вперед до тех пор, пока не будет остановлена программа. 
# Во время выполнения будут отображаться изображения с донной камеры и выделяться карсные объекты и их центры. 

# Функции get_image_front и get_image_bottom работают только в симуляторе. 
# Для получения видео на аппарате используйте cv2.VideoCapture из OpenCV!
# (https://docs.opencv.org/4.1.1/dd/d43/tutorial_py_video_display.html)


import pymurapi as mur
import cv2
import numpy as np


auv = mur.mur_init()

def detect_red_objects(image):
    img_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    lower_red = np.array([0,50,50])
    upper_red = np.array([10,255,255])
    mask0 = cv2.inRange(img_hsv, lower_red, upper_red)

    lower_red = np.array([170,50,50])
    upper_red = np.array([180,255,255])
    mask1 = cv2.inRange(img_hsv, lower_red, upper_red)

    mask = mask0 + mask1
    
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(image, contours, -1, (128,0,0), 1)
    
    for cnt in contours:
        (x,y),radius = cv2.minEnclosingCircle(cnt)
        center = (int(x),int(y))
        cv2.circle(image, center, int(3), (128, 0, 128))
    to_draw = image.copy()                    
    cv2.imshow("Image", to_draw)
    cv2.waitKey(5)
            


if __name__ == '__main__':
    while True:
        auv.set_motor_power(0, 40)
        auv.set_motor_power(1, 40)
        detect_red_objects(auv.get_image_bottom())