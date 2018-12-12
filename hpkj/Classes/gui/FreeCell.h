#pragma once
#include "cocos2d.h"

USING_NS_CC;


class FreeCell : public Node
{
public:
	static FreeCell* FreeCellWith(int type,bool isGet);
	bool initFreeCellWith(int type,bool isGet);
	void setCellIdx(int idx,bool isGet);
	void chooseFree(Object *obj);
	void setPesent(Object* obj);
	FreeCell(void);
	~FreeCell(void);

	void setMenuEnable(Object* obj);

private:

	int m_nType;
	bool m_nIsget;
	/**代表选择的为第几号图片**/
	int m_choose;
	Sprite *pSprite;
	Sprite *pChoose;

	LabelTTF *m_titile;
	LabelTTF *m_text;

	/**进度条**/
	ProgressTimer *pJinDuTiao;
	Sprite *pJinDuTiao0;
	Sprite *pJinDuTiao1;
	//领取字
	LabelTTF *pTextWaring;
	/**文字 显示下载进度**/
	LabelTTF *pTTF;

	/**csmj**/
	int m_csmj_per;
	/**pdk**/
	int m_pdk_per;
	/**phz**/
	int m_phz_per;
	/**sdh**/
	int m_sdh_per;

	MenuItemSprite *m_pMenuButton;
	
};