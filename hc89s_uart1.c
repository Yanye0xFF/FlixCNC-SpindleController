#include "hc89s_uart1.h"


void uart1_pinmux_setup() {
    //P21设置为推挽输出
    P2M0 = P2M0&0x0F|0x80;	
    //P03设置为上拉输入    
	P0M1 = P0M1&0x0F|0x20;	
    //TXD映射P21    
    TXD_MAP = 0x21;		
    //RXD映射P03	    
	RXD_MAP = 0x03;						          
}

void uart1_init() {
    //T4工作模式：UART1波特率发生器
    T4CON = 0x06;	
    //波特率9600    
    TH4 = 0xFF;
	TL4 = 0x98;		
    //8位UART，波特率可变    
	SCON2 = 0x02;	
    //允许串行接收    
	SCON = 0x10;	
    // 优先级 1 
    IP1 |= 0x01;
    //使能串口中断    
	IE |= 0X10;							             
}


