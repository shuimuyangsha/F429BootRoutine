/*
	应用说明:访问BH1750前，请先调用一次 bsp_InitI2C()函数配置好I2C相关的GPIO.

	光照度 lx
	勒克斯 (Lux) 被光均匀照射的物体，距离该光源1米处，在1m2面积上得到的光通量是1lm时，它的照度是1lux

	光照度可用照度计直接测量。光照度的单位是勒克斯，是英文lux的音译，也可写为lx。
	被光均匀照射的物体，在1平方米面积上得到的光通量是1流明时，它的照度是1勒克斯。有时为了充分利用光源，
	常在光源上附加一个反射装置，使得某些方向能够得到比较多的光通量，以增加这一被照面上的照度。例如
	汽车前灯、手电筒、摄影灯等。

	以下是各种环境照度值：单位lux
	黑夜：0.001—0.02；
	月夜：0.02—0.3；
	阴天室内：5—50；
	阴天室外：50—500；
	晴天室内：100—1000；
	夏季中午太阳光下的照度：约为10*9
	阅读书刊时所需的照度：50—60；
	家用摄像机标准照度：1400

	BH1750FVI 为光照度测量芯片。测量量程可通过命令进行调节。
	最小 0.11 lux， 最大 100000 lux

*/

#include "./Bsp/bh1750/bsp_bh1750.h"
#include "./Bsp/sersor_i2c/sersor_i2c.h"

static uint8_t bh1750_MTReg;	/* 灵敏度倍率 */ 
static uint8_t bh1750_Mode;	/* 测量模式 */

/*
*********************************************************************************************************
*	函 数 名: BH1750_WriteCmd
*	功能说明: 向芯片发送1字节长度的操作码
*	形    参: _ucOpecode : 寄存器地址
*	返 回 值: 无
*********************************************************************************************************
*/
static __inline uint8_t BH1750_WriteCmd(uint8_t _ucOpecode)
{
		return ST_Sensors_I2C_WriteNoRegister(BH1750_SLAVE_ADDRESS,_ucOpecode);
} 

/*
*********************************************************************************************************
*	函 数 名: BH1750_ReadData
*	功能说明: 读取BH1750测量结果.  主程序需要在 bsp_InitBH1750() 执行之后 180ms 才能读到正确数据。
*			  连续测量模式下，之后主程序可以定时调用本函数读取光强度数据，间隔时间需要大于180ms
*	形    参: 无
*	返 回 值: 原始的测量数据 ,结果未转换
*********************************************************************************************************
*/
static __inline uint16_t BH1750_ReadData(uint16_t *rdata)
{
	uint8_t buf[2],res=SENSORS_I2C_FAIL;
	res = ST_Sensors_I2C_ReadNoRegister(BH1750_SLAVE_ADDRESS,2,buf);
	*rdata = (buf[0]<<8)|buf[1];
	return res;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitBH1750
*	功能说明: 初始化BH1750
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_InitBH1750(void)
{
	return BH1750_WriteCmd(BHOP_POWER_ON);
}

uint8_t bsp_DeinitBH1750(void)
{
	return BH1750_WriteCmd(BHOP_POWER_DOWN);
}

/*
*********************************************************************************************************
*	函 数 名: BH1750_AdjustSensitivity
*	功能说明: 调节BH1750测量灵敏度
*	形    参: _ucMTReg : 量程倍率.  值域【31，254】，值越大 灵敏度越高
*	返 回 值: 无
*********************************************************************************************************
*/
void BH1750_AdjustSensitivity(uint8_t _ucMTReg)
{
	if (_ucMTReg <= 31)
	{
		_ucMTReg = 31;
	}
	else if (_ucMTReg >= 254)
	{
		_ucMTReg = 254;
	}
	
	bh1750_MTReg = _ucMTReg;
	
	BH1750_WriteCmd(0x40 + (_ucMTReg >> 5));		/* 更改高3bit */
	BH1750_WriteCmd(0x60 + (_ucMTReg & 0x1F));	/* 更改低5bit */
	
	/*　更改量程范围后，需要重新发送命令设置测量模式　*/
	BH1750_ChageMode(bh1750_Mode);
}

/*
*********************************************************************************************************
*	函 数 名: BH1750_ChageMode
*	功能说明: 修改BH1750测量模式，决定测量分辨率 
*	形    参: __ucMode : 测量模式 值域(1，2，3)
*	返 回 值: 无
*********************************************************************************************************
*/
void BH1750_ChageMode(uint8_t _ucMode)
{
	if (_ucMode == 1)		/* 连续高分测量模式1 */
	{
		BH1750_WriteCmd(BHOP_CON_H_RES);	
		bh1750_Mode = 1;			/* 测量模式1，分辨率 1 lux*/
	}
	else if (_ucMode == 2)	/* 连续高分测量模式2 */
	{
		BH1750_WriteCmd(BHOP_CON_H_RES2);	
		bh1750_Mode = 2;			/* 测量模式2, 分辨率 0.5 lux */		
	}
	else if (_ucMode == 3)	/* 连续低分测量模式 */
	{
		BH1750_WriteCmd(BHOP_CON_L_RES);	
		bh1750_Mode = 3;			/* 测量模式3，低分辨率 4 lux*/		
	}	
}



/*
*********************************************************************************************************
*	函 数 名: BH1750_GetLux
*	功能说明: 读取BH1750测量结果, 并转换为 Lux单位
*	形    参: 无
*	返 回 值: 光强度 [Lux]  浮点数
*********************************************************************************************************
*/
float BH1750_GetLux(void)
{
	uint16_t usLight;
	float lux;
	
	BH1750_ReadData(&usLight);
	
	/* 计算光强度 = 16位寄存器值 / 1.2  * (69 / X) */

	//lux = usLight * (((float)1 / 1.2) * ((float)69 / s_MTReg));
	lux = (float)(usLight * 5 * 69) / (6 * bh1750_MTReg);
	
	if (bh1750_Mode == 2)	/* 高分辨率测量模式2 */
	{
		lux = lux / 2;
	}
	else
	{
		;	/* 不必除2 */
	}
	
	return lux;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
