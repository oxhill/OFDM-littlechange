/************************************************************************/
/* 文件名:		ofdm.c													*/
/* 功能:    	实现发送端字节拆分,QPSK映射和编码，共轭扩展, 
                实现添加导频，信道估计			*/
/* Version:		2010--7-30         

												*/
/************************************************************************/
#include "ofdm.h"

/************************************************************************/
/**   导频相位  **/

static short randomseq[N_USE_CARR]={
1, 2, 0, 0, 1, 3, 3, 1, 0, 3, 0, 2, 3, 2, 3, 0, 0, 3, 1, 3, 2, 2, 1, 0, 0, 0, 1, 3, 1, 3, 1, 1, 1, 1, 3, 0, 1, 0, 2, 2, 3, 2, 1, 0, 2, 2, 1, 0, 1, 3, 3, 3, 3, 1, 2, 2, 0, 3, 2, 2, 0, 2, 3, 1, 2, 1, 2, 2, 1, 1, 2, 3, 1, 1, 2, 2, 2, 2, 3, 1, 0, 2, 3, 3, 2, 1, 0, 2, 1, 0, 2, 3, 2, 3, 3, 0, 1, 2, 1, 2, 0, 2, 2, 3, 3, 2, 1, 2, 3, 0, 3, 3, 2, 1, 3, 2, 2, 1, 3, 0, 2, 1, 3, 1, 2, 0, 1, 1, 1, 2, 2, 0, 3, 3, 3, 3, 2, 0, 1, 2, 2, 0, 0, 1, 1, 1, 3, 2, 1, 1, 2, 0, 3, 3, 2, 0, 3, 2, 2, 0, 1, 1, 0, 3, 3, 1, 3, 0, 3, 3, 1, 2, 3, 2, 0, 1, 0, 1, 1, 1, 2, 0, 1, 2, 0, 2, 0, 2, 2, 1, 2, 3, 1, 1, 1, 1, 1, 1, 2, 2, 1, 0, 1, 1, 0, 2, 2, 1, 1, 2, 3, 3, 1, 3, 1, 3, 3, 0, 1, 2, 2, 2, 1, 1, 0, 1, 1, 1, 3, 3, 3, 2, 0, 2, 3, 3, 0, 2, 2, 3, 2, 0, 1, 0, 3, 1, 0, 3, 3, 1, 1, 3, 0, 3, 2, 3, 0, 1, 0, 3, 2, 1, 1, 3, 1, 0, 0, 0, 2, 0, 2, 0, 3, 1, 3, 0, 2, 3, 1, 3, 0, 0, 0, 3, 0, 1, 3, 0, 1, 1, 2, 3, 3, 2, 0, 0, 0, 1, 3, 3, 0, 3, 2, 2, 0, 3, 1, 3, 2, 0, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2, 3, 0, 1, 3, 1, 3, 2, 0, 0, 0, 0, 2, 1, 2, 2, 2, 0, 0, 1, 1, 3, 1, 2, 3, 2, 3, 3, 3, 0, 3, 3, 0, 0, 3, 2, 2, 3, 2, 3, 1, 1, 0, 0, 0, 3, 1, 1, 3, 0, 3, 2, 3, 1, 3, 1, 2, 0, 2, 1, 2, 2, 3, 0, 0, 2, 3, 0, 3, 0, 2, 0, 2, 1, 3, 0, 2, 3, 3, 1, 1, 1, 0, 0, 2, 2, 1, 1, 1, 
};

/****************************************************/

//复平面映射表（实部+虚部）
short 	QPSKreal[4]	= {32767, -32767, -32767, 32767};
short 	QPSKimage[4] = {32767, 32767, -32767, -32767};

/*********************************************************************
* 函数名：	ofdmEncode()
* 功能:		实现字节拆分,QPSK映射和编码，共轭扩展,添加导频
* input:	short inbytes[n_symbols]
* output:	short outsymbos[row0 random ref. data + 4*row(size 2*n_fft) data]
*				rowi: real0 image0 r1 i1, ...;0,0;.... r1 -i1  real0 -image0;
*				i=0...,5
* NOTE：	n_symbols MUST BE MULTIPLES OF 4 !
************************************************************************/

void ofdmEncode(short *outsymbols, Uint32 n_fft, short *inbytes, Uint32 n_symbols, short * pilot)
{
	int i;
	int rnum=0; //row num
	int cnum=0; //carrier/colum num	
	short temp = 0;

	
 	short * ptrsym = outsymbols;
 
 	short * pbyte = inbytes;

	if(N_USE_CARR > BLS)	return;
			
	for(i=0; i<MAX_FFT_LEN; i++)
		outsymbols[i] = 0;

	for(i=0; i<2*N_USE_CARR; i++)
	{
		pilot[i] = 0;	
	}
	

/*  STEP1: 拆分字节，添加导频 bytes[N_use_carr/4 * 4line] =>src[2*N_use_carr * 4line] */

//添加块状导频 ，导频间隔LP，直接存放于outsymbols中。
 	
//数据部分，对inbytes的每个字节进行拆分为4个，存放于outsymbols中。
  	
        for(rnum=0; rnum<n_symbols; rnum++) 
     	{  		
	       if( fmod(rnum,Lp)==0)                    ///          导频
            {
                    for(cnum=0; cnum<N_USE_CARR; cnum++)
	                {
   		               *(ptrsym+2*cnum) = randomseq[cnum];
  	                }
            }
  	      else
            {
                     for(i=0; i<(N_USE_CARR/4); i++)           //// 
	  	             {   			
	                		*(ptrsym+8*i) 	= (*(pbyte+i)>>6) & 0x03;
	    	                *(ptrsym+8*i+2) = (*(pbyte+i)>>4) & 0x03;
           	    	        *(ptrsym+8*i+4) = (*(pbyte+i)>>2) & 0x03;
	            	        *(ptrsym+8*i+6) = *(pbyte+i) 	  & 0x03;
               	     }
                     
                      pbyte  += (N_USE_CARR/4);
	        }
            ptrsym += 2*n_fft;
	  
       	}


/**********************************************************/
/*  STEP3: 基带系统，填充到有效子载波的范围。*/	    
//映射到复平面,共轭填充到outsymbols格式: r0 i0 r1 i1 ...	 		
	ptrsym = outsymbols;  
			
  	for(rnum=0; rnum<(n_symbols); rnum++)	//
  	{ 
		for(cnum=0; cnum<N_USE_CARR; cnum++)
		{
			temp = *(ptrsym+2*cnum) & 0x03;
			*(ptrsym+2*cnum) = 0;			
			*(ptrsym+2*(cnum+1)+2*BLS) = QPSKreal[temp];
			*(ptrsym+2*(n_fft-1-cnum)-2*BLS) = QPSKreal[temp];
			*(ptrsym+2*(cnum+1)+2*BLS+1) = QPSKimage[temp];
			*(ptrsym+2*(n_fft-1-cnum)-2*BLS+1) = - QPSKimage[temp];			
		}
		ptrsym += 2*n_fft;
	}	
 
//将调制后的导频存放在 pilot[]中。

        ptrsym = outsymbols;  
    
        for(cnum=0; cnum< 2*N_USE_CARR; cnum++)
    	{
					
	      pilot[cnum] = *(ptrsym+cnum+2*BLS+2); 
					
        }

}//end of ofdmEncode


/*************************************************************************************/
/* ofdmDecode()
*func:	unmapping/ofdmDecode
*input:	short symbos[1 random ref. + 4 row(2*N_fft) data ]
*output:short bytes[n_symbols]
*/
/**************************************************************************************/
void ofdmDecode(short *bytes, Uint32 n_symbols, short *symbols, Uint32 n_fft, short *pilot)
{
	int i;
	int lNum=0; 			// row num
	int cNum=0; 			//carrier/colum num	

	short temp = 0;
	float pha = 0;
	double re,im;
	float data = 0;

    short a;
    short b;
    short c;
    short d;


	short H[2*N_USE_CARR];

 	short * pBytes = bytes;
 	short * pData = symbols;
 	short temp_re;
 	short temp_im;
    for(i=0; i<2*N_USE_CARR; i++)
    {
         H[i]=0;
    }


        
	pData = symbols;
	

//===============ls==============================	

    for(lNum=0; lNum<n_symbols/Lp; lNum++)         //  lNum个导频符号，进行lNum次信道估计
    {
         for(i=0; i<N_USE_CARR; i++)   
         {
         
           a=pilot[2*i]/32767;
           b=pilot[2*i+1]/32767;
           c=*(pData+2*i+2*(BLS+1));
           d=*(pData+2*i+1+2*(BLS+1));


            H[2*i] = ( a*c + b*d );
            H[2*i+1] = ( b*c - a*d );
         }

         pData +=2*n_fft;            // 指针移到数据符号，即下一个符号

		 
         for (cNum=0; cNum<(Lp-1);cNum++)  //将这个导频符号算出来的H， 用于其后的LP-1个数据符号的信道估计
          {
                
                for ( i=0; i< N_USE_CARR; i++)
                {     
                 
                     a=*(pData+2*i+2*(BLS+1));
                     b=*(pData+2*i+1+2*(BLS+1));
                     c=H[2*i];
                     d=H[2*i+1];
                 	
                 	temp_re = (a*c-b*d)/3276;
					temp_im =( b*c+a*d)/3276;
                 
                     *(pData+2*i+2*(BLS+1))=temp_re;
                     *(pData+2*i+1+2*(BLS+1))=temp_im;
                 }

                pData +=2*n_fft;
               
          }        

    }    

//=================QPSK解调======================
   pData=symbols;
	for(lNum=0; lNum<(n_symbols); lNum++)
  	{ 
		for(cNum=0; cNum<N_USE_CARR; cNum++)
		{
		
			re =(double) *(pData+2*(cNum+1)+2*BLS);
			im = (double)*(pData+2*(cNum+1)+2*BLS+1);
			pha = 360 * atan2(im, re)/(PI*2);

			data = fmod((pha)+360, 360);	
			temp = fmod(floor(data/90), 4);

			*(pData+2*cNum) = temp;
			*(pData+2*(cNum+1)+2*BLS) = 0;
			*(pData+2*(cNum+1)+2*BLS+1) = 0;
	
		}

		pData +=2*n_fft; 
		
	}

//=================组合字节==========================
  
  	pData = symbols;
	for(lNum=0;lNum<n_symbols;lNum++)
	{
       if( fmod(lNum,Lp ) != 0)
		{
		    for(i=0;i<N_USE_CARR/4;i++)
	       {
			*(pBytes+i)= ((*(pData+8*i)<<6) & 0xc0) | ((*(pData+8*i+2)<<4) & 0x30) | ((*(pData+8*i+4)<<2) & 0x0c) | ((*(pData+8*i+6)) & 0x03); 
		    }
		  pBytes += N_USE_CARR/4;
        }
          pData += 2*n_fft;

	}

}//end of ofdmDecode



