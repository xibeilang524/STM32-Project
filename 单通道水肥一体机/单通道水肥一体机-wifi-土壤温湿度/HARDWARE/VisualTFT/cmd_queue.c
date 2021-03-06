

#include "cmd_queue.h"

#define CMD_HEAD 0XEE  //֡ͷ
#define CMD_TAIL 0XFFFCFFFF //֡β

/*
//ָ����ж���Ϊһ���ṹ�壬��������ͷ������β�����е����ݻ�����
qsize _head;   �Ӵ����жϻ�ȡ�������ݻ��1
qsize _tail;   �Ӷ��л������ж�һ�����ݻ��1
*/
typedef struct _QUEUE
{
	qsize _head;     //����ͷ
	qsize _tail;     //����β
	qdata _data[QUEUE_MAX_SIZE];   //�������ݻ�����	
}QUEUE; 

QUEUE que = {0,0,0};   //ָ�����
static uint32 cmd_state = 0;   //����֡β���״̬
static qsize cmd_pos = 0;    //��ǰָ��ָ��λ��


//��ն��еĻ�����
void queue_reset()
{
	que._head = que._tail = 0;
	cmd_pos = cmd_state = 0;
}

/*
���ж��е��û�ȡָ������

����������ָ�MCU��MCU�Ĵ����жϻ���ú���queue_push��ȡ���ݵ����л�����
*/
void queue_push(qdata _data)
{
	qsize pos = (que._head + 1) % QUEUE_MAX_SIZE;
	
	if(pos != que._tail)//����״̬��û�дﵽ������������
	{
		que._data[que._head] = _data;
		que._head = pos;
	}
}


/*
�Ӷ��л�������ȡһ������

�����еĻ������������ݺ�queue_find_cmd���������л����������ݵ��ã�queue_popһ��һ�������Ȼ��ƴ�ӳ�һ��������ָ��
*/
static void queue_pop(qdata* _data)
{
	if(que._tail != que._head)//�ǿ�״̬
	{
		*_data = que._data[que._tail];
		que._tail = (que._tail + 1) % QUEUE_MAX_SIZE;
	}
}


//��ȡ��������Ч���ݸ���
static qsize queue_size()
{
	return ((que._head + QUEUE_MAX_SIZE - que._tail) % QUEUE_MAX_SIZE);
}


/*
�����еĻ������������ݺ�queue_find_cmd���������л����������ݵ��ã�queue_popһ��һ�������Ȼ��ƴ�ӳ�һ��������ָ��
*/
qsize queue_find_cmd(qdata *buffer, qsize buf_len)
{
	qsize cmd_size = 0;     //qsize�� typedef unsigned char qdata;
	qdata _data = 0;     //qdata�� typedef unsigned short qsize;
	
	while(queue_size() > 0)   //��ȡ��������Ч���ݸ�������������0
	{
		//ȡһ������
		queue_pop(&_data);

		if(cmd_pos==0&&_data!=CMD_HEAD)//ָ���һ���ֽڱ�����֡ͷ����������
		    continue;

		if(cmd_pos<buf_len)//��ֹ���������
			buffer[cmd_pos++] = _data;

		cmd_state = ((cmd_state<<8)|_data);//ƴ�����4���ֽڣ����һ��32λ����

		//���4���ֽ���֡βƥ�䣬�õ�����֡
		if(cmd_state==CMD_TAIL)
		{
			cmd_size = cmd_pos; //ָ���ֽڳ���
			cmd_state = 0;  //���¼��֡β��
			cmd_pos = 0; //��λָ��ָ��

#if(CRC16_ENABLE)
			//ȥ��ָ��ͷβEE��βFFFCFFFF����5���ֽڣ�ֻ�������ݲ���CRC
			if(!CheckCRC16(buffer+1,cmd_size-5))//CRCУ��
				return 0;

			cmd_size -= 2;//ȥ��CRC16��2�ֽڣ�
#endif

			return cmd_size;
		}
	}

	return 0;
}


