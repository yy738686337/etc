/********************************************************************
//DM430-L型开发板使用MSP430f169内部DAC模块DAC0输出矩形波
//可以用示波器查看，接在DAC0端口，也就是P66口
//通过示波器查看是完整的波形，频率粗略通过延时实现
//加上延时函数，可以通过LED灯查看效果，输出电压由小到大，LED灯由亮到暗，循环执行
//确保LED灯跳线连接正常，否则不显示，用示波器时取下跳线
//调试环境：EW430 V5.30
//作者：www.avrgcc.com
//时间：2014.03.01
********************************************************************/
#include <msp430x16x.h>
#include "Config.h"                     //开发板配置头文件，主要配置IO端口信息

/********************************************************************
			DAC12初始化设置程序
********************************************************************/
void DAC12_Init(void)
{
  DAC12_0CTL |= DAC12CALON;             //启动校验DAC
  while((DAC12_0CTL & DAC12CALON) != 0){_NOP();}//等待校验完成
  
  ADC12CTL0 = REFON + REF2_5V;                        // 参考电压为内部2.5v
  DAC12_0CTL = DAC12AMP_5 + DAC12IR + DAC12LSEL_0;//控制寄存器设置，自动更新数据，内基准为满量程
  DAC12_0CTL |= DAC12SREF_0;
  DAC12_0CTL |= DAC12ENC; // 启动DAC模块(DAC12LSEL_0时此句可以省，这里设置为1)
  DAC12_0DAT = 0x0000;    //DAC初值为0，更新这个寄存器就可以更改DAC输出数据
}

/********************************************************************
			主程序
********************************************************************/
void main(void)
{
  uint i;
  
  WDT_Init();                               // 关闭看门狗
  Clock_Init();                             //时钟系统初始化，设置为外部时钟
  DAC12_Init();                             //初始化DAC12
  delay_ms(10);
  while(1)
  {
    for(i=0;i<4096;i++)
    {
      DAC12_0DAT = i;                       //数据写入寄存器，写入激活DAC转换
      delay_us(300);                        //这里延时是为了LED灯有显示效果，由亮到暗，电压输出由小到大，用示波器时去掉
    }
    delay_ms(500);                          //延时，重复执行，可以便于LED灯显示，用示波器时去掉
  }
}