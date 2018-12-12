/*
** implement functions for io
** author: zhouwei
** date: 2012.10.25
*/

#ifndef __ANS_IO_H__
#define __ANS_IO_H__

#include <string>
using namespace std;

namespace ans{
namespace AnsIO{
	// -------------------------------------------------------------------
	// 创建文件夹或文件
	// 如果 path 为：aa/bb/cc/ 则只创建路径，如果 path 为：aa/bb/cc 则创建文件 cc
	// 如果创建成功或路径/文件已经存在则返回 true
	// -------------------------------------------------------------------
	bool mkpath(const char* path, unsigned mode=0777);
	inline bool mkpath(const string& path, unsigned mode=0777){ return mkpath(path.c_str(), mode); }
}}

#endif