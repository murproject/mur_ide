# Данный пример предназначен для работы на аппарате!  

# В данном примере мы устанавливаем цвет RGB ленты
# c периодом включения в 1 секунду и периодом выключения в 0.5 секунды 
# на 3 секунды для каждого цвета  (синий, зеленый красный) .


import pymurapi as mur
import time

auv = mur.mur_init()

if __name__ == '__main__':
    auv.set_on_delay(1)
    auv.set_off_delay(0.5)
    # Синий
    auv.set_rgb_color(0, 0, 50)
    time.sleep(3)
    # Зеленый
    auv.set_rgb_color(0, 50, 0)
    time.sleep(3)
    # Красный
    auv.set_rgb_color(0, 50, 0)
    time.sleep(3)

