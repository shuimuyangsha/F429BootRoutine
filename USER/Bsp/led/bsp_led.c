/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./Bsp/led/bsp_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|LED2_GPIO_CLK|LED3_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ��Ĭ��LED��*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/* �ر�����LED */
		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);
}

void LED_RGB111(uint8_t rgb111)
{
	assert_param(IS_RGB111_ALL_PERIPH(rgb111));
	if(rgb111&0x1)
		GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN);
	else
		GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);
	if(rgb111&0x2)
		GPIO_ResetBits(LED2_GPIO_PORT,LED2_PIN);
	else
		GPIO_SetBits(LED2_GPIO_PORT,LED2_PIN);
	if(rgb111&0x4)
		GPIO_ResetBits(LED3_GPIO_PORT,LED3_PIN);
	else
		GPIO_SetBits(LED3_GPIO_PORT,LED3_PIN);
}












/*********************************************END OF FILE**********************/
