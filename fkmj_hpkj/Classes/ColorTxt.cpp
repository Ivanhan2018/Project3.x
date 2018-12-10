
#include "ColorTxt.h"
#include "utility.h"

NS_CC_BEGIN;

static unsigned char utf8_look_for_table[] = 
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

#define UTF8LEN(_X_) utf8_look_for_table[(_X_)]

int str_utf8_char_len(unsigned char ch)
{
	return (int)(UTF8LEN((ch)));
}

ColorTxt::ColorTxt()
: m_strFontName("Helvetica")
, m_fFontSize(30.0f)
, m_uMaxLineLength(0)
, m_uCharCursor(0)
, m_fVerticalSpacing(0.0f)
{
}

ColorTxt::~ColorTxt()
{
	removeAllElements();
}

ColorTxt* ColorTxt::create()
{
	ColorTxt* pRet = new ColorTxt();
	if( pRet && pRet->init() )
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool ColorTxt::init()
{
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);
	return true;
}

void ColorTxt::insertElement(std::string pString
							 , const Color3B& tColor
							  , const char* pFontName
							  , float fFontSize)
{
	CCAssert(pString.size(), "should not null");

	if( m_uMaxLineLength != 0 )
	{
		if( m_uCharCursor == m_uMaxLineLength )
		{
			m_uCharCursor = 0;
			pushAtlasLine();
		}

		std::string str_buffer;
		const char* pstr = pString.c_str();

		while( *pstr != 0 )
		{
			int nCharLength;

			int nlen = str_utf8_char_len(*pstr);
			if( nlen > 1 )
			{
				nCharLength = 2; //default 2 (maybe a word of chinese, can be else)
			}
			else
			{
				nCharLength = 1; //English char
			}
			
			if( m_uCharCursor + nCharLength > m_uMaxLineLength )
			{
				Node* tElement = makeAtlasLabelElement(str_buffer.c_str(), pFontName, fFontSize, tColor);

				pushAtlasElementAtLast(tElement);
				pushAtlasLine();

				str_buffer.clear();
				str_buffer.append(pstr,(unsigned int)nlen);
				m_uCharCursor = 0;
			}else if (*pstr == '\n')
			{
				Node* tElement = makeAtlasLabelElement(str_buffer.c_str(), pFontName, fFontSize, tColor);

				pushAtlasElementAtLast(tElement);
				pushAtlasLine();

				str_buffer.clear();
				//str_buffer.append(pstr, (unsigned int)nlen);   don't append '\n'
				m_uCharCursor = 0;
			}
			else
			{
				str_buffer.append(pstr,(unsigned int)nlen);
			}

			m_uCharCursor += nCharLength;
			pstr += nlen;
		}

		if( str_buffer.empty() )
			return;

		Node* tElement = makeAtlasLabelElement(str_buffer.c_str(), pFontName, fFontSize, tColor);

		pushAtlasElementAtLast(tElement);
	}
	else
	{
		Node* tElement = makeAtlasLabelElement(pString, pFontName, fFontSize, tColor);

		pushAtlasElementAtLast(tElement);
	}
}

void ColorTxt::insertElement(Node* pNode
							  , unsigned int uLen)
{
	CCAssert(pNode, "should not null");

	if( m_uMaxLineLength != 0 )
	{
		CCAssert(uLen <= m_uMaxLineLength, "len should be less than m_uMaxLineLength");

		m_uCharCursor += uLen;

		if( m_uCharCursor > m_uMaxLineLength )
		{
			pushAtlasLine();
			m_uCharCursor = 0;
		}

		pNode->setAnchorPoint(Vec2::ZERO);

		pushAtlasElementAtLast(pNode);
	}
	else
	{
		pNode->setAnchorPoint(Vec2::ZERO);

		pushAtlasElementAtLast(pNode);
	}
}
void ColorTxt::setShowTxt(std::string kTxt)
{
	while (kTxt.find("[") != std::string::npos)
	{
		size_t stFindStart = kTxt.find("[");
		size_t stFindEnd = kTxt.find("]",stFindStart+1);
		std::string kCommand;
		kCommand = kTxt.substr(stFindStart+1,stFindEnd - stFindStart - 1);
		std::vector<std::string> kVec = utility::split(kCommand,":");
		insertElement(kVec[0],utility::parseColour(kVec[1]));
		kTxt.replace(0,stFindEnd + 1,"");
	}
	reloadData();
}

void ColorTxt::reloadData()
{
	CCAssert(!m_pRichTextAtlas.empty(), "reload data was empty");
	removeAllChildrenWithCleanup(true);
	m_uCharCursor = 0;

	Size tRealContentSize = Size::ZERO;

	int nRowBegin = m_pRichTextAtlas.size() - 1;
	unsigned int uRowEnd = 0;
	unsigned int uColumn = 0;
	unsigned int uColumnSize = 0;
	
	for(; nRowBegin >= (int)uRowEnd; --nRowBegin )
	{
		std::vector<Node*>& vAtlasLine = m_pRichTextAtlas.at(nRowBegin);
		uColumnSize = vAtlasLine.size();
		uColumn = 0;

		float fWidgetOfLine = 0.0f;
		float fMaxHeightOfLine = 0.0f;

		for(; uColumn < uColumnSize; ++uColumn )
		{
			Node* tElement = vAtlasLine.at(uColumn);

			tElement->setPosition(Vec2(fWidgetOfLine, tRealContentSize.height));
			addChild(tElement);

			fWidgetOfLine += tElement->getContentSize().width;
			fMaxHeightOfLine = MAX(fMaxHeightOfLine, tElement->getContentSize().height);
		}

		tRealContentSize.width = MAX(tRealContentSize.width, fWidgetOfLine);
		tRealContentSize.height += fMaxHeightOfLine + m_fVerticalSpacing;
	}

	m_pRichTextAtlas.clear();

	tRealContentSize.height -= m_fVerticalSpacing;
	setContentSize(tRealContentSize);
}

void ColorTxt::removeAllElements()
{
	m_pRichTextAtlas.clear();
	m_uCharCursor = 0;

	removeAllChildrenWithCleanup(true);
}

Node* ColorTxt::makeAtlasLabelElement( std::string  str
									  , const char* fontname
									  , float size
									  , const Color3B& color)
{
	cocos2d::Label* pLabel = cocos2d::Label::createWithSystemFont(str, 
		fontname && strlen(fontname) ? fontname : m_strFontName.c_str(),
		(int) size == 0 ? m_fFontSize : size);

	pLabel->setAnchorPoint(Vec2::ZERO);
	pLabel->setColor(color);
	return pLabel;
}

void ColorTxt::pushAtlasLine()
{
	std::vector<Node*> kTemp;
	m_pRichTextAtlas.push_back(kTemp);
}

void ColorTxt::pushAtlasElementAtLast(Node* tAtlasElement)
{
	if( m_pRichTextAtlas.empty() )
	{
		pushAtlasLine();
	}

	m_pRichTextAtlas.back().push_back(tAtlasElement);
}

const char* ColorTxt::getFontName()
{
	return m_strFontName.c_str();
}

void ColorTxt::setFontName(const char* pFontName)
{
	m_strFontName = pFontName;
}

float ColorTxt::getFontSize() const
{
	return m_fFontSize;
}

void ColorTxt::setFontSize(float fFontSize)
{
	m_fFontSize = fFontSize;
}

unsigned int ColorTxt::getMaxLineLength() const
{
	return m_uMaxLineLength;
}

float ColorTxt::getVerticalSpacing() const
{
	return m_fVerticalSpacing;
}

void ColorTxt::setVerticalSpacing(float fSpacing)
{
	m_fVerticalSpacing = fSpacing;
}

void ColorTxt::setMaxLineLength(unsigned int uLen)
{
	CCAssert(uLen == 0 || uLen >= 2, "len should bigger than 2");
	m_uMaxLineLength = uLen;
}
NS_CC_END;