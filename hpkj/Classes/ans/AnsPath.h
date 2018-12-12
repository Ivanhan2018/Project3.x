/*
** implement functions for path
** author: zhouwei
** date: 2012.10.15
*/

#ifndef __ANS_PATH_H__
#define __ANS_PATH_H__

#include <string>

using namespace std;

namespace ans{
namespace AnsPath{

	extern const char* PATH_SPLITTER;		// 路径分隔符
	extern const char* PATH_ANTI_SPLITTER;	// 兼容路径分隔符

	// -------------------------------------------------------------------
	// 拆分文件路径和文件名
	// 参数：
	//     path    : 输出路径
	//     pathSize: 输出路径缓冲大小
	//     name    : 输出文件名
	//     nameSize: 输出文件名缓冲大小
	//     filePath: 输入文件名及路径
	// 如：aa\bb\cc\dd\ee.txt 调用 split 后
	//     path == aa\bb\cc\dd 
	//     name == ee.txt
	// -------------------------------------------------------------------
	void split(char* path, size_t pathSize, char* name, size_t nameSize, const char* filePath);
	void split(string& path, string& name, const char* filePath);
	void split(string& path, string& name, const string& filePath);

	// -------------------------------------------------------------------
	// 标准化一个路径
	// 如在 Windows 平台有路径：
	//     c:\abc\def\ghi/jkl\mno\..\\pqr///stu\vwx\ 标准化后将变为：c:\abc\def\ghi\jkl\pqr\stu\vwx\
	//     ../aa/bb/cc//dd.txt 标准化后为：../aa/bb/cc/dd.txt 
	// -------------------------------------------------------------------
	string normalizePath(const string& path);
	string& normalizePath(string& path);

}
}

#endif