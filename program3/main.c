#include <STC90C5xAD.H>
typedef unsigned int u16;
typedef unsigned char u8;

u8 seg_ment[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80,
					0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; //定义段位
u8 led_buff[] = {0xff, 0xff, 0xff, 0xff};//确保在上电的时候不亮

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

void Timer0Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA =1;
	ET0 =1;
}

/*这个函数使用来显示的*/
void led_scan()
{
	static u8 index = 0; //这个静态变量使用来自动移位的
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0XFF; //这个是消影
	P2 = P2 & 0X1F;
	delay(5);
	
	P2 = (P2 & 0X1F) | 0XC0;
	P0 = 0X80 >> index; //这个就实现了自动移位
	P2 = 0x1f;
	
	P2 = (P2 &0x1f) | 0xe0;
	P0 = led_buff[index];
	P2 = P2 & 0X1F;
	
	if(index < 3)
		index ++;
	else
		index = 0;
}

/*这个函数是用来处理数据的，用了缓存的思想*/
void show_number(u16 dat)
{
	char i;
	u8 buf[4];
	
	for(i = 0; i < 4; i ++)
	{
		buf[i] = dat % 10; 
		dat = dat / 10;  //将数字是各个位分离出来并存入缓存
	}
	for(i = 3; i > 0; i --)
	{
		if(buf[i] == 0) //如果临时缓存区里代表高位的没有数字
			led_buff[i] = 0xff; //那么就为0xff,否则就可以正式填入数据
		else		
			break;
	}
	for(; i >= 0; i --)
	{
		led_buff[i] = seg_ment[buf[i]];
	}
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
				show_number(sec);
				led_scan();
			}
			else
				sec = 0;
		}
	}
}

void T0_interrput() interrupt 1
{
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF; 	//设置定时初值
	count ++;
	if(count > 100)
	{
		count = 0;
		sec_flag = 1;
	}
}