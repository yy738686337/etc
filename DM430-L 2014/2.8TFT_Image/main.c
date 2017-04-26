/********************************************************************
//DM430-L����Сϵͳ�����2.8��TFT����ͼƬ��ʾ���Գ��򣬽����ο��������и���
//��TFT�����Ӧ�ӿڼ��ɣ�����Ƚϴ�BSL���ػ�Ƚ���
//ע��ѡ��Һ���ĵ�Դ��λ�ڵ�λ����������ѡ5V��3.3V������Һ����ѹ����ѡ��Ĭ������Ϊ5V
//���Ի�����EW430 V5.30
//���ߣ�www.avrgcc.com
//ʱ�䣺2014.03.01
********************************************************************/
#include <msp430x14x.h>
#include "Config.h"                     //����ͷ�ļ�����Ӳ����ص��������������
#include "qqpic.h"			//80x80ͼ���ļ�������Image2LCD��������
#include "Ascii_8x16.h"                 //8x16��С�ַ�
#include "GB2424.h"                     //24x24���ش�С�ĺ���
#include "Chinese.h"                    //16x16���ش�С�ĺ���
#include "Touch.h"                      //TFT��������ͷ�ļ�
#include "Touch.c"                      //TFT����������ʼ��������
#include "TFT28.h"                      //TFT��ʾͷ�ļ�
#include "TFT28.c"                      //TFT��ʾ������ʼ��������

//******************ȫ�ֱ���***************************
#define White          0xFFFF           //��ʾ��ɫ����
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

uint Device_code;                       //TFT����IC�ͺţ�2.8��ΪILI9320

//**************�����ⲿ�����ͱ���********************
extern void delayms(unsigned int count);

/********************************************************************
			��ʼ��IO���ӳ���
********************************************************************/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;                                     //TFT���ݿ�
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;  //TFT��ʾ������
  
  P3SEL=0x00;                               //����IO��Ϊ��ͨI/Oģʽ
  P3DIR = 0xff;                             //����IO�ڷ���Ϊ���
  P3OUT = 0x00;                             //��ʼ����Ϊ00,LED������IO��
  
  P2SEL = 0x00;
  P2DIR |= BIT3 + BIT4 + BIT5 + BIT6;       //���������ߣ�P23~P26��2�������ߣ�4�������
}

/***************************************************************************************************
//	ʵ�ֹ��ܣ�	����λ����ʾ�����СͼƬ��ǰ���ǵ�Ƭ������װ������
//  ���������  x0��y0 ��ʼ����
//              x1��y1 ��������
//  ע�����	1��ͼƬ��С���鲻Ҫ̫�������Сͼ������40x40��80x80��̫��Ƭ��װ����
				2��ͼƬ�����Ƿ��ڵ�Ƭ��FLASH�еģ�����Ҫ��pgm_read_byte������ȡ����������GRAM��
				3����ʾǰLCD_SetPos��������ͼƬ��ʾ���򣬴�0��ʼ����������Ҫ��1
				4����ȡ���ݵ�λ�ú����������������
				5��Imag2LCD������ȡ��16��ɫ�����ֽ��ڸ�λ�����ֽ��ڵ�λ����С�����ݴ洢��ʽ
***************************************************************************************************/
void Show_Image(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1)
{
	unsigned int i,j;
	LCD_SetPos(x0,x1-1,y0,y1-1);      //������ʾλ��
	for (i=y0-y0;i<y1-y0;i++)         //��ʼλ��Ӧ�ô�0��ʼ��������ȡ����
	{
	   for (j=x0-x0;j<x1-x0;j++)      //��ʼλ��Ӧ�ô�0��ʼ��������ȡ����
	    //С�����ݸ�ʽ����pgm_read_byte������FLASH�ж�ȡͼ�����ݣ�ͼ��������Image2LCD������ȡ��16λ��ɫ
	    Write_Data(gImage_qqp[i*(x1-x0)*2+j*2+1],gImage_qqp[i*(x1-x0)*2+j*2] );//ע���㷨
	}
}

/***************************************************************************************************
//	ʵ�ֹ��ܣ�	ͼƬ��̬��ʾЧ��
//  ���������  x0��y0 ��ʼ����
//              x1��y1 ��������
//				xm, ym x,y�����ƶ�������
//				bColorΪ����ɫ���ƶ������ԭ�ȵ�ͼ������
//  ע�����	1��ͼƬ��С���鲻Ҫ̫�������Сͼ������40x40��80x80��̫��Ƭ��װ����
				2��ͼƬ�����Ƿ��ڵ�Ƭ��FLASH�еģ�����Ҫ��pgm_read_byte������ȡ����������GRAM��
				3����ʾǰLCD_SetPos��������ͼƬ��ʾ���򣬴�0��ʼ����������Ҫ��1
				4����ȡ���ݵ�λ�ú����������������
				5��Imag2LCD������ȡ��16��ɫ�����ֽ��ڸ�λ�����ֽ��ڵ�λ����С�����ݴ洢��ʽ
***************************************************************************************************/
void Image_Move(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1,uchar xm,uchar ym,uint bColor)
{
	uint i,j,k;
	
  for(k=0;k<7;k++)
  {
   //m=k*xm;
   //n=k*ym;
   LCD_SetPos(x0+k*xm,x1+k*xm-1,y0+k*ym,y1+k*ym-1);      //������ʾλ��
	for (i=y0-y0;i<y1-y0;i++)         //��ʼλ��Ӧ�ô�0��ʼ��������ȡ����
	{
	   for (j=x0-x0;j<x1-x0;j++)      //��ʼλ��Ӧ�ô�0��ʼ��������ȡ����
	    //С�����ݸ�ʽ����pgm_read_byte������FLASH�ж�ȡͼ�����ݣ�ͼ��������Image2LCD������ȡ��16λ��ɫ
	    Write_Data(gImage_qqp[i*(x1-x0)*2+j*2+1],gImage_qqp[i*(x1-x0)*2+j*2] );//ע���㷨
	}

	delay_ms(500);
	delay_ms(500);

	for (i=y0-y0;i<y1-y0;i++)         //��ʼλ��Ӧ�ô�0��ʼ��������ȡ����
	{
	   for (j=x0-x0;j<x1-x0;j++)      //��ʼλ��Ӧ�ô�0��ʼ��������ȡ����
	    //С�����ݸ�ʽ����pgm_read_byte������FLASH�ж�ȡͼ�����ݣ�ͼ��������Image2LCD������ȡ��16λ��ɫ
	    Write_Data_U16(bColor);//ע���㷨
	}
  }
}

/********************************************************************
	LED��˸һ�Σ�������Ҫ�鿴����ִ��λ��ʱ������ã����ƶϵ�
********************************************************************/
void LED_Light()
{
    LED8=0x00;                              //����LED
    delay_ms(500);
    LED8=0xff;                              //Ϩ��LED
    delay_ms(500);
}

/********************************************************************
     	      �ڴ�����λ�û���
********************************************************************/
void drawpoint(unsigned int x,unsigned int y,unsigned int color)
{
    LCD_SetPos(x,x+1,y,y+1);
    Write_Data_U16(color);
    Write_Data_U16(color);
    Write_Data_U16(color);
    Write_Data_U16(color);
} 

/********************************************************************
                      ������
********************************************************************/
main()
{
  uint i,j;
  
  WDT_Init();                        //���Ź�����
  Clock_Init();                      //ϵͳʱ������
  Port_Init();                       //ϵͳ��ʼ��������IO������
  LED_Light();                       //LED����һ�Σ���������˵����Ƭ����ʼ���ɹ�   
  Device_code=0x9320;                //TFT����IC�ͺ�
  TFT_Initial();                     //��ʼ��LCD	
    
  CLR_Screen(Magenta);               //�ñ���ɫ����
  LCD_PutString24(35,140,"�����͵��ӿƼ�",Yellow,Magenta); //��ӭ���棬24x24��С���֣���ģ��������С����
  delay_ms(500);    
  LCD_PutString24(83,170,"��",Yellow,Magenta);
  delay_ms(300);
  LCD_PutString24(107,170,"ӭ",Yellow,Magenta);
  delay_ms(300);
  LCD_PutString24(131,170,"��",Yellow,Magenta);
  delay_ms(300);
  LCD_PutString(69,300,"www.avrgcc.com",White,Magenta);   //�ַ���ʾ�ٷ���վ
  delay_ms(300);
  CLR_Screen(White);				        //�屳��ɫ
  for(i=0;i<4;i++)					//��ʾ12���׿ǳ�ͼ�꣬ÿ��80x80
   {
  	for(j=0;j<3;j++)
		{
			Show_Image(j*80,j*80+80,i*80,i*80+80);
		}
   }

  CLR_Screen(White);					//�屳��ɫ

  for(j=0;j<3;j++)            				//ͼ���ƶ���ʾ
  {
    Image_Move(0,80,0,80,15,15,White);			//ÿ���ƶ�15�����أ��������ƶ�
  }  
   
}