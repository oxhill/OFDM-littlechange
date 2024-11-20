/****************************************************************/
/*	�ļ���:		config.c										*/
/* 	����:		EMIFA���ú�����ϵͳ��ʼ�� 	*/
/*	�汾:		1.1											    */
/*				2010-4-13								*/
/*  Author�� Sayi Cheng                            */
/*                                       */
/****************************************************************/
#include "config.h"
/****************************************************************/
/*	������:												*/
/* 	����:		ϵͳӲ����ʼ��									*/
/****************************************************************/
/*****************************************************************/
/*SEEDDM642��emifa�����ýṹ*/
EMIFA_Config Seeddm642ConfigA ={
	   0x00052078,/*gblctl EMIFA(B)global control register value */
	   			  /*��CLK6��4��1ʹ�ܣ���MRMODE��1��ʹ��EK2EN,EK2RATE*/
	   0xffffffd3,/*cectl0 CE0 space control register value*/
	   			  /*��CE0�ռ���ΪSDRAM*/
	   0x73a28e01,/*cectl1 CE1 space control register value*/
	   			  /*Read hold: 1 clock;
	   			    MTYPE : 0000,ѡ��8λ���첽�ӿ�
	   			    Read strobe ��001110��14��clock���
	   			    TA��2 clock; Read setup 2 clock;
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
	/*Initialise CSL����ʼ��CSL��*/
	CSL_init();
/*----------------------------------------------------------*/
	/*EMIFA�ĳ�ʼ������CE0��ΪSDRAM�ռ䣬CE1��Ϊ�첽�ռ�
	 ע��DM642֧�ֵ���EMIFA������EMIF*/
	EMIFA_config(&Seeddm642ConfigA);
/*----------------------------------------------------------*/
	/*�ж�������ĳ�ʼ��*/
	//Point to the IRQ vector table
    IRQ_setVecs(vectors);  
/*----------------------------------------------------------*/
    
}
/********************************************************************/
/*������������������FFT�ģ�������ת����*/
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

