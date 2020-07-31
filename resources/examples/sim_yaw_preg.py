# Данный пример предназначен для работы в семуляторе!  

# В данном примере реализован простой пропорциональный регулятор по курсу.
# Аппарат будет плыть прямо с тягой в 60%, удерживая курс в -70 градусов, 30 секунд.

import pymurapi as mur
import cv2 as cv
import time

auv = mur.mur_init()

# Перевод угла >< 360 в 0 <=> 360
def clamp_to_360(angle):
    if angle < 0.0:
        return angle + 360.0
    if angle > 360.0:
        return angle - 360.0
    return angle

# Перевод угла из 0 <=> 360 в -180 <=> 180
def to_180(angle):
    if angle > 180.0:
        return angle - 360.0
    return angle

# Преобразовать v в промежуток между min max
def clamp(v, min, max):
	if v < min:
		return min
	if v > max:
		return max
	return v

# Функция удержания курса
def keep_yaw(yaw_to_set, power):
    current_yaw = auv.get_yaw()
    er = clamp_to_360(yaw_to_set - current_yaw)
    er = to_180(er)
    res = er * -0.8
    auv.set_motor_power(0, clamp(int(power - res), -100, 100))
    auv.set_motor_power(1, clamp(int(power + res), -100, 100))
                   

if __name__ == '__main__':
    now = time.time()
    while True:
        keep_yaw(-70, 60)
        print(auv.get_yaw())
        if (time.time() - now) > 30:
            break
                                                  