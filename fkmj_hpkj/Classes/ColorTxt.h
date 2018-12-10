#pragma once

#include "cocos2d.h"
#include <vector>

NS_CC_BEGIN;

class ColorTxt
	: public Node
{
public:
	ColorTxt();
	virtual ~ColorTxt();
	virtual bool init();
	static ColorTxt* create();

	const char* getFontName();
	void setFontName(const char* pFontName);
	float getFontSize() const;
	void setFontSize(float fFontSize);
	unsigned int getMaxLineLength() const;
	void setMaxLineLength(unsigned int uLen);
	float getVerticalSpacing() const;
	void setVerticalSpacing(float fSpacing);

	void insertElement(std::string pString, 
		const Color3B& tColor = Color3B::WHITE,
		const char* pFontName = NULL, 
		float fFontSize = 0.0f);

	void insertElement(Node* pNode,
		unsigned int uLen = 0);

	void setShowTxt(std::string kTxt);

	void removeAllElements();
	void reloadData();

protected:
	void pushAtlasLine();
	void pushAtlasElementAtLast(Node* tAtlas);
	Node* makeAtlasLabelElement(std::string str, const char* fontname, float size, const Color3B& color);

protected:
	float m_fVerticalSpacing;
	unsigned int m_uCharCursor;
	unsigned int m_uMaxLineLength;
	std::string m_strFontName;
	float m_fFontSize;

	std::vector<std::vector<Node*>> m_pRichTextAtlas;
};

NS_CC_END;