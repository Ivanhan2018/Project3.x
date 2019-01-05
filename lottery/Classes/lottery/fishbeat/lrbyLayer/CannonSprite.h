#ifndef __GAME_CANNON_SPRITE_H__
#define __GAME_CANNON_SPRITE_H__
#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "BulletSprite.h"
#include "MyNSString.h"
#include "Define.h"
using namespace cocostudio;
USING_NS_CC;

enum enCannonFireStatue
{
	CANNON_NORMAL=0,
	CANNON_FIRE
};

class CannonNode : public cocos2d::Node
{
public:
	CannonNode(int nChairID);
	~CannonNode();

	static CannonNode* create(int nChairID);
	virtual bool init(int nChairID);

	void setCannonAngle(float fangle);
	void setCannonNumber(int nNum);
	void setScoreNumber(LONGLONG nNum);
	void setCannonStatue(int nStatue);
	float getCannonAngle();
	int getCannonStatue();	
	int getCannonFireStatue();
	inline LONGLONG getScoreNumber() { return m_nScoreNumber; }
	inline int getCannonNumber() { return m_nCannonNumber; }

	void cannonFire();
	void setNickName(std::string strNickName);
	void setVIPLevel(int nVIPLevel);
	void setFace(int nFace);
	void setBoard(int nChairID, DWORD dwTicket);
public:
	Sprite* m_spCannonBg;
	Sprite* m_spCannon;
	Sprite* m_spCannonNumBg;
	LabelAtlas* m_labelNum;
	Sprite* m_spFishScore;
	LabelAtlas* m_labelFishScore;
	int		m_cannonStatue;
	int		m_fireStatue;

	Animate *m_fireAnimate;
private:
	const int m_nChairID;
	float	 m_fCannonAngle;
	LONGLONG	 m_nScoreNumber;
	int		 m_nCannonNumber;

	LabelTTF* m_labelNickName;
	Sprite* m_spVIPLevel;
	Sprite*									m_spFace;	
};


#endif 