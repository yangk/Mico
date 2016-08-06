#ifndef __OPT_H__
#define __OPT_H__

#include "oled.h"
#include "stdio.h"

#define STACK_SIZE_DISPLAY_THREAD           0x450
#define STACK_SIZE_HT_THREAD                0x450
#define STACK_SIZE_DEFAULT_THREAD           0x450

void display_time_thread(void *args);
void display_T_H_thread(void *args);
void sensor_thread(void *args);


#endif