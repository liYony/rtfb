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

static rt_err_t ssd_convert(rt_device_graphic_t gra)
{
    rt_uint8_t data, x, page, bit;
    rt_uint16_t i = 0;
    char pixel;

    for (page = 0; page < 8; page++)
    {
        for (x = 0; x < 128; x++)
        {
            data = 0;
            for (bit = 0; bit < 8; bit++)
            {
                rt_graphic_get_pixel(gra, &pixel, x, page * 8 + bit);
                data |= pixel << bit;
            }
            convert_buf[i++] = data;
        }
    }
    return RT_EOK;
}

static rt_err_t ssd_refresh(rt_device_graphic_t gra)
{
    rt_uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00, OLED_CMD);     //设置低列起始地址
        OLED_WR_Byte(0x10, OLED_CMD);     //设置高列起始地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(((fbarr)convert_buf)[i][n], OLED_DATA);
    }

    return RT_EOK;
}

static rt_err_t ssd_init(rt_device_graphic_t gra)
{
    OLED_Init();
    ssd_refresh(gra);
    return RT_EOK;
}

static const struct rt_device_graphic_ops ssd_ops =
{
    .init = ssd_init,
    .refresh = ssd_refresh,
    .convert = ssd_convert,
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
