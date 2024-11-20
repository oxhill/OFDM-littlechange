/*
计算误码率的函数
2010-4-13
Author: Sayi Cheng
*/
#include "config.h"

int testerror(short *x, short *y, short n)
{
	short i,j;
	int errcount=0;	
	short temp;

	for (i=0; i<n; i++)
    {
    	if(y[i] != x[i])
    	{
    		temp = y[i]^x[i];
    		
    		for(j=0; j<8; j++) 		
    		{
        		errcount += (temp & 0x01);
        		temp = temp>>1;
        	}
        }
    }

   return(errcount);
}
