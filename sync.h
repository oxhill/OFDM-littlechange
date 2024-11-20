#ifndef __SYNC_H__
#define __SYNC_H__

#include "config.h"
#include "dsp_fir_r4.h"

#define LFM_LEN 4096				//线性调频信号长度
#define LFM_F0_DELTAT  1/6			//f0=15k  fs=60k,deltat=1/fs 
#define LFM_U		1/(10*4096) 		//deltaf=5k deltat=1/60k T=deltat*LFM_LEN t=i*deltat
	                                //!!cos(2pi*f0*t + 1/2 * (2pi*deltf/T) * t^2)

#define COSLEN 256


void syncInit(short *SyncFrame);

int syncGetIndex(short *x, short *h, short *r, int nh, int nr);

#endif







