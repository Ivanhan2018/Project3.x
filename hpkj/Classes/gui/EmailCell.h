#pragma once
#include "cocos2d.h"

USING_NS_CC;


class EmailCell : public Sprite
{
public:
	static EmailCell* EmailCellWith(int type);
	bool initEmailCellWith(int type);
	void setCellIdx(int idx);
	EmailCell(void);
	~EmailCell(void);

	void DeleteMessage(Object* obj);

private:
	LabelTTF* messageContext;
	LabelTTF* messageTitle;
	int m_nType;
};

