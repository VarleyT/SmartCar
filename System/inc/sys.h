//
// Created by 19634 on 2023/8/10.
//

#ifndef __SYS_H
#define __SYS_H

#include "stm32f10x.h"

#define ON          1
#define OFF         0
#define HIGH        1
#define LOW         0

//#define repeat(x)   for(int i = 0; i < x; i++)
template<typename Function>
inline void repeat(int times, Function func) {
    while (times--) {
        func();
    }
}

#endif //__SYS_H
