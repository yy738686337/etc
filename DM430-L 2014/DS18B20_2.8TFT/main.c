/********************************************************************
//DM430-L����Сϵͳ�����DS18B20�¶ȴ�������ʾ�¶���2.8��TFT����ʾ������
//ֱ�ӽ�TFT�����ӦTFT�ӿڣ�ע��TFT��װ�����ǳ���
//ע��ѡ��Һ���ĵ�Դ��λ��TFT�ӿڸ�������ѡ5V��3.3V������Һ����ѹ����ѡ��Ĭ������Ϊ5V
//����ʱD1�ƻ���˸
//DS18B20��ȡ��ǰ�����¶ȣ���ʾ���ȴ�0.1�ȣ��¶ȷ�Χ0-99��
//DS18B20 ��ϸ���Ź�������1��GND ���źţ�2��DQ������������š���©�����߽ӿ����š��������ڼ�����Դ��
//Ҳ�����������ṩ��Դ��3��VDD��ѡ���VDD ���š��������ڼ�����Դʱ�������ű���ӵء�
//DS18B20 ��ʹ�÷�����DS18B20 ���õ���1��Wire ����Э�鷽ʽ
//����һ��������ʵ�����ݵ�˫���䣬���ݽŽ���P17��
//���Ի�����EW430 V5.30
//���ߣ�www.avrgcc.com
//ʱ�䣺2014.03.01
********************************************************************/
#include <msp430x14x.h>
#include "Config.h"                     //����ͷ�ļ�����Ӳ����ص��������������
#include "Ascii_8x16.h"                 //8x16��С�ַ�
#include "GB2424.h"                     //24x24���ش�С�ĺ���
#include "Chinese.h"                    //16x16���ش�С�ĺ���
#include "TFT28.h"                      //TFTͷ�ļ�
#include "DS18B20.c"                    //DS18B20��������غ���
#include "TFT28.c"                      //TFT������ʼ��������

//**************�����ⲿ�����ͱ���********************
unsigned int Device_code;               //TFT����IC�ͺţ�2.8��ΪILI9320
extern void delayms(unsigned int count);

/********************************************************************
        ��ʼ��IO���ӳ�������TFT���ݿںͿ��ƿ�����
********************************************************************/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;
  
  P6DIR |= (1<<0);
}

/********************************************************************
	LED��˸һ�Σ�������Ҫ�鿴����ִ��λ��ʱ������ã����ƶϵ�
********************************************************************/
void LED_Light()
{
    LED8=0x00;                    //����LED
    delay_ms(500);
    LED8=0xff;                    //Ϩ��LED
    delay_ms(500);
}

/********************************************************************
                      �¶�ֵ��ʾ����
********************************************************************/
void  LCD_DisplayTemp(uchar A1,uchar B1,uchar C1)
{
  LCD_PutChar(169,0,A1+0x30,White,Black);
  LCD_PutChar(177,0,B1+0x30,White,Black);
  LCD_PutChar(185,0,'.',White,Black);
  LCD_PutChar(193,0,C1+0x30,White,Black);
  LCD_PutChar(201,0,'C',White,Black);
}

/********************************************************************
                      ������
********************************************************************/
main()
{
   uint i,y,j;

  WDT_Init();                        //���Ź�����
  Clock_Init();                      //ϵͳʱ������
  Port_Init();                       //ϵͳ��ʼ��������IO������
  LED_Light();                       //LED����һ�Σ���������˵����Ƭ����ʼ���ɹ�   
  Device_code=0x9320;                //TFT����IC�ͺ�
  TFT_Initial();                     //��ʼ��LCD	 

  while(1)                           //ѭ��ִ�г���
   {
        
    CLR_Screen(Magenta);               //�ñ���ɫ����
    LCD_PutString24(35,140,"�����͵��ӿƼ�",Yellow,Magenta); //��ӭ���棬24x24��С���֣���ģ��������С����
    delay_ms(500);    
    LCD_PutString24(83,170,"��",Yellow,Magenta);
    delay_ms(300);
    LCD_PutString24(107,170,"ӭ",Yellow,Magenta);
    delay_ms(300);
    LCD_PutString24(131,170,"��",Yellow,Magenta);
    delay_ms(300);
    LCD_PutString(69,300,"www.avrgcc.com",White,Magenta);  //�ַ���ʾ�ٷ���վ
    delay_ms(300);
    
    CLR_Screen(Black);                                     //�ñ���ɫ����
    
    LCD_PutString(0,0,"The Temperature is : ",White,Black);  //�¶���ʾ��ʾ�ַ�
    
    DS18B20_Reset();			  //��λD18B20
    while(1)
    {
    ds1820_start();		          //����һ��ת��
    ds1820_read_temp();		          //��ȡ�¶���ֵ
    data_do(temp_value);                  //�������ݣ��õ�Ҫ��ʾ��ֵ
    LED8 &= ~(1<<0);                      //D1����˸��ʾ���ڲ����¶�
    for(j=0;j<30;j++)
    {	    
     LCD_DisplayTemp(A1,A2,A3);           //�����¶���ʾ�����������������ʾ
     LED8 |= 1<<0;                        //LED����˸
    }
   }
    
   }
 
    
}