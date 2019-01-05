//  [3/27/2014 wucan]

#ifndef			__VIEW_CARD_EFFECT__
#define			__VIEW_CARD_EFFECT__
#include "cocos2d.h"
#include "DDZRes.h"
#include "SoundControl.h"
#include "CardTableLayer.h"
USING_NS_CC;

enum EffectType
{
	Effect_LandLord=1,
	Effect_NoLandLord,
	Effect_RobLord,
	Effect_NoRobLord,
	Effect_PassCard,
	Effect_OpenStart,						//--明牌开始
	Effect_gameStart,						//--普通开始
	Effect_openCard,						//--发牌时候明牌
	Effect_jiaBei,							//加倍
	Effect_alarm,							//警报
	Effect_outcardErr,						//出牌错误
	Effect_noBigCard,						//没牌能打过上家
	Effect_task,								//任务提示
	Effect_LlinkOFF,						//自动重连
};

class ViewCardEffect:public Node
{
public:
	CREATE_FUNC(ViewCardEffect);
	virtual bool init();
	void runEffect(EffectType _type,int userId);									//--动作动画
	void runCardEffect(int cardType,int userId,int cardNum,int userOutCardState);						//--手牌牌型动画
	void runRobot(bool isOnRobot);													//--机器人托管动画
	void runVoice(string voiceString,int _userID );
	void runVoiceAndFace(int _type,int _key,int _userID);					//--表情语音动画
	void runSettlementEffect(Integer * obj,int tableConsume,CardTableLayer * layer);		//结算动画
	void runHfdEffect(Integer * obj);							//话费点动画

private:
	void effectOpenStart(bool isOpen,int userId);					//--明牌和直接开始
	void effectOpenCard(int userId);										//--发牌明牌
	void effectLandLord(bool isLand,int userId);					//--有无叫地主
	void effectRodLord(bool isRob,int userId);		//--有无抢地主
	void effectPassCard(int userId);						//--过牌
	void effectJiaBei(int userId,bool isJiaBei);				//加倍
	void effectAlarm(int userId);	//警报
	void removeAlarm();//移除所有警报
	void addEffect(const char* key,Vec2 point,Color3B  color,int fontSize,bool showDi);	//添加说的话（文字）
	void addUserInputMsgEffect(const char* key,Vec2 point,Color3B color,int fontSize,bool showDi);
	void addPictureEffect(const char* key,Vec2 point,bool isAddPartiCar=false);	//添加图片效果
	void removerSelf(Node* node);
	void changeCoinNumber(Node * node);//改变金币数量
	void changeHfdNumber(Node * node);//改变话费点数量

private:
	bool m_leftAlarm;
	bool m_rightAlarm;
/*	Animation * getAnimation (const char * key,int frameNum);*/

	Vec2 getEffectPos(int _userID);
	//--机器人相关
	void RobotCallBack(Object* obj);
private:
	Node*     m_RobotNode;
	unsigned int g_udpCoinNumber;//变化的金币数量
	unsigned int g_hfdCoinNumber;//话费点变化数量
	CardTableLayer * g_cardTableLayer;
};

#endif