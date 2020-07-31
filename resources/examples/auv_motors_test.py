# Данный пример предназначен для тестирования моторов на аппарате!  

# В данном примере мы подаем тягу на 4 мотора продолжительностью в 5 секунд. 


import pymurapi as mur
import time

auv = mur.mur_init()

if __name__ == '__main__':
    auv.set_motor_power(0, -50)
    auv.set_motor_power(1, 50)
    auv.set_motor_power(2, -50)
    auv.set_motor_power(3, 50)
    time.sleep(5)

