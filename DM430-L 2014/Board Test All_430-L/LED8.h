//***********************************************************************
//               MSP430配置头文件
//***********************************************************************
#include <msp430x14x.h>


void LED8Port_Init()
{
  P6SEL=0x00;                    //设置IO口为普通I/O模式
  P6DIR = 0xff;                  //设置IO口方向为输出
  P6OUT = 0xff;                  //初始设置为00
}

/********************************************************************
				LED指定位显示函数
********************************************************************/
void LED_Runing(unsigned char LED_NUM)
{
	switch(LED_NUM)
	{
        case 0:
          LED8 &= ~(1<<0);  //点亮D1灯
          break;
        case 1:
          LED8 &= ~(1<<1);  //点亮D2灯
          break;
        case 2:
          LED8 &= ~(1<<2);  //点亮D3灯
          break;
        case 3:
          LED8 &= ~(1<<3);  //点亮D4灯
          break; 
        case 4:
          LED8 &= ~(1<<4);  //点亮D5灯
          break;
        case 5:
          LED8 &= ~(1<<5);  //点亮D6灯
          break;
        case 6:
          LED8 &= ~(1<<6);  //点亮D7灯
          break;
        case 7:
          LED8 &= ~(1<<7);  //点亮D8灯
          break;				
        default:
          LED8 = 0x00;     //点亮所有的LED灯
          break;
	}
}
/********************************************************************
				LED流水显示函数
********************************************************************/
void LED8_Run_1Time(void)
{ 
  uchar count,i;
  count = 0x00;
  LED8Port_Init();                          //端口初始化，用于控制IO口输入或输出
  for(i=0;i<8;i++)
  {
    LED8 = 0xFF; 
    LED_Runing(count%8);											//点亮一个LED灯		
    count++;
    delay_ms(2);
  }
  LED8 = 0xFF;
 
}
/********************************************************************
				LED全部闪烁显示函数
********************************************************************/
void LED8_8(void)
{ 
  uchar count,i;
  count = 0x00;
  LED8Port_Init();                          //端口初始化，用于控制IO口输入或输出
  for(i=0;i<3;i++)
  {
    LED8 = 0x00;
    delay_ms(500);
    LED8 = 0xFF;
    delay_ms(500);
  }
}

//***********************************************************************
//      2个LED闪烁5次测试程序
//***********************************************************************
void LED_Glow3Times(void)
{ 
  uchar j;
  
  LED8Port_Init();                        //端口初始化，用于控制IO口输入或输出
  for(j=0;j<3;j++)
  {
    LED8=0x00;                        //点亮LED
    delay_ms(500);
    LED8=0xff;                        //熄灭LED
    delay_ms(500);
  }
}