/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <mach/hardware.h>
#include <mach/board.h>
#include "../sensor_drv_sprd.h"

static int sensor_s5k4ecgx_poweron(struct sensor_power *main_cfg, struct sensor_power *sub_cfg)
{
	int ret = 0;
#if 0
	/*set default status for main and sub sensor*/
	sensor_k_sensor_sel(SENSOR_SUB);//select sub sensor(sensor hi255);
	sensor_k_set_pd_level(0);//power down valid for hi255
	sensor_k_set_rst_level(0);//reset valid for hi255

	sensor_k_sensor_sel(SENSOR_MAIN);//select main sensor(sensor hi544);
	sensor_k_set_pd_level(0);//power down valid for hi544
	sensor_k_set_rst_level(0);//reset valid for hi544
	mdelay(1);

	/*power on sequence*/
	sensor_k_set_voltage_cammot(SENSOR_VDD_2800MV);//AF monitor
	mdelay(1);//delay
	sensor_k_set_voltage_avdd(SENSOR_VDD_2800MV);//anolog vdd
	udelay(500);//delay >2ms
	sensor_k_sensor_sel(SENSOR_SUB);
	sensor_k_set_voltage_dvdd(SENSOR_VDD_1800MV);
	udelay(300);
	sensor_k_set_voltage_iovdd(SENSOR_VDD_1800MV);//IO vdd
	udelay(300);//delay >2ms
	sensor_k_set_pd_level(1);
	udelay(220);
	sensor_k_set_mclk(24);
	mdelay(4);//delay > 0us
	sensor_k_set_rst_level(1);
	udelay(1500);
	sensor_k_set_pd_level(0);
	sensor_k_sensor_sel(SENSOR_MAIN);
	udelay(300);
	sensor_k_set_voltage_dvdd(SENSOR_VDD_1200MV);//core vdd
	udelay(1000);//delay >= 10us
	sensor_k_set_pd_level(1);//power down
	udelay(30);//delay >= 15us
	sensor_k_set_rst_level(1);//reset
	udelay(100);//delay > 60us
	printk("sensor_s5k4ecgx_poweron OK \n");
#endif
	
	sensor_k_sensor_sel(SENSOR_MAIN);	//select main sensor(sensor S5K4ECGX);
	sensor_k_set_pd_level(0);			//power down valid for hi544
	sensor_k_set_rst_level(0);			//reset valid for hi544
	mdelay(1);

	/*power on sequence*/
	sensor_k_set_voltage_cammot(SENSOR_VDD_2800MV);	//AF monitor
	mdelay(1);
	sensor_k_set_voltage_iovdd(SENSOR_VDD_1800MV);	//IO vdd (VDDIO 1.8V)
	udelay(100);	//delay <2ms
	sensor_k_set_voltage_avdd(SENSOR_VDD_2800MV);		//anolog vdd (AVDD 2.8V)
	mdelay(1);	//delay >0us
	sensor_k_set_mclk(24);
	mdelay(1);	//delay >=0us
	sensor_k_set_voltage_dvdd(SENSOR_VDD_1200MV);		//core vdd (5M CORE 1.2V)
	udelay(1000);	//delay >10us
	sensor_k_set_pd_level(1);						//power down (5Mp STBY)
	udelay(30);//delay > 15us
	sensor_k_set_rst_level(1);						//reset (5Mp RESET)
	udelay(100);//delay > 60us
	printk("sensor_s5k4ecgx_poweron OK \n");

	
	return ret;
}

static int sensor_s5k4ecgx_poweroff(struct sensor_power *main_cfg, struct sensor_power *sub_cfg)
{
	int ret = 0;
#if 0
	sensor_k_sensor_sel(SENSOR_MAIN);//select main sensor(sensor hi544);
	udelay(2);//delay 2us > 16MCLK = 16/24 us
	sensor_k_set_rst_level(0);//reset valid for hi544	
	udelay(100);//delay >50us
	sensor_k_set_mclk(0);// disable mclk
	udelay(100);//delay >= 0us
	sensor_k_set_pd_level(0);
	udelay(100);
	sensor_k_sensor_sel(SENSOR_SUB);
	sensor_k_set_rst_level(0);
	udelay(100);//delay < 10ms
	sensor_k_set_voltage_iovdd(SENSOR_VDD_CLOSED);
	udelay(10);//delay < 10ms
	sensor_k_set_voltage_dvdd(SENSOR_VDD_CLOSED);
	udelay(10);//delay < 10ms
	sensor_k_set_voltage_avdd(SENSOR_VDD_CLOSED);//close anolog vdd
	mdelay(5);//delay >= 0us
	sensor_k_sensor_sel(SENSOR_MAIN);
	sensor_k_set_voltage_dvdd(SENSOR_VDD_CLOSED);//close core vdd
	mdelay(1);//delay
	sensor_k_set_voltage_cammot(SENSOR_VDD_CLOSED);//AF monitor
	udelay(10);//delay

	printk("sensor_s5k4ecgx_poweroff OK \n");
#endif

	sensor_k_sensor_sel(SENSOR_MAIN);	//select main sensor(sensor S5K4ECGX);

	/*power off sequence*/
	udelay(2);	//delay >0us
	sensor_k_set_rst_level(0);						//reset valid for S5K4ECGX
	udelay(100);	//delay >50us
	sensor_k_set_mclk(0);							// disable mclk
	udelay(100);	//delay >0us
	sensor_k_set_pd_level(0);						// power down (5Mp STBY)
	udelay(100);	//delay >0us
	sensor_k_set_voltage_avdd(SENSOR_VDD_CLOSED);		//close anolog vdd (AVDD 2.8V)
	mdelay(1);	//delay <10ms
	sensor_k_set_voltage_iovdd(SENSOR_VDD_CLOSED);	//close IO vdd (VDDIO 1.8V)
	mdelay(5);	//delay > 0us
	sensor_k_set_voltage_dvdd(SENSOR_VDD_CLOSED);		//close core vdd (5M CORE 1.2V)	
	mdelay(1);	//delay
	sensor_k_set_voltage_cammot(SENSOR_VDD_CLOSED);	//AF monitor
	udelay(10);	//delay

	printk("sensor_s5k4ecgx_poweroff OK \n");

	
	return ret;

}


static int sensor_sr030pc50_poweron(struct sensor_power *main_cfg, struct sensor_power *sub_cfg)
{
	int ret = 0;

//	sensor_k_sensor_sel(SENSOR_MAIN);//select main sensor
//	sensor_k_set_pd_level(0);//power down valid for main sensor
//	sensor_k_set_rst_level(0);//reset valid for main sensor
	sensor_k_sensor_sel(SENSOR_SUB);//select main sensor
	sensor_k_set_pd_level(0);//power down valid for main sensor
	sensor_k_set_rst_level(0);//reset valid for main sensor
	udelay(1);
	sensor_k_set_voltage_avdd(SENSOR_VDD_2800MV);
	mdelay(1);//delay 6ms < 10ms
	sensor_k_set_voltage_dvdd(SENSOR_VDD_1800MV);
	mdelay(1);//delay 6ms < 10m
	sensor_k_set_voltage_iovdd(SENSOR_VDD_1800MV);//IO vdd
//	mdelay(1);
//	sensor_k_sensor_sel(SENSOR_MAIN);
//	sensor_k_set_voltage_dvdd(SENSOR_VDD_1200MV);
//	mdelay(2);
//	sensor_k_set_voltage_dvdd(SENSOR_VDD_CLOSED);
	mdelay(3);
//	sensor_k_sensor_sel(SENSOR_SUB);
	sensor_k_set_mclk(24);
	mdelay(2);
	sensor_k_set_pd_level(1);
	mdelay(30);//delay 30ms >= 30ms
	sensor_k_set_rst_level(1);
	mdelay(55);//delay 2us > 16MCLK = 16/24 us
	printk("sr030pc50_poweron OK \n");

	return ret;
}

static int sensor_sr030pc50_poweroff(struct sensor_power *main_cfg, struct sensor_power *sub_cfg)
{
	int ret = 0;

	sensor_k_sensor_sel(SENSOR_SUB);//select sub sensor
	udelay(2);//delay 2us > 16MCLK = 16/24 us
	sensor_k_set_rst_level(0);//reset valid for sub sensor
	udelay(2);//delay 2us > 16MCLK = 16/24 us
	sensor_k_set_pd_level(0);
	udelay(1);
	sensor_k_set_mclk(0);//delay 1us > 0ns
	//power down valid for sub sensor
	mdelay(6);//delay 6ms < 10ms
	sensor_k_set_voltage_iovdd(SENSOR_VDD_CLOSED);
	udelay(30);
	sensor_k_set_voltage_dvdd(SENSOR_VDD_CLOSED);
	udelay(30);
	sensor_k_set_voltage_avdd(SENSOR_VDD_CLOSED);
	//mdelay(6);//delay 6ms < 10ms
	
	udelay(1);
	printk("sr030pc50_poweroff OK \n");

	return ret;
}

int sensor_power_on(uint8_t sensor_id, struct sensor_power *main_cfg, struct sensor_power *sub_cfg)
{
	int ret = 0;

	if (!main_cfg || !sub_cfg) {
		printk("sensor_power_on, para err 0x%x 0x%x \n", main_cfg, sub_cfg);
	}
	if (SENSOR_MAIN == sensor_id) {
		ret = sensor_s5k4ecgx_poweron(main_cfg, sub_cfg);
	} else {
		ret = sensor_sr030pc50_poweron(main_cfg, sub_cfg);
	}
	return ret;
}

int sensor_power_off(uint8_t sensor_id, struct sensor_power *main_cfg, struct sensor_power *sub_cfg)
{
	int ret = 0;

	if (!main_cfg || !sub_cfg) {
		printk("sensor_power_off, para err 0x%x 0x%x \n", main_cfg, sub_cfg);
	}

	if (SENSOR_MAIN == sensor_id) {
		ret = sensor_s5k4ecgx_poweroff(main_cfg, sub_cfg);
	} else {
		ret = sensor_sr030pc50_poweroff(main_cfg, sub_cfg);
	}

	return ret;
}
