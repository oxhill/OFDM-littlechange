/****************************************************************/
/*	文件名:		config.c										*/
/* 	功能:		EMIFA配置函数、系统初始化 	*/
/*	版本:		1.1											    */
/*				2010-4-13								*/
/*  Author： Sayi Cheng                            */
/*                                       */
/****************************************************************/
#include "config.h"
/****************************************************************/
/*	函数名:												*/
/* 	功能:		系统硬件初始化									*/
/****************************************************************/
/*****************************************************************/
/*SEEDDM642的emifa的设置结构*/
EMIFA_Config Seeddm642ConfigA ={
	   0x00052078,/*gblctl EMIFA(B)global control register value */
	   			  /*将CLK6、4、1使能；将MRMODE置1；使能EK2EN,EK2RATE*/
	   0xffffffd3,/*cectl0 CE0 space control register value*/
	   			  /*将CE0空间设为SDRAM*/
	   0x73a28e01,/*cectl1 CE1 space control register value*/
	   			  /*Read hold: 1 clock;
	   			    MTYPE : 0000,选择8位的异步接口
	   			    Read strobe ：001110；14个clock宽度
	   			    TA：2 clock; Read setup 2 clock;
	   			    Write hold :2 clock; Write strobe: 14 clock
	   			    Write setup :7 clock
	   			    --					 ---------------
	   			  	  \		 14c		/1c
	   			 	   \----------------/ */
	   0x22a28a22, /*cectl2 CE2 space control register value*/
       0x22a28a42, /*cectl3 CE3 space control register value*/
	   0x57115000, /*sdctl SDRAM control register value*/
	   0x0000081b, /*sdtim SDRAM timing register value*/
	   0x001faf4d, /*sdext SDRAM extension register value*/
	   0x00000002, /*cesec0 CE0 space secondary control register value*/
	   0x00000002, /*cesec1 CE1 space secondary control register value*/
	   0x00000002, /*cesec2 CE2 space secondary control register value*/
	   0x00000073 /*cesec3 CE3 space secondary control register value*/	
};
/*********************************************************************/

void SysInit(void)
{
/*-------------------------------------------------------*/
/* perform all initializations                           */
/*-------------------------------------------------------*/
	/*Initialise CSL，初始化CSL库*/
	CSL_init();
/*----------------------------------------------------------*/
	/*EMIFA的初始化，将CE0设为SDRAM空间，CE1设为异步空间
	 注，DM642支持的是EMIFA，而非EMIF*/
	EMIFA_config(&Seeddm642ConfigA);
/*----------------------------------------------------------*/
	/*中断向量表的初始化*/
	//Point to the IRQ vector table
    IRQ_setVecs(vectors);  
/*----------------------------------------------------------*/
    
}
/********************************************************************/
/*以下两个函数是用于FFT的，产生旋转因子*/
/* ======================================================================== */
/*  D2S -- Truncate a 'double' to a 'short', with clamping.                 */
/* ======================================================================== */
static short d2s(double d)
{
    if (d >=  32767.0) return  32767;
    if (d <= -32768.0) return -32768;
    return (short)d;
}

/* ======================================================================== */
/*  GEN_TWIDDLE -- Generate twiddle factors for fft16x16r().                */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is called as follows:                                  */
/*                                                                          */
/*          int gen_twiddle_fft16x16r(short *w, int n, double scale)        */
/*                                                                          */
/*          short  *w     Pointer to twiddle-factor array                   */
/*          int    n      Size of FFT                                       */
/*          double scale  Scale factor to apply to values.                  */
/*                                                                          */
/*      The routine will generate the twiddle-factors directly into the     */
/*      array you specify.  The array needs to be approximately 2*N         */
/*      elements long.  (The actual size, which is slightly smaller, is     */
/*      returned by the function.)                                          */
/* ======================================================================== */
int gen_twiddle_fft16x16r(short *w, int n, double scale)
{
    int i, j, k;

    for (j = 1, k = 0; j < n >> 2; j = j << 2)
    {
        for (i = 0; i < n >> 2; i += j << 1)
        {
            w[k + 11] = d2s(scale * cos(6.0 * PI * (i + j) / n));
            w[k + 10] = d2s(scale * sin(6.0 * PI * (i + j) / n));
            w[k +  9] = d2s(scale * cos(6.0 * PI * (i    ) / n));
            w[k +  8] = d2s(scale * sin(6.0 * PI * (i    ) / n));

            w[k +  7] = d2s(scale * cos(4.0 * PI * (i + j) / n));
            w[k +  6] = d2s(scale * sin(4.0 * PI * (i + j) / n));
            w[k +  5] = d2s(scale * cos(4.0 * PI * (i    ) / n));
            w[k +  4] = d2s(scale * sin(4.0 * PI * (i    ) / n));

            w[k +  3] = d2s(scale * cos(2.0 * PI * (i + j) / n));
            w[k +  2] = d2s(scale * sin(2.0 * PI * (i + j) / n));
            w[k +  1] = d2s(scale * cos(2.0 * PI * (i    ) / n));
            w[k +  0] = d2s(scale * sin(2.0 * PI * (i    ) / n));

            k += 12;
        }
    }

    return k;
}






/******************************************************************************/
/*	No more																	  */
/******************************************************************************/

