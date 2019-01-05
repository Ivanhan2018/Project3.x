#ifndef __CCTOUCHSPRITE_H__
#define __CCTOUCHSPRITE_H__

#include "cocos2d.h"
USING_NS_CC;

class CCTouchSprite : public Sprite
{
public:
	CCTouchSprite();
	~CCTouchSprite();

	static CCTouchSprite* createWithPic(const char *fileName, int row, int cell); //静态创建（内存自动释放）
	static CCTouchSprite* createWithPic(const char *fileName, int row, int cell, Object *rec, SEL_CallFuncN selector); //静态创建（内存自动释放）
	static CCTouchSprite* createWithSpriteFrame(SpriteFrame *pSpriteFrame,int row, int cell, Object *rec,SEL_CallFuncN selector);  //静态创建（内存自动释放）

	//void myInit();//自定义初始化函数
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	virtual void onTouchCancelled(Touch* pTouch, Event* pEvent);

	CC_SYNTHESIZE(int, rowIndex, RowIndex);//sprite在第几行
	
	CC_SYNTHESIZE(int, cellIndex, CellIndex);//sprite在第几列

private:
	//判断是否点击按钮
	bool isClickSprite(Touch* pTouch);
protected:
	SEL_CallFuncN c_pfnSelector;
	Object* m_pListener;
private:
	//判断是否移动
	bool m_bMovedSprite;
	bool m_bDoubleClick;

};

#endif