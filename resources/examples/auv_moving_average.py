# В данном примере реализовано сглаживание тяги методом "простое скользящее среднее"
# Простое скользящее среднее представляет собой сумму последних чисел деленную на их количество.
# Использование скользящего среднего необходимо для плавного увеличения тяги на движителях.

import pymurapi as mur
import time

auv = mur.mur_init() 

class MovingAverage: 
    _storage_size = 50

    def __init__(self):
        self._storage = [0] * self._storage_size
        self._counter = 0
        
    # Добавление элементов в список
    def add(self, value):
        self._storage[self._counter] = value

        if(self._counter < (self._storage_size - 1)):            
            self._counter += 1
        else:
            self._counter = 0
            
    # Получение среднего арифметического из списка
    def get(self):
        result = sum(self._storage) / self._storage_size
        return result
        
    # Приравнивание элементов списка нулю
    def clean(self):
        for i in range(self._storage_size):
            self._storage[i] = 0
        
        
power1 = MovingAverage()
power2 = MovingAverage()
now = time.time()

while True:
    if (time.time() - now) < 7:
        power1.add(-50)
        power2.add(100)
    elif 7 < (time.time() - now) < 15:
        power1.add(50)
        power2.add(-100)
    else:
        power1.add(0)
        power2.add(0)
    
    p1 = power1.get()
    p2 = power2.get()
    auv.set_motor_power(1, p1)
    auv.set_motor_power(2, p2)

    time.sleep(0.1)