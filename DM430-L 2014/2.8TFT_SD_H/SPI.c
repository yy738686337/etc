//**********************************************************************
//SD卡读写中SPI接口相关函数，主要是SPI口初始化以及字节数据传输函数
//调试环境：EW430 V5.30
//作者：阿迪 www.avrgcc.com
//时间：2011.09.19
//版权：版权所有，转载请注明
//**********************************************************************

volatile uchar spiTransferComplete;  //传输完成标志

//**********************************************************************
//              函数声明
//**********************************************************************

void   spiInit(void);
uchar  spiSendByte(uchar data);
uint spiTransferWord(uint data);


/*************************************************
函数功能：SPI初始化，使用串口0工作在SPI模式下
操作内容：配置串口为SPI模式以及设置相关寄存器
*************************************************/

void initSPI (void)
{
  P3SEL |= 0x0E;                            //设置USART0相关IO口功能，CS片选设置为普通IO，3线SPI设置为第二功能
  P3SEL &= ~0x11;
  P3OUT |= 0x01;                            //初始化时设置CS为高，SD卡不选
  P3DIR |= 0x1B;                            //对于输入输出方向，第二功能也需要配置
  P3DIR |= BIT0;
  
  UCTL0 = SWRST;                            // 配置SPI接口，初始化前需先置SWRST高
  UTCTL0 = CKPH | SSEL1 | SSEL0 | STC;      // SPI时钟为SMCLK, 3线模式(STE无效), UCLK延时半个周期, 数据在上升沿输出或输入锁存
  UBR00 = 0x02;                             // 0x02: UCLK/2 (4 MHz), works also with 3 and 4
  UBR10 = 0x00;                             // -"-
  UMCTL0 = 0x00;                            // SPI模式下，必须设置该寄存器为00
  UCTL0 = CHAR | SYNC | MM | SWRST;         // 8位SPI主机模式、复位状态
  UCTL0 &= ~SWRST;                          // 配置完后清零SWRST，必须这样的过程，否则可能出现不可预料错误
  ME1 |= USPIE0;                            // 使能USART0为SPI模式，允许
  while (!(IFG1 & UTXIFG0));                // USART0发送缓冲器空
}

/***********************************************************************
函数功能：SPI低速模式
***********************************************************************/

void spiLow(void)
{

}

/***********************************************************************
函数功能：SPI倍速模式
***********************************************************************/

void spiHigh(void)
{

}

/*************************************************
函数功能：SPI发送和接收数据函数
操作内容：1、所有数据通过SPI接口送出或读入
          2、读出或者写入数据
*************************************************/
uchar spiSendByte(const uchar data)
{
  while ((IFG1&UTXIFG0) ==0);                 // 如果未准备好，则等待
  TXBUF0 = data;                              //写数据到发送缓冲器
  while ((IFG1 & URXIFG0)==0);                //等待接收
  return (RXBUF0);                            //从接收缓冲器读取数据，比如响应值
}

/***********************************************************************
函数功能：SPI双字节传输函数
操作内容：先传输高字节，再传输低字节
***********************************************************************/
uint spiTransferWord(uint data)
{
  uint rxData = 0;

  rxData = (spiSendByte((data>>8) & 0x00FF))<<8;   //先传输高字节
  rxData |= (spiSendByte(data & 0x00FF));          //再传输低字节
  return rxData;
}
