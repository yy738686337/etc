//***********************************************************************
//               MSP430����ͷ�ļ�
//***********************************************************************
#include <msp430x14x.h>


//**********************************************************************
//                  ��TFT����ʱϵͳ����Թ��ܺ���
//**********************************************************************
void TestMode_LCDFunction()
{
  delay_ms(200);
  while(1)
    {  
      while(Key_Scan()==0xff);               //�ȴ���������ѡ�����ģʽ
      if(key!=0xff)                          //����а������£�����ʾ�ð�����ֵ1��4
        {
	  {
            switch(key)
              {
	        case 1: Test_TFT_Show_Touch();break;  //����ͬ�ļ�����ֵ����ֵ1����2��LED��
                case 2: Test_TFT_Show_H();break;      //����ͬ�ļ�����ֵ����ֵ2����2��LED��
                case 3: Test_TFT_SD();break;	      //����ͬ�ļ�����ֵ����ֵ3����2��LED��
                case 4: Test_TFT_18B20();break;	      //����ͬ�ļ�����ֵ����ֵ4����2��LED��
              }

          }
	
       }
     else
      {
        //LED=key;              	//û�а�����ʱ����ʾ�ϴεļ�ֵ
      }
    }
  
}