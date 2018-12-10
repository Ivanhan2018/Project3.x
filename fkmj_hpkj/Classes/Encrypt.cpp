#include <string.h>
#include "Encrypt.h"

namespace o_o
{
const char* DEFAULT_KEY = "PASsword";

// 解密
bool Decrypt(unsigned char* buff, unsigned int size, const char* key)
{
	if (key == NULL || key[0] == 0)
	{
		key = DEFAULT_KEY;
	}

	size_t len = strlen(key);

	for (size_t i=0;i<size;i++)
	{

		switch (i%7)
		{
		case 1: buff[i] = (key[(i+0)%len])-buff[i]; break;
		case 2: buff[i] = (key[(i+1)%len])-buff[i]; break;
		case 3: buff[i] = (key[(i+2)%len])+buff[i]; break;
		case 4: buff[i] = (key[(i+3)%len])-buff[i]; break;
		case 5: buff[i] = (key[(i+4)%len])+buff[i]; break;
		}
	}

	unsigned char c = buff[0];
	for (unsigned int i=0;i<size-1;i++)
	{
		buff[i] = buff[i + 1];
	}

	buff[size-1] = c;
	return true;
}

// 加密
bool Encrypt(unsigned char* buff, unsigned int size, const char* key)
{
	if (key == NULL || key[0] == 0)
	{
		key = DEFAULT_KEY;
	}

	size_t len = strlen(key);

	unsigned char c = buff[size-1];
	for (size_t i=size-1; i >= 1; --i)
	{
		buff[i] = buff[i - 1];
	}

	buff[0] = c;

	for (unsigned int i=0;i<size;i++)
	{
		switch (i%7)
		{
		case 1: buff[i] = (key[(i+0)%len])-buff[i]; break;
		case 2: buff[i] = (key[(i+1)%len])-buff[i]; break;
		case 3: buff[i] = buff[i] - (key[(i+2)%len]); break;
		case 4: buff[i] = (key[(i+3)%len])-buff[i]; break;
		case 5: buff[i] = buff[i] - (key[(i+4)%len]); break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
const int gKeys1[]={
	1,3,5,2,3,4,5,0,8,9,6,7,8,9,6,1,8,9,7,4,3,7,8,5,
	2,3,5,8,9,6,7,8,9,6,1,2,3,1,4,5,4,5,6,8,9,7,4,3,
	7,8,9,3,7,5,6,7,5,7,6,9,1,0,1,0,4,5,7,6,6,7,8,9,
	5,6,7,8,9,7,8,3,6,1,2,3,1,4,5,4,5,6,8,9,5,8,9,6,
	6,1,2,3,1,4,5,4,5,6,8,9,7,4,3,7,8,9,3,7,5,6,7,5,
	2,3,1,4,5,4,5,6,8,9,7,4,3,7,8,9,3,7,5,6,7,5,7,6
};

const int gKeys2[]={
	6,7,5,7,6,4,1,0,1,0,4,5,7,6,6,7,8,9,5,6,7,8,9,7,
	8,3,6,1,2,3,1,4,5,4,5,6,8,9,5,8,9,6,6,1,2,3,1,4,
	5,4,5,6,1,3,5,2,3,4,5,0,8,9,6,7,8,9,6,1,8,9,7,4,
	3,7,8,5,2,3,5,8,9,6,7,8,9,6,1,2,3,1,4,5,4,5,6,8,
	9,7,4,3,7,8,9,3,7,5,8,9,7,4,3,7,8,9,3,7,5,6,7,5,
	2,3,1,4,5,4,5,6,8,9,7,4,3,7,8,9,3,7,5,6,7,5,7,6
};

const int gKeySize1 = sizeof(gKeys1) / sizeof(int);
const int gKeySize2 = sizeof(gKeys2) / sizeof(int);

// 简单加密
bool EncryptA(unsigned char* buf, unsigned int size, int offset)
{
	if (buf == 0 || size == 0)
	{
		return false;
	}

	int keySize		= offset != 0 ? gKeySize2 : gKeySize1;
	const int* keys = offset != 0 ? gKeys2 : gKeys1;
	
	for (unsigned int i = 0; i < size; ++i)
	{
		unsigned char ch = buf[i];

		if (ch >= '0' && ch <= '9')
		{
			ch = '9' - ch + '0';
		}
		else if (ch >= 'A' && ch <= 'Z')
		{
			ch = 'Z' - ch + 'A';
		}

		ch += keys[i%keySize];
		buf[i] = ch;
	}

	return true;
}

// 简单解密
bool DecryptA(unsigned char* buf, unsigned int size, int offset)
{
	if (buf == 0 || size == 0)
	{
		return false;
	}

	int keySize		= offset != 0 ? gKeySize2 : gKeySize1;
	const int* keys = offset != 0 ? gKeys2 : gKeys1;

	for (unsigned int i = 0; i < size; ++i)
	{
		unsigned char ch = buf[i];
		ch -= keys[i%keySize];

		if (ch >= '0' && ch <= '9')
		{
			ch = '9' - ch + '0';
		}
		else if (ch >= 'A' && ch <= 'Z')
		{
			ch = 'Z' - ch + 'A';
		}

		buf[i] = ch;
	}

	return true;
}

}