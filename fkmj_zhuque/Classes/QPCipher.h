//////////////////////////////////////////////////////////////////////////
// 加密解密类
//////////////////////////////////////////////////////////////////////////
typedef unsigned char byte;

/** 验证包大小 **/
static const int SIZE_VALIDATE = 136;

class QPCipher
{
public:
	static byte* encryptBuffer(byte* data, int dataSize);
	static byte* decryptBuffer(byte* data, int start, int dataSize);

	static int   getCipherMode();

	static int   getMainCommand(byte* data, int start);
	static int   getSubConmmand(byte* data, int start);
	static int   getPackSize(byte* data, int start);
	static void  setPackInfo(byte* data, int dataSize , int main, int sub);

	static int   getCipher(byte* data, int start);
	static int   getCipherCode(byte* data, int start);

	static byte* tcpValidate(byte* data, int start);
};
