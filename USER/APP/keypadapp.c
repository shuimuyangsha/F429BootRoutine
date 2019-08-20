#include "includes.h"
#include "app.h"
#include "pysearch.h"
/*
*********************************************************************************************************
*                                         �궨�� 
*********************************************************************************************************
*/ 

#define COLOR_BORDER           0xBD814F
#define COLOR_KEYPAD0          0xA8D403
#define COLOR_KEYPAD1          0xBF5C00
#define ID_BUTTON              (GUI_ID_USER + 0)

#define BUTTON_WHITH           75
#define BUTTON_HIGHT           37
#define BUTTON_SpaceX          (4+BUTTON_WHITH) 
#define BUTTON_SpaceY          (2+BUTTON_HIGHT)

/*
*********************************************************************************************************
*                                      ����
*********************************************************************************************************
*/ 
WM_HWIN hkeypad=NULL;
uint8_t keypadflag=0;//�����Ƿ���ʹ�ã�1��ʹ���У�0:��Ч��

static TEXT_Handle ahText[2];
static WM_HWIN hEdit;

static uint8_t s_SelectLangue = 0;	/* 0��ʾ���ģ�1��ʾӢ�� */
static uint8_t s_LeftFlag = 0;	    /* 1��ʾ��ǰ��Ŀ�����δ��ʾ����, ����֮ */
static uint8_t s_RightFlag = 0;	    /* 1��ʾ��ǰ��Ŀ�Ҳ���δ��ʾ����, ����֮ */
static uint8_t s_CapsLock = 0;      /* 0��ʾСд��1��ʾ��д */

static uint8_t s_Key;

static unsigned char s_tempbuf[32]={0};
static char s_Textbuf[4]={0};
static char s_Hzbuf[512]={0};
static char s_Bufview[50]={0};
static uint16_t  s_Offset = 0;
static uint16_t  s_Hznum=0;          
static uint16_t  s_Hzoff=0;
static  uint8_t  editflag=0;//�ж���edit����multiedit��1:multiedit��0��edit

typedef struct {
  int          xPos;
  int          yPos;
  int          xSize;
  int          ySize;
  const char * acLabel;   /* ��ť��Ӧ��Сд�ַ� */
  const char * acLabel1;  /* ��ť��Ӧ�Ĵ�д�ַ� */
  char         Control;   /* ��ť��Ӧ�Ŀ���λ��1��ʾ���ܰ�ť��0��ʾ��ͨ��ť */
} BUTTON_DATA;

static const BUTTON_DATA _aButtonData[] = 
{

	/* ��1�Ű�ť */
	{ 8,                   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "1" ,   	"1",  	0},//0
	{ 8+BUTTON_SpaceX,     5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "2",    	"2",  	0},
	{ 8+BUTTON_SpaceX*2,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "3",    	"3",  	0},
	{ 8+BUTTON_SpaceX*3,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "4",    	"4",  	0},
	{ 8+BUTTON_SpaceX*4,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "5",    	"5",  	0},
	{ 8+BUTTON_SpaceX*5,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "6",    	"6",  	0},
	{ 8+BUTTON_SpaceX*6,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "7",    	"7",  	0},
	{ 8+BUTTON_SpaceX*7,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "8",    	"8",  	0},
	{ 8+BUTTON_SpaceX*8,   5,  					      BUTTON_WHITH, 		   BUTTON_HIGHT, "9",    	"9",  	0},
	{ 8+BUTTON_SpaceX*9,   5,  					      BUTTON_WHITH-1, 	   BUTTON_HIGHT, "0",    	"0",  	0},

	/* ��2�Ű�ť */
	{ 8,                   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "q" ,   	"Q",  	0},//10
	{ 8+BUTTON_SpaceX,     5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "w",    	"W",  	0},
	{ 8+BUTTON_SpaceX*2,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "e",    	"E",  	0},
	{ 8+BUTTON_SpaceX*3,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "r",    	"R",  	0},
	{ 8+BUTTON_SpaceX*4,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "t",    	"T",  	0},
	{ 8+BUTTON_SpaceX*5,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "y",    	"Y",  	0},
	{ 8+BUTTON_SpaceX*6,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "u",    	"U",  	0},
	{ 8+BUTTON_SpaceX*7,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "i",    	"I",  	0},
	{ 8+BUTTON_SpaceX*8,   5+BUTTON_SpaceY,  	BUTTON_WHITH, 		   BUTTON_HIGHT, "o",    	"O",  	0},
	{ 8+BUTTON_SpaceX*9,   5+BUTTON_SpaceY,  	BUTTON_WHITH-1, 	   BUTTON_HIGHT, "p",    	"P",  	0},

	/* ��3�Ű�ť */
	{ 42,                   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "a" ,   	"A",  	0},//20
	{ 42+BUTTON_SpaceX,     5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "s",    	"S",  	0},
	{ 42+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "d",    	"D",  	0},
	{ 42+BUTTON_SpaceX*3,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "f",    	"F",  	0},
	{ 42+BUTTON_SpaceX*4,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "g",    	"G",  	0},
	{ 42+BUTTON_SpaceX*5,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "h",    	"H",  	0},
	{ 42+BUTTON_SpaceX*6,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "j",    	"J",  	0},
	{ 42+BUTTON_SpaceX*7,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "k",    	"K",  	0},
	{ 42+BUTTON_SpaceX*8,   5+BUTTON_SpaceY*2,  BUTTON_WHITH, 		  BUTTON_HIGHT, "l",    	"L",  	0},

	/* ��4�Ű�ť */
	{ 12,                   5+BUTTON_SpaceY*3,  BUTTON_WHITH+5,  	  BUTTON_HIGHT, "cl",   	"CL", 	1},//29
	{ 17+BUTTON_SpaceX,     5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "z",    	"Z",  	0},
	{ 17+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "x",    	"X",  	0},
	{ 17+BUTTON_SpaceX*3,   5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "c",    	"C", 	  0},
	{ 17+BUTTON_SpaceX*4,   5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "v",    	"V",  	0},
	{ 17+BUTTON_SpaceX*5,   5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "b",    	"B",  	0},
	{ 17+BUTTON_SpaceX*6,   5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "n",    	"N",  	0},
	{ 17+BUTTON_SpaceX*7,   5+BUTTON_SpaceY*3,  BUTTON_WHITH, 		  BUTTON_HIGHT, "m",    	"M",  	0},
	{ 17+BUTTON_SpaceX*8,   5+BUTTON_SpaceY*3,  BUTTON_WHITH*2-6, 	BUTTON_HIGHT, "Back", 	"Back", 1},//37


	/* ��5�Ű�ť */
	{ 10,                   5+BUTTON_SpaceY*4,  BUTTON_WHITH+7, 		BUTTON_HIGHT, "c/E",    "c/E", 	1},//38
	{ 18+BUTTON_SpaceX,     5+BUTTON_SpaceY*4,  BUTTON_WHITH, 	  	BUTTON_HIGHT, "_",   	  "_", 	  0},
	{ 18+BUTTON_SpaceX*2,   5+BUTTON_SpaceY*4,  BUTTON_WHITH, 		  BUTTON_HIGHT, ".",    	".",  	0},
	{ 20+BUTTON_SpaceX*3,   5+BUTTON_SpaceY*4,  BUTTON_WHITH*3, 		BUTTON_HIGHT, " ",  	  " ",	  0},
	{ 15+BUTTON_SpaceX*6,   5+BUTTON_SpaceY*4,  BUTTON_WHITH,		    BUTTON_HIGHT, ",",    	",",  	0},
	{ 15+BUTTON_SpaceX*7,   5+BUTTON_SpaceY*4,  BUTTON_WHITH+8, 		BUTTON_HIGHT, "En",    	"En",  	1},//43
	{ 23+BUTTON_SpaceX*8,   5+BUTTON_SpaceY*4,  BUTTON_WHITH*2-14, 	BUTTON_HIGHT, "Ok",    	"Ok",	  1},//44
};
/*
*********************************************************************************************************
*	�� �� ��: _cbKeyPad
*	����˵��: �ص�����
*	��    �Σ�pMsg  ָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbKeyPad(WM_MESSAGE * pMsg) 
{
	WM_HWIN    hWin;
	WM_HWIN    hButton;
	int        Id;
	int        NCode;
	int        xSize;
	int        ySize;
	unsigned char  i;
	char s_Editbuf[100]={0};
	char s_Edit[100]={0};
	hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		/* ��������İ�ť */
		case WM_CREATE:
			for (i = 0; i < GUI_COUNTOF(_aButtonData); i++) 
			{
				hButton = BUTTON_CreateEx(_aButtonData[i].xPos, _aButtonData[i].yPos, _aButtonData[i].xSize, _aButtonData[i].ySize, 
				hWin, WM_CF_SHOW, 0, ID_BUTTON + i);
				BUTTON_SetFont(hButton, GUI_FONT_32B_ASCII);
				BUTTON_SetText(hButton, _aButtonData[i].acLabel);
				BUTTON_SetTextAlign(hButton,GUI_TA_HCENTER|GUI_TA_VCENTER);
				BUTTON_SetFocussable(hButton, 0);
			}
			break;
		case WM_DELETE:
			s_Offset=0;
			s_SelectLangue = 1;
			s_CapsLock = 0;
			keypadflag=0;//ʹ�ܼ���
			editflag=0;//Ĭ����edit�ؼ���д
			WM_DeleteWindow(ahText[0]);
			WM_DeleteWindow(ahText[1]);
      hkeypad=NULL;
		break;
		/* ���Ʊ��� */	
		case WM_PAINT:	
			xSize = WM_GetWindowSizeX(hWin);
			ySize = WM_GetWindowSizeY(hWin);
			GUI_SetColor(COLOR_BORDER);
			GUI_DrawRect(0, 0, xSize - 1, ySize - 1);
			GUI_DrawGradientV(1, 1, xSize - 2, ySize - 2, COLOR_KEYPAD0, COLOR_KEYPAD1);
			break;
		
		/* ���ڴ���ť����Ϣ */
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (NCode) 
			{
				case WM_NOTIFICATION_RELEASED:
					if (_aButtonData[Id - ID_BUTTON].acLabel) 
					{
						/* ��һ����ʵ����ĸ��Сд���л� *******************************************/
						if(Id==(ID_BUTTON+29))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel,"CL", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
						{
							/* ��ǰ��Сд���л�����д��������Ӣ��ģʽ��ʵ�ִ�Сд�л�*/
							if((s_CapsLock == 0)&&(s_SelectLangue == 1))
							{
								for (i = 0; i < GUI_COUNTOF(_aButtonData); i++) 
								{
									BUTTON_SetText(WM_GetDialogItem(hWin, ID_BUTTON + i), _aButtonData[i].acLabel1);
								}
								s_CapsLock = 1;
							}
							else if((s_CapsLock == 1)&&(s_SelectLangue == 1))
							{
								for (i = 0; i < GUI_COUNTOF(_aButtonData); i++) 
								{
									BUTTON_SetText(WM_GetDialogItem(hWin, ID_BUTTON + i), _aButtonData[i].acLabel);
								}
								s_CapsLock = 0;
							}

							/* ���ܼ�������Ҫ�˳� */
							break;
						}

						/* �ڶ����� ʵ����Ӣ�ĵ��л� ********************************************/
						if(Id==(ID_BUTTON+38))
//						if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "C/e", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0
//						|strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "c/E", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
						{
							/* ��ǰ��Ӣ�ģ�������Сд��ĸ��ʱ��ſ����л�  */
							if((s_SelectLangue == 1)&&(s_CapsLock == 0))
							{
								s_SelectLangue = 0;
								BUTTON_SetText(WM_GetDialogItem(hWin, Id), "C/e");
							}
							else if((s_SelectLangue == 0)&&(s_CapsLock == 0))
							{
								s_SelectLangue = 1;
								BUTTON_SetText(WM_GetDialogItem(hWin, Id), "c/E");
							}
							WM_HideWindow(ahText[0]);
							WM_HideWindow(ahText[1]);
							WM_InvalidateWindow(ahText[0]);
							WM_InvalidateWindow(ahText[1]);

							/* ���ܼ�������Ҫ�˳� */
							break;
						}

						/* ����������ĸ��Сд��ʶ�� ********************************************/
						if(s_CapsLock == 0)
						{
							s_Key = _aButtonData[Id - ID_BUTTON].acLabel[0];
						}
						/* ��д��ĸ */
						else
						{
							s_Key = _aButtonData[Id - ID_BUTTON].acLabel1[0];
						}

						/* ���Ĳ������Ĵ��� **************************************************/
						if(s_SelectLangue == 0)
						{		
							int j;
							char *ptr;

							/* ����1����***********************************************************/
							if(s_Key == '1')
							{
								if(!s_Offset)break;
								/* ��������δ��ʾ�ĺ��� */
								if(s_LeftFlag == 1)
								{
									/* ��Ϊÿ����������ʾ6�����֣����Գ�������6�ĵط��ܶ� */
									if(s_Hzoff%8 == 1)
									{
										s_Hzoff -= 8;	
									}
									else
									{
										s_Hzoff = s_Hzoff - (s_Hzoff % 8)+1;	
									}
									s_Hzoff -= 8;

									s_RightFlag = 1;

									/* ���ƫ�Ƶ�ַ֮ǰ����6�����ϵĺ��� */
									if(s_Hzoff > 9)
									{
                    s_Bufview[0] = '1';
										s_Bufview[1] = '<';
										s_Bufview[2] = ' ';                    
										for (j = 0; j < 8;j++)
										{
											s_Bufview[j*5+3] = j+'2';
											s_Bufview[j*5+4] = s_Hzbuf[(s_Hzoff)*3];
											s_Bufview[j*5+5] = s_Hzbuf[(s_Hzoff)*3+1];
											s_Bufview[j*5+6] = s_Hzbuf[(s_Hzoff)*3+2];
											s_Bufview[j*5+7] = ' ';
											s_Hzoff++;
										}

										s_Bufview[43] = '0';
										s_Bufview[44] = '>';
										s_Bufview[45] = 0;
										s_LeftFlag = 1;
									}
									/* ���ƫ�Ƶ�ַ֮ǰ������С��7 */
									else 
									{	
										s_LeftFlag = 0;
										s_Hzoff=0;
										for (j = 0; j < 9;j++)
										{
											s_Bufview[j*5+0] = j+'1';
											s_Bufview[j*5+1] = s_Hzbuf[(s_Hzoff)*3];
											s_Bufview[j*5+2] = s_Hzbuf[(s_Hzoff)*3+1];
											s_Bufview[j*5+3] = s_Hzbuf[(s_Hzoff)*3+2];
											s_Bufview[j*5+4] = ' ';
											s_Hzoff++;
										}

										s_Bufview[45] = '0';
										s_Bufview[46] = '>';
										s_Bufview[47] = '\0';
									}
								}
								/* ���û�к��֣�ֱ����ʾ��һ������ */
								else
								{
									s_Textbuf[0] = s_Hzbuf[0];
									s_Textbuf[1] = s_Hzbuf[1];
									s_Textbuf[2] = s_Hzbuf[2];
									s_Textbuf[3] = '\0';
									if(editflag)MULTIEDIT_AddText(hEdit, s_Textbuf);
									else
									{								
										EDIT_GetText(hEdit,s_Editbuf,100);
										sprintf(s_Edit,"%s%s",s_Editbuf,s_Textbuf);
										EDIT_SetText(hEdit,s_Edit);		
										//EDIT_SetText(hEdit,s_Textbuf);										
									}
									s_Bufview[0]='\0';
									while(--s_Offset)
										s_tempbuf[s_Offset]= '\0';
									TEXT_SetText(ahText[0],s_Bufview);
									WM_HideWindow(ahText[0]);
									WM_HideWindow(ahText[1]);
								}								
								TEXT_SetText(ahText[1],s_Bufview);
							}
							/* ����8����***********************************************************/
							else if(s_Key == '0')
							{		
								if(!s_Offset)break;
								if(s_RightFlag == 1)
								{
									s_LeftFlag = 1;
                  s_Bufview[0] = '1';
                  s_Bufview[1] = '<';
                  s_Bufview[2] = ' ';              
									if (s_Hznum <= (s_Hzoff + 8))
									{
										int k = s_Hznum - s_Hzoff;
										for (j = 0; j < k;j++)
										{
											s_Bufview[j*5+3] = j+'2';
											s_Bufview[j*5+4] = s_Hzbuf[(s_Hzoff)*3];
											s_Bufview[j*5+5] = s_Hzbuf[(s_Hzoff)*3+1];
											s_Bufview[j*5+6] = s_Hzbuf[(s_Hzoff)*3+2];
											s_Bufview[j*5+7] = ' ';
											s_Hzoff++;
										}
										s_Bufview[k*5+3] = 0;	
										s_RightFlag = 0;
									}
									else
									{
										for (j = 0; j < 8;j++)
										{
											s_Bufview[j*5+5] = j + '2';
											s_Bufview[j*5+6] = s_Hzbuf[s_Hzoff*3];
											s_Bufview[j*5+7] = s_Hzbuf[s_Hzoff*3+1];
											s_Bufview[j*5+8] = s_Hzbuf[s_Hzoff*3+2];
											s_Bufview[j*5+9] = ' ';
											s_Hzoff++;
										}

										s_Bufview[45] = '0';
										s_Bufview[46] = '>';
										s_Bufview[47] = '\0';

										s_RightFlag = 1;
									}
									TEXT_SetText(ahText[1],s_Bufview);
								}
							}
							/* ����2-7����***********************************************************/
							else if((s_Key>='2')&&(s_Key<='9'))
							{
								if(!s_Offset)break;
								if(s_LeftFlag == 1)
								{
									if(s_Hzoff%8 == 1)
									{
										s_Textbuf[0]=s_Hzbuf[(s_Key-'2' + s_Hzoff - 8)*3];
										s_Textbuf[1]=s_Hzbuf[(s_Key-'2' + s_Hzoff - 8)*3+1];
										s_Textbuf[2]=s_Hzbuf[(s_Key-'2' + s_Hzoff - 8)*3+2];
									}
									else
									{
										s_Textbuf[0]=s_Hzbuf[(s_Key-'2' + (s_Hzoff/8)*8+1)*3];
										s_Textbuf[1]=s_Hzbuf[(s_Key-'2' + (s_Hzoff/8)*8+1)*3+1];
										s_Textbuf[2]=s_Hzbuf[(s_Key-'2' + (s_Hzoff/8)*8+1)*3+2];
									}
									
									s_Textbuf[3] = '\0';				
								}
								else
								{
									s_Textbuf[0] = s_Hzbuf[(s_Key-'1')*3];
									s_Textbuf[1] = s_Hzbuf[(s_Key-'1')*3+1];
									s_Textbuf[2] = s_Hzbuf[(s_Key-'1')*3+2];
									s_Textbuf[3] = '\0';
								}
								if(editflag)MULTIEDIT_AddText(hEdit, s_Textbuf);
								else
								{								
										EDIT_GetText(hEdit,s_Editbuf,100);
										sprintf(s_Edit,"%s%s",s_Editbuf,s_Textbuf);
										EDIT_SetText(hEdit,s_Edit);		
										//EDIT_SetText(hEdit,s_Textbuf);										
								}
								s_Bufview[0]='\0';
								while(--s_Offset)
									s_tempbuf[s_Offset]= '\0';
								TEXT_SetText(ahText[0],s_Bufview);
								TEXT_SetText(ahText[1],s_Bufview);
								WM_HideWindow(ahText[0]);
								WM_HideWindow(ahText[1]);
							}
							/* ������������ ***************************************************/
							else
							{			
								/* �ڻ�û�����뺺�ֵ�����£����ڹ��ܼ��Ĵ��� */
								if(s_Offset == 0)
								{
									/* ���»س��� */
									if(Id==(ID_BUTTON+43))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "En", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
									{
										if(editflag)MULTIEDIT_AddKey(hEdit, GUI_KEY_ENTER);
										break;
									}
									/* ���»ظ�� */
									if(Id==(ID_BUTTON+37))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "Back", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
									{
										if(editflag)MULTIEDIT_AddKey(hEdit, GUI_KEY_BACKSPACE);
										else EDIT_AddKey(hEdit,GUI_KEY_BACKSPACE);
										break;
									}
									/* ���ȷ�ϼ� */
									if(Id==(ID_BUTTON+44))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "Ok", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
									{
//										WM_HideWindow(hWin);
//										WM_InvalidateWindow(hWin);
										WM_DeleteWindow(hWin);
										keypadflag=0;
										break;
									}
								}
								
								TEXT_SetBkColor(ahText[0],GUI_LIGHTGRAY);							
								/* ����ظ��*/
								if(Id==(ID_BUTTON+37))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "Back", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
								{
									/* ʵ�ֺ���ƴ�������ɾ�� */
									if(s_Offset > 0)
									{
										s_Offset--;
										s_tempbuf[s_Offset]= '\0';
									}
									
									/* ƴ��ɾ����ϣ��˳����ֺ�ƴ������ʾ */
									if(s_Offset == 0)
									{
										WM_HideWindow(ahText[0]);
										WM_HideWindow(ahText[1]);
										WM_InvalidateWindow(ahText[0]);
										WM_InvalidateWindow(ahText[1]);
										break;
									}
								}
								else
								{
									/* ����������������Ƽ������������Ŀ��Ƽ��˳���ʾ */
									if(_aButtonData[Id - ID_BUTTON].Control == 1)
									{
										break;
									}
									
									/* ���������ƴ����װ�뻺�� */
									s_tempbuf[s_Offset++]= s_Key;
									
									WM_ShowWindow(ahText[0]);
									WM_ShowWindow(ahText[1]);
								}

								TEXT_SetText(ahText[0], (const char *)s_tempbuf);
								ptr = PYSearch(s_tempbuf, (int *)&s_Hznum);
                
								if (ptr != '\0')
								{
									for (j = 0; j < s_Hznum*3; j++)
									{
										s_Hzbuf[j] = *ptr++;
									}
									s_Hzbuf[s_Hznum*3] = '\0';						
									TEXT_SetBkColor(ahText[1],GUI_LIGHTGRAY);

									if (s_Hznum > 0)
									{	
										//printf("s_Hznum = %d\r\n", s_Hznum);
										s_Hzoff = 0;
										s_LeftFlag = 0;	//1��ʾ��ǰ��Ŀ�����δ��ʾ����

										if (9 >= s_Hznum )
										{
											int k = s_Hznum;									
											for (j = 0; j < k;j++)
											{
												s_Bufview[j*5+0] = j+'1';
												s_Bufview[j*5+1] = s_Hzbuf[s_Hzoff*3];
												s_Bufview[j*5+2] = s_Hzbuf[s_Hzoff*3+1];
												s_Bufview[j*5+3] = s_Hzbuf[s_Hzoff*3+2];
												s_Bufview[j*5+4] = ' ';
												s_Hzoff++;
											}
											s_Bufview[k*5] = '\0';
										}
										else
										{
											for (j = 0; j < 9;j++)
											{
												s_Bufview[j*5+0] = j + '1';
												s_Bufview[j*5+1] = s_Hzbuf[s_Hzoff*3];
												s_Bufview[j*5+2] = s_Hzbuf[s_Hzoff*3+1];
												s_Bufview[j*5+3] = s_Hzbuf[s_Hzoff*3+2];
												s_Bufview[j*5+4] = ' ';
												s_Hzoff++;
											}

											s_Bufview[45] = '0';
											s_Bufview[46] = '>';
											s_Bufview[47] = '\0';

											s_RightFlag = 1;	//1��ʾ��ǰ��Ŀ�Ҳ���δ��ʾ����
										}
									}
									TEXT_SetText(ahText[1],s_Bufview);
								}
								else
								{
									TEXT_SetText(ahText[1],"");
								}
							}
						}
					/* �ַ���ʾ*******************************************************************/
					else
					{
						/* ����س��� */
						if(Id==(ID_BUTTON+43))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "En", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
						{
							if(editflag)MULTIEDIT_AddKey(hEdit, GUI_KEY_ENTER);
							break;
						}
						
						/* ����ظ�� */
						if(Id==(ID_BUTTON+37))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "Back", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
						{
							if(editflag)MULTIEDIT_AddKey(hEdit, GUI_KEY_BACKSPACE);
							else EDIT_AddKey(hEdit,GUI_KEY_BACKSPACE);
							break;
						}
						/* ���ȷ�ϼ� ����������ɣ�ɾ�����̴���*/
						if(Id==(ID_BUTTON+44))//if(strncmp((const char *)_aButtonData[Id - ID_BUTTON].acLabel, "Ok", sizeof(_aButtonData[Id - ID_BUTTON].acLabel)) == 0)
						{
							WM_DeleteWindow(hWin);
							keypadflag=0;
							break;
						}

						/* �����������������ǹ��ܼ� */
						if(_aButtonData[Id - ID_BUTTON].Control == 0)
						{
							s_Textbuf[0] = s_Key;
							s_Textbuf[1] = '\0';
							if(editflag)MULTIEDIT_AddText(hEdit, s_Textbuf);
							else EDIT_AddKey(hEdit,s_Textbuf[0]);
						}
					}
				}
				break;
			}
	}
}
/*
*********************************************************************************************************
*	�� �� ��: MainAPP
*	����˵��: GUI������ 
*	��    �Σ�hwin:�ؼ����
*         ��flag��0����ʾ��EDIT�ؼ�
*                 1����ʾ��MULTIEDIT�ؼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void KeyPad_Interface(uint16_t hwin,uint8_t flag)
{
	s_Offset=0;
	s_SelectLangue = 1;
	s_CapsLock = 0;
	
	editflag=0;//Ĭ����edit�ؼ���д
	/*************************************************************************/
  if(keypadflag==0)
  {
    keypadflag=1;//ʹ�ܼ���
    /* �������̴��� */
    hkeypad=WM_CreateWindowAsChild(0, 480-40*5, 800, 40*5, HDTWIN, WM_CF_SHOW | WM_CF_STAYONTOP|WM_CF_MEMDEV, _cbKeyPad, 0);
    WM_BringToTop(hkeypad);
    
#if LCD_NUM_LAYERS == 2
    GUI_SelectLayer(1);
#endif
    
    /* �����ı��ؼ����ں�������ʱ��ƴ���ͺ��ֵ���ʾ */
    ahText[0] = TEXT_Create( 0, 231, 800, 24, GUI_ID_TEXT0, WM_CF_SHOW | WM_CF_STAYONTOP,"",TEXT_CF_LEFT | TEXT_CF_TOP);
    TEXT_SetTextColor(ahText[0],GUI_BLACK);
    TEXT_SetFont(ahText[0],GUI_FONT_24B_ASCII);

    ahText[1] = TEXT_Create( 0, 255, 800, 25, GUI_ID_TEXT1, WM_CF_SHOW | WM_CF_STAYONTOP,"",TEXT_CF_LEFT | TEXT_CF_BOTTOM);
    TEXT_SetFont(ahText[1],&XBF_XINSONGTI25);	
    TEXT_SetTextColor(ahText[1],GUI_RED);
  }
  
	//�ؼ����
	hEdit = hwin;
  
	//��ǰ�༭����multiedit�ؼ�
	if(flag)	editflag=1;
	
	/* ���þ۽� */
	WM_SetFocus(hEdit);
}
