/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��1.5.1�汾�⽨�Ĺ���ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "includes.h"

/*
*********************************************************************************************************
*                                         �궨�� 
*********************************************************************************************************
*/
#define ICONVIEW_Width      70  /* �ؼ�ICONVIEW�Ŀ�                         */  
#define ICONVIEW_Height     70  /* �ؼ�ICONVIEW�ĸߣ�����ͼ���Y������    */  
#define ICONVIEW_XSpace     30  /* �ؼ�ICONVIEW��X������                  */ 
#define ICONVIEW_YSpace     40  /* �ؼ�ICONVIEW��Y������                  */ 
#define ICONVIEW_XPos       10  /* �ؼ�ICONVIEW��X�����������              */
#define BOTWIN_YSpace       60  /* ��Ļ�·����ִ���Y������                */
#define MIDWIN_yPos         40  /* �м䴰��Y������ʼλ�ü��������ֿ��ø߶�  */
#define MIDWIN_xPos          0  /* �м䴰��Y������ʼλ��                    */

/*
*********************************************************************************************************
*                                      ����
*********************************************************************************************************
*/ 
extern GUI_CONST_STORAGE GUI_BITMAP bmIdea;
extern GUI_CONST_STORAGE GUI_BITMAP bmCalculator;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock;
extern GUI_CONST_STORAGE GUI_BITMAP bmCompass;
extern GUI_CONST_STORAGE GUI_BITMAP bmMessages;
extern GUI_CONST_STORAGE GUI_BITMAP bmMusic;
extern GUI_CONST_STORAGE GUI_BITMAP bmPhone;
extern GUI_CONST_STORAGE GUI_BITMAP bmPhotos;
extern GUI_CONST_STORAGE GUI_BITMAP bmVideos;
extern GUI_CONST_STORAGE GUI_BITMAP bmSafari;
extern GUI_CONST_STORAGE GUI_BITMAP bmWeather;
extern GUI_CONST_STORAGE GUI_BITMAP bmwifi;
extern GUI_CONST_STORAGE GUI_BITMAP bmSiri;
extern GUI_CONST_STORAGE GUI_BITMAP bmCamera;
extern GUI_CONST_STORAGE GUI_BITMAP bmUSB;

uint8_t UserApp_Flag=0;
extern RTC_TIME rtc_time;

uint8_t Flag_ICON0   = 0;   /* ICONVIEW�ؼ����µı�־��0��ʾδ���£�1��ʾ���� */
uint8_t Flag_ICON1   = 0;
uint8_t Flag_ICON2   = 0;
uint8_t Flag_ICON3   = 0;
uint8_t Flag_ICON4   = 0;
uint8_t Flag_ICON5   = 0;
uint8_t Flag_ICON6   = 0;
uint8_t Flag_ICON7   = 0;
uint8_t Flag_ICON8   = 0;
uint8_t Flag_ICON9   = 0;
uint8_t Flag_ICON10  = 0;
uint8_t Flag_ICON11  = 0;
uint8_t Flag_ICON12  = 0;

/* ��������ICONVIEWͼ��Ĵ��� */
typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;

typedef struct WIN_PARA{			//����ʹ�õ����û���������������ڻص�������ʹ��
	int xSizeLCD, ySizeLCD;			//LCD��Ļ�ߴ�
	int xPosWin,  yPosWin;			//���ڵ���ʼλ��
	int xSizeWin, ySizeWin;			//���ڳߴ�	
	int xSizeBM,  ySizeBM;
	WM_HWIN hWinMid;				//����ʾ���ľ�����в���	
}WIN_PARA;

WIN_PARA WinPara;			//�û�ʹ�õĴ��ڶ�������

/* ��������ICONVIEW0ͼ��Ĵ��� */
static const BITMAP_ITEM _aBitmapItem[] = 
{
  {&bmIdea,              "RGBLED"},
  {&bmMusic,    	        "Music"},
  {&bmSiri,            "Recorder"},
	{&bmWeather,     	    "Weather"}, 
  
  {&bmCompass,          "Compass"},  
	{&bmClock,              "Clock"},
	{&bmSafari, 		       "Safari"}, 
  {&bmCamera,    	       "Camera"}, 
  
	{&bmPhone,  	 	        "Phone"},    
	{&bmMessages,  	 	   "Messages"},
  {&bmPhotos,   	      "Picture"},		
	{&bmUSB,  	              "USB"},   	
	{&bmCalculator,    "Calculator"}, 
};

//void FUN_ICON0Clicked(void)  {printf("FUN_ICON0Clicked\n");}
//void FUN_ICON1Clicked(void)  {printf("FUN_ICON1Clicked\n");}
//void FUN_ICON2Clicked(void)  {printf("FUN_ICON2Clicked\n");}
//void FUN_ICON3Clicked(void)  {printf("FUN_ICON3Clicked\n");}

//void FUN_ICON4Clicked(void)  {printf("FUN_ICON4Clicked\n");}
//void FUN_ICON5Clicked(void)  {printf("FUN_ICON5Clicked\n");}
//void FUN_ICON6Clicked(void)  {printf("FUN_ICON6Clicked\n");}
//void FUN_ICON7Clicked(void)  {printf("FUN_ICON7Clicked\n");}

//void FUN_ICON8Clicked(void)  {printf("FUN_ICON8Clicked\n");}
//void FUN_ICON9Clicked(void)  {printf("FUN_ICON9Clicked\n");}
//void FUN_ICON10Clicked(void) {printf("FUN_ICON10Clicked\n");}
//void FUN_ICON11Clicked(void) {printf("FUN_ICON11Clicked\n");}

//void FUN_ICON12Clicked(void) {printf("FUN_ICON12Clicked\n");}
/*
*********************************************************************************************************
*	�� �� ��: _cbBkWindow
*	����˵��: ���洰�ڵĻص����� 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _cbBkWindow(WM_MESSAGE * pMsg) 
{
  uint8_t sec, min, hour;
  char text_buffer[20]={0};
	switch (pMsg->MsgId) 
	{
		/* �ػ���Ϣ*/
		case WM_PAINT:		
#if   LCD_NUM_LAYERS == 1 
				GUI_SetBkColor(DTCOLOR);
#elif LCD_NUM_LAYERS == 2
          GUI_SetBkColor(GUI_TRANSPARENT);
#endif
				GUI_Clear();	
        ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW0),-1);
		break;
    case WM_TIMER:
      if(UserApp_Flag==1)
      {
        WM_RestartTimer(pMsg->Data.v, 1000);
        break; 
      }      
      if(!(bsp_result&BSP_RTC))
      {
        //printf("app timer\n");
        RTC_GetTime(RTC_Format_BIN, &rtc_time.RTC_Time);
        sec    =  rtc_time.RTC_Time.RTC_Seconds;
        min    =  rtc_time.RTC_Time.RTC_Minutes;
        hour   =  rtc_time.RTC_Time.RTC_Hours;
        sprintf(text_buffer,"%02d:%02d:%02d",hour,min,sec);
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),text_buffer);
      }
      sprintf(text_buffer,"CPU:%02d%%",(uint8_t)(OSStatTaskCPUUsage/100));
      TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),text_buffer);
      WM_RestartTimer(pMsg->Data.v, 1000);
      break;
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: _cbMidWin
*	����˵��: 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _cbMidWin(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
			switch (Id) 
			{
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						/* ICON�ؼ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
              UserApp_Flag=1;
							break;
						
						/* ICON�ؼ��ͷ���Ϣ */
						case WM_NOTIFICATION_RELEASED: 							
							/* ����Ӧѡ�� */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{
								
								/* RGBLED *******************************************************************/
								case 0:	
									Flag_ICON0 = 1;
									FUN_ICON0Clicked();			
									break;	
								
								/* Music ***********************************************************************/
								case 1:
									Flag_ICON1 = 1;
									FUN_ICON1Clicked();
									break;
								
								/* Recorder *********************************************************************/
								case 2:
									Flag_ICON2 = 1;
									FUN_ICON2Clicked();	
									break;
								
								/* Weather *******************************************************************/
								case 3:
									Flag_ICON3 = 1;
									FUN_ICON3Clicked();
									break;
								
								/* Compass  ******************************************************************/
								case 4:	
									Flag_ICON4 = 1;
									FUN_ICON4Clicked();            
									break;	
								
								/* Clock ***********************************************************************/
								case 5:
									Flag_ICON5 = 1;
									FUN_ICON5Clicked();
									break;
								
								/* Safari *********************************************************************/
								case 6:
									Flag_ICON6 = 1;
 									FUN_ICON6Clicked();	
									break;
								
								/* Camera ********************************************************************/
								case 7:
									Flag_ICON7 = 1;
									FUN_ICON7Clicked();
                  GUI_Delay(50);
                  LCD_BL_ON();
									break;
								
								/* Phone **********************************************************************/
								case 8:
									Flag_ICON8 = 1;
									FUN_ICON8Clicked();
									break;
								
								/* Message ********************************************************************/
								case 9:
									Flag_ICON9 = 1;
									FUN_ICON9Clicked();
									break;
								
								/* Photos ******************************************************************/
								case 10:					
									Flag_ICON10 = 1;
									FUN_ICON10Clicked();
									break;
								
								/* USB *******************************************************************/
								case 11:
									Flag_ICON11 = 1;
									FUN_ICON11Clicked();
									break;		
								/* Calculator *******************************************************************/
								case 12:
									Flag_ICON12 = 1;
									FUN_ICON12Clicked();
									break;                           
								}
							 break;
						}
					break;
			}
			break;
			
		/* �ػ���Ϣ*/
		case WM_PAINT:
			{
#if   LCD_NUM_LAYERS == 1 
				GUI_SetBkColor(DTCOLOR);
#elif LCD_NUM_LAYERS == 2
        GUI_SetBkColor(GUI_TRANSPARENT);
#endif
				GUI_Clear();
			}			
		break;			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}


/**
  * @brief  CreateTopWin�����������Ĵ���
  * @param  none
  * @retval none
  */
static void CreateTopWin(void)
{
	WM_HWIN hText;
	/* ������ "wildfire"�ı� */
	hText = TEXT_CreateEx(0, 2, 200 , 30, HDTWIN, WM_CF_SHOW|WM_CF_MEMDEV, GUI_TA_LEFT|TEXT_CF_VCENTER, GUI_ID_TEXT0, "");
	TEXT_SetFont(hText,GUI_FONT_24B_ASCII);
	TEXT_SetText(hText,"BH-STM32F429");
	TEXT_SetTextColor(hText,GUI_WHITE);
	
	/* ״̬����ʱ����ʾ�ı� */
	hText = TEXT_CreateEx(WinPara.xSizeLCD/2-50,2,100,30,HDTWIN,WM_CF_SHOW|WM_CF_MEMDEV,GUI_TA_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT1,"11:56:00");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_WHITE);
	TEXT_SetFont(hText,GUI_FONT_20B_ASCII);
  
  	/* ״̬����ʱ����ʾ�ı� */
	hText = TEXT_CreateEx(WinPara.xSizeLCD-200,2,200,30,HDTWIN,WM_CF_SHOW|WM_CF_MEMDEV,GUI_TA_RIGHT|TEXT_CF_VCENTER,GUI_ID_TEXT2,"CPU: 10%");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_WHITE);
	TEXT_SetFont(hText,GUI_FONT_24B_ASCII);
}
/**
  * @brief  CreateMidWin�������м�Ĵ���
  * @param  none
  * @retval none
  */
static void CreateMidWin(void)
{
	uint8_t i=0;
	WM_HWIN MIDWIN;
	WinPara.hWinMid= WM_CreateWindowAsChild(
											WinPara.xPosWin,											
											WinPara.yPosWin,
											WinPara.xSizeWin,
											WinPara.ySizeWin,	
											HDTWIN, 
											WM_CF_SHOW|WM_CF_MEMDEV,
											_cbMidWin, 
											sizeof(WIN_PARA *)
											);
/*-------------------------------------------------------------------------------------*/
					/*��ָ��λ�ô���ָ���ߴ��ICONVIEW1 С����*/
					MIDWIN=ICONVIEW_CreateEx(
											 ICONVIEW_XPos, 					          /* С���ߵ��������أ��ڸ������У�*/
											 0, 								                /* С���ߵ��������أ��ڸ������У�*/
											 WinPara.xSizeLCD-ICONVIEW_XPos,	  /* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
											 3*(WinPara.ySizeBM+35), 			      /* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
											 WinPara.hWinMid, 				          /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
											 WM_CF_SHOW|WM_CF_HASTRANS,         /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
											 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
											 GUI_ID_ICONVIEW0, 			            /* С���ߵĴ���ID */
											 ICONVIEW_Width, 				            /* ͼ���ˮƽ�ߴ� */
											 ICONVIEW_Height+20);				        /* ͼ��Ĵ�ֱ�ߴ� */
											 
		/* ��ICONVIEW С���������ͼ�� */
		for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++) 
		{
			ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem[i].pBitmap,_aBitmapItem[i].pText);
			ICONVIEW_SetTextColor(MIDWIN,i,GUI_WHITE);
		}	
		/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
		ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, 0xffffc0 | 0x80000000);
		/* �������� */
		ICONVIEW_SetFont(MIDWIN, GUI_FONT_16B_ASCII);
		/* ���ó�ʼѡ���ͼ��Ϊ -1 (��ʾ��δѡ��)*/
		ICONVIEW_SetSel(MIDWIN,-1);
		/* ����ͼ����x ��y �����ϵļ�ࡣ*/
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
		/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
		ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
		
/*-------------------------------------------------------------------------------------*/
}

/*
*********************************************************************************************************
*	�� �� ��: emWinMainApp
*	����˵��: GUI������ 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void emWinMainApp(void)
{
  FRAMEWIN_SKINFLEX_PROPS FRAMEWIN_pProps;
  RADIO_SKINFLEX_PROPS    RADIO_pProps;

#if LCD_NUM_LAYERS == 2
  /*  ���ʹ��˫����ʾ������Ҫ�����ϲ�Ϊͨ͸�ģ���Ȼ�²��޷���ʾ */
  GUI_SelectLayer(1);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
#endif
  
	//׼������3�����ڣ�������ʹ�õ����û���������������ڻص�������ʹ��
	WinPara.xSizeLCD = LCD_GetXSize();				            //LCD��Ļ�ߴ�
	WinPara.ySizeLCD = LCD_GetYSize();				            //LCD��Ļ�ߴ�
	WinPara.xSizeBM  = ICONVIEW_Width;				            //ͼ����
	WinPara.ySizeBM  = ICONVIEW_Height;				            //ͼ��߶�
	WinPara.xPosWin	 = MIDWIN_xPos;							          //���ڵ���ʼλ��
	WinPara.yPosWin  = MIDWIN_yPos;							          //���ڵ���ʼλ��
	WinPara.xSizeWin = WinPara.xSizeLCD;						      //���ڳߴ�
	WinPara.ySizeWin = WinPara.ySizeLCD-WinPara.yPosWin;	//���ڳߴ�
  
  PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
  
  /***************************����Ƥ��*****************************/
  FRAMEWIN_GetSkinFlexProps(&FRAMEWIN_pProps,FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_pProps.aColorFrame[0]=GUI_BLACK;
  FRAMEWIN_pProps.aColorFrame[1]=GUI_BLACK;
  FRAMEWIN_pProps.aColorFrame[2]=GUI_BLACK;
  FRAMEWIN_pProps.Radius=0;
  FRAMEWIN_pProps.aColorTitle[0]=GUI_BLACK;
  FRAMEWIN_pProps.aColorTitle[1]=GUI_BLACK;
  FRAMEWIN_SetSkinFlexProps(&FRAMEWIN_pProps,FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_SetDefaultClientColor(APPBKCOLOR);
  FRAMEWIN_SetDefaultFont(&XBF_XINSONGTI19);
  FRAMEWIN_SetDefaultTextColor(1,TEXTCOLOR);
  FRAMEWIN_SetDefaultTitleHeight(30);
  FRAMEWIN_SetDefaultTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);

  WINDOW_SetDefaultBkColor(APPBKCOLOR);
  
  EDIT_SetDefaultBkColor(EDIT_CI_ENABLED,APPBKCOLOR);
  EDIT_SetDefaultTextColor(EDIT_CI_ENABLED,TEXTCOLOR);
  
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_UNSEL,APPBKCOLOR);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_SEL,GUI_BLUE);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_SELFOCUS,GUI_BLUE);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_DISABLED,APPBKCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_UNSEL,TEXTCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_SEL,TEXTCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_SELFOCUS,TEXTCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_DISABLED,TEXTCOLOR);
  
  TEXT_SetDefaultTextColor(TEXTCOLOR);
  
  RADIO_SetDefaultTextColor(TEXTCOLOR);
  RADIO_GetSkinFlexProps(&RADIO_pProps,0);
  RADIO_pProps.aColorButton[3]=GUI_YELLOW;
  RADIO_SetSkinFlexProps(&RADIO_pProps,0);
  
  CHECKBOX_SetDefaultTextColor(TEXTCOLOR);
  
	/*************************************************************************/
	WM_SetCallback(HDTWIN, _cbBkWindow);
	WM_CreateTimer(HDTWIN, 0, 1000, 0);
	/* ������������ ״̬������������������*/
	CreateMidWin();
	CreateTopWin();
  GUI_Delay(20);
#if LCD_NUM_LAYERS == 1
  LCD_BL_ON();
#endif
	while(1)
	{
		GUI_Delay(5);
	}
}
/*************************** End of file ****************************/
