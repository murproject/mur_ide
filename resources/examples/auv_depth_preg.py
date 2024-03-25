# Данный пример предназначен для работы на аппарате!  

# В данном примере реализован простой пропорциональный регулятор по глубине.
# Аппарат будет удерживать заданную глубину 10 секунд.


import pymurapi as mur
import time

auv = mur.mur_init()


def clamp(v, min, max):
    if v < min:
        return min
    if v > max:
        return max
    return v


def keep_depth(depth_to_set):
    motor = 250 * (auv.get_depth() - depth_to_set)    
    auv.set_motor_power(0, clamp(int(motor), -100, 100))
    auv.set_motor_power(3, clamp(int(motor), -100, 100))


if __name__ == '__main__':
    depth = 0.2
    now = time.time()
    while True:
        keep_depth(depth)
        if (time.time() - now) > 10:
            break

