/************************************************************/
/* 文件名:		sync.c										*/
/* 功能:    	在发送端的数据包中加入同步头				*/
/* Version:		2010-4-13									*/
/***********************************************************/
#include "sync.h"

int cosTable[COSLEN]={
32767, 32757, 32728, 32678, 32609, 32521, 32412, 32285, 32137, 31971, 31785, 31580, 31356, 31113, 30852, 30571, 30273, 29956, 29621, 29268, 28898, 28510, 28105, 27683, 27245, 26790, 26319, 25832, 25329, 24811, 24279, 23731, 23170, 22594, 22005, 21403, 20787, 20159, 19519, 18868, 18204, 17530, 16846, 16151, 15446, 14732, 14010, 13279, 12539, 11793, 11039, 10278, 9511.8, 8739, 7962, 7179, 6392.5, 5602, 4808, 4011, 3212, 2411, 1608, 804.14, 2.0064e-012, -804.14, -1607.8, -2410.5, -3211.7, -4011, -4807.9, -5601.9, -6392.5, -7179.3, -7961.7, -8739.4, -9511.8, -10278, -11039, -11793, -12539, -13279, -14010, -14732, -15446, -16151, -16846, -17530, -18204, -18868, -19519, -20159, -20787, -21403, -22005, -22594, -23170, -23731, -24279, -24811, -25329, -25832, -26319, -26790, -27245, -27683, -28105, -28510, -28898, -29268, -29621, -29956, -30273, -30571, -30852, -31113, -31356, -31580, -31785, -31971, -32137, -32285, -32412, -32521, -32609, -32678, -32728, -32757, -32767, -32757, -32728, -32678, -32609, -32521, -32412, -32285, -32137, -31971, -31785, -31580, -31356, -31113, -30852, -30571, -30273, -29956, -29621, -29268, -28898, -28510, -28105, -27683, -27245, -26790, -26319, -25832, -25329, -24811, -24279, -23731, -23170, -22594, -22005, -21403, -20787, -20159, -19519, -18868, -18204, -17530, -16846, -16151, -15446, -14732, -14010, -13279, -12539, -11793, -11039, -10278, -9511.8, -8739.4, -7961.7, -7179.3, -6392.5, -5602, -4808, -4011, -3212, -2411, -1608, -8040, 0, 804, 1608, 2411, 3212, 4011, 4808, 5602, 6393, 7179, 7962, 8740, 9512, 10278, 11039, 11793, 12539, 13279, 14010, 14732, 15446, 16151, 16846, 17530, 18204, 18868, 19519, 20159, 20787, 21403, 22005, 22594, 23170, 23731, 24279, 24811, 25329, 25832, 26319, 26790, 27245, 27683, 28105, 28510, 28898, 29268, 29621, 29956, 30273, 30571, 30852, 31113, 31356, 31580, 31785, 31971, 32137, 32285, 32412, 32521, 32609, 32678, 32728, 32757
};

/****************************************************************/
/*	函数名:		syncInit() 										*/
/* 	功能:		初始化线性调频信号（用于同步）和保护间隔		*/
/* 	Note:		线性调频信号的幅度被归一化至-32768~32767之间		*/
/****************************************************************/
void syncInit(short *SyncFrame)
{
  int i;
  long tempindex;
  for(i=0; i<LFM_LEN; i++)
  {
    tempindex=(long)COSLEN*i*LFM_F0_DELTAT +(long)COSLEN*i*i*LFM_U/2;
    SyncFrame[i]=cosTable[(tempindex) & 0xff];
  }
}
/****************************************************************/
int mymaxidx(short *x, int nx)		//寻找最大值对应的序号
{
	int idx=0;
	short max_val=0;
	int max_idx=0;

	max_val = x[0];
	max_idx = 0;
	for(idx=0; idx<nx; idx++)
	{
		if(x[idx] > max_val)	
		{
			max_val = x[idx];
			max_idx = idx;
		}	
	}

	return (max_idx);
}	 			

/* 用FIR滤波器实现同步检测*/
int syncGetIndex(short *x, short *h, short *r, int nh, int nr)
{
	int syncindex = 0;

    DSP_fir_r4(x, h, r, nh, nr);
    syncindex = mymaxidx(r, nr);   
    
    return (syncindex);                    

}






