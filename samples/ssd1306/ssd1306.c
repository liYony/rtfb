/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-06     liYony       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <ssd1306.h>

#define OLED_I2C_BUS_NAME "i2c1"
#define OLED_ADDR 0x3c

static struct rt_i2c_bus_device *i2c_bus = RT_NULL;

static rt_err_t WriteCmd(struct rt_i2c_bus_device *bus, rt_uint8_t data)
{
	rt_uint8_t buf[2];
	struct rt_i2c_msg msgs;

	buf[0] = 0x00;
	buf[1] = data;

	msgs.addr = OLED_ADDR;
	msgs.flags = RT_I2C_WR;
	msgs.buf = buf;
	msgs.len = 2;

	if (rt_i2c_transfer(bus, &msgs, 1) == 1)
	{
		return RT_EOK;
	}
	else
	{
		return -RT_ERROR;
	}
}

static rt_err_t WriteDat(struct rt_i2c_bus_device *bus, rt_uint8_t data)
{
	rt_uint8_t buf[2];
	struct rt_i2c_msg msgs;

	buf[0] = 0x40;
	buf[1] = data;

	msgs.addr = OLED_ADDR;
	msgs.flags = RT_I2C_WR;
	msgs.buf = buf;
	msgs.len = 2;

	if (rt_i2c_transfer(bus, &msgs, 1) == 1)
	{
		return RT_EOK;
	}
	else
	{
		return -RT_ERROR;
	}
}
rt_uint8_t OLED_GRAM[144][8];

//发送一个字节
//向SSD1306写入一个字节。
// mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(rt_uint8_t dat, rt_uint8_t mode)
{
	if (mode)
	{
		WriteDat(i2c_bus, dat);
	}
	else
	{
		WriteCmd(i2c_bus, dat);
	}
}

void OLED_Init(void)
{
	i2c_bus = (struct rt_i2c_bus_device *)rt_device_find("i2c1");

	OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
	OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00, OLED_CMD); //-not offset
	OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
	OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
	OLED_WR_Byte(0x12, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
	OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
	OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02, OLED_CMD); //
	OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
	OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	OLED_WR_Byte(0xAF, OLED_CMD);
}
