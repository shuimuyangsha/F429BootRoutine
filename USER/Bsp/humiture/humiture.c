/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��ʪ�ȴ�����Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./Bsp/bsp.h"
#include "./Bsp/humiture/humiture.h"


/*
 * ��������TEMP_HUM_GPIO_Config
 * ����  ������TEMP_HUM�õ���I/O��
 * ����  ����
 * ���  ����
 */
void TEMP_HUM_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����DHT11_PORT������ʱ��*/
	RCC_AHB1PeriphClockCmd(TEMP_HUM_CLK, ENABLE); 

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  
  /*�������ŵ��������Ϊ�������*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  
  /*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
  GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure); 
}

/*
 * ��������TEMP_HUM_Mode_IPU
 * ����  ��ʹTEMP_HUM-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void TEMP_HUM_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/	
	  GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;

	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ; 
  
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  	/*������������Ϊ50MHz */   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  
	  /*���ÿ⺯������ʼ��DHT11_PORT*/
	  GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure);	 
}

/*
 * ��������TEMP_HUM_Mode_Out_PP
 * ����  ��ʹTEMP_HUM-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void TEMP_HUM_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/															   
  GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  
  /*�������ŵ��������Ϊ�������*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  
  /*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	/*������������Ϊ50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_PORT*/
  	GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * ��DHT11��ȡһ���ֽڣ�MSB����
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while(TEMP_HUM_DATA_IN()==Bit_RESET);

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		bsp_DelayUS(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

		if(TEMP_HUM_DATA_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(TEMP_HUM_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}
/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
  uint16_t count;
	/*���ģʽ*/
	TEMP_HUM_Mode_Out_PP();
	/*��������*/
	TEMP_HUM_DATA_OUT(HUMITURE_LOW);
	/*��ʱ18ms*/
	bsp_DelayUS(20000);

	/*�������� ������ʱ30us*/
	TEMP_HUM_DATA_OUT(HUMITURE_HIGH); 

	bsp_DelayUS(30);   //��ʱ30us

	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	TEMP_HUM_Mode_IPU();

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(TEMP_HUM_DATA_IN()==Bit_RESET)     
	{
    count=0;
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(TEMP_HUM_DATA_IN()==Bit_RESET)
    {
      count++;
      if(count>1000)  return 0;
      bsp_DelayUS(10); 
    }    
    
    count=0;
		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(TEMP_HUM_DATA_IN()==Bit_SET)
    {
      count++;
      if(count>1000)  return 0;
      bsp_DelayUS(10); 
    }  
		/*��ʼ��������*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();

		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		TEMP_HUM_Mode_Out_PP();
		/*��������*/
		TEMP_HUM_DATA_OUT(HUMITURE_HIGH);

		/*����ȡ�������Ƿ���ȷ*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 1;
		else 
			return 0;
	}
	else
	{		
		return 0;
	}   
}


/*********************  DS18B20 **************************/
/*
 *�������ӻ����͸�λ����
 */
static void DS18B20_Rst(void)
{
	/* ��������Ϊ������� */
	TEMP_HUM_Mode_Out_PP();
	
	TEMP_HUM_DATA_OUT(HUMITURE_LOW);
	
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	bsp_DelayUS(750);

	/* �����ڲ�����λ�źź��轫�������� */
	TEMP_HUM_DATA_OUT(HUMITURE_HIGH);
	
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	bsp_DelayUS(15);
}

/*
 * ���ӻ����������صĴ�������
 * 0���ɹ�
 * 1��ʧ��
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	TEMP_HUM_Mode_IPU();
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( TEMP_HUM_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		bsp_DelayUS(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 0;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while( !TEMP_HUM_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		bsp_DelayUS(1);
	}	
	if( pulse_time >=240 )
		return 0;
	else
		return 1;
}

/*
 * ��DS18B20��ȡһ��bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	TEMP_HUM_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	TEMP_HUM_DATA_OUT(HUMITURE_LOW);
	bsp_DelayUS(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	TEMP_HUM_Mode_IPU();
	//Delay_us(2);
	
	if( TEMP_HUM_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	bsp_DelayUS(45);
	
	return dat;
}

/*
 * ��DS18B20��һ���ֽڣ���λ����
 */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*
 * дһ���ֽڵ�DS18B20����λ����
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	TEMP_HUM_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			TEMP_HUM_DATA_OUT(HUMITURE_LOW);
			/* 1us < �����ʱ < 15us */
			bsp_DelayUS(8);
			
			TEMP_HUM_DATA_OUT(HUMITURE_HIGH);
			bsp_DelayUS(58);
		}		
		else
		{			
			TEMP_HUM_DATA_OUT(HUMITURE_LOW);
			/* 60us < Tx 0 < 120us */
			bsp_DelayUS(70);
			
			TEMP_HUM_DATA_OUT(HUMITURE_HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			bsp_DelayUS(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* ���� ROM */
	DS18B20_Write_Byte(0X44);		/* ��ʼת�� */
}

uint8_t DS18B20_Init(void)
{
	TEMP_HUM_GPIO_Config();
	DS18B20_Rst();
	
	return DS18B20_Presence();
}
/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}
