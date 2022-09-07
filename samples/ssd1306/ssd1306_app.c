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

char buff[10] = {0,0xaa,0xf0,0x0f};

int test()
{
    rt_device_t fb0;
    fb0 = rt_device_find("fb0");
    rt_device_open(fb0,RT_DEVICE_FLAG_ACTIVATED);
    rt_device_write(fb0,0,buff,4);
    rt_device_control(fb0,RTGRAPHIC_CTRL_REFRESH,RT_NULL);
    return 0;
}

 INIT_APP_EXPORT(test);


static void ssd_test(int argc, char *argv[])
{
    char p,p2=1;
    struct rt_device_graphic *gra = (struct rt_device_graphic *)rt_device_find("fb0");
    if(0 == rt_strcmp("get", argv[1]))
    {
        rt_graphic_get_pixel(gra,&p,atoi(argv[2]),atoi(argv[3]));
        rt_kprintf("get the (x,y)->(%d,%d) is %d\n",atoi(argv[2]),atoi(argv[3]),p);
    }
    if(0 == rt_strcmp("set", argv[1]))
    {
        p2 = atoi(argv[4]);
        rt_graphic_set_pixel(gra,&p2,atoi(argv[2]),atoi(argv[3]));
        rt_kprintf("set the (x,y)->(%d,%d) is %d\n",atoi(argv[2]),atoi(argv[3]),p2);
    }
	rt_device_control((rt_device_t)gra,RTGRAPHIC_CTRL_REFRESH,RT_NULL);
    return;
}

MSH_CMD_EXPORT(ssd_test, ssd_test);