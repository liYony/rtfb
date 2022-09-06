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
#include <rtfb.h>

rt_err_t _graphic_init(rt_device_t dev)
{
    struct rt_device_graphic *gra = (struct rt_device_graphic *)dev;
    rt_err_t result = RT_EOK;

    RT_ASSERT(gra != RT_NULL);

    rt_memset(gra->info.framebuffer, 0, gra->info.smem_len);

    if (gra->ops->init)
        result = gra->ops->init(gra);

    return result;
}

rt_size_t _graphic_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct rt_device_graphic *gra = (struct rt_device_graphic *)dev;

    RT_ASSERT(gra != RT_NULL);

    if (size >= gra->info.smem_len - pos)
        size = gra->info.smem_len - pos;

    rt_memcpy(buffer, &gra->info.framebuffer[pos], size);

    return size;
}

rt_size_t _graphic_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct rt_device_graphic *gra = (struct rt_device_graphic *)dev;

    RT_ASSERT(gra != RT_NULL);

    if (!gra->info.framebuffer)
        return -1;

    rt_memcpy(&gra->info.framebuffer[pos], buffer, size);

    return size;
}

rt_err_t _graphic_control(rt_device_t dev, int cmd, void *args)
{
    struct rt_device_graphic *gra = (struct rt_device_graphic *)dev;
    rt_err_t result = RT_EOK;

    RT_ASSERT(gra != RT_NULL);

    switch (cmd)
    {
    case RTGRAPHIC_CTRL_REFRESH:
        rt_graphic_refresh(gra);
        break;

    default:
        if (gra->ops->contorl)
            gra->ops->contorl(gra, cmd, args);
        break;
    }
    return result;
}

rt_err_t _graphic_close(rt_device_t dev)
{
    struct rt_device_graphic *gra = (struct rt_device_graphic *)dev;
    rt_err_t result = RT_EOK;

    RT_ASSERT(gra != RT_NULL);

    if (gra->ops->destroy)
        result = gra->ops->destroy(gra);

    return result;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops graphic_ops =
    {
        _graphic_init,
        RT_NULL,
        _graphic_close,
        _graphic_read,
        _graphic_write,
        _graphic_control,
};
#endif

rt_err_t rt_hw_graphic_register(rt_device_graphic_t device, const char *name, const struct rt_device_graphic_ops *ops, const void *user_data)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    device->parent.type = RT_Device_Class_Graphic;
    device->parent.rx_indicate = RT_NULL;
    device->parent.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->parent.ops = &graphic_ops;
#else
    device->parent.init = _graphic_init;
    device->parent.open = RT_NULL;
    device->parent.close = _graphic_close;
    device->parent.read = _graphic_read;
    device->parent.write = _graphic_write;
    device->parent.control = _graphic_control;
#endif
    device->ops = ops;
    device->parent.user_data = (void *)user_data;

    result = rt_device_register(&device->parent, name, RT_DEVICE_FLAG_RDWR);

    return result;
}

rt_err_t rt_graphic_init(rt_device_graphic_t gra)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(gra != RT_NULL);
    RT_ASSERT(gra->info.framebuffer != RT_NULL);

    rt_memset(gra->info.framebuffer, 0, gra->info.smem_len);

    if (gra->ops->init)
        result = gra->ops->init(gra);

    return result;
}

rt_err_t rt_graphic_set_pixel(rt_device_graphic_t gra, const char *pixel, int x, int y)
{
    rt_err_t result = RT_EOK;
    rt_uint8_t pixel_bytes, *pixel_addr;

    RT_ASSERT(gra != RT_NULL);
    RT_ASSERT(gra->info.framebuffer != RT_NULL);

    pixel_bytes = gra->info.bits_per_pixel / 8;
    pixel_addr = gra->info.framebuffer + y * gra->info.pitch + ((pixel_bytes == 0) ? x / 8 : x * pixel_bytes);

    if (pixel_bytes == 0)
    {
        rt_int16_t bit = x & 0x7;

        if (*pixel)
            *pixel_addr |= (1 << bit);
        else
            *pixel_addr &= ~(1 << bit);
    }
    else
    {
        rt_memcpy(pixel_addr, pixel, pixel_bytes);
    }

    return result;
}

rt_err_t rt_graphic_get_pixel(rt_device_graphic_t gra, char *pixel, int x, int y)
{
    rt_err_t result = RT_EOK;
    rt_uint8_t pixel_bytes, *pixel_addr;

    RT_ASSERT(gra != RT_NULL);
    RT_ASSERT(gra->info.framebuffer != RT_NULL);

    pixel_bytes = gra->info.bits_per_pixel / 8;
    pixel_addr = gra->info.framebuffer + y * gra->info.pitch + ((pixel_bytes == 0) ? x / 8 : x * pixel_bytes);

    if (pixel_bytes == 0)
    {
        rt_int16_t bit = x & 0x7;

        if (*pixel_addr & (1 << bit))
            *pixel = 1;
        else
            *pixel = 0;
    }
    else
    {
        rt_memcpy(pixel, pixel_addr, pixel_bytes);
    }

    return result;
}

rt_err_t rt_graphic_refresh(rt_device_graphic_t gra)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(gra != RT_NULL);

    if (gra->ops->convert)
        result = gra->ops->convert(gra);

    if (gra->ops->refresh)
        result = gra->ops->refresh(gra);

    return result;
}
