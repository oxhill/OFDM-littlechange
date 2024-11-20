
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "sync.h"
#include "ofdm.h"
#include "dsp_maxval.h"
#include <csl_timer.h>


/*************************************************/
#pragma	DATA_SECTION(SyncFrame, ".syncframebuf")
#pragma	DATA_ALIGN(SyncFrame, 8)	//double-word align

#pragma	DATA_SECTION(sendbytes, ".sendbytesbuf")
#pragma	DATA_ALIGN(sendbytes, 8)

#pragma	DATA_SECTION(outsendbytes, ".outsendbytesbuf")
#pragma	DATA_ALIGN(outsendbytes, 8)

#pragma	DATA_SECTION(sendsyms, ".sendsymsbuf")
#pragma	DATA_ALIGN(sendsyms, 8)

#pragma	DATA_SECTION(outsendsyms, ".outsendsymsbuf")
#pragma	DATA_ALIGN(outsendsyms, 8)

#pragma	DATA_SECTION(recv, ".recvbuf")
#pragma	DATA_ALIGN(recv, 8)

#pragma	DATA_SECTION(lfmdetect, ".lfmdetectbuf")
#pragma	DATA_ALIGN(lfmdetect, 8)

#pragma	DATA_SECTION(highpoint, ".highpointbuf")
#pragma	DATA_ALIGN(highpoint, 4)	//word-align

#pragma DATA_SECTION(pilot,".pilotbuf")
#pragma DATA_ALIGN(pilot,8)

#pragma DATA_SECTION(fftinput,".fftinputbuf")
#pragma DATA_ALIGN(fftinput,8)

TIMER_Handle hTimer0;   //Handle for the timer device               
/*EDMA_Handle hEdma;     //Handle for the EDMA channel               
EDMA_Handle hEdmaPing; //Handle for the ping EDMA reload parameters
EDMA_Handle hEdmaPong; //Handle for the pong EDMA reload parameters
EDMA_Config cfgEdma;*/   //EDMA configuration structure              
TIMER_Config baiConfig = {

    TIMER_CTL_RMK   
    (   
        TIMER_CTL_SPND_EMUSTOP,   
        TIMER_CTL_INVINP_NO,            
        TIMER_CTL_CLKSRC_CPUOVR8,       
        TIMER_CTL_CP_PULSE,             
        TIMER_CTL_HLD_NO,            
        TIMER_CTL_GO_NO,            
                                       
        TIMER_CTL_PWID_TWO,           
                                        
        TIMER_CTL_DATOUT_1,             
        TIMER_CTL_INVOUT_YES,             
        TIMER_CTL_FUNC_TOUT            
    ),      
 0xffffffff,
 0x0
};//Create the EDMA configuration structure for ping transfers

/************************* ************************/
short w[2*N_FFT];		//twiddle factors

static short SyncFrame[LFM_LEN];
short sendbytes[(N_USE_CARR/4)*N_DATASYMBOLS];
short outsendbytes[(N_USE_CARR/4)*N_DATASYMBOLS];
short sendsyms[MAX_FFT_LEN];

short outsendsyms[MAX_FFT_LEN];
short lfmdetect[DETECTLEN];
short highpoint[HIGHPOINT];
short pilot[2*N_USE_CARR];
short fftinput[2*N_FFT];
short outfft[2*N_FFT];


/*=================================*/
short recv[ADC_LEN];
short *adcptr = recv;
short *ptrsyms = sendsyms;
short *ptroutsyms = outsendsyms;
/************************************************/
short index = 0;

int biterr = 0;

unsigned char brev[64] = { 
	0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
	0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
	0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
	0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
	0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
	0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
	0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
	0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f 
};

/*********************************************/
void main()
{	
	Uint32 i=0, j=0;
	unsigned int baitime;
	short max_val = 0;
	/*****************************/
	SysInit();
	syncInit(SyncFrame);
	gen_twiddle_fft16x16r(w, N_FFT, 32767.);
  hTimer0 = TIMER_open(TIMER_DEV0, TIMER_OPEN_RESET);
   TIMER_config(hTimer0, &baiConfig);
	for(i=0; i<ADC_LEN; i++)
	{
		recv[i] = 0;
	}
	for(i=0; i<HIGHPOINT; i++)
	{
		highpoint[i] = 0;
	}	

	for(i=0; i<MAX_FFT_LEN; i++)
	{
		sendsyms[i] = 0;
		outsendsyms[i] = 0;
	
	}
    
	for( i=0; i<2*N_USE_CARR;i++)
	{
	    pilot[i]=0;
	}
/*******************************************************/
	for(i=0; i<(N_USE_CARR/4)*N_DATASYMBOLS; i++)
	{
		outsendbytes[i] = 0;
		sendbytes[i] = rand() & 0xff;
	}

	adcptr = recv;
	for(i=0; i<ADC_LEN; i++)
	{
		*(adcptr+i) = 0;
	}
/**================================***/
	for(i=0; i<N_DDS; i++)
	{
		*(adcptr+i) = 32767.*sin(2*PI*F1*i/FS);	
		*(adcptr+i+N_DDS) = 32767.*sin(2*PI*F2*i/FS);
		*(adcptr+i+2*N_DDS) = 32767.*sin(2*PI*F3*i/FS);
	}
/*===============================*/
	adcptr += (DDS_GAP+DDS_LEN);
	for(i=0; i<LFM_LEN; i++)
	{
		*(adcptr+i) = SyncFrame[i];
	}

/*******************************************************/
	ofdmEncode(sendsyms, N_FFT, sendbytes, N_SYMBOLS, pilot);

	for(i=0; i<MAX_FFT_LEN; i++)	//输入数据归一化。除以SCALE
	{								//防止IFFT运算溢出
		sendsyms[i] /= 18;
		outsendsyms[i] = 0;
	}
	/*===============================*/
	ptrsyms = sendsyms;
	ptroutsyms = outsendsyms;
	for(i=0; i<(N_SYMBOLS); i++)
	{
		/******    IFFT     ************/
	 	for(j=0; j<N_FFT; j++)
		{
		    *(ptrsyms+2*j+1) = -(*(ptrsyms+2*j+1));
		}

	    DSP_fft16x16r(N_FFT, ptrsyms, w, brev, ptroutsyms, RADIX, 0, N_FFT);

		for(j=0; j<2*N_FFT; j++)
		{
		    if(j<N_FFT)  *(ptroutsyms+2*j+1) = -(*(ptroutsyms+2*j+1));		    	 	  
	       *(ptroutsyms+j) /= (N_FFT>>8);

		}
		/********   END OF   IFFT    ***************/
		ptrsyms += 2*N_FFT;
		ptroutsyms += 2*N_FFT;
	}
	
	ptroutsyms = outsendsyms;	
	adcptr += (LFM_LEN + SYNC_GAP);	
	for(i=0; i<(N_SYMBOLS); i++)
	{
		max_val = DSP_maxval(ptroutsyms, 2*N_FFT);
		
		for(j=0; j<CP_LEN; j++)		//加入循环前缀
		{
			*(adcptr+j)	= (*(ptroutsyms+2*(3*N_FFT/4)+2*j))*(32767./max_val);
		}
		
		for(j=0; j<N_FFT; j++)
		{
			*(adcptr+j+CP_LEN) = (*(ptroutsyms+2*j))*(32767./max_val);		
		}
		
		ptroutsyms += 2*N_FFT;
		adcptr += (N_FFT+CP_LEN);
	}


/************************   receiver    ***************************/
		/*************    SYNC CAPTURE   ********************/
			for(i=0; i<DETECTLEN; i++)
			{
				lfmdetect[i] = recv[i+2*N_DDS];
			}
			max_val = DSP_maxval(lfmdetect, DETECTLEN);
			for(i=0; i<DETECTLEN; i++)							//接收到的数据进行归一化/N_FFT
			{													//为了防止FIR寻找同步点运算溢出
				lfmdetect[i] = lfmdetect[i]*(32767./max_val)/(N_FFT);
			}

			index = syncGetIndex(lfmdetect, SyncFrame, highpoint, LFM_LEN, HIGHPOINT)+2*N_DDS-150;

			/*****************************************************/
			for(i=0; i<MAX_FFT_LEN; i++)
			{
				sendsyms[i] = 0;
				outsendsyms[i] = 0;
			}

			ptroutsyms = outsendsyms;
			adcptr = recv;
			adcptr += (index+LFM_LEN+SYNC_GAP);

			for(j=0; j<(N_SYMBOLS); j++)
			{
			
			
				max_val = DSP_maxval(adcptr, (N_FFT+CP_LEN));					//去循环前缀
				for(i=0; i<N_FFT; i++)
				{
					*(ptroutsyms+2*i) = (*(adcptr+i+CP_LEN))*(32767/max_val)/SCALE;
				}
				ptroutsyms += 2*N_FFT;
				
   adcptr += (N_FFT+CP_LEN);
			}
			/*********************************/

			ptroutsyms = outsendsyms;
			ptrsyms = sendsyms;
			for(i=0; i<(N_SYMBOLS); i++)
			{
				//TIMER_start(hTimer0);
				DSP_blk_move(ptroutsyms,fftinput,N_FFT<<1);
      //  baitime=TIMER_getCount(hTimer0);  //读取计数器中的周期数
       ////         TIMER_pause(hTimer0);       //停止计数
        //        printf("the time spended is %fs\n",(float)baitime*4/(3*100000000)); //将周期数转化成具体时间
              
	        	DSP_fft16x16r(N_FFT,fftinput, w, brev, outfft, RADIX, 0, N_FFT);
				TIMER_start(hTimer0);
				DSP_blk_move(outfft,ptrsyms,N_FFT<<1);
                baitime=TIMER_getCount(hTimer0);  //读取计数器中的周期数
                TIMER_pause(hTimer0);       //停止计数
                printf("the time spended is %fs\n",(float)baitime*4/(3*100000000)); //将周期数转化成具体时间
              
               
    //    transferCount=0;
    //    TIMER_start(hTimer);
				
	 //Waiting for interrupts
//	 while (transferCount < TRANSFER_CNT);

				ptrsyms += N_FFT<<1;
				ptroutsyms += N_FFT<<1;
			
			}


          

			ofdmDecode(outsendbytes, N_SYMBOLS,sendsyms, N_FFT, pilot);

			biterr = testerror(sendbytes, outsendbytes, (N_USE_CARR/4)*N_DATASYMBOLS);
			printf("index = %d, biterr is %d.\n", index, biterr);


	while(1)
	{
    }

}










