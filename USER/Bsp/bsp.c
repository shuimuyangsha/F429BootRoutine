/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                         STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"
#include "./Bsp/bsp.h"

FATFS   fs[2];													  /* Work area (file system object) for logical drives */
FIL     f_file;
FRESULT f_result; 
UINT    f_num;

GUI_XBF_DATA XBF_Data[2];
GUI_FONT     XBF_XINSONGTI25;
GUI_FONT     XBF_XINSONGTI19;

RTC_TIME rtc_time;
uint16_t bsp_result=BSP_OK;
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  BSP_INT_SRC_NBR                                 91

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)

/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

// USER START (Optionally insert additional static code)
#if 0
FIL     xbffile[2];
/*
*********************************************************************************************************
*
*       _cbGetData
*
* Function description
*   Callback function for getting font data
*
* Parameters:
*   Off      - Position of XBF file to be read
*   NumBytes - Number of requested bytes
*   pVoid    - Application defined pointer
*   pBuffer  - Pointer to buffer to be filled by the function
*
* Return value:
*   0 on success, 1 on error
*********************************************************************************************************
*/
static int _cbGetData(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
{
	FRESULT result;
	FIL *FontFile;
	/* The pVoid pointer may be used to get a file handle */
	FontFile = (FIL *)pVoid;
	/*
	* Set file pointer to the required position
	*/
	result =f_lseek(FontFile, Offset);
	if (result != FR_OK) 
	{
		return 1; /* Error */
	}
	/*
	* Read data into buffer
	*/
	result = f_read(FontFile, pBuffer, NumBytes,0);
	if(result!=FR_OK)
	{
		return 1; /* Error */
	}
	return 0;
}

static void Create_XBF_XINSONGTI25(void) 
{
	FRESULT result; 
	result = f_open(&xbffile[0], "1:������25.xbf", FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;

	}
	//
	// Create XBF font
	//
	GUI_XBF_CreateFont(&XBF_XINSONGTI25,       // Pointer to GUI_FONT structure in RAM
					 &XBF_Data[0],         // Pointer to GUI_XBF_DATA structure in RAM
					 GUI_XBF_TYPE_PROP_EXT, 		// Font type to be created
					 _cbGetData,        // Pointer to callback function
					 &xbffile[0]);        // Pointer to be passed to GetData function
}

static void Create_XBF_XINSONGTI19(void) 
{
	FRESULT result; 
	result = f_open(&xbffile[1], "1:������19.xbf", FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;

	}
	//
	// Create XBF font
	//
	GUI_XBF_CreateFont(&XBF_XINSONGTI19,       // Pointer to GUI_FONT structure in RAM
					 &XBF_Data[1],         // Pointer to GUI_XBF_DATA structure in RAM
					 GUI_XBF_TYPE_PROP_EXT, 		// Font type to be created
					 _cbGetData,        // Pointer to callback function
					 &xbffile[1]);        // Pointer to be passed to GetData function
}

#else

#include "./drivers/fatfs_flash_spi.h"
//ʹ��XBF����֧��֮ǰ��Ҫ��������������д�ֿ��ļ���SPI Flash��fatfs�ļ�ϵͳ�ռ�
/*
*********************************************************************************************************
*
*       _cbGetData
*
* Function description
*   Callback function for getting font data
*
* Parameters:
*   Off      - Position of XBF file to be read
*   NumBytes - Number of requested bytes
*   pVoid    - Application defined pointer
*   pBuffer  - Pointer to buffer to be filled by the function
*
* Return value:
*   0 on success, 1 on error
*********************************************************************************************************
*/
static int _cbGetData_XINSONGTI25(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
{
	SPI_FLASH_BufferRead(pBuffer,Offset+60*4096,NumBytes);
	return 0;
}

static void Create_XBF_XINSONGTI25(void) 
{
	//
	// Create XBF font
	//
	GUI_XBF_CreateFont(&XBF_XINSONGTI25,       // Pointer to GUI_FONT structure in RAM
					   &XBF_Data[0],                   // Pointer to GUI_XBF_DATA structure in RAM
					   GUI_XBF_TYPE_PROP_EXT, 		     // Font type to be created
					   _cbGetData_XINSONGTI25,         // Pointer to callback function
					   0);                             // Pointer to be passed to GetData function
}

static int _cbGetData_XINSONGTI19(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
{
	SPI_FLASH_BufferRead(pBuffer,Offset+710*4096,NumBytes);
	return 0;
}

static void Create_XBF_XINSONGTI19(void) 
{
	//
	// Create XBF font
	//
	GUI_XBF_CreateFont(&XBF_XINSONGTI19,       // Pointer to GUI_FONT structure in RAM
					   &XBF_Data[1],                   // Pointer to GUI_XBF_DATA structure in RAM
					   GUI_XBF_TYPE_PROP_EXT, 		     // Font type to be created
					   _cbGetData_XINSONGTI19,         // Pointer to callback function
					   0);                             // Pointer to be passed to GetData function
}
#endif

#if 1
/*
*********************************************************************************************************
* Description : ��ʼ��BL8782 wifiģ��ʹ�����ţ����ر�ʹ��
* Argument(s) : none.
* Return(s)   : none.
*********************************************************************************************************
*/
static void BL8782_PDN_INIT(void)
{
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( WIFI_PDN_GPIO_CLK, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = WIFI_PDN_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(WIFI_PDN_GPIO_PORT, &GPIO_InitStructure);	
  
  WIFI_PDN_DISABLE();  //�ر�BL_8782wifiʹ��
}

#endif

//ϵͳ��λ   
void Bsp_Soft_Reset(void)
{	  
  __set_FAULTMASK(1);   /* �ر������ж� */  
  NVIC_SystemReset();   /* ϵͳ��λ */
} 

// USER END
extern const unsigned char _acDesktop[79238UL + 1];
/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*********************************************************************************************************
*/
void  BSP_Init (void)
{	 
  OS_ERR   err;
	/*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	/* ����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
  /* ��ʼ���δ�ʱ�� */
	BSP_Tick_Init();
  
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 :115200 N 8 1 */
	/* һ�㶼���ڴ��ڴ�ӡ��ص�����Ϣ���������ȳ�ʼ������ */
	Debug_USART_Config();
	
	/*��ʼ��SDRAMģ��*/
  SDRAM_Init();
//	if(SDRAM_Test()==0) bsp_result |= BSP_SDRAM;
  
  /* ��ʼ��wifiʹ�����Ų�ʧ�� */
  BL8782_PDN_INIT();
  
	/* ��ʼ��LED */
	LED_GPIO_Config();
	/* ��ʼ�������� */
	Beep_GPIO_Config();
	/* ��ʼ������ */
	Key1_GPIO_Config();
	Key2_GPIO_Config();
  
	/* ��ʼ�������� */
	if(GTP_Init_Panel()!=0) bsp_result |= BSP_GTP;
  
  /* ��ʼ����������WM8978���õ�I2C */
  I2cMaster_Init();  
  
	/* �����ļ�ϵͳ -- SPI FLASH */
  f_result = f_mount(&fs[1],"1:",1);	/* Mount a logical drive */
	if(f_result!=FR_OK) bsp_result |= BSP_SPIFLASH;

  WM_SetCreateFlags(WM_CF_MEMDEV);

	/* ��ʼ��LCD��emWin */
	/* Init the STemWin GUI Library */
  if(GUI_Init()!=0) bsp_result |= BSP_GUI;  
#if LCD_NUM_LAYERS == 1
  GUI_SetColor(DTCOLOR);
  GUI_FillRect(0,0,800-1,480-1);
  //GUI_Delay(10);  
#elif LCD_NUM_LAYERS == 2  

  GUI_SelectLayer(0);
  GUI_SetColor(DTCOLOR);  
  GUI_FillRect(0,0,800-1,480-1);
  GUI_Delay(50);  
  GUI_SelectLayer(1);
  GUI_SetColor(DTCOLOR);
  GUI_FillRect(0,0,800-1,480-1);
  GUI_Delay(50);
  LCD_BL_ON();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(GUI_FONT_32B_ASCII);
  GUI_DispStringHCenterAt("Starting...\nWait a moment",400,100);
  GUI_Delay(50);
  /* Setup layer configuration during startup */
  GUI_SelectLayer(0);
  GUI_JPEG_DrawScaled(&_acDesktop,79238UL + 1,0,0,100,100);
  GUI_Delay(100);
#endif
 
   /* ���SPI Flash���������ͼ���XBF����֧�� */
  if(!(bsp_result&BSP_SPIFLASH))
	{ 
    uint8_t data[4]={0};
    SPI_FLASH_BufferRead(data,60*4096,2);
    SPI_FLASH_BufferRead(&data[2],710*4096,2);
    if((data[0]==0x47)&&(data[1]==0x55)&&(data[2]==0x47)&&(data[3]==0x55))
    {
      Create_XBF_XINSONGTI25();
      Create_XBF_XINSONGTI19();
      GUI_UC_SetEncodeUTF8();
      GUI_SetDefaultFont(&XBF_XINSONGTI25);    
    }
    else
      bsp_result |= BSP_XBF;
	}
  
  rtc_time.RTC_Time.RTC_H12=RTC_H12_AM;
  rtc_time.RTC_Time.RTC_Hours   =11;
  rtc_time.RTC_Time.RTC_Minutes =55;
  rtc_time.RTC_Time.RTC_Seconds =30;
  rtc_time.RTC_Date.RTC_WeekDay =05;
  rtc_time.RTC_Date.RTC_Date    =16;
  rtc_time.RTC_Date.RTC_Month   =10;
  rtc_time.RTC_Date.RTC_Year    =15;
  
  OSSchedLock(&err);
  if(RTC_CheckAndConfig(&rtc_time,1)==1)  bsp_result |=BSP_RTC;//ʹ���ⲿ����ΪRTCʱ��Դ
  OSSchedUnlock(&err);
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}




/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
//	SysTick_Config(cnts);   //����Ĭ�ϵ���������ȼ�
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayUS
*	����˵��: �������ʱ����CPU���ڲ�����ʵ�֣�32λ������
*             	OSSchedLock(&err);
*				bsp_DelayUS(5);
*				OSSchedUnlock(&err); ����ʵ����������Ƿ���Ҫ�ӵ�������ѡ����ж�
*	��    �Σ�n : �ӳٳ��ȣ���λ1 us
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t _ulDelayTime)
{
  uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
		    	 
	ticks = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		 
	tcnt = 0;
	told = (uint32_t)CPU_TS_TmrRd();         /* �ս���ʱ�ļ�����ֵ */

	while(1)
	{
		tnow = (uint32_t)CPU_TS_TmrRd();	
		if(tnow != told)
		{	
		    /* 32λ�������ǵ��������� */    
			if(tnow > told)
			{
				tcnt += tnow - told;	
			}
			/* ����װ�� */
			else 
			{
				tcnt += UINT32_MAX - told + tnow;	
			}	
			
			told = tnow;

			/*ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
			if(tcnt >= ticks)break;
		}  
	}
}

int get_tick_count(unsigned long *count)
{
  OS_ERR      err;
  count[0] = OSTimeGet(&err);
	return count[0];
}

/*
*********************************************************************************************************
*                                              BSP_IntDis()
*
* Description : Disable interrupt.
*
* Argument(s) : int_id      Interrupt to disable.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDis (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcDis(int_id + 16);
    }
}

/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    CPU_IntDis();
}
/*
*********************************************************************************************************
*                                               BSP_IntEn()
*
* Description : Enable interrupt.
*
* Argument(s) : int_id      Interrupt to enable.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntEn (CPU_DATA  int_id)
{
    if (int_id < BSP_INT_SRC_NBR) {
        CPU_IntSrcEn(int_id + 16);
    }
}
