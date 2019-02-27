
#include <stdio.h>

typedef unsigned short  uin16_t;
typedef unsigned char   uin8_t;
/**********************************************直接计算法******************************************/
#define POLY        0x1021

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
#define POLYNOMIAL          0x1021   
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
		remainder = crcTable[byte] ^ (remainder << 8);
	}
	return (remainder ^ FINAL_XOR_VALUE);
}
/**********************************************查表法******************************************/

int main()
{
	uint8_t a[60] = {0x15,0x68,0x11,0x68,0x01,0x00,0x68,0xff,0xff,0x01,0x00,0x55,0x25,0x16};
	uint16_t b,c;
	crcInit();
	b = crc16(a, 14,0xffff);
	c = crcCompute(a, 14);
    printf("0X%04X 0X%04X\r\n",b,c); 
}
