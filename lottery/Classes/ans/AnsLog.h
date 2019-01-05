/*
** this module implement debug log printer
** author: zhouwei
** date: 2012.09.20
*/

#ifndef __ANS_LOG_H__
#define __ANS_LOG_H__

#include <stdarg.h>
#include <ctime>
#include <string>

using namespace std;

namespace ans{
namespace debug{

// -----------------------------------------------------------------------
// implement AnsLog class
// -----------------------------------------------------------------------
class AnsLog
{
public:
	typedef clock_t TIME_TEST_INFO;

protected:
	AnsLog();
	AnsLog(const char*);	
	void setPath(const char*);

	// -------------------------------------------------------------------
	// 格式化并输出带日期、所在文件、所属线程等 log：
	// 参数：1、提示前缀；2、所在文件；3、所在行号；4、输出文本；5、格式化参数列表
	// -------------------------------------------------------------------
	bool printOut(const char*, const char*, unsigned int, const char*, va_list);
	string formatOut(const char*, const char*, unsigned int, const char*, va_list);

	// -------------------------------------------------------------------
	// 输出纯粹文本：
	// 参数：1、输出文本；2、格式化参数；3、是否马上写出外存
	// -------------------------------------------------------------------
	void messageOut(const char*, va_list, bool=true);

	// -------------------------------------------------------------------
	// 用时测试：
	// 参数：1、提示前缀；2、所在文件；3、所在行号；4、输出文本；5、格式化参数列表
	// 将 timeTestBegin 的返回值作为参数传给 timeTestEnd 完成一个测试单元
	// -------------------------------------------------------------------
	TIME_TEST_INFO timeTestBegin(const char*, const char*, unsigned int, const char*, va_list);
	void timeTestEnd(TIME_TEST_INFO);

private:
	bool print(string&, bool=true);

private:
	string m_strPth;
	FILE *m_pFile;
};


}}
#endif