#include "json.h"
#ifndef GLOBAL_VALUE
#define GLOBAL_VALUE "GlobalValue.json"
#endif

class JsonHelper
{
public:
	static Json::Value getJsonFromFile(const char *filename);
	static Json::Value getJsonFromData(const char *data, int size);
};