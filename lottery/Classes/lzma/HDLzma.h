#ifndef _HDLZMA_H_
#define _HDLZMA_H_

#include "cocos2d.h"

USING_NS_CC;

class HDLzma
{
public:

	HDLzma(){}
	~HDLzma(){}

	static unsigned int CompressBuffer(const char *inBuffer, size_t inSize,unsigned char *outBuffer,size_t outSize, size_t *outSizeProcessed);
	static unsigned int UncompressBuffer(const char *inBuffer, size_t *inSize,unsigned char *outBuffer, size_t *outSize, size_t *outSizeProcessed);

};

#endif //_HDLZMA_H_