#ifndef __WidgetScenceXMLparse_H__
#define __WidgetScenceXMLparse_H__

#include "platform/CCSAXParser.h"
#include "WidgetDefine.h"

#include <map>
#include <vector>

class ScenceBase;


class WidgetScenceXMLparse : public cocos2d::SAXDelegator
{
public:

	WidgetScenceXMLparse(std::string xmlFileName,cocos2d::Node* pNode);
	~WidgetScenceXMLparse(){}

	virtual void startElement(void *ctx, const char *name, const char **atts);
	virtual void endElement(void *ctx, const char *name);
	virtual void textHandler(void *ctx, const char *s, int len);

	static void setTest(bool bValue);
private:
	std::vector<cocos2d::Node*> m_pAddNodes;
	std::vector<WidgetInfo*> m_pWidgetInfos;
	cocos2d::Node*           m_pRootNode;
};
#endif // __XMLParse_H__