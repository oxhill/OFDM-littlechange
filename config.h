#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <csl.h>
#include <cslr_emifa2.h>/*ԭ����csl_emifa*/
#include <csl_irq.h>
#include <csl_chip.h>
#include <csl_chiphal.h>
#include <csl_mcbsp.h>
#include <math.h>
#include <csl_gpio.h>
#include <csl_timer.h>

#include "seeddm642.h"
#include "dsp_fft16x16r.h"


#define Lp              (3)
#define	N_SYMBOLS		(6)		//OFDM������
#define N_DATASYMBOLS   (4)

#define N_FFT			(4096)
#define	CP_LEN			(1024)
#define	MAX_FFT_LEN		(2*(N_SYMBOLS+1)*N_FFT)

#define	N_USE_CARR		(408)
#define SCALE			(128)	//2^(log2(N_FFT)-ceil[log4(N_FFT)-1])

//BLS=FFTLEN*10KHz/60KHz�����ʣ���Ч�ز���ifft��������е�ƫ����
#define BLS 			(683) 		//��K�����ز���λ��
#define SYNC_GAP 		(600)
#define	DDS_GAP			(4800) //80ms

#define	N_DDS			(3600)
#define	DDS_LEN			(N_DDS*3)

#define	ADC_LEN			(DDS_LEN+DDS_GAP+LFM_LEN+SYNC_GAP+MAX_FFT_LEN+100) 

#define	DETECTLEN		(16000)
#define	HIGHPOINT		(10000)

//DDS frequency(Hz)
#define		F1			(8000)
#define		F2			(9000)
#define		F3			(10000)
#define		FS			(60000)

//TIMER0
#define PRDVAL			(0x4e2)	/*  sample rate freq=60khz  */
//MCBSP
#define	CLKGDVVAL		(34)


#define PI  (3.141592654)
#define	RADIX	(4)		/* FFT  2 if NN is a power of 2, 4 if NN is a power of 4 */


static short d2s(double d);

int gen_twiddle_fft16x16r(short *w, int n, double scale);


extern far void vectors();

/* system configure  */
void SysInit(void);


#endif
