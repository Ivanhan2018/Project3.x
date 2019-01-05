#ifndef __Game__IconTextSprite__
#define __Game__IconTextSprite__

#include "Inc.h"

class IconTextSprite : public Layer
{

public:
	IconTextSprite();
	virtual ~IconTextSprite();
	CREATE_FUNC(IconTextSprite);

public:
	virtual bool init();
	static IconTextSprite* createWith(CCDictionary* plistFrame,CCTexture2D* pTexture,const std::string,const std::string text);
	static IconTextSprite* createWith(CCSprite* pIcon,const char* text);
	static IconTextSprite* createWith(const char* icon,const char* text);
	bool initUI(CCDictionary* plistFrame,CCTexture2D* pTexture,const std::string,const std::string);
	bool initUI(CCSprite* pIcon,const char* text);
	void setText(const char* text);
    void setScale(float scale);
	void setMargin(UNS_INT margin);
	void setColor(ccColor3B textColor);
	void setFontSize(UNS_INT fontSize);
	void setIconScale(float scale);

private:
	void resetContentSize();
	void setTextPoint();

private:
	CCSprite* pIcon;
	CCLabelTTF* pTextTTF;
	ccColor3B textColor;
	UNS_INT margin;

};

#endif