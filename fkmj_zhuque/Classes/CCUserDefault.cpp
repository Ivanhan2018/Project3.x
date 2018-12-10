#include "cocos2d.h"
#include "CCUserDefault.h"

USING_NS_CC;
using namespace std;
//////////////////////////////////////////////////////////////////////////
MyUserDefault::MyUserDefault()
{
}

MyUserDefault::~MyUserDefault()
{
}

bool MyUserDefault::getBoolForKey(const char* section, const char* pKey)
{
	return getBoolForKey(section, pKey, false);
}

bool MyUserDefault::getBoolForKey(const char* section, const char* pKey, bool defaultValue)
{
	string key = string(section)+"_"+pKey;
	return CCUserDefault::sharedUserDefault()->getBoolForKey(key.c_str(), defaultValue);
}

int MyUserDefault::getIntegerForKey(const char* section, const char* pKey)
{
    return getIntegerForKey(section, pKey, 0);
}

int MyUserDefault::getIntegerForKey(const char* section, const char* pKey, int defaultValue)
{
	string key = string(section)+"_"+pKey;
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str(), defaultValue);
}

float MyUserDefault::getFloatForKey(const char* section, const char* pKey)
{
    return getFloatForKey(section, pKey, 0.0f);
}

float MyUserDefault::getFloatForKey(const char* section, const char* pKey, float defaultValue)
{
	string key = string(section)+"_"+pKey;
	char buf[128]={0};
	sprintf(buf, "%f", defaultValue);

	std::string szValue = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), buf);
	float fValue = defaultValue;
	sscanf(szValue.c_str(), "%f", &fValue);
	return fValue;
}

double  MyUserDefault::getDoubleForKey(const char* section, const char* pKey)
{
    return getDoubleForKey(section, pKey, 0.0f);
}

double MyUserDefault::getDoubleForKey(const char* section, const char* pKey, double defaultValue)
{	
	string key = string(section)+"_"+pKey;
	char buf[128]={0};
	sprintf(buf, "%f", defaultValue);

	std::string szValue = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), buf);
	float fValue = defaultValue;
	sscanf(szValue.c_str(), "%f", &fValue);
	return (double)fValue;
}

std::string MyUserDefault::getStringForKey(const char* section, const char* pKey)
{
    return getStringForKey(section, pKey, "");
}

string MyUserDefault::getStringForKey(const char* section, const char* pKey, const std::string & defaultValue)
{
	string key = string(section)+"_"+pKey;
	return CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), defaultValue);
}

void MyUserDefault::setBoolForKey(const char* section, const char* pKey, bool value)
{
	setIntegerForKey(section, pKey, value ? 1 : 0);
}

void MyUserDefault::setIntegerForKey(const char* section, const char* pKey, int value)
{
	char buf[128]={0};
	sprintf(buf, "%d", value);
	setValueForKey(section, pKey, buf);
}

void MyUserDefault::setFloatForKey(const char* section, const char* pKey, float value)
{
	char buf[128]={0};
	sprintf(buf, "%f", value);
	setValueForKey(section, pKey, buf);
}

void MyUserDefault::setDoubleForKey(const char* section, const char* pKey, double value)
{
	char buf[128]={0};
	sprintf(buf, "%f", value);
	setValueForKey(section, pKey, buf);
}

void MyUserDefault::setStringForKey(const char* section, const char* pKey, const std::string & value)
{
	setValueForKey(section, pKey, value.c_str());
}

void MyUserDefault::setValueForKey(const char* section, const char* pKey, const char* value)
{
	string key = string(section)+"_"+pKey;
	CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), value);
	CCUserDefault::sharedUserDefault()->flush();
}

MyUserDefault* MyUserDefault::getInstance()
{
	static MyUserDefault sInstance;
    return &sInstance;
}

void MyUserDefault::destroyInstance()
{
}
