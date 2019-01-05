#ifndef _OperalayerBJL_H_
#define _OperalayerBJL_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "RoomLayer.h"//by hxh

USING_NS_CC;
USING_NS_CC_EXT;
enum MyEnumItem
{
	m_pCKPX_100EnumItem,//1
	m_pCKPX_1000EnumItem,//3
	m_pGRYZ_1WEnumItem,//5
	m_pGRYZ_10WEnumItem,//10
	m_pGRYZ_100WEnumItem,//50
	m_pGRYZ_500WEnumItem,//100
	m_pGRYZ_1000WEnumItem,//1000
	m_pGRYZ_ChipSpriteWEnumItem,

};

// add by hxh 20160707
#if 0
const LONGLONG CHIP_SCORE_1=100;
const LONGLONG CHIP_SCORE_2=1000;
const LONGLONG CHIP_SCORE_3=10000;
const LONGLONG CHIP_SCORE_4=100000;
const LONGLONG CHIP_SCORE_5=1000000;
const LONGLONG CHIP_SCORE_6=5000000;
const LONGLONG CHIP_SCORE_7=10000000;
#else
const LONGLONG CHIP_SCORE_1=1;
const LONGLONG CHIP_SCORE_2=3;
const LONGLONG CHIP_SCORE_3=5;
const LONGLONG CHIP_SCORE_4=10;
const LONGLONG CHIP_SCORE_5=50;
const LONGLONG CHIP_SCORE_6=100;
const LONGLONG CHIP_SCORE_7=1000;
#endif

inline int CHIP_SCORE_ENUM(LONGLONG llVal){
	const LONGLONG s_llArr[7]={CHIP_SCORE_1,CHIP_SCORE_2,CHIP_SCORE_3,CHIP_SCORE_4,CHIP_SCORE_5,CHIP_SCORE_6,CHIP_SCORE_7};
	for(int i=0;i<7;i++){
	    if(llVal==s_llArr[i])
			return i;
	}
	return -1;
}

// add by hxh 20160630
inline const char * BJL_diss_x_png(MyEnumItem x){
#if 0
	const char *szArr[]={ \
		"BJL_diss_1.png", \
		"BJL_diss_10.png", \
		"BJL_diss_100.png", \
		"BJL_diss_1000.png", \
		"BJL_diss_10000.png", \
		"BJL_diss_50000.png", \
		"BJL_diss_100000.png", \
	};
#else
	const char *szArr[]={ \
		"cm_02.png", \
		"cm_04.png", \
		"cm_06.png", \
		"cm_08.png", \
		"cm_10.png", \
		"cm_12.png", \
		"cm_14.png", \
	};
#endif

	return szArr[x];
}

// add by hxh 20160630
inline const char * BJL_Cannger_x_png(MyEnumItem x){
#if 0
	const char *szArr[]={ \
		"BJL_Cannger_1.png", \
		"BJL_Cannger_10.png", \
		"BJL_Cannger_100.png", \
		"BJL_Cannger_1000.png", \
		"BJL_Cannger_10000.png", \
		"BJL_Cannger_50000.png", \
		"BJL_Cannger_100000.png", \
	};
#else
	const char *szArr[]={ \
		"cm_01.png", \
		"cm_03.png", \
		"cm_05.png", \
		"cm_07.png", \
		"cm_09.png", \
		"cm_11.png", \
		"cm_13.png", \
	};
#endif

	return szArr[x];
}

class OperaLayer:public cocos2d::Layer
{
public:
	OperaLayer();
	~OperaLayer();
	//创建函数
	static OperaLayer *create();  
	//初始化函数
	bool initOperaLayer();

	// 筹码按钮动画<add by hxh 20160721>
	void setChipButtonAnimation(int idx);
	void initOpenRaChipTTf();
	//void registerWithTouchDispatcher();

	Menu *pCloseMenu_100;
	Menu *pCloseMenu_1000;
	Menu *pCloseMenu_1W;
	Menu *pCloseMenu_10W;
	Menu *pCloseMenu_100W;
	Menu *pCloseMenu_500W;
	Menu *pCloseMenu_1000W;

	//点击事件的监听
	bool onTouchBegan(Touch *pTouch, Event *pEvent); 
	void onTouchMoved(Touch* touch,Event* event);
	void onTouchEnded(Touch *pTouch, Event *pEvent); 
	void onTouchCancelled(Touch* touch, Event* event);

	//显示筹码下注按钮
	void showOperMenut();

	//隐藏
	void remaggerdisflase();
	void menuCloseCallbackboy(Object* obj);
	//可以点击Touch
	void initClickTouch();
	//不可以点击并且变色
	void dissUnclickTouch();
	//删除下注精灵筹码
	void resetManagerOper();
	//设置那些按钮可以按
	void update(float delta);
	//获取分数
	void UpdatePlacelinitUnClickTouch(Object *obl);

	//设置已下注筹码值
	void setChouMaNumber(BYTE cbBetArea,LONGLONG lBetScore);

	Sprite *m_pCKPX_100Sprite_1;
	Sprite *m_pCKPX_100Sprite_2;
	Sprite *m_pCKPX_1000Sprite_1;
	Sprite *m_pCKPX_1000Sprite_2;
	Sprite *m_pGRYZ_1WSprite_1;
	Sprite *m_pGRYZ_1WSprite_2;
	Sprite *m_pGRYZ_10WSprite_1;
	Sprite *m_pGRYZ_10WSprite_2;
	Sprite *m_pCKPX_100WSprite_1;
	Sprite *m_pCKPX_100WSprite_2;
	Sprite *m_pGRYZ_500WSprite_1;
	Sprite *m_pGRYZ_500WSprite_2;
	Sprite *m_pGRYZ_1000WSprite_1;
	Sprite *m_pGRYZ_1000WSprite_2;
	int indexScore0;
	int indexScore1;
	int indexScore2;
	int indexScore3;
	int indexScore4;
	int indexScore5;
	int indexScore6;
	int indexScore7;

	LONGLONG myScoreSum[8];		//我的下注总和
	bool IsXIazhuNumber;

	LONGLONG ManNumberScore;//<by hxh>
private:
	//界面大小
	cocos2d::Size winSize;
	Node * anode;
	Array *chipOfMine;
	Sprite *currentChipSprite;
	LabelAtlas *m_pTotalChipValueTTf[8];
	Sprite *m_pMySpriteNunber[8];

	bool isControl;
	int stasteat;             //判断是那个

	LONGLONG   XIazNumberScore;
	float nodeScale;

	//筹码按钮
	MenuItemSprite *m_pCKPX_100Image;
	MenuItemSprite *m_pCKPX_1000Image;
	MenuItemSprite *m_pGRYZ_1WImage;
	MenuItemSprite *m_pGRYZ_10WImage;
	MenuItemSprite *m_pGRYZ_100WImage;
	MenuItemSprite *m_pGRYZ_500WImage;
	MenuItemSprite *m_pGRYZ_1000WImage;

	//筹码显示
	Sprite *pCKPX_100;
	Sprite *pCKPX_1000;
	Sprite *pCKPX_1W;
	Sprite *pCKPX_10W;
	Sprite *pCKPX_100W;
	Sprite *pCKPX_500W;
	Sprite *pCKPX_1000W;

	Sprite *m_pLight;//<by hxh>
};

#endif