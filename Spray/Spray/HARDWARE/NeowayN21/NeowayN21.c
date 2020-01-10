#include "Rs485.h"
#include "delay.h"
#include "sys.h"
#include "NeowayN21.h"

//��Ȩ��Ϣ
char ProductKey0[20]=	"a1gCrIXMaIW";
char DeviceName0[50]=	"mjwlpwj-01";
char DeviceSecret0[50]=	"caVZKCujTnNcwF4p0eI5VvRobzyLGTsZ";

extern u8 Nsendok;

char CCID[25];

void	RESET_N21()						//N21��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}

void	NeoWayN21_init()				//�з�N21��ʼ��
{
	u8 buf[250];
	u8 num,num1;
	char *msg;
	RESET_N21();
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	delay_ms(1000);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	delay_ms(1000);
	do
	{
		printf_num=1;
		printf("AT\r\n");
		delay_ms(500);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,'K')[0]=='K')break;
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	printf_num=1;
	printf("AT&F\r\n");
	delay_ms(500);
	comClearRxFifo(COM1);
	
	do
	{
		printf_num=1;
		printf("AT+CCID\r\n");
		delay_ms(500);
		COM1GetBuf(buf,200);
		if(strstr((const char *)buf,"+CCID:")[0]=='+')break;
	}
	while(1);
	msg=strstr((const char *)buf,"+CCID:")+7;
	for(num=0;*msg!=0x0d;msg++)CCID[num++]=*msg;
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
//	do
//	{
//		printf_num=1;
//		printf("AT+CGMR\r\n");
//		delay_ms(500);
//		COM1GetBuf(buf,200);
//		if(strstr((const char *)buf,"OK")[0]=='O')break;
//	}
//	while(1);
	
	do
	{
		printf_num=1;
		printf("AT+NVSETBAND=1,8\r\n");
		delay_ms(500);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,'K')[0]=='K')break;
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
//	do
//	{
//		printf_num=1;
//		printf("AT+CGDCONT=1,\"IP\",\"snbiot\"\r\n");
//		delay_ms(500);
//		COM1GetBuf(buf,200);
//		if(strchr((const char *)buf,'K')[0]=='K')break;
//	}
//	while(1);
//	comClearRxFifo(COM1);
//	memset(buf,0,sizeof buf);
	
	do
	{
		printf_num=1;
		printf("AT+CSQ\r\n");
		delay_ms(500);
		COM1GetBuf(buf,200);
		num=strchr((const char *)buf,':')[2]-0x30;
		num=num*10+(strchr((const char *)buf,':')[3]-0x30);
		num1=strchr((const char *)buf,':')[5]-0x30;
		num1=num1*10+(strchr((const char *)buf,':')[6]-0x30);
		if((num!=99)&(num1==99))break;
		comClearRxFifo(COM1);
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	do
	{
		printf_num=1;
		printf("AT+CREG?\r\n");
		delay_ms(500);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,'0')[2]=='1')break;
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	do
	{
		printf_num=1;
		printf("AT+XIIC=1\r\n");
		delay_ms(2000);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,'K')[0]=='K')
		{
			if(strchr((const char *)buf,'E')[0]=='E')
			{
				comClearRxFifo(COM1);
				memset(buf,0,sizeof buf);
				continue;
			}
			break;
		}	
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	do
	{
		printf_num=1;
		printf("AT+XIIC?\r\n");
		delay_ms(500);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,',')[1]!='0')break;
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	delay_ms(500);
}

void	conN21()						//N21���ӵ�������
{
	u8 i,k;
	u8 buf[250];
	i=k=0;
	do			//�豸��Ȩ
	{
		i=0;
		printf_num=1;
		printf("AT+IMQTTAUTH=\"%s\",\"%s\",\"%s\"\r\n",ProductKey0,DeviceName0,DeviceSecret0);
		do{
			COM1GetBuf(buf,100);
			delay_ms(500);
			if(strstr((const char *)buf,"+IMQTTAUTH:")[0]=='+')break;
			if(++i==40)break;
		}while(1);
		
		if(strstr((const char *)buf,"+IMQTTAUTH:OK")[0]=='+')break;
		memset(buf,0,sizeof buf);
		comClearRxFifo(COM1);
		if(++k==5){Nsendok=0;return;}
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	do
	{
		printf_num=1;
		printf("AT+IMQTTPARA=\"TIMEOUT\",10,\"CLEAN\",1,\"KEEPALIVE\",180,\"VERSION\",\"3.1\"\r\n");
		delay_ms(1000);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,'K')[0]=='K')break;
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	
	do			//�豸����
	{
		i=0;
		printf_num=1;
		printf("AT+IMQTTCONN\r\n");
		do
		{
			COM1GetBuf(buf,200);
			delay_ms(500);
			if(strchr((const char *)buf,'K')[0]=='K')break;
			if(strchr((const char *)buf,'E')[0]=='E')break;
			if(++i==40)break;
	
		}while(1);
		comClearRxFifo(COM1);
		if(strchr((const char *)buf,'K')[0]=='K')break;
		memset(buf,0,sizeof buf);
		delay_ms(1000);
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	 
	do
	{
		printf_num=1;
		printf("AT+IMQTTSUB=\"/%s/%s/user/get\",1\r\n",ProductKey0,DeviceName0);
		delay_ms(1000);
		COM1GetBuf(buf,200);
		if(strchr((const char *)buf,'K')[0]=='K')break;
	}
	while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	Nsendok=1;
}

void	disconN21()						//�Ͽ�N21����
{
	u8 buf[250];
	do
	{
		printf_num=1;
		printf("AT+DISIMQTTCONN\r\n");
		COM1GetBuf(buf,200);
		delay_ms(500);
		if(strchr((const char *)buf,'K')[0]=='K')break;
		if(strchr((const char *)buf,'E')[0]=='E')break;
	}while(1);
	comClearRxFifo(COM1);
	memset(buf,0,sizeof buf);
	delay_ms(1000);
}

u8	sendN21(char *data)				//���ĵ�topic��������
{
	char hc[300];u8 buf[100];

//	sprintf(hc,"AT+IMQTTPUB=\"/sys/%s/%s/thing/event/property/post\",1,\"{\\\"params\\\":{%s}}\"\r\n",ProductKey0,DeviceName0,data);
	sprintf(hc,"AT+IMQTTPUB=\"/sys/%s/%s/thing/event/property/post\",1,\"{%s}\"\r\n",ProductKey0,DeviceName0,data);
//	len=strlen(hc);						//ɾ���ϱ�����ֵʱ���һ������
//	if(hc[len-6]==',')hc[len-6]=' ';
	printf_num=1;
	printf("%s",hc);
	delay_ms(2000);
	COM1GetBuf(buf,330);
	comClearRxFifo(COM1);
	if(strstr((const char *)buf,"+CME")[0]=='+')return 0;	//�ж��Ƿ��ϱ�����
	else return 1;
}