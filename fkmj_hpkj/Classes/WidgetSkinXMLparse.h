#ifndef __WidgetSkinXMLparse_H__
#define __WidgetSkinXMLparse_H__

#include "platform/CCSAXParser.h"
#include "WidgetDefine.h"
#include "QYActionDefine.h"

#include <map>
#include <vector>

class WidgetSkinXMLparse : public cocos2d::SAXDelegator
{
public:

	WidgetSkinXMLparse(std::string xmlFileName);
	~WidgetSkinXMLparse(){}

	void parseAction(std::string& elementName,std::map<std::string, std::string>& attributeDict);
	void parseSkin(std::string& elementName,std::map<std::string, std::string>& attributeDict);
	virtual void startElement(void *ctx, const char *name, const char **atts);
	virtual void endElement(void *ctx, const char *name);
	virtual void textHandler(void *ctx, const char *s, int len);
private:
	std::vector<QYActionInfo*> m_pActionInfos;
	std::vector<WidgetInfo*> m_pWidgetInfos;
};
#endif // __XMLParse_H__