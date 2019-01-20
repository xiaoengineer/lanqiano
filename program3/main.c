#include <STC90C5xAD.H>
typedef unsigned int u16;
typedef unsigned char u8;

u8 seg_ment[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80,
					0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; //�����λ
u8 led_buff[] = {0xff, 0xff, 0xff, 0xff};//ȷ�����ϵ��ʱ����

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

void Timer0Init(void)		//100΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA =1;
	ET0 =1;
}

/*�������ʹ������ʾ��*/
void led_scan()
{
	static u8 index = 0; //�����̬����ʹ�����Զ���λ��
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0XFF; //�������Ӱ
	P2 = P2 & 0X1F;
	delay(5);
	
	P2 = (P2 & 0X1F) | 0XC0;
	P0 = 0X80 >> index; //�����ʵ�����Զ���λ
	P2 = 0x1f;
	
	P2 = (P2 &0x1f) | 0xe0;
	P0 = led_buff[index];
	P2 = P2 & 0X1F;
	
	if(index < 3)
		index ++;
	else
		index = 0;
}

/*��������������������ݵģ����˻����˼��*/
void show_number(u16 dat)
{
	char i;
	u8 buf[4];
	
	for(i = 0; i < 4; i ++)
	{
		buf[i] = dat % 10; 
		dat = dat / 10;  //�������Ǹ���λ������������뻺��
	}
	for(i = 3; i > 0; i --)
	{
		if(buf[i] == 0) //�����ʱ������������λ��û������
			led_buff[i] = 0xff; //��ô��Ϊ0xff,����Ϳ�����ʽ��������
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
	TL0 = 0x9C;		//���ö�ʱ��ֵ
	TH0 = 0xFF; 	//���ö�ʱ��ֵ
	count ++;
	if(count > 100)
	{
		count = 0;
		sec_flag = 1;
	}
}