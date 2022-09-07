/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-06     liYony       first version
 */

#include <rtfb.h>
#include <ssd1306.h>

struct rt_device_graphic ssd1306;
typedef rt_uint8_t (*fbarr)[128];
rt_uint8_t *convert_buf;


static rt_err_t ssd_refresh(rt_device_graphic_t gra)
{
    rt_uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00, OLED_CMD);     //设置低列起始地址
        OLED_WR_Byte(0x10, OLED_CMD);     //设置高列起始地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(((fbarr)gra->info.framebuffer)[i][n], OLED_DATA);
    }

    return RT_EOK;
}

static rt_err_t ssd_init(rt_device_graphic_t gra)
{
    OLED_Init();
    ssd_refresh(gra);
    return RT_EOK;
}

rt_size_t ssd_write(rt_device_graphic_t gra, rt_off_t pos, const void *buffer, rt_size_t size)
{
    int cur_pos,byte8, bit, i;
    for(cur_pos = pos ;cur_pos<(pos+size);cur_pos++)
    {	
        byte8 = cur_pos % 16 * 8 + (cur_pos /128 *128);
        bit = cur_pos / 16 % 8;
        
        for (i = 0; i < 8; i++)
        {
            if (((rt_uint8_t *)buffer)[cur_pos-pos] & (1 << i))
            {
                gra->info.framebuffer[byte8 + i] |= (1 << (bit));
            }
            else
            {
                gra->info.framebuffer[byte8 + i] &= ~(1 << (bit));
            }
        }
    }
    return size;
}

rt_size_t ssd_read(rt_device_graphic_t gra, rt_off_t pos, void *buffer, rt_size_t size)
{
    int cur_pos,byte8, bit, i;
    for(cur_pos = pos ;cur_pos<(pos+size);cur_pos++)
    {	
        byte8 = cur_pos % 16 * 8 + (cur_pos /128 *128);
        bit = cur_pos / 16 % 8;
        
        for (i = 0; i < 8; i++)
        {
            if (gra->info.framebuffer[byte8 + i] & (1 << bit))
            {
                ((rt_uint8_t *)buffer)[cur_pos-pos] |= (1 << i);
            }
            else
            {
                ((rt_uint8_t *)buffer)[cur_pos-pos] &= ~(1 << i);
            }
        }
    }
    return size;
}
static const struct rt_device_graphic_ops ssd_ops =
    {
        .init = ssd_init,
        .refresh = ssd_refresh,
        .write = ssd_write,
        .read = ssd_read,
};

int ssd1306_hw_init(void)
{
    ssd1306.info.width = 128;
    ssd1306.info.height = 64;
    ssd1306.info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_MONO;
    ssd1306.info.bits_per_pixel = 1;
    ssd1306.info.pitch = ssd1306.info.width * ssd1306.info.bits_per_pixel / 8;
    ssd1306.info.smem_len = ssd1306.info.width * ssd1306.info.height * ssd1306.info.bits_per_pixel / 8;
    ssd1306.info.framebuffer = (rt_uint8_t *)rt_calloc(1, ssd1306.info.smem_len);

    convert_buf = (rt_uint8_t *)rt_calloc(1, ssd1306.info.smem_len);

    rt_hw_graphic_register(&ssd1306, "fb0", &ssd_ops, RT_NULL);

    return RT_EOK;
}

INIT_BOARD_EXPORT(ssd1306_hw_init);
