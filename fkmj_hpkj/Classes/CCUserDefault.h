#ifndef _CCUserDefault_H_
#define _CCUserDefault_H_

#include <string>

class MyUserDefault
{
public:
	bool		getBoolForKey(const char* section, const char* pKey);
	bool		getBoolForKey(const char* section, const char* pKey, bool defaultValue);
	int			getIntegerForKey(const char* section, const char* pKey);
	int			getIntegerForKey(const char* section, const char* pKey, int defaultValue);
	float		getFloatForKey(const char* section, const char* pKey);
	float		getFloatForKey(const char* section, const char* pKey, float defaultValue);
	double		getDoubleForKey(const char* section, const char* pKey);
	double		getDoubleForKey(const char* section, const char* pKey, double defaultValue);
	std::string	getStringForKey(const char* section, const char* pKey);
	std::string	getStringForKey(const char* section, const char* pKey, const std::string & defaultValue);
	
	void		setBoolForKey(const char* section, const char* pKey, bool value);
	void		setIntegerForKey(const char* section, const char* pKey, int value);
	void		setFloatForKey(const char* section, const char* pKey, float value);
	void		setDoubleForKey(const char* section, const char* pKey, double value);
	void		setStringForKey(const char* section, const char* pKey, const std::string & value);
	
	void		setValueForKey(const char* section, const char* pKey, const char* value);
	static MyUserDefault*	getInstance();
	static void destroyInstance();
	
private:
	MyUserDefault();
	~MyUserDefault();
};

#endif // _CCUserDefault_H_
