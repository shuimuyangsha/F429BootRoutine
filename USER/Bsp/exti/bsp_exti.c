/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./Bsp/exti/bsp_exti.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI_INT_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_MPU_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*��������GPIO�ڵ�ʱ��*/
	RCC_AHB1PeriphClockCmd(EXTI_INT_GPIO_CLK ,ENABLE);
  
  /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = EXTI_INT_GPIO_PIN;         /*ѡ�񰴼�������*/ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    /*��������Ϊ����ģʽ*/ 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      /*��������Ϊ����*/
  GPIO_Init(EXTI_INT_GPIO_PORT, &GPIO_InitStructure);            /*ʹ������Ľṹ���ʼ������*/

	/* ���� EXTI �ж�Դ ��key2 ���� */
  SYSCFG_EXTILineConfig(EXTI_INT_EXTI_PORTSOURCE, EXTI_INT_EXTI_PINSOURCE);

  /* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = EXTI_INT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  

  /* ���� NVIC */
  NVIC_Configuration();

}

void EXTI_MPU_STOP(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line = EXTI_INT_EXTI_LINE; 
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
}
/*********************************************END OF FILE**********************/
