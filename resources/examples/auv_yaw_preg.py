# Данный пример предназначен для работы на аппарате!  

# В данном примере реализован простой пропорциональный регулятор по курсу.
# Аппарат будет плыть прямо с тягой в 25%, удерживая изначальный курс, 10 секунд.

import pymurapi as mur
import cv2 as cv
import time

auv = mur.mur_init()

# Перевод угла из -180 <=> 180 в 0 <=> 360
def to_360(angle):
    if angle > 0.0:
        return angle
    if angle <= 0.0:
        return 360.0 + angle

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
    current_yaw = to_360(auv.get_yaw())
    er = clamp_to_360(yaw_to_set - current_yaw)
    er = to_180(er)
    res = er * 0.7
    auv.set_motor_power(1, clamp(int(power - res), -100, 100))
    auv.set_motor_power(2, clamp(int(power + res), -100, 100))


if __name__ == '__main__':
	time.sleep(0.5)
	yaw = to_360(auv.get_yaw())
	now = time.time()
	while True:
		keep_yaw(yaw, -25)
		if (time.time() - now) > 10:
			break


