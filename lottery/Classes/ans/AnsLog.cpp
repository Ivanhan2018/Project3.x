/*
** this module implement debug log printer
** author: zhouwei
** date: 2012.09.20
*/

#include <stdio.h>
#include <cassert>
#include "AnsMacros.h"
#include "AnsThread.h"
#include "AnsString.h"
#include "AnsLog.h"

using namespace std;

namespace ans{
namespace debug{
// -----------------------------------------------------------------------
// inners
// -----------------------------------------------------------------------
static void formatTime(string &strBuff)
{
	time_t now;
	time(&now);
	struct tm tmNow;
#if (_MSC_VER) && (_MSC_VER >= 1200)
	localtime_s(&tmNow, &now);	
#else
	struct tm *pTmNow = localtime(&now);	
	memcpy(&tmNow, pTmNow, sizeof(tmNow));
#endif	
	char buff[32];
	strftime(buff, sizeof(buff), "%Y/%m/%d - %H:%M:%S", &tmNow);
	strBuff.append(buff);

	ANS_SNPRINTF(buff, sizeof(buff), "::%d ", clock());
	strBuff.append(buff);
}
	
static void formatThreadID(string &strBuff)
{
	char buff[32];
	ANS_SNPRINTF(buff, sizeof(buff), "(thread: %d)\n", sp_thread_self());
	strBuff.append(buff);
}

static void formatSource(string &strBuff, const char *file, unsigned lineNo)
{
	strBuff.append(file);
	char buff[16];
	ANS_SNPRINTF(buff, sizeof(buff), "(%d)\n", lineNo);
	strBuff.append(buff);
}

static void formatMessage(string &strBuff, const char *prefix, const char *msg, va_list args)
{
	strBuff.append(prefix);
	size_t msgLen = strlen(msg) + 100;
	char *buff = new char[msgLen];
	vsnprintf(buff, msgLen, msg, args);
	strBuff.append(buff);
	strBuff.append("\n");
	delete[] buff;
}


// -----------------------------------------------------------------------
// private
// -----------------------------------------------------------------------
bool AnsLog::print(string &strMsg, bool save)
{
	assert(!m_strPth.empty());
	FILE *file = m_pFile ? m_pFile : fopen(m_strPth.c_str(), "a+"); 
	assert(file);

	fseek(file, 0, SEEK_END);
	fwrite(strMsg.c_str(), strMsg.length(), 1, file);
	if(save)
	{
		fwrite("\n", 1, 1, file);
		fclose(file);
		m_pFile = NULL;
	}
	else
	{
		m_pFile = file;
	}
	return true;
}


// -----------------------------------------------------------------------
// protected
// -----------------------------------------------------------------------
AnsLog::AnsLog()
	: m_pFile(NULL)
{
}

AnsLog::AnsLog(const char *pszPath)
	: m_pFile(NULL)
{
	setPath(pszPath);
}


void AnsLog::setPath(const char *pszPath)
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	FILE *file = fopen(pszPath, "w");
	if(file) 
	{
		fclose(file);
		m_strPth = pszPath;
	}
	else 
	{
		assert(false);
	}	
}

// ---------------------------------------------------
string AnsLog::formatOut(const char *prefix, const char *src, 
	unsigned int lineNo, const char *msg, va_list args)
{
	string strMsg;
	formatTime(strMsg);
	formatThreadID(strMsg);
	formatSource(strMsg, src, lineNo);
	formatMessage(strMsg, prefix, msg, args);
	print(strMsg);
	return strMsg;
}


bool AnsLog::printOut(const char *prefix, const char *src, 
	unsigned int lineNo, const char *msg, va_list args)
{
	string strMsg;
	formatTime(strMsg);
	formatThreadID(strMsg);
	formatSource(strMsg, src, lineNo);
	formatMessage(strMsg, prefix, msg, args);
	return print(strMsg);
}

void AnsLog::messageOut(const char *pszMsg, va_list args, bool save)
{
	char *buff = new char[strlen(pszMsg) + 100];
	vsnprintf(buff, sizeof(buff), pszMsg, args);
	string strMsg = buff;
	print(strMsg, save);
	delete[] buff;
}


// ---------------------------------------------------
AnsLog::TIME_TEST_INFO AnsLog::timeTestBegin(const char *prefix, const char *src, 
	unsigned int lineNo, const char *msg, va_list args)
{
	string strMsg("time test begin: ");
	char splitter[59];
	ans::AnsCString::makeCharLine(splitter, '-', sizeof(splitter));
	strMsg.append(splitter);
	strMsg.append("\n");
	formatTime(strMsg);
	formatThreadID(strMsg);
	formatSource(strMsg, src, lineNo);

	formatMessage(strMsg, prefix, msg, args);	

	print(strMsg, false);
	return clock();
}

void AnsLog::timeTestEnd(AnsLog::TIME_TEST_INFO start)
{
	AnsLog::TIME_TEST_INFO end = clock();
	char buff[66];
	ANS_SNPRINTF(buff, sizeof(buff), "waste time: %fs\n", 
		((double)(clock() - start)) / CLOCKS_PER_SEC);
	string strMsg(buff);

	strMsg.append("test end. ");
	ans::AnsCString::makeCharLine(buff, '-', sizeof(buff));
	strMsg.append(buff);
	strMsg.append("\n");

	print(strMsg);
}

}}