/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-06     liYony       first version
 */

#ifndef __OLED_H
#define __OLED_H
#include <rtthread.h>
#include <rtdevice.h>

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

void OLED_WR_Byte(rt_uint8_t dat,rt_uint8_t mode);
void OLED_Init(void);

#endif
