// ConsoleApplication3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//编译环境：VS2017

#include "pch.h"
#include <conio.h>
#include <iostream>

typedef unsigned short  uin16_t;
typedef unsigned char   uin8_t;
typedef unsigned        char    INT8U;                                  // SRAM区，8位无符号整数
typedef signed          char    INT8S;                                  // SRAM区，8位有符号整数
typedef unsigned        short   INT16U;                                 // SRAM区，16位无符号整数
typedef signed          short   INT16S;                                 // SRAM区，16位有符号整数
typedef unsigned        long    INT32U;                                 // SRAM区，32位无符号整数
typedef signed          long    INT32S;                                 // SRAM区，32位有符号整数
uint8_t invert_8(uin8_t invert)
{
	invert = ((invert & 0x55) << 1 | (invert & 0xAA) >> 1);
	invert = ((invert & 0x33) << 2 | (invert & 0xCC) >> 2);
	invert = ((invert & 0x0F) << 4 | (invert & 0xF0) >> 4);
	return invert;
}
uint16_t invert_16(uin16_t invert)
{
	invert = ((invert & 0x5555) << 1 | (invert & 0xAAAA) >> 1);
	invert = ((invert & 0x3333) << 2 | (invert & 0xCCCC) >> 2);
	invert = ((invert & 0x0F0F) << 4 | (invert & 0xF0F0) >> 4);
	invert = ((invert & 0x00FF) << 8 | (invert & 0xFF00) >> 8);
	return invert;
}
INT16U Modbus_Crc16(INT8U *addr, INT16U num)
{
#define POLY        0x8005
	INT8U i;
	INT16U crc = 0XFFFF;
	INT8U invert;
	while (num--)
	{
		//输入反转
		invert = *addr++;
		invert = ((invert & 0x55) << 1 | (invert & 0xAA) >> 1);
		invert = ((invert & 0x33) << 2 | (invert & 0xCC) >> 2);
		invert = ((invert & 0x0F) << 4 | (invert & 0xF0) >> 4);//&0x0F,&0xF0可省
		crc = crc ^ (invert << 8);
		//计算crc
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x8000)
				crc = (crc << 1) ^ POLY;
			else
				crc <<= 1;
		}
		crc &= 0xFFFF;
	}
	//输出数据反转
	crc = ((crc & 0x5555) << 1 | (crc & 0xAAAA) >> 1);
	crc = ((crc & 0x3333) << 2 | (crc & 0xCCCC) >> 2);
	crc = ((crc & 0x0F0F) << 4 | (crc & 0xF0F0) >> 4);
	crc = ((crc & 0x00FF) << 8 | (crc & 0xFF00) >> 8);
	return (crc);
}
/**********************************************直接计算法******************************************/
//#define POLY        0x010a

uint16_t crc16(unsigned char *addr, int num, uint16_t crc)
{//调用时，第三个参数填0XFFFF
	int i;
	for (; num > 0; num--)             
	{
		crc = crc ^ (*addr++ << 8);    
		for (i = 0; i < 8; i++)        
		{
			if (crc & 0x8000)          
			crc = (crc << 1) ^ POLY;   
			else                       
			crc <<= 1;                 
		}                              
		crc &= 0xFFFF;                 
	}                                  
	return(crc);                       
}
/**********************************************直接计算法******************************************/

/**********************************************查表法******************************************/
typedef unsigned short width_t;
#define POLYNOMIAL          0x8005 
#define INITIAL_REMAINDER   0xFFFF
#define FINAL_XOR_VALUE     0x0000  

#define WIDTH (8 * sizeof(width_t))  
#define TOPBIT (1 << (WIDTH - 1))  

static width_t crcTable[256];
void crcInit(void)
{
	width_t remainder;
	width_t dividend;
	int bit;
	for (dividend = 0; dividend < 256; dividend++)
	{
		remainder = dividend << (WIDTH - 8);
		for (bit = 0; bit < 8; bit++)
		{
			if (remainder & TOPBIT)
			{
				remainder = (remainder << 1) ^ POLYNOMIAL;
			}
			else
			{
				remainder = remainder << 1;
			}
		}
		crcTable[dividend] = remainder;
	}
}

width_t crcCompute(unsigned char * message, unsigned int nBytes)
{
	unsigned int offset;
	unsigned char byte;
	width_t remainder = INITIAL_REMAINDER;
	for (offset = 0; offset < nBytes; offset++)
	{
		byte = (remainder >> (WIDTH - 8)) ^ message[offset];
		remainder = invert_16(remainder);
		remainder = (remainder >> 8) | (remainder<<8);
		remainder = crcTable[byte] ^ (remainder << 8);
	}
	remainder = invert_16(remainder);
	return (remainder ^ FINAL_XOR_VALUE);
}
/**********************************************查表法******************************************/
void test(void)
{
	uint16_t c;
	uint8_t m[100] = { 0xAA,0x03,0x00,0x00,0x00,0x01};
	c = Modbus_Crc16(m,6);
	printf("0X%04X\r\n", c);
}
int main()
{
	uint8_t  a[300],b[100],k=0;
	uint16_t i,j,c;
	//test();
	while (1)
	{
		printf("请输入十六进制数：\r\n");
		for (i = 0; k != 0x0a; i++)
		{
			//k = _getch();
			scanf_s("%c", &k);
			if (k <= '9'&&k >= '0')
			{
				a[i] = k - '0';
			}
			else if (k <= 'F'&&k >= 'A')
			{
				a[i] = k - 'A' + 10;
			}
			else if (k <= 'f'&&k >= 'a')
			{
				a[i] = k - 'a' + 10;
			}
			else if(0x1b == k)
			{
				//esc退出
				return 0;
			}
		}
		for (j = 0; j < i; j += 3)
		{

			b[j / 3] = a[j] * 16 + a[j + 1];
		}
		c = Modbus_Crc16(b, j / 3);
		printf("CRC16校验结果： ");
		printf("0X%04X\r\n", c);
		k = _getch();
		if (0x1b == k)
		{
			//esc退出
			return 0;
		}
	}
	//c = Modbus_Crc16(m,6);
	//printf("0X%04X\r\n", c);
	//test();
}
