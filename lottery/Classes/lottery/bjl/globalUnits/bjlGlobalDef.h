#ifndef _BJLGLOBALDEF_H_
#define _BJLGLOBALDEF_H_

// add by hxh 20160729
#ifdef WIN32
#define STRCMP(x,y)	_stricmp(x,y)
#define ATOI64(x) _atoi64(x)
#define SNPRINTF(charArray,format,...) _snprintf(charArray,sizeof(charArray)-1,format,__VA_ARGS__)
#else
#define STRCMP(x,y)	strcasecmp(x,y)
#define ATOI64(x) atoll(x)
#define SNPRINTF(charArray,format,...) snprintf(charArray,sizeof(charArray),format,##__VA_ARGS__)
#endif

#endif //_BJLGLOBALDEF_H_