#pragma once
#include "cocos2d.h"
#include "StructType.h"

struct QYActionInfo
{
	static const char* getType()
	{
		return "EmptyAction";
	}

	QYActionInfo()
		:bSaveAction(false)
		,bRepeat(false)
		,pParent(NULL)
		,kType("EmptyAction")
	{
	}
	virtual ~QYActionInfo()
	{
		for (int i = 0;i<(int)kNextAction.size();i++)
		{
			delete kNextAction[i];
		}
		kNextAction.clear();
	}

	std::string kName;
	std::string kType;
	bool		bSaveAction;
	bool		bRepeat;
	QYActionInfo* pParent;
	std::vector<QYActionInfo*> kNextAction;
};

struct QYActionWaiteTime:QYActionInfo
{
	static const char* getType()
	{
		return "WaiteTime";
	}
	RandFloat fWaiteTime;
};

struct QYActionSetPos:QYActionInfo
{
	static const char* getType()
	{
		return "SetPos";
	}
	RandPos kDestPos;
};
struct QYActionRemoveSelf:QYActionInfo
{
	static const char* getType()
	{
		return "RemoveSelf";
	}
	int iParentTag;
};

struct QYActionSetVisible:QYActionInfo
{
	static const char* getType()
	{
		return "Visible";
	}
	bool  bVisible;
};

struct QYActionStopAllAction:QYActionInfo
{
	static const char* getType()
	{
		return "StopAllAction";
	}
};

struct QYActionSetOder:QYActionInfo
{
	static const char* getType()
	{
		return "SetOder";
	}
	int  iOder;
};

struct QYActionSplineTo:QYActionInfo
{
	static const char* getType()
	{
		return "SplineTo";
	}
	RandFloat fTime;
	RandFloat fTension;
	std::vector<RandPos> kPosition;
};


struct QYActionBizerTo:QYActionInfo
{
	static const char* getType()
	{
		return "BizerTo";
	}
	RandFloat fTime;
	RandPos   kPos1;
	RandPos   kDestPos;
};

struct QYActionBizerBy:QYActionInfo
{
	static const char* getType()
	{
		return "BizerBy";
	}
	RandFloat fTime;
	RandPos   kPos1;
	RandPos   kDestPos;
};

struct QYActionMoveTo:QYActionInfo
{
	static const char* getType()
	{
		return "MoveTo";
	}
	RandFloat	fTime;
	RandPos		kDestPos;
	bool		bDestX;
	bool		bDestY;
};

struct QYActionMoveBy:QYActionInfo
{
	static const char* getType()
	{
		return "MoveBy";
	}
	RandFloat	fTime;
	RandPos		kDestPos;
};

struct QYActionMoveToBySpd:QYActionInfo
{
	static const char* getType()
	{
		return "MoveToBySpd";
	}
	RandFloat	fMoveSpd;
	RandPos		kDestPos;
	bool		bDestX;
	bool		bDestY;
};

struct QYActionGravity:QYActionInfo
{
	static const char* getType()
	{
		return "Gravity";
	}
	RandFloat	fTime;
	RandFloat	fGravity;
};

struct QYActionSetScale:QYActionInfo
{
	static const char* getType()
	{
		return "SetScale";
	}
	RandFloat fDestScale;
};

struct QYActionScaleTo:QYActionInfo
{
	static const char* getType()
	{
		return "ScaleTo";
	}
	RandFloat fTime;
	RandFloat fDestScale;
};

struct QYActionScaleXTo:QYActionInfo
{
	static const char* getType()
	{
		return "ScaleXTo";
	}
	RandFloat fTime;
	RandFloat fDestScaleX;
};

struct QYActionScaleYTo:QYActionInfo
{
	static const char* getType()
	{
		return "ScaleYTo";
	}
	RandFloat fTime;
	RandFloat fDestScaleY;
};

struct QYActionSetProgress:QYActionInfo
{
	static const char* getType()
	{
		return "SetProgress";
	}
	RandFloat	fPercent;
};

struct QYActionProgressTo:QYActionInfo
{
	static const char* getType()
	{
		return "ProgressTo";
	}
	RandFloat fTime;
	RandFloat fPercent;
};


struct QYActionSetSize:QYActionInfo
{
	static const char* getType()
	{
		return "SetSize";
	}
	RandPos		kDestSize;
};

struct QYActionSizeTo:QYActionInfo
{
	static const char* getType()
	{
		return "SizeTo";
	}
	RandFloat	fTime;
	RandPos		kDestSize;
};

struct QYActionSetAple:QYActionInfo
{
	static const char* getType()
	{
		return "SetAple";
	}
	RandFloat fDestAple;
};

struct QYActionApleTo:QYActionInfo
{
	static const char* getType()
	{
		return "ApleTo";
	}
	RandFloat fTime;
	RandFloat fDestAple;
};

struct QYActionSetRota:QYActionInfo
{
	static const char* getType()
	{
		return "SetRota";
	}
	RandFloat fDestRota;
};

struct QYActionRotaTo:QYActionInfo
{
	static const char* getType()
	{
		return "RotaTo";
	}
	RandFloat fTime;
	RandFloat fDestRota;
};

struct QYActionRotaBy:QYActionInfo
{
	static const char* getType()
	{
		return "RotaBy";
	}
	RandFloat fTime;
	RandFloat fDestRota;
};

struct QYActionCallAction:QYActionInfo
{
	static const char* getType()
	{
		return "CallAction";
	}
	std::string kOtherNodeName;
	std::string kSelfActionName;
	std::string kOtherActionName;
	std::string kActionParentName;
};

struct QYActionCallButton:QYActionInfo
{
	static const char* getType()
	{
		return "CallButton";
	}
	std::string kButtonName;
};

struct QYActionCreateWidget:QYActionInfo
{
	static const char* getType()
	{
		return "CreateWidget";
	}
	cocos2d::Vec2 kOffectPos;
	std::string kWidgetName;
	bool bRootNode;
};

struct QYActionRunAnim:QYActionInfo
{
	static const char* getType()
	{
		return "RunAnim";
	}
	std::string kAnimName;
};

struct QYActionPlaySound:QYActionInfo
{
	static const char* getType()
	{
		return "PlaySound";
	}
	std::string kSoundName;
};

struct QYActionSetTxt:QYActionInfo
{
	static const char* getType()
	{
		return "SetTxt";
	}
	std::string kTxt;
};
struct QYActionSetImage:QYActionInfo
{
	static const char* getType()
	{
		return "SetImage";
	}
	UserString kImageFile;
};
