/*
** implement functions for path
** author: zhouwei
** date: 2012.10.15
*/

#include <cassert>
#include <vector>
#include "AnsPath.h"
#include "AnsString.h"

namespace ans{
namespace AnsPath{
#if defined(WIN32)
const char* PATH_SPLITTER = "\\";				// 路径分隔符
const char* PATH_ANTI_SPLITTER = "/";			// 兼容路径分隔符
static const char* PATH_DSPLITTER = "\\\\";		// 双路径分隔符（等于单个）
#else
const char* PATH_SPLITTER = "/";
const char* PATH_ANTI_SPLITTER = "\\";
static const char* PATH_DSPLITTER = "//";		// 双路径分隔符（等于单个）
#endif
}		// AnsPath

// -----------------------------------------------------------------------
void AnsPath::split(char* path, size_t pathSize, char* name, size_t nameSize, const char* filePath)
{
	assert(pathSize > 1 && nameSize > 1);
	size_t len = strlen(filePath);
	if (len == 0){ *path = 0; *name = 0; return; }
	const char* p = filePath + (--len);
	while(1)
	{
		if(*p == *PATH_SPLITTER || 
			*p == *PATH_ANTI_SPLITTER)			// 路径分隔符都是单字符
		{ ++p; break; }
		if(p == filePath) { break; }
		 --p; --len; 
	}
	strncpy(name, p, nameSize);
	name[nameSize-1] = 0;
	len = len < pathSize ? len : pathSize;
	strncpy(path, filePath, len);
	path[len] = 0;	
}

inline void AnsPath::split(string& path, string& name, const char* filePath)
{
	AnsPath::split(path, name, string(filePath));
}

void AnsPath::split(string& path, string& name, const string& filePath)
{
	if(filePath.empty()) return;
	string::const_iterator spIter = filePath.end() -1;
	string::const_iterator iter = filePath.end() - 1;
	while(1)
	{
		if(*iter == *PATH_SPLITTER ||
			*iter == *PATH_ANTI_SPLITTER)		// 路径分隔符都是单字符
		{ ++spIter; break; }
		if(iter == filePath.begin()) { break; }
		--iter; --spIter;
	}
	path.assign(filePath.begin(), iter);
	name.assign(spIter, filePath.end());
}

// -----------------------------------------------------------------------
string AnsPath::normalizePath(const string& path)
{
	string s = AnsString::replace(path, PATH_ANTI_SPLITTER, PATH_SPLITTER);	// 替换兼容分隔符
	unsigned len = s.length();
	while(true)
	{
		s = AnsString::replace(s, PATH_DSPLITTER, PATH_SPLITTER);			// 双分隔符换成单分隔符
		if(len == s.length()) break;
		len = s.length();
	}

	vector<string> folders, dirs;
	AnsString::split(dirs, s, PATH_SPLITTER);
	bool isPath = dirs.size() && dirs.back().empty();						// 以“/”或“\”结尾
	vector<string>::iterator iter = dirs.begin();
	while(iter != dirs.end())
	{
		if(*iter == "..")
		{
			if(folders.size())
				folders.pop_back();
			else
				folders.push_back(*iter);
		}
		else if(!iter->empty())
		{
			folders.push_back(*iter);
		}
		++iter;
	}
	if(isPath) { folders.push_back(""); }
	return AnsString::join(PATH_SPLITTER, folders.begin(), folders.end());
}

string& AnsPath::normalizePath(string& path)
{
	path = AnsPath::normalizePath((const string&)path);
	return path;
}


}