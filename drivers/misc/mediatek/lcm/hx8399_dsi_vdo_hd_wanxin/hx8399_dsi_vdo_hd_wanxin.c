/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH     (1080)
#define FRAME_HEIGHT    (1920)
#define LCM_DENSITY		(480)

#define LCM_PHYSICAL_WIDTH		(64500)//(74520)//(64800)
#define LCM_PHYSICAL_HEIGHT		(129000)//(132480)//(129600)

#define LCM_ID                                          (0x98)

#define REGFLAG_DELAY             								(0XFD)
#define REGFLAG_END_OF_TABLE      								(0x100)	// END OF REGISTERS MARKER

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static struct LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    									(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 										(lcm_util.udelay(n))
#define MDELAY(n) 										(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};
#if 0
static void init_lcm_registers(void)
{
	unsigned int data_array[16];

#ifdef BUILD_LK
	printf("%s, LK\n", __func__);
#else
	pr_debug("%s, KE\n", __func__);
#endif


	
	data_array[0] = 0x00043902;
	data_array[1] = 0x9983FFB9;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(5);
	data_array[0] = 0x00103902;
	data_array[1] = 0x6B0402B1;
	data_array[2] = 0x3332018B;
	data_array[3] = 0x01531111;
	data_array[4] = 0x02027356;
	dsi_set_cmdq(data_array, 5, 1);
	MDELAY(5);
	data_array[0] = 0x000C3902;
	data_array[1] = 0x808000B2;
	data_array[2] = 0x5A0705AE;
	data_array[3] = 0x10100011;
	dsi_set_cmdq(data_array, 4, 1);
	MDELAY(5);
	data_array[0] = 0x002D3902;
	data_array[1] = 0x00FF00B4;
	data_array[2] = 0x009C00AC;
	data_array[3] = 0x00000800;
	data_array[4] = 0x02260002;
	data_array[5] = 0x07770907;
	data_array[6] = 0xA4040000;
	data_array[7] = 0x00AC0088;
	data_array[8] = 0x0800009C;
	data_array[9] = 0x00020000;
	data_array[10] = 0x09070224;
	data_array[11] = 0xA4040000;
	data_array[12] = 0x00000012;
	dsi_set_cmdq(data_array, 13, 1);
	MDELAY(5);
	data_array[0] = 0x00223902;
	data_array[1] = 0x000000D3;
	data_array[2] = 0x00000000;
	data_array[3] = 0x05103200;
	data_array[4] = 0x00000500;
	data_array[5] = 0x00000000;
	data_array[6] = 0x00000000;
	data_array[7] = 0x07070001;
	data_array[8] = 0x00000003;
	data_array[9] = 0x00004005;
	dsi_set_cmdq(data_array, 10, 1);

	MDELAY(5);

	data_array[0] = 0x00213902;
	data_array[1] = 0x000000D5;
	data_array[2] = 0x00030201;
	data_array[3] = 0x19000000;
	data_array[4] = 0x20001800;
	data_array[5] = 0x00180021;
	data_array[6] = 0x00000000;
	data_array[7] = 0x31000000;
	data_array[8] = 0x2F303031;
	data_array[9] = 0x0000002F;
	dsi_set_cmdq(data_array, 10, 1);
	MDELAY(5);
	MDELAY(5);
	data_array[0] = 0x00213902;
	data_array[1] = 0x034040D6;
	data_array[2] = 0x40000102;
	data_array[3] = 0x18404040;
	data_array[4] = 0x21401940;
	data_array[5] = 0x40184020;
	data_array[6] = 0x40404040;
	data_array[7] = 0x31404040;
	data_array[8] = 0x2F303031;
	data_array[9] = 0x0000002F;
	dsi_set_cmdq(data_array, 10, 1);
	
	MDELAY(5);

	data_array[0] = 0x00113902;
	data_array[1] = 0x000228D8;
	data_array[2] = 0xC002282A;
	data_array[3] = 0x0000002A;
	data_array[4] = 0x00000000;
	data_array[5] = 0x00000000;
	dsi_set_cmdq(data_array, 6, 1);

	data_array[0] = 0x01BD1500;
	dsi_set_cmdq(data_array, 1, 1);
		
	data_array[0] = 0x00113902;
	data_array[1] = 0x000000D8;
	data_array[2] = 0x00000000;
	data_array[3] = 0x002A2800;
	data_array[4] = 0xC002282A;
	data_array[5] = 0x0000002A;
	dsi_set_cmdq(data_array, 6, 1);
	MDELAY(5);
	data_array[0] = 0x02BD1500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(5);
	data_array[0] = 0x00093902;
	data_array[1] = 0x002A28D8;
	data_array[2] = 0xC002282A;
	data_array[3] = 0x0000002A;
	dsi_set_cmdq(data_array, 4, 1);
	MDELAY(5);
	data_array[0] = 0x00BD1500;
	dsi_set_cmdq(data_array, 1, 1);
	data_array[0] = 0x00373902;
	 data_array[1] = 0x493F1CE0;
	 data_array[2] = 0x90888441;
	 data_array[3] = 0x98928D87;
	 data_array[4] = 0xA9A59F9B;
	 data_array[5] = 0xB8B6AFAC;
	 data_array[6] = 0xC4C1B3C0;
	 data_array[7] = 0x69645E64;
	 data_array[8] = 0x41493F1C;
	 data_array[9] = 0x87908884;
	 data_array[10] = 0x9B98928D;
	 data_array[11] = 0xACA9A59F;
	 data_array[12] = 0xC0B8B6AF;
	 data_array[13] = 0x64C4C1B3;
	 data_array[14] = 0x0069645E;
	dsi_set_cmdq(data_array, 15, 1);	

	MDELAY(5);

	data_array[0] = 0x33D21500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(5);
	data_array[0] = 0x00033902;
	data_array[1] = 0x008D8DB6;
	dsi_set_cmdq(data_array, 2, 1);
	MDELAY(5);
	data_array[0] = 0x00083902;
	data_array[1] = 0x504140BF;
	data_array[2] = 0xCD801A09;
	dsi_set_cmdq(data_array, 3, 1);
	MDELAY(5);
	data_array[0] = 0x08CC1500;  //0x0ACC1500
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(5);
	//data_array[0] = 0x00211500;
	//dsi_set_cmdq(data_array, 1, 1);
	//MDELAY(5);	
	
	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(150);

	data_array[0] = 0x00290500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(50);
	

}
#endif



#if 1
static struct LCM_setting_table lcm_initialization_setting[] = {
	{0xB9,3,{0xFF,0x83,0x99}},
	{0xB1,15,{0x02,0x04,0x6B,0x8B,0x01,0x32,0x33,0x11,0x11,0x53,0x01,0x56,0x73,0x02,0x02}},
	{0xB2,11,{0x00,0x80,0x80,0xAE,0x05,0x07,0x5A,0x11,0x00,0x10,0x10}},
	{0xB4,44,{0x00,0xFF,0x00,0xAC,0x00,0x9C,0x00,0x00,0x08,0x00,0x00,0x02,0x00,0x26,0x02,0x07,0x09,0x21,0x03,0x00,0x00,0x04,0xA4,0x88,0x00,0xAC,0x00,0x9C,0x00,0x00,0x08,0x00,0x00,0x02,0x00,0x24,0x02,0x07,0x09,0x00,0x00,0x04,0xA4,0x12}},
	{0xD3,33,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0x10,0x05,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x07,0x07,0x03,0x00,0x00,0x00,0x05,0x40}},
	{0xD5,32,{0x00,0x00,0x00,0x01,0x02,0x03,0x00,0x00,0x00,0x00,0x19,0x00,0x18,0x00,0x20,0x21,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x31,0x30,0x30,0x2F,0x2F}},
	{0xD6,32,{0x40,0x40,0x03,0x02,0x01,0x00,0x40,0x40,0x40,0x40,0x18,0x40,0x19,0x40,0x21,0x20,0x40,0x18,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x31,0x31,0x30,0x30,0x2F,0x2F}},
	{0xD8,16,{0x28,0x02,0x00,0x2A,0x28,0x02,0xC0,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0xBD,1,{0x01}},
	{0xD8,16,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x2A,0x00,0x2A,0x28,0x02,0xC0,0x2A}},
	{0xBD,1,{0x02}},
	{0xD8,8,{0x28,0x2A,0x00,0x2A,0x28,0x02,0xC0,0x2A}},
	{0xBD,1,{0x00}},
	{0xE0,54,{0x1C,0x3F,0x49,0x41,0x84,0x88,0x90,0x87,0x8D,0x92,0x98,0x9B,0x9F,0xA5,0xA9,0xAC,0xAF,0xB6,0xB8,0xC0,0xB3,0xC1,0xC4,0x64,0x5E,0x64,0x69,0x1C,0x3F,0x49,0x41,0x84,0x88,0x90,0x87,0x8D,0x92,0x98,0x9B,0x9F,0xA5,0xA9,0xAC,0xAF,0xB6,0xB8,0xC0,0xB3,0xC1,0xC4,0x64,0x5E,0x64,0x69}},
	{0xD2,1,{0x33}},
	{0xB6,2,{0x7F,0x7F}},
	{0xBF,7,{0x40,0x41,0x50,0x09,0x1A,0x80,0xCD}},
	{0xCC,1,{0x08}},

        {0x11,1,{0x00}},  
		{ REGFLAG_DELAY, 120, {} },
		{0x29,1,{0x00}},
		{ REGFLAG_DELAY, 20, {} },
		{REGFLAG_END_OF_TABLE, 0x00, {}} 
};

#endif



#if 1
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}
#endif

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

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
	// enable tearing-free
	params->dbi.te_mode 			= LCM_DBI_TE_MODE_DISABLED;
	params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

	params->dsi.mode   = SYNC_PULSE_VDO_MODE;	//SYNC_PULSE_VDO_MODE;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM			= LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine. 
	params->dsi.data_format.color_order 	= LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   	= LCM_DSI_TRANS_SEQ_MSB_FIRST; 
	params->dsi.data_format.padding     	= LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      	= LCM_DSI_FORMAT_RGB888;
	// Highly depends on LCD driver capability.
	// Not support in MT6573
	params->dsi.packet_size=256;
	// Video mode setting		
	params->dsi.intermediat_buffer_num 	= 2;
	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		
        params->dsi.word_count = FRAME_WIDTH * 3;
	params->dsi.vertical_sync_active				= 4;//4
	params->dsi.vertical_backporch					= 10;//12
	params->dsi.vertical_frontporch					= 30;//15
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 
	params->dsi.horizontal_sync_active				= 20;
	params->dsi.horizontal_backporch				= 20;//66
	params->dsi.horizontal_frontporch				= 20;//70
	//params->dsi.horizontal_blanking_pixel		       		= 60;
	params->dsi.horizontal_active_pixel		       		= FRAME_WIDTH;
	// Bit rate calculation

	params->dsi.PLL_CLOCK=430;//205

}




static void lcm_init(void)
{

    SET_RESET_PIN(1);
    MDELAY(20);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(150);
//lcm_initialization_setting[15].para_list[0]=lcm_initialization_setting[15].para_list[0]+1;
//lcm_initialization_setting[15].para_list[1]=lcm_initialization_setting[15].para_list[1]+1;
 //printk("value=%x\n",lcm_initialization_setting[15].para_list[0]);
 // printk("value=%x\n",lcm_initialization_setting[15].para_list[1]);
  push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
   //init_lcm_registers();
}

static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},
	{ REGFLAG_DELAY, 50, {} },
    // Sleep Mode On
	{0x10, 0, {0x00}},
	{ REGFLAG_DELAY, 100, {} },
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};
static void lcm_suspend(void)
{
	push_table(lcm_sleep_in_setting, sizeof(lcm_sleep_in_setting) / sizeof(struct LCM_setting_table), 1);
    MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(20);

}

static unsigned int lcm_compare_id(void);

static void lcm_resume(void)
{
#ifndef BUILD_LK
//        lcm_compare_id();
	lcm_init();
#endif
}



static unsigned int lcm_compare_id(void)
{
	int array[4];
	char buffer[3];
	//char id_high=0;
	//char id_low=0;
	int id=0;

	SET_RESET_PIN(1);
	MDELAY(20); 
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);

	//{0x39, 0xFF, 5, { 0xFF,0x98,0x06,0x04,0x01}}, // Change to Page 1 CMD
	array[0] = 0x00043902;
	array[1] = 0x018198FF;
	dsi_set_cmdq(array, 2, 1);

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x00, &buffer[0], 1);  //0x98

	id = buffer[0];

	return 1;

}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
struct LCM_DRIVER hx8399_dsi_vdo_hd_wanxin_lcm_drv = 
{
	.name			  = "hx8399_dsi_vdo_hd_wanxin",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,	
	.compare_id     = lcm_compare_id,	
//	.esd_check   = lcm_esd_check,	
//	.esd_recover   = lcm_esd_recover,	
//	.update         = lcm_update,
};

