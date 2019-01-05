#ifndef __LiuHe__Game__ChongQingWuFen__
#define __LiuHe__Game__ChongQingWuFen__
#include "LiuHeBetArea.h"
#include "cocos2d.h"
#include "cocos-ext.h"

#define TAG_EDITBOX 9527	

USING_NS_CC;
USING_NS_CC_EXT;

class LiuHeWuFen : public LiuHeBetArea, public EditBoxDelegate
{

public:
	LiuHeWuFen();
	~LiuHeWuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(LiuHeWuFen);

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);

	//倍数 肯定是整数
	virtual void editBoxEditingDidBegin(EditBox* editBox) {};
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text){};
	virtual void editBoxReturn(EditBox* editBox){};

	virtual void getBetNumber();
	virtual String* getBetNumberAt(const int index);
	virtual int getBetBeiShuAt(const int index);

	virtual void setBetKind(LiuHeCaiKind kind);
	virtual void clearNumbers();
private:
	int areaNum;
	Node *m_node;
	float m_start;
	float m_end;
	//Vec2 m_endPos;
	Vec2 startPoint;
	Vec2 endPoint;
	Vec2 pt;

	//数组
	std::vector<Vec2>  m_vecPot;
	bool m_bDrag;
private:

	void initBetArea();	 //单个数字
	void initDXDSBetArea();
	void initSXBetArea();
	void initBSBetArea();
	
	int ballStatus[50];
};
#endif  //