//  [3/26/2014 xieyi]
#ifndef __SETTLEMENT__
#define __SETTLEMENT__
#include "cocos2d.h"
#include "DDZRes.h"
#include "User.h"
#include "CardTableLayer.h"
USING_NS_CC;

enum{
	tag_showcard = 1,	//明牌
	tag_jiabei,			//加倍
	tag_qiangdizhu,		//抢地主
	tag_bomb,			//炸弹
	tag_spring,			//春天
	tag_lowcardtype,	//底牌类型
	tag_overcardtype,	//结束牌型
	tag_basescore,		//底分
	tag_tableconsume,	//桌面消耗
	tag_total,			//总倍率
};

class Settlement : public Node
{
public:

	Settlement();
	~Settlement();
	virtual bool init();
	CREATE_FUNC(Settlement);
	void setUserMessage(Integer * obj);		//玩家信息
	void setTimesMessage(CMD_S_GameEnd* _gameEnd,WORD _userID);	//设置倍率信息
private:
	void addString(char* key,Vec2 point,Color3B  color,int fontSize,int tag,int num);		//添加要显示的字符串
	void addUserString(const char * res,float pointY,Color3B color,int fontSize,long udpScore,const char * nikeName,bool isJiaBei,bool isDiZhu,bool isSelf);
	Sprite * m_bgLeftSprite;	//背景图片
	SpriteBatchNode * m_batchNode;	//
};

#endif