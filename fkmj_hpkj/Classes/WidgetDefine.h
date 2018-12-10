#pragma once
#include "cocos2d.h"

struct QYActionInfo;

#define WidgetFindUserInfoError "Find Error"
#define WidgetNotFindUserInfo "Not Find"

typedef std::map<std::string,std::string> StringMap;

struct stringKeyValue
{
	stringKeyValue()
	{}
	stringKeyValue(std::string _key,std::string _value)
		:kKey(_key)
		,kValue(_value)
	{}
	std::string kKey;
	std::string kValue;
};
typedef std::vector<stringKeyValue> stringKeyValueList;

struct WidgetInfo
{
	WidgetInfo()
	{
	}
	virtual ~WidgetInfo()
	{
	}
	std::string kName;
	std::string kType;
	int iOder;
	StringMap kUserInfo;
	stringKeyValueList kAction;
	stringKeyValueList kProperty;

	std::vector<WidgetInfo*> kChild;
	std::vector<QYActionInfo*> kQYAction;

	virtual WidgetInfo* copyNode()
	{
		WidgetInfo* pWidget = new WidgetInfo;
		*pWidget = *this;
		return pWidget;
	}
	WidgetInfo* copyAll()
	{
		WidgetInfo* pRootInfo = copyNode();
		pRootInfo->kChild.clear();
		for (int i = 0;i<(int)kChild.size();i++)
		{
			WidgetInfo* pChildInfo = kChild[i]->copyAll();
			pRootInfo->kChild.push_back(pChildInfo);
		}
		return pRootInfo;
	}
};

struct WidgeTextureInfo
{
	std::string kName;
	cocos2d::Rect kRect;
	bool			 bRect;
};

struct WidgetImagic:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetImagic";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetImagic* pWidget = new WidgetImagic;
		*pWidget = *this;
		return pWidget;
	}
	WidgeTextureInfo kTexture;
	bool			 bUsePixWidth;
};

struct WidgetNode:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetNode";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetNode* pWidget = new WidgetNode;
		*pWidget = *this;
		return pWidget;
	}
};

struct WidgetNodeUI:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetNodeUI";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetNodeUI* pWidget = new WidgetNodeUI;
		*pWidget = *this;
		return pWidget;
	}
};


struct WidgetTxt:WidgetInfo
{

	static const char* getType()
	{
		return "WidgetTxt";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetTxt* pWidget = new WidgetTxt;
		*pWidget = *this;
		return pWidget;
	}

	std::string kShowTxt;
	std::string kFontName;
	int			kFontSize;
	cocos2d::Size kDimensions;
};

struct WidgetColorTxt:WidgetInfo
{

	static const char* getType()
	{
		return "WidgetColorTxt";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetColorTxt* pWidget = new WidgetColorTxt;
		*pWidget = *this;
		return pWidget;
	}

	std::string kFontName;
	int			kFontSize;
	cocos2d::Size kDimensions;
};

struct WidgetTxtAtlas:WidgetInfo
{

	static const char* getType()
	{
		return "WidgetTxtAtlas";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetTxtAtlas* pWidget = new WidgetTxtAtlas;
		*pWidget = *this;
		return pWidget;
	}

	std::string kShowTxt;
	std::string kImagic;
	int			iWidth;
	int			iHeight;
	int			iStartChar;
	bool		bActionNum;
};


struct WidgetEdit:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetEdit";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetEdit* pWidget = new WidgetEdit;
		*pWidget = *this;
		return pWidget;
	}


	std::string kBackTexture;
	std::string kShowTxt;
	std::string kFontName;
	int			kFontSize;
	cocos2d::Size kSize;
	
};

struct WidgetButton:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetButton";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetButton* pWidget = new WidgetButton;
		*pWidget = *this;
		return pWidget;
	}
	
	std::string kNomalTexture;
	std::string kPressTexture;
	std::string kDisableTexture;
	bool		bUseViewMenu;

	std::string kShowTxt;
	std::string kFontName;
	int			kFontSize;
};

struct WidgetCheckBox:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetCheckBox";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetCheckBox* pWidget = new WidgetCheckBox;
		*pWidget = *this;
		return pWidget;
	}

	std::string kUnSelectTexture;
	std::string kSelectTexture;
};

struct WidgetSelectBox:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetSelectBox";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetSelectBox* pWidget = new WidgetSelectBox;
		*pWidget = *this;
		return pWidget;
	}

	std::string kUnSelectTexture;
	std::string kSelectTexture;
	bool		bUseViewMenu;
};

struct WidgetSider:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetSider";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetSider* pWidget = new WidgetSider;
		*pWidget = *this;
		return pWidget;
	}

	std::string kNomalTexture;
	std::string kPressTexture;
	std::string kDisableTexture;

	float fMaxValue;
	float fMaxPosY;

	bool bMoveByX;
};

struct WidgetProgressBar:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetProgressBar";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetProgressBar* pWidget = new WidgetProgressBar;
		*pWidget = *this;
		return pWidget;
	}
	std::string kBackTexture;
	std::string kBarTexture;
	std::string kShowTxt;
	std::string kFontName;
	int			kFontSize;

};

struct WidgetProgressTimer:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetProgressTimer";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetProgressTimer* pWidget = new WidgetProgressTimer;
		*pWidget = *this;
		return pWidget;
	}
	std::string kTexture;
};


struct Widget9Sprite:WidgetInfo
{
	static const char* getType()
	{
		return "Widget9Sprite";
	}
	virtual WidgetInfo* copyNode()
	{
		Widget9Sprite* pWidget = new Widget9Sprite;
		*pWidget = *this;
		return pWidget;
	}
	std::string		  kBackTexture;
	cocos2d::Size      kSize;
	cocos2d::Rect      kInsetsRect;

};
struct WidgetList:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetList";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetList* pWidget = new WidgetList;
		*pWidget = *this;
		return pWidget;
	}
};
struct WidgetRichText:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetRichText";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetRichText* pWidget = new WidgetRichText;
		*pWidget = *this;
		return pWidget;
	}
};

struct WidgetAnimation:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetAnimation";
	}

	virtual WidgetInfo* copyNode()
	{
		WidgetAnimation* pWidget = new WidgetAnimation;
		*pWidget = *this;
		return pWidget;
	}
	std::string kPlistFile;
	std::string kImgFile;
};

struct WidgetPanle:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetPanle";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetPanle* pWidget = new WidgetPanle;
		*pWidget = *this;
		return pWidget;
	}
	cocos2d::Size kSize;
	cocos2d::Color3B kColor;
};

struct WidgetSwitch_Button:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetSwitch_Button";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetSwitch_Button* pWidget = new WidgetSwitch_Button;
		*pWidget = *this;
		return pWidget;
	}
};

struct WidgetLayout:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetLayout";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetLayout* pWidget = new WidgetLayout;
		*pWidget = *this;
		return pWidget;
	}
};

struct WidgetClipper:WidgetInfo
{
	static const char* getType()
	{
		return "WidgetClipper";
	}
	virtual WidgetInfo* copyNode()
	{
		WidgetClipper* pWidget = new WidgetClipper;
		*pWidget = *this;
		return pWidget;
	}
	cocos2d::Size kSize;
};

typedef std::map<std::string,WidgetInfo*> WidgetInfoListP;

struct WidgetUserInfo:cocos2d::Ref
{
	WidgetUserInfo()
		:bIsSkin(true)
		,pWidgetInfo(NULL)
		,bRoot(false)
	{
		autorelease();
	}
	WidgetUserInfo(std::string _name)
		:bIsSkin(true)
		,kName(_name)
		,pWidgetInfo(NULL)
		,bRoot(false)
	{
		autorelease();
	}
	~WidgetUserInfo()
	{
	}
	std::string kType;
	std::string kName;
	std::string kSkinTempName;
	StringMap kUserInfo;
	stringKeyValueList kAction;
	bool bIsSkin;
	bool bRoot;
	WidgetInfo* pWidgetInfo;
};

typedef void (cocos2d::Ref::*CB_BUTTON_Handler)(cocos2d::Ref*,WidgetUserInfo*);
#define button_selector(_SELECTOR) (CB_BUTTON_Handler)(&_SELECTOR)

struct Button_CBInfo
{
	std::string kCommand;
	cocos2d::Ref* pObject;
	CB_BUTTON_Handler pCallBack;
};

typedef std::vector<Button_CBInfo> ButtonCallBackList;

struct BigTextureInfo 
{
	cocos2d::Texture2D* pTexture;
	std::string			  kName;
};
typedef std::vector<BigTextureInfo> BigTextureInfoList;

