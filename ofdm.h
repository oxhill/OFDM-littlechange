#ifndef __OFDM_H__
#define __OFDM_H__

#include <math.h>
#include "config.h"


#define PI  (3.141592654)

void ofdmEncode(short *outsymbols, Uint32 n_fft, short *inbytes, Uint32 n_symbols, short *pilot);

void ofdmDecode(short *outbytes, Uint32 n_symbols, short *insymbols, Uint32 n_fft,short *pilot);


#endif
