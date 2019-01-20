#include <STC89C5xRC.H>
typedef unsigned int u16;
typedef unsigned char u8;


void close()
{
	P2 = (P2 & 0X1F) | 0XA0;
	P0 = 0XAF;
	P2 = 0x1f; //关闭蜂鸣器，继电器
}

void main()
{
	close();
	P2 = (P2 & 0X1F) | 0XC0; //这个是位选使能，是不是感觉和普通的
								//开发板不一样？那是由于开发板的设计决定的
								//这样做是可以保证我可以修改我想要的位而保证
								//其他位不变
	P0 = 0X01;   //位选，选第一个位
	P2 = P2 & 0X1F; //禁止使能
		
	P2 = (P2 & 0X1F) | 0XE0; //这个是段选，也不太一样，理由同上
	P0 = 0xC0; //段选，显示一个数
	P2 = 0X1F;
	while(1);
}
