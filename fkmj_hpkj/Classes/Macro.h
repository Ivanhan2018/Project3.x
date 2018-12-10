#ifndef MACRO_HEAD_FILE
#define MACRO_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_BYTE				((byte)(0xFF))						//无效数值
#define INVALID_WORD				((word)(0xFFFF))					//无效数值
#define INVALID_DWORD				((dword)(0xFFFFFFFF))				//无效数值

#ifndef INADDR_NONE
#define INADDR_NONE					0xffffffff
#endif // !INADDR_NONE

//////////////////////////////////////////////////////////////////////////////////

//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//////////////////////////////////////////////////////////////////////////////////

////存储长度
//#ifdef _UNICODE
//	#define CountStringBuffer CountStringBufferW
//#else
//	#define CountStringBuffer CountStringBufferA
//#endif
//
////存储长度
//#define CountStringBufferA(String) ((uint)((strlen(String)+1)*sizeof(char)))
//#define CountStringBufferW(String) ((uint)((wcslen(String)+1)*sizeof(wchar)))

//////////////////////////////////////////////////////////////////////////////////

//接口释放
#define SafeRelease(pObject) { if (pObject!=0) { pObject->Release(); pObject=0; } }

//删除指针
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=0; } 

////关闭句柄
//#define SafeCloseHandle(hHandle) { if (hHandle!=0) { CloseHandle(hHandle); hHandle=0; } }

//删除数组
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=0; } 

//////////////////////////////////////////////////////////////////////////////////

#endif