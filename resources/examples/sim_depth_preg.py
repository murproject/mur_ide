# Данный пример предназначен для работы в симуляторе!  

# В данном примере реализован простой пропорциональный регулятор по глубине.
# Аппарат убет удерживать заданную глубину 10 секунд.


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
    power = 80 * (auv.get_depth() - depth_to_set)
    auv.set_motor_power(2, clamp(int(power), -100, 100))
    auv.set_motor_power(3, clamp(int(power), -100, 100))
    print(clamp(int(power), -100, 100))
                    
                
if __name__ == '__main__':
    depth = 1.5
    now = time.time()
    while True:
        keep_depth(depth)
        if (time.time() - now) > 15:
            break
                                                            