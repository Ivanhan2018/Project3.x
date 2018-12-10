#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

namespace GameLog
{
	template<typename T>
	std::string toString (T p)
	{
		std::ostringstream stream;
		stream << p;
		return (stream.str());
	}

	template<typename T1,  typename T2>
	inline std::string toString (T1 p1, T2 p2)
	{
		std::ostringstream stream;
		stream << p1 << p2;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3>
	inline std::string toString (T1 p1, T2 p2, T3 p3)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3;
		return stream.str();
	}


	inline void Log(std::string kStr){}
	inline void Assert(bool bAssert,std::string kStr){}
};