/* 
 * File:   switchFab.h
 * Author: func
 *
 * Created on May 5, 2013, 8:23 PM
 */

#ifndef SWITCHFAB_H
#define SWITCHFAB_H
#include <stdbool.h>


#ifdef	__cplusplus
extern "C" {
#endif

    int sF_Init();
    bool sF_Switch();
    void sF_turnOn();
    void sF_turnOff();
    void sF_write(char *c, int i);


#ifdef	__cplusplus
}
#endif
#endif

