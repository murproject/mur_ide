# Данный пример предназначен для симулятора!  

# В данном примере аппарат плывет вперед до тех пор, пока не будет остановлена программа. 
# Во время выполнения будут отображаться изображения с передней и донной камер. 

# Функции get_image_front и get_image_bottom работают только в симуляторе. 
# Для получения видео на аппарате используйте cv2.VideoCapture из OpenCV!
# (https://docs.opencv.org/4.1.1/dd/d43/tutorial_py_video_display.html)


import pymurapi as mur
import cv2 as cv


auv = mur.mur_init()

if __name__ == '__main__':
    while True:
        auv.set_motor_power(0, 40)    
        auv.set_motor_power(1, 40)    
        cv.imshow("Front", auv.get_image_front())
        cv.imshow("Bottom", auv.get_image_bottom())
        cv.waitKey(5)
        