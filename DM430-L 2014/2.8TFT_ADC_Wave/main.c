/********************************************************************
//DM430-L型最小系统板控制2.8寸TFT彩屏横屏波形绘制测试程序
//将ADC0（P60口）采集到的电压值显示到坐标轴，绘制成波形曲线
//ADC0连续采样，内部2.5V基准参考，最大2.5V，大于2.5V的输入会是直线
//该程序仅作参考，用户可自行根据该参考做更改优化
//直接将TFT插入对应TFT接口，注意TFT安装方向，是朝外
//注意选择液晶的电源，位于TFT接口附近，可选5V或3.3V，根据液晶电压进行选择，默认设置为5V
//调试环境：EW430 V5.30
//作者：www.avrgcc.com
//时间：2014.03.01
********************************************************************/
#include <msp430x14x.h>
#include "Config.h"                     //配置头文件，与硬件相关的配置在这里更改
#include "GUI.h"
#include "Ascii_8x16.h"                 //8x16大小字符
#include "GB2424.h"                     //24x24像素大小的汉字
#include "Chinese.h"                    //16x16像素大小的汉字
#include "Touch.h"                      //TFT触摸操作头文件
#include "Touch.c"                      //TFT触摸操作初始化及函数
#include "TFT28.h"                      //TFT显示头文件
#include "TFT28.c"                      //TFT显示操作初始化及函数
#include "GUI.c"

//******************全局变量***************************
#define White          0xFFFF           //显示颜色代码
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

uint Device_code;      //TFT控制IC型号，新版2.8寸为ILI9320

static uchar Flag=0;                    //标志变量
uint TEMP=0;                            //ADC值暂存变量 
static uint Precent=0;                  //设置为静态很重要，为了保留上一次采样值
uint count;
uint yback=0;                           //用于存放前一次的电压显示点的Y坐标
//**************声明外部函数和变量********************
extern void delayms(uint count);

/********************************************************************
			初始化IO口子程序
********************************************************************/
void Port_Init()
{
  P4SEL = 0x00;
  P4DIR = 0xFF;                                     //TFT数据口
  P5SEL = 0x00;
  P5DIR|= BIT0 + BIT1 + BIT3 + BIT5 + BIT6 + BIT7;  //TFT显示控制线
}

//*************************************************************************
//	ADC初始化程序，用于配置ADC相关寄存器
//*************************************************************************
void ADC_Init()
{
  P6SEL|=0x01;                                    //选择ADC通道，设置对应IO口的功能模式
  ADC12CTL0|= ADC12ON + SHT0_2 + REF2_5V + REFON; //ADC电源控制开，16个CLK，内部基准2.5V
  ADC12CTL1|= ADC12SSEL1 + ADC12SSEL0;            //SMCLK做时钟源
  ADC12MCTL0= SREF0 + INCH_0;                     //参考控制位及通道选择
  ADC12IE|= 0x01;                                 //中断允许
  ADC12CTL0|= ENC;                                //使能转换器
}
//*************************************************************************
//		电压数据处理函数
//*************************************************************************
void Data_do(uint temp_d)
{
  ulong temp_3;
  temp_3=(ulong)(temp_d)*100;            //转换公式，ADC数字量转换为电压大小，注意数据类型
  Precent = temp_3/0xFFF;	             //算出百分比

}
//*************************************************************************
//	ADC中断服务程序
//*************************************************************************
#pragma vector=ADC_VECTOR
__interrupt void ADC12ISR(void)
{
  uchar j;
  yback=190-(1.50*Precent);             //为了波形平滑，要保留前一次采样值对应的坐标点
  while((ADC12CTL1&0x01)==1);           //如果ADC忙，则等待，否则读取ADC转换数值
  Flag = 1 ;
  TEMP = ADC12MEM0;                     //读取ADC转换值
  count++;
  Data_do(TEMP);                        //处理ADC值，用于显示
  if(count<235)                         //决定显示到X轴的什么位置，显示到此位置后要清除数据，else语句执行清除
  {
    GUIpoint(30+count,190-(1.50*Precent),Red);     //把电压值打点在坐标区域，采用百分比算法
   
    GUIline(30+count-1,yback,30+count,190-(1.50*Precent),Red);  //将每个点之间用直线连接，形成波形
    delay_ms(50);
  }
  else
  {
    count=1;                      
    Show_RGB(31,265,40,189,Black);     //当显示完满屏时，用背景色清除显示区
  }
}

/********************************************************************
                      主函数
********************************************************************/
main()
{
  WDT_Init();                        //看门狗设置
  Clock_Init();                      //系统时钟设置
  Port_Init();                       //系统初始化，设置IO口属性

  Device_code=0x9320;                //TFT控制IC型号
  TFT_Initial();                     //初始化LCD	
  
  CLR_Screen(Black);                                        //用背景色清屏 
  CLR_Screen(Black);               //用背景色清屏
  LCD_PutString24(75,85,"欣世纪电子科技",Yellow,Black); //欢迎界面，24x24大小汉字，字模软件隶书小二号

  LCD_PutString24(115,119,"欢",Yellow,Black);

  LCD_PutString24(145,119,"迎",Yellow,Black);

  LCD_PutString24(175,119,"您",Yellow,Black);

  LCD_PutString(102,222,"www.avrgcc.com",White,Black);  	//字符显示官方网站
  delay_ms(800);
  delay_ms(300);  
  CLR_Screen(Black);                                          //用背景色清屏
  
  GUICoordinate(30,190,280,180,2,Yellow,Black);   //绘制坐标轴，尽量不要完全靠边缘绘制，如果单像素粗细可以
  LCD_PutString(104,220,"ADC Input Wave",White,Black);      //显示原点0
  count = 1;                          //计数值初始化为0
  ADC_Init();                         //初始化ADC配置
  _EINT();                            //使能中断
  Flag=1;                             //标志位先置1
  
  while(1)
  {
    while(Flag==1)
    {
      ADC12CTL0 |= ADC12SC;           //开启转换
      ADC12CTL0 &= ~ADC12SC;          //清零
      Flag=0;                         //清零标志位
    }
  }
    
}