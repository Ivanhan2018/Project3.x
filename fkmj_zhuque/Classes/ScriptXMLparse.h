#ifndef __XMLParse_H__
#define __XMLParse_H__

#include "platform/CCSAXParser.h"

#include <map>
#include <vector>

NS_CC_BEGIN;

class ScriptXMLparse : public cocos2d::SAXDelegator
{
public:

	ScriptXMLparse( std::string xmlFileName);
	~ScriptXMLparse()
	{
	}
	virtual void startElement(void *ctx, const char *name, const char **atts);
	virtual void endElement(void *ctx, const char *name);
	virtual void textHandler(void *ctx, const char *s, int len);
};
NS_CC_END;
#endif // __XMLParse_H__