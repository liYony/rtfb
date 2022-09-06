/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-06     liYony       first version
 */

#include <rtdevice.h>
#include <rtthread.h>
#include <rtfb.h>

char buf[1] = {1};

int test()
{
    rt_device_t fb0;

    fb0 = rt_device_find("fb0");
    rt_device_open(fb0,RT_DEVICE_FLAG_ACTIVATED);

    struct rt_device_graphic *gra = (struct rt_device_graphic *)fb0;
    rt_graphic_set_pixel(gra,buf,0,0);
    rt_graphic_set_pixel(gra,buf,2,0);
    rt_graphic_set_pixel(gra,buf,8,1);
    rt_graphic_set_pixel(gra,buf,3,1);
    rt_device_control(fb0,RTGRAPHIC_CTRL_REFRESH,RT_NULL);

    return 0;
}

INIT_APP_EXPORT(test);
