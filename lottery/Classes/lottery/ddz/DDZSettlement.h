//  [3/26/2014 xieyi]
#ifndef DDZ___SETTLEMENT__
#define DDZ___SETTLEMENT__
#include "cocos2d.h"
#include "BJLRes.h"
#include "DDZUser.h"
#include "CMD_DDZ.h"
#include "DDZCardTableLayer.h"
USING_NS_CC;

class DDZSettlement : public Layer
{
public:

	DDZSettlement();
	~DDZSettlement();
	static DDZSettlement* create(DDZCMD_S_GameEnd* sGameData, int nLandlordID);
	virtual bool init(DDZCMD_S_GameEnd* sGameData, int nLandlordID);

	bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void onClickBtnClose(Ref* pSender);

private:

	Sprite* m_pBG;	//背景图片
	Sprite* m_pTitle;	//	标题
	Sprite* m_pImgText;	//	标识文字

	Sprite* m_pImgPlayer[DDZGAME_PLAYER];	//	玩家头像数组
	Label* m_pLabPlayerName[DDZGAME_PLAYER];	//	玩家昵称
	Label* m_pLabBomb[DDZGAME_PLAYER];	//	炸弹数字
	Label* m_pLabOnceScore[DDZGAME_PLAYER];	//	单次得分数字
	Label* m_pLabDiFen;	//	底分

};

#endif