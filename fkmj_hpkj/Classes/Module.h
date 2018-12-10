#ifndef MODULE_HEAD_FILE
#define MODULE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//版本比较

//产品版本
#define BULID_VER					0									//授权版本
#define PRODUCT_VER					6									//产品版本

//接口版本
#define INTERFACE_VERSION(cbMainVer,cbSubVer)							\
	(DWORD)(														\
	(((BYTE)(PRODUCT_VER))<<24)+									\
	(((BYTE)(cbMainVer))<<16)+										\
	((BYTE)(cbSubVer)<<8))+											\
	((BYTE)(BULID_VER))

//模块版本
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)					\
		(dword)(														\
		(((byte)(PRODUCT_VER))<<24)+									\
		(((byte)(cbMainVer))<<16)+										\
		((byte)(cbSubVer)<<8)+											\
		(byte)(cbBuildVer))

//产品版本
inline byte GetProductVer(dword dwVersion)
{
	return ((byte *)&dwVersion)[3];
}

//主要版本
inline byte GetMainVer(dword dwVersion)
{
	return ((byte *)&dwVersion)[2];
}

//次要版本
inline byte GetSubVer(dword dwVersion)
{
	return ((byte *)&dwVersion)[1];
}

//编译版本
inline byte GetBuildVer(dword dwVersion)
{
	return ((byte *)&dwVersion)[0];
}

//版本比较
inline bool InterfaceVersionCompare(dword dwQueryVer, dword dwInterfaceVer)
{
	if (GetSubVer(dwQueryVer)>GetSubVer(dwInterfaceVer)) return false;
	if (GetMainVer(dwQueryVer)!=GetMainVer(dwInterfaceVer)) return false;
	if (GetBuildVer(dwQueryVer)!=GetBuildVer(dwInterfaceVer)) return false;
	if (GetProductVer(dwQueryVer)!=GetProductVer(dwInterfaceVer)) return false;
	return true;
};

//////////////////////////////////////////////////////////////////////////////////

#endif