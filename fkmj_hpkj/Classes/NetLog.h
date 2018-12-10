#pragma once
#include <string>
#include "UtilityBase.h"

#define NetAssert(bAssert,kStr) CCAssert(bAssert,kStr);

namespace NetLog
{
	void Log(std::string kStr); 
}