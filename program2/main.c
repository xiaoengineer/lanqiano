#include <STC89C5xRC.H>
typedef unsigned int u16;
typedef unsigned char u8;

u8 seg_ment[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80,
					0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; //定义段位
u8 seg_bit[] = {0x80, 0x40, 0x20, 0x10}; //定义位选
u16 count, sec_flag;

void close()
{
	P2 = (P2 & 0X1F) | 0XA0;
	P0 = 0XAF;
	P2 = P2 & 0X1F;
}

void delay(u8 ms)
{
	u8 i, j;
	for(i = ms; i > 0; i --)
		for(j = 114; j > 0; j --);
}

void led_show(u16 number)
{
	/*这种处理数字的方法很简单，C语言课上老师应该都讲过*/
	u16 kilobit = number / 1000;
	u16 hundreds_place = number % 1000 / 100;
	u16 decade = number % 1000 % 100 / 10;
	u16 unit = number - (kilobit * 1000 + hundreds_place * 100 + decade * 10);
	
	/*下面是显示函数*/  
	/*显示个位*/
	P2 = (P2 & 0X1F) | 0XC0;
	P0 = seg_bit[0];
	P2 = P2 & 0X1f;
	
	P2 = (P2 & 0X1F) | 0XE0;
	P0 = seg_ment[unit];
	P2 = P2 & 0X1f;
	delay(10);
	
	/*显示十位*/
	P2 = (P2 & 0X1F) | 0XC0;
	P0 = seg_bit[1];
	P2 = P2 & 0X1f;
	
	P2 = (P2 & 0X1f) | 0XE0;
	P0 = seg_ment[decade];
	P2 = P2 & 0X1f;
	delay(10);
	
	/*显示百位*/
	P2 = (P2 & 0X1f) | 0XC0;
	P0 = seg_bit[2];
	P2 = P2 & 0X1f;
	
	P2 = (P2 & 0X1F) | 0XE0;
	P0 = seg_ment[hundreds_place];
	P2 = P2 & 0x1f;
	delay(10);
	
	/*显示千位*/
	P2 = (P2 & 0X1F) | 0XC0;
	P0 = seg_bit[3];
	P2 = P2 & 0X1f;
	
	P2 = (P2 & 0X1F) | 0XE0;
	P0 = seg_ment[kilobit];
	P2 = P2 & 0X1f;
	delay(10);
}

void Timer0Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;		//开启总中断
	ET0 =1;	    //开启定时器中断
}

void main()
{
	u16 sec;
	close();
	Timer0Init();
	while(1)
	{
		if(sec_flag)
		{
			sec_flag = 0;
			if(sec < 9999)
			{
				sec ++;
				led_show(sec);
			}
			else
				sec = 0;
		}
	}
}

void interruptTimer0() interrupt 1
{
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	count ++;
	if(count > 100)
	{
		count = 0;
		sec_flag = 1;
	}
}