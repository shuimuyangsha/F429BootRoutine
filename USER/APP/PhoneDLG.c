/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.28                          *
*        Compiled Jan 30 2015, 16:41:06                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "includes.h"
#include "app.h"
#include "DIALOG.h"
#include "./Bsp/sim900a/bsp_sim900a.h"
// USER END
/*
**************************************************************************************
*                                         �궨�� 
**************************************************************************************
*/ 
// USER START (Optionally insert additional defines)
/*--------------------  (ʹ��U2C.exeС��������)  ------------------------------------*/
#define UTF8_PHONEDLG        "\xe7\x94\xb5\xe8\xaf\x9d"//�绰
#define UTF8_SPEAKING        "\xe9\x80\x9a\xe8\xaf\x9d\xe4\xb8\xad..."//ͨ����
#define UTF8_RING            "\xe6\x9d\xa5\xe7\x94\xb5\xe5\x91\xbc\xe5\x8f\xab..."//����
#define UTF8_CALL            "\xe6\x8b\xa8\xe5\x8f\xb7..."//����    
#define UTF8_GSMERROR        "GSM\xe6\xa8\xa1\xe5\x9d\x97\xe9\x80\x9a\xe4\xbf\xa1\xe5\xa4\xb1\xe8\xb4\xa5"//GSMģ��ͨ��ʧ��
#define UTF8_GSMERROR1       "\xe8\xaf\xb7\xe6\xa3\x80\xe6\x9f\xa5\xe6\x8e\xa5\xe7\xba\xbf"//�������       
/*-----------------------------------------------------------------------------------*/

#define ID_BUTTON             (GUI_ID_USER + 0)

#define BUTTON_WHITH          80
#define BUTTON_HIGHT          60
#define BUTTON_SpaceX         (10+BUTTON_WHITH) 
#define BUTTON_SpaceY         (10+BUTTON_HIGHT)
#define TEXTLENGTH            25
// USER END
/*
**************************************************************************************
*                                      ����
**************************************************************************************
*/ 
extern GUI_CONST_STORAGE GUI_BITMAP bmhangon;
extern GUI_CONST_STORAGE GUI_BITMAP bmhangoff;
extern GUI_CONST_STORAGE GUI_BITMAP bmdelete;

static WM_HWIN hKey,hBack,hText;

static uint8_t 		s_Key;
static char 		  text[TEXTLENGTH];
static uint8_t 		IsCall=0;//1:calling  0:waiting
static uint8_t		textoff=0;

typedef struct {
  int          xPos;
  int          yPos;
  int          xSize;
  int          ySize;
  const char * acLabel;   /* ��ť��Ӧ���ַ� */
} BUTTON;

static const BUTTON ButtonData[] = 
{

	/* ��1�Ű�ť */
	{ 5,                   3,                   BUTTON_WHITH, 		BUTTON_HIGHT, "1"  },
	{ 5+BUTTON_SpaceX,     3,                   BUTTON_WHITH, 		BUTTON_HIGHT, "2"  },
	{ 5+BUTTON_SpaceX*2,   3,                   BUTTON_WHITH, 		BUTTON_HIGHT, "3"  },
	/* ��2�Ű�ť */
	{ 5,                   3+BUTTON_SpaceY,     BUTTON_WHITH, 		BUTTON_HIGHT, "4"  },	
	{ 5+BUTTON_SpaceX,     3+BUTTON_SpaceY,     BUTTON_WHITH, 		BUTTON_HIGHT, "5"  },
	{ 5+BUTTON_SpaceX*2,   3+BUTTON_SpaceY,     BUTTON_WHITH, 		BUTTON_HIGHT, "6"  },
	/* ��3�Ű�ť */
	{ 5,                   3+BUTTON_SpaceY*2,  	BUTTON_WHITH, 		BUTTON_HIGHT, "7"  },
	{ 5+BUTTON_SpaceX,     3+BUTTON_SpaceY*2,  	BUTTON_WHITH, 		BUTTON_HIGHT, "8"  },	
	{ 5+BUTTON_SpaceX*2,   3+BUTTON_SpaceY*2,  	BUTTON_WHITH, 		BUTTON_HIGHT, "9"  },
	/* ��4�Ű�ť */
	{ 5,                   3+BUTTON_SpaceY*3,  	BUTTON_WHITH, 		BUTTON_HIGHT, "*"  },
	{ 5+BUTTON_SpaceX,     3+BUTTON_SpaceY*3,  	BUTTON_WHITH, 		BUTTON_HIGHT, "0"  },
	{ 5+BUTTON_SpaceX*2,   3+BUTTON_SpaceY*3,  	BUTTON_WHITH, 		BUTTON_HIGHT, "#"  }
};

/***********************************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePhone[] = {
	{ FRAMEWIN_CreateIndirect, "Phone",                0, 0,   0,   800, 480, 0, 0x0,  0 },
	{ TEXT_CreateIndirect,     "number",    GUI_ID_TEXT0, 140, 20,  280, 40,  0, 0x64, 0 },
	{ TEXT_CreateIndirect,     "calling",   GUI_ID_TEXT1, 180, 120, 200, 40,  0, 0x64, 0 },
	{ BUTTON_CreateIndirect,   "back",    GUI_ID_BUTTON0, 395, 26,  24,  24,  0, 0x0,  0 },
	{ BUTTON_CreateIndirect,   "hangon",  GUI_ID_BUTTON1, 190, 350, 48,  48,  0, 0x0,  0 },
	{ BUTTON_CreateIndirect,   "hangoff", GUI_ID_BUTTON2, 320, 350, 48,  48,  0, 0x0,  0 },
	{ BUTTON_CreateIndirect,   "call",    GUI_ID_BUTTON3, 255, 350, 48,  48,  0, 0x0,  0 },
	// USER START (Optionally insert additional widgets)
	// USER END
};
/*
*************************************************************************************
*	�� �� ��: _cbKeyPad
*	����˵��: �ص�����
*	��    �Σ�pMsg  ָ�����
*	�� �� ֵ: ��
*************************************************************************************
*/
static void _cbDialogPhone(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int        Id;
	int        NCode;
	switch (pMsg->MsgId) 
	{
  case WM_DELETE:
    OS_INFO("PhoneDLG delete\n");
    Flag_ICON8 = 0;
    textoff=0;
    text[0]='\0';
    IsCall=0;  
    USART_ITConfig(GSM_USART, USART_IT_RXNE, DISABLE);	
    USART_Cmd(GSM_USART, DISABLE);
    GSM_USART_CLKCMD(GSM_USART_CLK, DISABLE);
    UserApp_Flag = 0;
    tpad_flag=0;
    break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Phone'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetText(hItem,UTF8_PHONEDLG);
    FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    //
    // Initialization of 'number'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
    hText=hItem;
    TEXT_SetTextColor(hItem, GUI_BLACK);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_32B_ASCII);
    TEXT_SetText(hItem, "112");
    //
    // Initialization of 'calling'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
    TEXT_SetTextColor(hItem, GUI_BLACK);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, &XBF_XINSONGTI25);
    TEXT_SetText(hItem, UTF8_CALL);
    WM_HideWindow(hItem);
    //
    // Initialization of 'back'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
    hBack=hItem;
    BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmdelete);			
    BUTTON_SetText(hItem,"");
    //WM_HideWindow(hBack);
    //
    // Initialization of 'hangon'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
    BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmhangon);
    BUTTON_SetText(hItem,"");
    WM_BringToTop(hItem);
    WM_HideWindow(hItem);
    //
    // Initialization of 'hangoff'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
    BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmhangoff);
    BUTTON_SetText(hItem,"");
    WM_BringToTop(hItem);
    WM_HideWindow(hItem);
    //
    // Initialization of 'call'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);
    BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmhangon);
    BUTTON_SetText(hItem,"");
    WM_BringToTop(hItem);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
      case GUI_ID_BUTTON0: // Notifications sent by 'back'
        switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
          // USER START (Optionally insert code for reacting on notification message)
          textoff--;
          if(textoff==255)
          {
            textoff=0;
          }
          // USER END
          break;
          case WM_NOTIFICATION_RELEASED:
          // USER START (Optionally insert code for reacting on notification message)
          text[textoff]='\0';
          TEXT_SetText(hText,text);
          //printf("text:%s\n",text);
          if(textoff==0)WM_HideWindow(hBack);
          // USER END
          break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
        }
        break;
      case GUI_ID_BUTTON1: // Notifications sent by 'hangon'
        switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
          // USER START (Optionally insert code for reacting on notification message)							
          // USER END
          break;
          case WM_NOTIFICATION_RELEASED:
          // USER START (Optionally insert code for reacting on notification message)
          IsCall=1;							
          SIM900A_HANGON();
          TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), UTF8_SPEAKING);
          SIM900A_CLEAN_RX();//������ջ���
          // USER END
          break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
        }
        break;
      case GUI_ID_BUTTON2: // Notifications sent by 'hangoff'
        switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
          // USER START (Optionally insert code for reacting on notification message)										
          // USER END
          break;
          case WM_NOTIFICATION_RELEASED:
          // USER START (Optionally insert code for reacting on notification message)
          /*�ҵ绰*/
          SIM900A_HANGOFF();	
          IsCall=0;
          
          WM_ShowWindow(hKey);
          TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), UTF8_CALL);
          WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1));
          textoff=0;
          text[0]='\0';
          TEXT_SetText(hText,text);						
          WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1));
          WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2));
          WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3));
          SIM900A_CLEAN_RX();//������ջ���
          // USER END
          break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
        }
        break;
      case GUI_ID_BUTTON3: // Notifications sent by 'call'
        switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
          // USER START (Optionally insert code for reacting on notification message)							
          // USER END
          break;
          case WM_NOTIFICATION_RELEASED:
          // USER START (Optionally insert code for reacting on notification message)
          if(IsCall==0)
          {
            //����绰����
            sim900a_call(text);
            BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3),BUTTON_BI_UNPRESSED,&bmhangoff);
            WM_HideWindow(hKey);
            WM_HideWindow(hBack);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), UTF8_SPEAKING);
            WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1));			
            IsCall=1;
          }else	if(IsCall==1)
          {
            /*�ҵ绰*/
            SIM900A_HANGOFF();	
            BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3),BUTTON_BI_UNPRESSED,&bmhangon);
            WM_ShowWindow(hKey);
            WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1));
            textoff=0;
            text[0]='\0';
            TEXT_SetText(hText,text);
            IsCall=0;
          }
          SIM900A_CLEAN_RX();//������ջ���
          // USER END
          break;
          // USER START (Optionally insert additional code for further notification handling)
          // USER END
        }
        break;
        // USER START (Optionally insert additional code for further Ids)
      // USER END
    }
    break;
  /* ���Ʊ��� */	
  case WM_PAINT:	
#if   LCD_NUM_LAYERS == 1 
		GUI_SetBkColor(APPBKCOLOR);
#elif LCD_NUM_LAYERS == 2
     GUI_SetBkColor(GUI_TRANSPARENT);
#endif
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_FillRect(140,10,420,410);
    break;
  default:
  WM_DefaultProc(pMsg);
  break;
	}
}

static void _cbKeypad9(WM_MESSAGE * pMsg)
{
	WM_HWIN hButton;
	uint8_t i;
	int        Id;
	int        NCode;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			// USER START (Optionally insert additional code for further widget initialization)
			/* ��������İ�ť */
			for (i = 0; i < GUI_COUNTOF(ButtonData); i++) 
			{
				hButton = BUTTON_CreateEx(ButtonData[i].xPos, ButtonData[i].yPos, ButtonData[i].xSize, ButtonData[i].ySize, 
				WM_GetClientWindow(pMsg->hWin), WM_CF_SHOW, 0, ID_BUTTON + i);
				BUTTON_SetFont(hButton, &GUI_Font32B_ASCII);
				BUTTON_SetText(hButton, ButtonData[i].acLabel);
				BUTTON_SetTextAlign(hButton,GUI_TA_HCENTER|GUI_TA_VCENTER);
				BUTTON_SetFocussable(hButton, 0);
			}
			// USER END
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					// USER START (Optionally insert code for reacting on notification message)
					LED1_ON;
				#ifdef USERBEEP
					BEEP_ON;
				#endif
					WM_ShowWindow(hBack);
					// USER END
				break;
				case WM_NOTIFICATION_RELEASED:					
					// USER START (Optionally insert code for reacting on notification message)
					LED1_OFF;
				#ifdef USERBEEP
					BEEP_OFF;
				#endif
					s_Key = ButtonData[Id - ID_BUTTON].acLabel[0];
					text[textoff]=s_Key;
					text[textoff+1]='\0';
					TEXT_SetText(hText,text);
					//printf("text:%s\n",text);
					textoff++;
					// USER END
				break;
			    // USER START (Optionally insert additional code for further notification handling)
			    // USER END
			}
			break;
		/* ���Ʊ��� */	
		case WM_PAINT:	
			GUI_SetBkColor(GUI_WHITE);
			GUI_Clear();
			break;
		default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: MainAPP
*	����˵��: GUI������ 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void FUN_ICON8Clicked(void)
{
	WM_HWIN hWin;
	char num[20]={0};
	uint8_t timecount=0;
	IsCall=0;
	sprintf(text,"112");
	OS_INFO("PhoneDLG create\n");
	hWin=GUI_CreateDialogBox(_aDialogCreatePhone, GUI_COUNTOF(_aDialogCreatePhone), _cbDialogPhone, HDTWIN, 0, 0);
	hKey=WM_CreateWindowAsChild(150,90,270,280,hWin,WM_CF_SHOW,_cbKeypad9,0);
	WM_BringToTop(hKey);
	GUI_Delay(100);
	/* ��ʼ�������ģ�� */
	if(sim900a_init()!= SIM900A_TRUE)
	{    
    ErrorDialog(hWin,UTF8_GSMERROR,UTF8_GSMERROR1);
    while(1)
    {
      if(tpad_flag)WM_DeleteWindow(hWin);
       if(!Flag_ICON8)return;
       GUI_Delay(10);
    }
	}
	SIM900A_CLEAN_RX();//������ջ���

	SIM900A_TX("AT+CLIP=1\r");
	timecount=0;
	while(Flag_ICON8)
	{
		if(timecount>50)
		{
			if(IsRing(num)== SIM900A_TRUE)
			{
				BEEP_ON;
				TEXT_SetText(hText,num);
				WM_HideWindow(hKey);
				WM_HideWindow(hBack);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1),UTF8_RING);
				WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT1));
				WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
				WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON2));
				WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));				
			}
			if(IsCall&&(IsNOCARRIER()==SIM900A_TRUE))
      {
        BUTTON_SetBitmap(WM_GetDialogItem(hWin, GUI_ID_BUTTON3),BUTTON_BI_UNPRESSED,&bmhangon);
        WM_ShowWindow(hKey);
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT1));
        textoff=0;
        text[0]='\0';
        TEXT_SetText(hText,text);
        IsCall=0;
      }
			BEEP_OFF;
      SIM900A_CLEAN_RX();//������ջ���
			timecount=0;
		}
		timecount++;
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10);
	}
}