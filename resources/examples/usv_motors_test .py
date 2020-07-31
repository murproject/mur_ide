# Данный пример предназначен для тестирования моторов на аппарате!  

# В данном примере мы подаем тягу на 4 мотора продолжительностью в 5 секунд. 


import pymurapi as mur
import time

usv = mur.usv_init()

if __name__ == '__main__':
    usv.set_motor_power(0, -50)
    usv.set_motor_power(1, 50)
    usv.set_servo(150)
    time.sleep(5)

