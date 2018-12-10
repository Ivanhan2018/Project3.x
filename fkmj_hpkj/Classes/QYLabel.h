#pragma once

#include "cocos2d.h"

NS_CC_BEGIN

class QYLabel : public cocos2d::LabelTTF
{
public:
	QYLabel();
public:
	static QYLabel* create(const std::string& string, const std::string& fontName, float fontSize,
		const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::CENTER,
		TextVAlignment vAlignment = TextVAlignment::TOP);
public:
	void QYEnabelOutLine(const Color4B& color,int iLineWidth);
	void upOutLinePos();

	virtual void setString(const std::string& text) override;
	virtual void setSystemFontSize(float fontSize);
	virtual void setDimensions(const Size &dim);
	virtual void setColor(const Color3B& color);
private:
	int m_iLineWidth;
	LabelTTF* m_pUp;
	LabelTTF* m_pDown;
	LabelTTF* m_pLeft;
	LabelTTF* m_pRight;
};
NS_CC_END
