/*
** overload new operator to trace the new and delete operation
** author: zhouwei
** date: 2012.10.22
*/

#ifndef __ANS_NEW_H__
#define __ANS_NEW_H__

#include <new>
#include <fstream>
#include "AnsDebug.h"

#if defined(_WIN32) && defined(__ANS_DEBUG)
namespace ans{
namespace debug{
// -----------------------------------------------------------------------
// inners
// -----------------------------------------------------------------------
void addNewTrack(void*, const char*, unsigned);
void removeNewTrack(void*);

// -------------------------------------------------------------
// 输出没被删除的 new 操作，undbl 表示不输出重复的
void dumpUnfree(std::ofstream&, bool undbl=true);

// 以字符串形式返回没删除 new 操作，undbl 表示不录入重复的
std::string getUnfreeInfo(bool undbl=true);

}}

// -----------------------------------------------------------------------
// overload new/delete and new[]/delete[] operator
// -----------------------------------------------------------------------
inline void* operator new(unsigned size, const char* file, unsigned line)
{
	void* p = (void*)malloc(size);
	ans::debug::addNewTrack(p, file, line);
	return p;
}

inline void operator delete(void* p, const char* file, unsigned line){}
inline void operator delete(void* p)
{
	if(p)
	{
		ans::debug::removeNewTrack(p);
		free(p);
	}
}

inline void* operator new[](unsigned size, const char* file, unsigned line)
{
	void* p = (void*)malloc(size);
	ans::debug::addNewTrack(p, file, line);
	return p;
}

inline void operator delete[](void* p, const char* file, unsigned line){}
inline void operator delete[](void* p)
{
	if(p)
	{
		ans::debug::removeNewTrack(p);
		free(p);
	}
}


#endif // __ANS_DEBUG
#endif // __ANS_NEW_H__