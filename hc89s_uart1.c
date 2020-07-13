#include "hc89s_uart1.h"


void uart1_pinmux_setup() {
    //P21����Ϊ�������
    P2M0 = P2M0&0x0F|0x80;	
    //P03����Ϊ��������    
	P0M1 = P0M1&0x0F|0x20;	
    //TXDӳ��P21    
    TXD_MAP = 0x21;		
    //RXDӳ��P03	    
	RXD_MAP = 0x03;						          
}

void uart1_init() {
    //T4����ģʽ��UART1�����ʷ�����
    T4CON = 0x06;	
    //������9600    
    TH4 = 0xFF;
	TL4 = 0x98;		
    //8λUART�������ʿɱ�    
	SCON2 = 0x02;	
    //�����н���    
	SCON = 0x10;	
    // ���ȼ� 1 
    IP1 |= 0x01;
    //ʹ�ܴ����ж�    
	IE |= 0X10;							             
}


