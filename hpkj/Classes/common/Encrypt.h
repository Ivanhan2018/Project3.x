#ifndef ENCRYPT_HEAD_FILE
#define ENCRYPT_HEAD_FILE
#pragma once
#include "Define.h"


//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#else
#endif
//MD5 加密类
class CMD5Encrypt
{
	//函数定义
private:
	//构造函数
	CMD5Encrypt() {}

	//功能函数
public:
	//生成密文
	static void EncryptData(unsigned char * pszSrcData, char szMD5Result[33]);

	//字符串MD5加密
	static std::string MD5Encrypt(const std::string &str);
};

//////////////////////////////////////////////////////////////////////////

//异或加密类
class CXOREncrypt
{
	//函数定义
private:
	//构造函数
	CXOREncrypt() {}

	//功能函数
public:
	//生成密文
	static WORD EncryptData(unsigned char * pszSrcData, unsigned char* pszEncrypData, WORD wSize);
	//解开密文
	static WORD CrevasseData(unsigned char * pszEncrypData, unsigned char* pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////

#endif