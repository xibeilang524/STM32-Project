/************************************版权申明********************************************
**                             广州大彩光电科技有限公司                                
**                             http://www.gz-dc.com
**-----------------------------------文件信息--------------------------------------------
** 文件名称:   hmi_user_uart.c 
** 修改时间:   2011-05-18
** 文件说明:   用户MCU串口驱动函数库
** 技术支持：  Tel: 020-22077476  Email: hmi@gz-dc.com  
**--------------------------------------------------------------------------------------*/
        
/**--------------------------------------------------------------------------------------    
                                  使用必读                      
   hmi_user_uart.c中的串口发送接收函数共3个函数：串口初始化Uartinti()、发送1个字节SendChar()、
   发送字符串SendStrings().目前暂时只提供8051驱动平台，如果用户MCU非8051平台，需要修改底层寄
   存器设置,但禁止修改函数名称，否则无法与HMI驱动库(hmi_driver.c)匹配。 
**--------------------------------------------------------------------------------------*/

#include "hmi_user_uart.h"
#include "Rs485.h"


/****************************************************************************
* 名    称： UartInit()
* 功    能： 串口初始化
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void UartInit(uint16_t BaudRate)
{ 
  
}
/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节       
* 出口参数： 无                  
 *****************************************************************/
void  SendChar(uchar t) 
{
	//USART_SendData(HMI_UART, (uint16_t)t);     
	comSendChar(COM5,t);
} 

