#ifndef _Encript_H_
#define _Encript_H_

#ifdef  __cplusplus
extern "C" {
#endif

namespace o_o
{
// 解密
bool Decrypt(unsigned char* buff, unsigned int size, const char* key="");
// 加密
bool Encrypt(unsigned char* buff, unsigned int size, const char* key="");

// 简单加密
bool EncryptA(unsigned char* buf, unsigned int size, int offset = 0);
// 简单解密
bool DecryptA(unsigned char* buf, unsigned int size, int offset = 0);

};

#ifdef  __cplusplus
}
#endif

#endif // _Encript_H_