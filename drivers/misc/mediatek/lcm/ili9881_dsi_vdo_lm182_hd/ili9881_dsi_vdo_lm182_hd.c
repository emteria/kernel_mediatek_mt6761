/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#define LOG_TAG "LCM"

#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"


#define LCM_ID_ILI9881 (0x98)

static const unsigned int BL_MIN_LEVEL = 20;
static struct LCM_UTIL_FUNCS lcm_util;


#define SET_RESET_PIN(v)	(lcm_util.set_reset_pin((v)))
#define MDELAY(n)		(lcm_util.mdelay(n))
#define UDELAY(n)		(lcm_util.udelay(n))

#define dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update) \
	lcm_util.dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) \
	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) \
		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd) lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) \
		lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) \
	  lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size) \
		lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
/* #include <linux/jiffies.h> */
/* #include <linux/delay.h> */
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#endif

/* static unsigned char lcd_id_pins_value = 0xFF; */
static const unsigned char LCD_MODULE_ID = 0x01;
#define LCM_DSI_CMD_MODE	0
#define FRAME_WIDTH		(720)
#define FRAME_HEIGHT	(1280)
#define LCM_DENSITY		(320)

#define LCM_PHYSICAL_WIDTH		(64800)
#define LCM_PHYSICAL_HEIGHT		(129600)

#define REGFLAG_DELAY		0xFFFC
#define REGFLAG_UDELAY	0xFFFB
#define REGFLAG_END_OF_TABLE	0xFFFD
#define REGFLAG_RESET_LOW	0xFFFE
#define REGFLAG_RESET_HIGH	0xFFFF

static struct LCM_DSI_MODE_SWITCH_CMD lcm_switch_mode_cmd;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct LCM_setting_table {
	unsigned int cmd;
	unsigned char count;
	unsigned char para_list[64];
};

static struct LCM_setting_table lcm_suspend_setting[] = {
        {0xFF,3,{0x98,0x81,0x00}},
	{0x28, 0, {} },
	{REGFLAG_DELAY, 20, {} },
	{0x10, 0, {} },
	{REGFLAG_DELAY, 150, {} },
};

static struct LCM_setting_table init_setting[] = {

{0xFF,3,{0x98,0x81,0x03 }},
{0x01,01,{0x00}},
{0x02,01,{0x00}},
{0x03,01,{0x73}},
{0x04,01,{0x00}},
{0x05,01,{0x00}},
{0x06,01,{0x09}},
{0x07,01,{0x00}},
{0x08,01,{0x00}},
{0x09,01,{0x01}},
{0x0A,01,{0x01}},
{0x0B,01,{0x01}},
{0x0C,01,{0x01}},
{0x0D,01,{0x01}},
{0x0E,01,{0x01}},
{0x0F,01,{0x26}},
{0x10,01,{0x26}},
{0x11,01,{0x00}},
{0x12,01,{0x00}},
{0x13,01,{0x00}},
{0x14,01,{0x00}},
{0x15,01,{0x00}},
{0x16,01,{0x00}},
{0x17,01,{0x00}},
{0x18,01,{0x00}},
{0x19,01,{0x00}},
{0x1A,01,{0x00}},
{0x1B,01,{0x00}},
{0x1C,01,{0x00}},
{0x1D,01,{0x00}},
{0x1E,01,{0x40}},
{0x1F,01,{0xC1}},
{0x20,01,{0x06}},
{0x21,01,{0x01}},
{0x22,01,{0x06}},
{0x23,01,{0x01}},
{0x24,01,{0x8C}},
{0x25,01,{0x8C}},
{0x26,01,{0x00}},
{0x27,01,{0x00}},
{0x28,01,{0x33}},
{0x29,01,{0x03}},
{0x2A,01,{0x88}},
{0x2B,01,{0x00}},
{0x2C,01,{0x0A}},
{0x2D,01,{0x0A}},
{0x2E,01,{0x05}},
{0x2F,01,{0x05}},
{0x30,01,{0x00}},
{0x31,01,{0x00}},
{0x32,01,{0x30}},
{0x33,01,{0x02}},
{0x34,01,{0xA3}},
{0x35,01,{0x0A}},
{0x36,01,{0x03}},
{0x37,01,{0x00}},
{0x38,01,{0x00}},
{0x39,01,{0x00}},
{0x3A,01,{0x00}},
{0x3B,01,{0x00}},
{0x3C,01,{0x00}},
{0x3D,01,{0x00}},
{0x3E,01,{0x00}},
{0x3F,01,{0x00}},
{0x40,01,{0x00}},
{0x41,01,{0x00}},
{0x42,01,{0x00}},
{0x43,01,{0x08}},
{0x44,01,{0x00}},

{0x50,01,{0x01}},
{0x51,01,{0x23}},
{0x52,01,{0x45}},
{0x53,01,{0x67}},
{0x54,01,{0x89}},
{0x55,01,{0xab}},
{0x56,01,{0x01}},
{0x57,01,{0x23}},
{0x58,01,{0x45}},
{0x59,01,{0x67}},
{0x5a,01,{0x89}},
{0x5b,01,{0xab}},
{0x5c,01,{0xcd}},
{0x5d,01,{0xef}},

{0x5e,01,{0x11}},
{0x5f,01,{0x17}},     //GOUT_L[1]_GCL_R  }},
{0x60,01,{0x00}},     //GOUT_L[2]_VSD    }},
{0x61,01,{0x0E}},     //GOUT_L[3]_CLK1R  }},
{0x62,01,{0x0F}},     //GOUT_L[4]_CLK2R  }},
{0x63,01,{0x0C}},//GOUT_L[5]_CLK3R  }},
{0x64,01,{0x0D}},//GOUT_L[6]_CLK4R  }},
{0x65,01,{0x02}},//GOUT_L[7]_VGL}},
{0x66,01,{0x02}},//GOUT_L[8]_VGL}},
{0x67,01,{0x02}},//GOUT_L[9]_VGL}},
{0x68,01,{0x02}},//GOUT_L[10]}},
{0x69,01,{0x02}},//GOUT_L[11]}},
{0x6a,01,{0x02}},//GOUT_L[12]}},
{0x6b,01,{0x02}},//GOUT_L[13]}},
{0x6c,01,{0x02}},//GOUT_L[14]}},
{0x6d,01,{0x02}},//GOUT_L[15]}},
{0x6e,01,{0x02}},//GOUT_L[16]}},
{0x6f,01,{0x02}},//GOUT_L[17]}},
{0x70,01,{0x02}},//GOUT_L[18]}},
{0x71,01,{0x16}},//GOUT_L[19]_GCH}},
{0x72,01,{0x01}},//GOUT_L[20]_VDS}},
{0x73,01,{0x06}},//GOUT_L[21]_STV1R}},
{0x74,01,{0x07}},//GOUT_L[22]_STV2R}},

{0x75,01,{0x17}},//GOUT_R[1]_GCL_L}},
{0x76,01,{0x00}},//GOUT_R[2]_VSD}},
{0x77,01,{0x0E}},//GOUT_R[3]_CLK1L}},
{0x78,01,{0x0F}},//GOUT_R[4]_CLK2L}},
{0x79,01,{0x0C}},//GOUT_R[5]_CLK3L}},
{0x7a,01,{0x0D}},//GOUT_R[6]_CLK4L}},
{0x7b,01,{0x0F}},//GOUT_R[7]_VGL}},
{0x7c,01,{0x0F}},//GOUT_R[8]_VGL}},
{0x7d,01,{0x02}},//GOUT_R[9]_VGL}},
{0x7e,01,{0x02}},//GOUT_R[10]}},
{0x7f,01,{0x02}},//GOUT_R[11]}},
{0x80,01,{0x02}},//GOUT_R[12]}},
{0x81,01,{0x02}},//GOUT_R[13]}},
{0x82,01,{0x02}},//GOUT_R[14]}},
{0x83,01,{0x02}},//GOUT_R[15]}},
{0x84,01,{0x02}},//GOUT_R[16]}},
{0x85,01,{0x02}},//GOUT_R[17]}},
{0x86,01,{0x02}},//GOUT_R[18]}},
{0x87,01,{0x06}},//GOUT_R[19]_GCH}},
{0x88,01,{0x01}},//GOUT_R[20]_VDS}},
{0x89,01,{0x06}},//GOUT_R[21]_STV1L}},
{0x8A,01,{0x07}},//GOUT_R[22]_STV2L }},
{0xFF,3,{0x98,0x81,0x04}},
//{0x00,1,00}},
{0x6C,01,{0x15}},
{0x6E,01,{0x1A }},
{0x6F,01,{0x25 }},
{0x3A,01,{0xA4 }},
{0x8D,01,{0x20 }},
{0x87,01,{0xBA }},
{0x26,01,{0x76 }},
{0xB2,01,{0xD1 }},

{0xFF,3,{0x98,0x81,0x01}},
{0x22,01,{0x0A }},
{0x31,01,{0x00 }},
{0x53,01,{0x62 }},     //VCOM1 ä»?x50-0x70æ¯æ¬¡è°?ä¸ªå•ä½?{0x55,01,{0x5A }},
{0x50,01,{0x75 }},
{0x51,01,{0x71 }},
{0x60,01,{0x1B }},
{0x61,01,{0x01 }},
{0x62,01,{0x0C }},
{0x63,01,{0x00 }},

{0xA0,01,{0x00}}, //gamma2.2_0912}},
{0xA1,01,{0x16}},//10 }},
{0xA2,01,{0x23}},//1c }},
{0xA3,01,{0x12}},//12 }},
{0xA4,01,{0x15}},//14 }},
{0xA5,01,{0x28}},//27 }},
{0xA6,01,{0x1D}},//1b }},
{0xA7,01,{0x1E}},//1d }},
{0xA8,01,{0x80}},//7a }},
{0xA9,01,{0x1D}},//1e }},
{0xAA,01,{0x29}},//2a }},
{0xAB,01,{0x6E}},//6f }},
{0xAC,01,{0x18}},//18 }},
{0xAD,01,{0x14}},//14 }},
{0xAE,01,{0x48}},//48 }},
{0xAF,01,{0x1E}},//1d }},
{0xB0,01,{0x26}},//27 }},
{0xB1,01,{0x4D}},//4c}},
{0xB2,01,{0x60}},//5e }},
{0xB3,01,{0x39}},

{0xC0,01,{0x00}},
{0xC1,01,{0x15}},
{0xC2,01,{0x23}},//1c }},
{0xC3,01,{0x12}},//12 }},
{0xC4,01,{0x15}},//14 }},
{0xC5,01,{0x27}},//26 }},
{0xC6,01,{0x1D}},//1a }},
{0xC7,01,{0x1F}},//1d }},
{0xC8,01,{0x7F}},//7a }},
{0xC9,01,{0x1D}},//1e }},
{0xCA,01,{0x29}},//2a }},
{0xCB,01,{0x6F}},//71 }},
{0xCC,01,{0x19}},//19 }},
{0xCD,01,{0x16}},//16 }},
{0xCE,01,{0x49}},//49 }},
{0xCF,01,{0x1F}},//1e }},
{0xD0,01,{0x26}},//27 }},
{0xD1,01,{0x4D}},//4b }},
{0xD2,01,{0x60}},//5e }},
{0xD3,01,{0x39}},

{0xFF,3,{0x98,0x81,0x00}},
{0x35,01,{0x00}},

{0xFF,3,{0x98,0x81,0x00}},
{0x28,01,{0x00}},
{REGFLAG_DELAY,20,{0x00}},
{0x10,01,{0x00}},
{REGFLAG_DELAY,150,{0x00}},

{0xFF,3,{0x98,0x81,0x00}},
{0x11,01,{0x00}},
{REGFLAG_DELAY,150,{0x00}},
{0x29,01,{0x00}},
{REGFLAG_DELAY,20,{0x00}},

};

#if 0
static struct LCM_setting_table lcm_set_window[] = {
	{0x2A, 4, {0x00, 0x00, (FRAME_WIDTH >> 8), (FRAME_WIDTH & 0xFF)} },
	{0x2B, 4, {0x00, 0x00, (FRAME_HEIGHT >> 8), (FRAME_HEIGHT & 0xFF)} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};
#endif
#if 0
static struct LCM_setting_table lcm_sleep_out_setting[] = {
	/* Sleep Out */
	{0xFF,3,{0x98,0x81,0x00}},
	{0x11, 1, {0x00} },
	{REGFLAG_DELAY, 150, {} },

	/* Display ON */
	{0x29, 1, {0x00} },
	{REGFLAG_DELAY, 20, {} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	/* Display off sequence */
	{0xFF,3,{0x98,0x81,0x00}},
	{0x28, 1, {0x00} },
	{REGFLAG_DELAY, 50, {} },

	/* Sleep Mode On */
	{0x10, 1, {0x00} },
	{REGFLAG_DELAY, 150, {} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};
#endif
static struct LCM_setting_table bl_level[] = {
	{0x51, 1, {0xFF} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static void push_table(void *cmdq, struct LCM_setting_table *table,
	unsigned int count, unsigned char force_update)
{
	unsigned int i;
	unsigned int cmd;

	for (i = 0; i < count; i++) {
		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY:
			if (table[i].count <= 10)
				MDELAY(table[i].count);
			else
				MDELAY(table[i].count);
			break;

		case REGFLAG_UDELAY:
			UDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V22(cmdq, cmd, table[i].count,
				table[i].para_list, force_update);
		}
	}
}


static void lcm_set_util_funcs(const struct LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(struct LCM_UTIL_FUNCS));
}


static void lcm_get_params(struct LCM_PARAMS *params)
{
	memset(params, 0, sizeof(struct LCM_PARAMS));

	params->type = LCM_TYPE_DSI;

	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	params->physical_width = LCM_PHYSICAL_WIDTH/1000;
	params->physical_height = LCM_PHYSICAL_HEIGHT/1000;
	params->physical_width_um = LCM_PHYSICAL_WIDTH;
	params->physical_height_um = LCM_PHYSICAL_HEIGHT;
	params->density		   = LCM_DENSITY;

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode = CMD_MODE;
	params->dsi.switch_mode = SYNC_PULSE_VDO_MODE;
	lcm_dsi_mode = CMD_MODE;
#else
	params->dsi.mode = SYNC_PULSE_VDO_MODE;
	params->dsi.switch_mode = CMD_MODE;
	lcm_dsi_mode = SYNC_PULSE_VDO_MODE;
#endif
	pr_debug("[LCM]lcm_get_params lcm_dsi_mode %d\n", lcm_dsi_mode);
	params->dsi.switch_mode_enable = 0;

	/* DSI */
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	/* The following defined the fomat for data coming from LCD engine. */
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	/* Highly depends on LCD driver capability. */
	params->dsi.packet_size = 256;
	/* video mode timing */

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;

	params->dsi.vertical_sync_active = 8;
	params->dsi.vertical_backporch = 16;
	params->dsi.vertical_frontporch = 16;
	params->dsi.vertical_frontporch_for_low_power = 540;
	params->dsi.vertical_active_line = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active = 20;
	params->dsi.horizontal_backporch = 60;
	params->dsi.horizontal_frontporch = 60;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;
	/*params->dsi.ssc_disable = 1;*/
#ifndef CONFIG_FPGA_EARLY_PORTING
#if (LCM_DSI_CMD_MODE)
	params->dsi.PLL_CLOCK = 210;
#else
	params->dsi.PLL_CLOCK = 210;
#endif
	params->dsi.PLL_CK_CMD = 210;
	params->dsi.PLL_CK_VDO = 210;
#else
	params->dsi.pll_div1 = 0;
	params->dsi.pll_div2 = 0;
	params->dsi.fbk_div = 0x1;
#endif
	//params->dsi.CLK_HS_POST = 36;
	params->dsi.clk_lp_per_line_enable = 0;
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 0;
	params->dsi.lcm_esd_check_table[0].cmd = 0x0A;
	params->dsi.lcm_esd_check_table[0].count = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;

#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	params->round_corner_en = 1;
	params->full_content = 0;
	params->corner_pattern_width = 720;
	params->corner_pattern_height = 32;
	params->corner_pattern_height_bot = 32;
#endif
}

static void lcm_init_power(void)
{
	display_bias_enable();
}

static void lcm_suspend_power(void)
{
	display_bias_disable();
}

static void lcm_resume_power(void)
{
	SET_RESET_PIN(0);
	display_bias_enable();
}

static void lcm_init(void)
{
	unsigned int size;

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(150);

	size = sizeof(init_setting) /sizeof(struct LCM_setting_table);
	push_table(NULL, init_setting, size, 1);
	pr_debug("[LCM]ili9881----tps6132----lcm mode = vdo mode :%d----\n", lcm_dsi_mode);
}

static void lcm_suspend(void)
{
	push_table(NULL, lcm_suspend_setting,
		sizeof(lcm_suspend_setting) / sizeof(struct LCM_setting_table),
		1);
	MDELAY(10);
	/* SET_RESET_PIN(0); */
}

static void lcm_resume(void)
{
	lcm_init();
}

static void lcm_update(unsigned int x, unsigned int y, unsigned int width,
	unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = (x0 & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = (x1 & 0xFF);
	unsigned char y0_MSB = ((y0 >> 8) & 0xFF);
	unsigned char y0_LSB = (y0 & 0xFF);
	unsigned char y1_MSB = ((y1 >> 8) & 0xFF);
	unsigned char y1_LSB = (y1 & 0xFF);

	unsigned int data_array[16];

	data_array[0] = 0x00053902;
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00053902;
	data_array[1] = (y1_MSB << 24) | (y0_LSB << 16) | (y0_MSB << 8) | 0x2b;
	data_array[2] = (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0, version_id = 0;
	unsigned char buffer[2];
	unsigned int array[16];
	struct LCM_setting_table switch_table_page1[] = {
		{ 0xFF, 0x03, {0x98, 0x81, 0x06} }
	};
	struct LCM_setting_table switch_table_page0[] = {
		{ 0xFF, 0x03, {0x98, 0x81, 0x00} }
	};

  	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(150);


	push_table(NULL,
		switch_table_page1,
		sizeof(switch_table_page1) / sizeof(struct LCM_setting_table),
		1);

	array[0] = 0x00023700;	/* read id return two byte,version and id */
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xF0, buffer, 1);
	id = buffer[0];		/* we only need ID */

	read_reg_v2(0xF1, buffer, 1);
	version_id = buffer[0];

	pr_debug("kernel %s,ili9881c id=0x%08x,version_id=0x%x\n", __func__, id, version_id);
	push_table(NULL,
		switch_table_page0,
		sizeof(switch_table_page0) / sizeof(struct LCM_setting_table),
		1);

	if (id == LCM_ID_ILI9881 && version_id == 0x81)
		return 1;
	else
		return 0;

}

/* return TRUE: need recovery */
/* return FALSE: No need recovery */
static unsigned int lcm_esd_check(void)
{
#ifndef BUILD_LK
	char buffer[3];
	int array[4];

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x53, buffer, 1);

	if (buffer[0] != 0x24) {
		pr_debug("[LCM][LCM ERROR] [0x53]=0x%02x\n", buffer[0]);
		return TRUE;
	}
	pr_debug("[LCM][LCM NORMAL] [0x53]=0x%02x\n", buffer[0]);
	return FALSE;
#else
	return FALSE;
#endif

}

static unsigned int lcm_ata_check(unsigned char *buffer)
{
#ifndef BUILD_LK
	unsigned int ret = 0;
	unsigned int x0 = FRAME_WIDTH / 4;
	unsigned int x1 = FRAME_WIDTH * 3 / 4;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = (x0 & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = (x1 & 0xFF);

	unsigned int data_array[3];
	unsigned char read_buf[4];

	pr_debug("[LCM]ATA check size = 0x%x,0x%x,0x%x,0x%x\n",
		x0_MSB, x0_LSB, x1_MSB, x1_LSB);
	data_array[0] = 0x0005390A;	/* HS packet */
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	/* read id return two byte,version and id */
	data_array[0] = 0x00043700;
	dsi_set_cmdq(data_array, 1, 1);

	read_reg_v2(0x2A, read_buf, 4);

	if ((read_buf[0] == x0_MSB) && (read_buf[1] == x0_LSB)
	    && (read_buf[2] == x1_MSB) && (read_buf[3] == x1_LSB))
		ret = 1;
	else
		ret = 0;

	x0 = 0;
	x1 = FRAME_WIDTH - 1;

	x0_MSB = ((x0 >> 8) & 0xFF);
	x0_LSB = (x0 & 0xFF);
	x1_MSB = ((x1 >> 8) & 0xFF);
	x1_LSB = (x1 & 0xFF);

	data_array[0] = 0x0005390A;	/* HS packet */
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);
	return ret;
#else
	return 0;
#endif
}

static void lcm_setbacklight_cmdq(void *handle, unsigned int level)
{

	pr_debug("[LCM]%s,ili9881 backlight: level = %d\n", __func__, level);

	bl_level[0].para_list[0] = level;

	push_table(handle, bl_level,
		sizeof(bl_level) / sizeof(struct LCM_setting_table), 1);
}

static void *lcm_switch_mode(int mode)
{
#ifndef BUILD_LK
/* customization: 1. V2C config 2 values, C2V config 1 value;
 * 2. config mode control register
 */
	if (mode == 0) {	/* V2C */
		lcm_switch_mode_cmd.mode = CMD_MODE;
		/* mode control addr */
		lcm_switch_mode_cmd.addr = 0xBB;
		/* enabel GRAM firstly, ensure writing one frame to GRAM */
		lcm_switch_mode_cmd.val[0] = 0x13;
		/* disable video mode secondly */
		lcm_switch_mode_cmd.val[1] = 0x10;
	} else {		/* C2V */
		lcm_switch_mode_cmd.mode = SYNC_PULSE_VDO_MODE;
		lcm_switch_mode_cmd.addr = 0xBB;
		/* disable GRAM and enable video mode */
		lcm_switch_mode_cmd.val[0] = 0x03;
	}
	return (void *)(&lcm_switch_mode_cmd);
#else
	return NULL;
#endif
}


struct LCM_DRIVER ili9881_dsi_vdo_lm182_hd_lcm_drv = {
	.name = "ili9881_dsi_vdo_lm182_hd_drv",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
	.compare_id = lcm_compare_id,
	.init_power = lcm_init_power,
	.resume_power = lcm_resume_power,
	.suspend_power = lcm_suspend_power,
	.esd_check = lcm_esd_check,
	.set_backlight_cmdq = lcm_setbacklight_cmdq,
	.ata_check = lcm_ata_check,
	.update = lcm_update,
	.switch_mode = lcm_switch_mode,
};
