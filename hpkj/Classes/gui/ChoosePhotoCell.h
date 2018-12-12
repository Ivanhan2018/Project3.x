#pragma once
#include "cocos2d.h"
#include "MovingLabelLayer.h"
USING_NS_CC;


class ChoosePhotoCell : public Node
{
public:
	static ChoosePhotoCell* PhotoCellWith(int type);
	bool initPhotoCellWith(int type);
	void setCellIdx(int idx);
	void choosePhoto(Object *obj);
	void callbackC();
	ChoosePhotoCell(void);
	~ChoosePhotoCell(void);
	

private:
	cocos2d::Size winSize;
	int m_nType;
	/**代表选择的为第几号图片**/
	int m_choose;
	Sprite *pPhotoBJ1;
	Sprite *pChoose;
	Menu *pMenu;
	MovingLabelLayer* layer;
};