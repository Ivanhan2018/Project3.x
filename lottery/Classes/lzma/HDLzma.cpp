#include "HDLzma.h"
#include "Lzmalib.h"
#include "Alloc.h"

unsigned int HDLzma::CompressBuffer(const char *inBuffer, size_t inSize,unsigned char *outBuffer,size_t outSize, size_t *outSizeProcessed)
{
	int level = 9; /* 0 <= level <= 9, default = 5 */
	unsigned dictSize = 1 << 24; /* use (1 << N) or (3 << N). 4 KB < dictSize <= 128 MB */
	int lc = 3; /* 0 <= lc <= 8, default = 3  */
	int lp = 0; /* 0 <= lp <= 4, default = 0  */
	int pb = 2; /* 0 <= pb <= 4, default = 2  */
	int fb = 32;  /* 5 <= fb <= 273, default = 32 */
	int numThreads = 1; /* 1 or 2, default = 2 */

	//
	unsigned char cbBuffer[5] = {93, 0, 0, 0, 1}; //д	
	size_t propSize = 5;	

	unsigned int result = LzmaCompress(outBuffer,&outSize,(unsigned char*)inBuffer,inSize,cbBuffer,&propSize,level,dictSize,lc,lp,pb,fb,numThreads);

	*outSizeProcessed = outSize;

	return result;
}

unsigned int HDLzma::UncompressBuffer(const char *inBuffer, size_t *inSize,unsigned char *outBuffer,size_t *outSize, size_t *outSizeProcessed)
{
	unsigned char cbBuffer[5] = {93, 0, 0, 0, 1}; //д	
	size_t propSize = 5;	

	unsigned int result = LzmaUncompress(outBuffer,outSize,(unsigned char*)inBuffer,inSize,cbBuffer,propSize);

	*outSizeProcessed = *outSize;
	
	return result;
}