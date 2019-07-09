typedef struct __kfifo 
{     
    uint8_t *buffer;                 /* the buffer holding the data */     
    uint16_t size;                   /* the size of the allocated buffer */     
    uint16_t in;                     /* data is added at offset (in % size) */     
    uint16_t out;                    /* data is extracted from off. (out % size) */     
}kfifo;
Kfifo RFrxbuf,RFtxbuf;               //256byte
Kfifo UARTrxbuf,UARTtxbuf;           //256byte

#define min(a,b) {a<b?a:b}

uint16_t kfifo_put(kfifo *fifo, uint8_t *buffer, uint16_t len)
{
    uint16_t tail_len;
    len = min(len, fifo->size - fifo->in + fifo->out);                      //待存储长度、FIFO剩余空间长度，取较小值
    tail_len = (len, fifo->size - (fifo->in & (fifo->size - 1)));           //实际储存长度、in到FIFO尾部的长度，取较小值
    mencpy(fifo->buffer + (fifo->in & (fifo->size - 1), buffer，tail_len)); //将部分数据存储在FIFO尾部
    mencpy(fifo->buffer, buffer + tail_len, len - tail_len);                //将剩余数据储存在FIFO头部(如果len - tail_len > 0)
    fifo->in += len;                                                        //更新已存储数据的位置
    return len;                                                             //返回实际存储的数据长度
}


uint16_t kfifo_get(kfifo *fifo, uint8_t *buffer, uint16_t len)
{
    uint16_t tail_len;
    len = min(len, fifo->in - fifo->out);                                   //待读取长度、剩余未读取长度，取较小值
    tail_len = min(len, fifo->size - (fifo->out & (fifo->size - 1)));       //实际读取长度、out到FIFO尾部的长度，取较小值
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);       //读取out到FIFO尾部的数据
    memcpy(buffer + l, fifo->buffer, len - l);                              //读取剩余头部数据
    fifo->out += len;                                                       //更新已读取数据的位置
    return len;                                                             //返回实际读取的数据长度
}







#define SMALL_BLOCK_NUM         8
#define LARGE_BLOCK_NUM         5
#define SMALL_BLOCK_SIZE        256
#define LARGE_BLOCK_SIZE        2048


struct __BLOCK_INFO_ 
{
    LTimer  life _timer;              //数据小块生命周期
    uint8_t *object；                 //位置索引
    uint8_t  deal;                    //完整处理标识
    uint8_t  type;                    //数据小块存储类型
}BLOCK_INFO;                          //每个数据块保存的必要说明信息


typedef struct __MEMPOOL_
{
    uint32_t    sbfree;                  
    uint32_t    lbfree;
    BLOCK_INFO  sbinfo[SMALL_BLOCK_NUM];
    BLOCK_INFO  lbinfo[LARGE_BLOCK_NUM];
    uint8_t     sblock[SMALL_BLOCK_NUM][SMALL_BLOCK_SIZE];               
    uint8_t     lblock[LARGE_BLOCK_NUM][LARGE_BLOCK_SIZE];             
}MEMPOOL;


MEMPOOL mempool;

Mem_init(void);//内存空间初始化
Mem_malloc(u8,u8);//请求分配数据存储块
Mem_free(u8 *,u8);//释放分配的数据内存块


Mem_init(void)
{

}

uint8_t *Mem_malloc(uint16_t size, BLOCK_INFO blinfo)
{
    uint8_t block_msg = 0; //bit7:    0申请失败,1申请成功;  
                           //bit6:    0分配小块(长度256),1分配大块(长度2048);
                           //bit5~3:  保留;
                           //bit2~0:  申请块序号0~7.
    
    if(size <= SMALL_BLOCK_SIZE)                                    //申请小块
    {
        if(mempool.sbfree & 0x00FFFFFF)                             //优先分配小块
        {
            
            block_msg = (uint8_t)(mempool.sbfree & 0x00000007) - 1;          //申请块序号
            block_msg |= 0x80;                                      //申请成功;分配小块 
            mempool.sbfree = mempool.sbfree >> 3;                   //将块移除空闲队列
        }
        else if(mempool.lbfree & 0x00007FFF)                        //无可用小块则分配大块
        {
            block_msg = (uint8_t)(mempool.lbfree & 0x00000007) - 1;          //申请块序号
            block_msg |= 0xC0;                                      //申请成功;分配大块 
            mempool.lbfree = mempool.lbfree >> 3;                   //将块移除空闲队列
        }
        else                                                        //无可用空间块，申请失败
        {
            //.....
        }
    }
    else                                                            //申请大块
    {
        block_msg = 0x80
        if(mempool.lbfree & 0x00007FFF)                             //分配大块
        {
            block_msg |= (uint8_t)(mempool.lbfree & 0x00007FFF) - 1;
            mempool.lbfree = mempool.lbfree >> 3;
        }
        else                                                        //无可用空间块，申请失败
        {
            //.....
        }
    }

    if(block_msg & 0x80)
    {
        if(block_msg & 0x40)
        {
            memcpy(&lbinfo[block_msg & 0x07][0],blinfo,size(BLOCK_INFO));
            return &lblock[block_msg & 0x07][0];
        }
        else
        {
            memcpy(&sbinfo[block_msg & 0x07][0],blinfo,size(BLOCK_INFO));
            return &sblock[block_msg & 0x07][0];
        }
    }
    return 0x20000000; //申请失败
}

Mem_free(u8 *,u8)
{

}




