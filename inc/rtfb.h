/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-06     liYony       first version
 */

#ifndef __RT_FB_H_
#define __RT_FB_H_

#include <rtthread.h>

/**
 * graphic device control command
 */
#define RTGRAPHIC_CTRL_RECT_UPDATE      (RT_DEVICE_CTRL_BASE(Graphic) + 0)
#define RTGRAPHIC_CTRL_POWERON          (RT_DEVICE_CTRL_BASE(Graphic) + 1)
#define RTGRAPHIC_CTRL_POWEROFF         (RT_DEVICE_CTRL_BASE(Graphic) + 2)
#define RTGRAPHIC_CTRL_GET_INFO         (RT_DEVICE_CTRL_BASE(Graphic) + 3)
#define RTGRAPHIC_CTRL_SET_MODE         (RT_DEVICE_CTRL_BASE(Graphic) + 4)
#define RTGRAPHIC_CTRL_GET_EXT          (RT_DEVICE_CTRL_BASE(Graphic) + 5)
#define RTGRAPHIC_CTRL_SET_BRIGHTNESS   (RT_DEVICE_CTRL_BASE(Graphic) + 6)
#define RTGRAPHIC_CTRL_GET_BRIGHTNESS   (RT_DEVICE_CTRL_BASE(Graphic) + 7)
#define RTGRAPHIC_CTRL_GET_MODE         (RT_DEVICE_CTRL_BASE(Graphic) + 8)
#define RTGRAPHIC_CTRL_GET_STATUS       (RT_DEVICE_CTRL_BASE(Graphic) + 9)
#define RTGRAPHIC_CTRL_PAN_DISPLAY      (RT_DEVICE_CTRL_BASE(Graphic) + 10)
#define RTGRAPHIC_CTRL_WAIT_VSYNC       (RT_DEVICE_CTRL_BASE(Graphic) + 11)
#define RTGRAPHIC_CTRL_REFRESH          (RT_DEVICE_CTRL_BASE(Graphic) + 12)

/* graphic device */
enum
{
    RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
    RTGRAPHIC_PIXEL_FORMAT_GRAY4,
    RTGRAPHIC_PIXEL_FORMAT_GRAY16,
    RTGRAPHIC_PIXEL_FORMAT_RGB332,
    RTGRAPHIC_PIXEL_FORMAT_RGB444,
    RTGRAPHIC_PIXEL_FORMAT_RGB565,
    RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    RTGRAPHIC_PIXEL_FORMAT_BGR565 = RTGRAPHIC_PIXEL_FORMAT_RGB565P,
    RTGRAPHIC_PIXEL_FORMAT_RGB666,
    RTGRAPHIC_PIXEL_FORMAT_RGB888,
    RTGRAPHIC_PIXEL_FORMAT_BGR888,
    RTGRAPHIC_PIXEL_FORMAT_ARGB888,
    RTGRAPHIC_PIXEL_FORMAT_ABGR888,
    RTGRAPHIC_PIXEL_FORMAT_RESERVED,
};

/**
 * build a pixel position according to (x, y) coordinates.
 */
#define RTGRAPHIC_PIXEL_POSITION(x, y)  ((x << 16) | y)

typedef struct rt_device_graphic *rt_device_graphic_t;

/**
 * graphic device information structure
 */
struct rt_device_graphic_info
{
    rt_uint8_t  pixel_format;                           /**< graphic format */
    rt_uint8_t  bits_per_pixel;                         /**< bits per pixel */
    rt_uint16_t pitch;                                  /**< bytes per line */

    rt_uint16_t width;                                  /**< width of graphic device */
    rt_uint16_t height;                                 /**< height of graphic device */

    rt_uint8_t *framebuffer;                            /**< frame buffer */
    rt_uint32_t smem_len;                               /**< allocated frame buffer size */
};

/**
 * rectangle information structure
 */
struct rt_device_rect_info
{
    rt_uint16_t x;                                      /**< x coordinate */
    rt_uint16_t y;                                      /**< y coordinate */
    rt_uint16_t width;                                  /**< width */
    rt_uint16_t height;                                 /**< height */
};

/**
 * graphic operations
 */
struct rt_device_graphic_ops
{
    rt_err_t (*init)(rt_device_graphic_t gra);
    rt_err_t (*destroy)(rt_device_graphic_t gra);
    rt_err_t (*convert) (rt_device_graphic_t gra);
    rt_err_t (*refresh)(rt_device_graphic_t gra);
};

struct rt_device_graphic
{
    struct rt_device parent;
    struct rt_device_graphic_info info;
    const struct rt_device_graphic_ops *ops;
};

rt_err_t rt_hw_graphic_register(rt_device_graphic_t device, const char *name, const struct rt_device_graphic_ops *ops, const void *user_data);
rt_err_t rt_graphic_init(rt_device_graphic_t gra);
rt_err_t rt_graphic_set_pixel(rt_device_graphic_t gra, const char *pixel, int x, int y);
rt_err_t rt_graphic_get_pixel(rt_device_graphic_t gra, char *pixel, int x, int y);
rt_err_t rt_graphic_refresh(rt_device_graphic_t gra);

#endif /* __RT_FB_H_ */
