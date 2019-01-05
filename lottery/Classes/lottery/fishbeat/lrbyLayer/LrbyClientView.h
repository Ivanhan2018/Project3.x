#ifndef _LKPY_GAMECLIENTVIEW_H__
#define _LKPY_GAMECLIENTVIEW_H__
#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos-ext.h"
#include "CMD_Fish.h"
#include "SimpleAudioEngine.h"
#include "CannonSprite.h"
#include "FishManager.h"
#include "Sound.h"
#include "NetPool.h"
#include "MenuLayer.h"
#include "AccountLayer.h"
#include "AwardLayer.h"
#include "SceneView.h"
#include "LockFishManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;

enum
{
	TAG_ARROW_ADD_SCORE = 1234,		//提示自己上分箭头
	TAG_ARROW_MY_POSITION,		//提示自己位置箭头
	TAG_TEXT_SCORE,				//提示加分文字
	TAG_TEXT_POSITION			//提示位置文字
};

enum 
{
	TPYE_ZIPPO = -4,
	TPYE_WHITE,
	TPYE_MP3,
	TPYE_IPAD
};
class SceneView;
class LrbyClientView: public cocos2d::Layer//, public cocos2d::extension::TableViewDelegate, public cocos2d::extension::TableViewDataSource
{
public:
	LrbyClientView();

	~LrbyClientView();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	//static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	virtual void onExit();

	virtual void onEnter();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(LrbyClientView);


	void initUi();

	void initData();


	bool OnSwitchScene(NS_lrbyFish::CMD_S_SwitchScene* switscene);

	//
	inline void setGameConfigStatue(bool bConfig) { m_bGameConfig = bConfig; }

	std::string getSdCardLuJing(std::string szFileName);

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	void onTouchCancelled(Touch *touch, Event *event);

	//更新用户信息
	void updateUserInfo(Object* obj);

	virtual void update(float dt);

	void onGameFishMessage(Object* obj);

	void menuCloseRechargeCallback(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void menuCloseSetLayerCallback(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);

	//FishSprite* getLockFishSprite(int nChair, int fishID);
	bool InsideScreen(Vec2 ptPos);
	//
	void showBingoGold(Vec2 ptBingoPos, int nGold);
	void showCatchFishGold(Vec2 ptGoldBeginPos, Vec2 ptGoldEndPos, int nGold);
	void removeBingo(Node* sender, Node* bingo);
	void removeGold(Node* sender, Node* gold);
	void removeTicket(Node* sender, Node* ticket);
	void removeLabelGold(Node* sender, LabelAtlas* label);
	void setScreenLock(bool bLock);
	void isNext();

	void showTicketEffect(Vec2 ptStart, int nNum);
	void setBulletMulriple(int nMulriple);
	void setOtherBulletMulriple(int nPlayer, int nMulriple);
	void autoShootBullet(float dt);
	void onAutoShoot(Ref* sender);
	void onLockFish(Ref* sender);
	void onUnLockFish(Ref* sender);
	void onAddFishScore(Ref* sender);
	void onMinusFishScore(Ref* sender);
	void onAddBulletMultiple(Ref* sender);
	void onSubBulletMultiple(Ref* sender);

	void onPressSetting(Ref* sender);
	void onPressHelp(Ref* sender);

	void onQuitGame(Ref* sender);
	void onOnLineAward(int nType);
	void onPayForGold(Ref* sender);
	bool isCanFire();
	//是否能锁定鱼，判断条件为，在场景内是否出现大鱼
	bool isCanLockFish(int chairId);

	void onAccountGold();
	void onAccountGoldAndQuit();
	void onAccountCancle();
	void onAwardQuit();

	void onAwardQuit(float dt);

	virtual void onLayerExit();	

	static LrbyClientView* GetInstance() { return m_pLrbyClientView; }

	static WORD switchChairID(WORD wCharID);
	
	// add by hxh 20161227
	static unsigned long long GetClientTickCount();

	CC_SYNTHESIZE(bool, m_bAutoShoot, AutoShoot);
	CC_SYNTHESIZE(bool, m_bScheduleAutoShoot, ScheduleAutoShoot);

	//new add 
	void onBackPress(Ref* sender);

	CC_SYNTHESIZE(bool, m_isLockFish, LockFish);
	//是否有鱼能锁定
	//bool IsCanLockFish();
	//创建鱼锁定线
	void createLockLineView(int chairId, FishSprite* fish);

public:
	void OnMessage(Ref* sender);//notification,通知过来的信息，用于收到信息后的界面展示
private:
	void userInforDeal(Integer* _data);//更新用户信息

	void onLoginFaild(Object* obj);

	void initUserInfo(std::string nickName, SCORE lScore);//设置当前玩家信息

	void updateUserMoney(const int money);

	void initMenuBtn();

	void addTipsView();//提示箭头

	void removeTipScoreView();

	void addUserPositionTipsView(int posIndex);//位置箭头

	void removePositionTipsView();

	void netDissconnect(Ref* pSender);//网络断开

	void checkUserEnterBackground(float dt);

	//展示锁定鱼icon
	void showLockFishIcon(int chairId, int fishId, int fishKind);

private:
	bool isShowTipArrow; //是否显示过提示箭头
	bool isOk;  //初始化玩家炮台等位置
	MenuItemImage* menu_menu;
	MenuItemImage* menu_AutoShoot;
	MenuItemImage* menu_LockFish;
	MenuItemImage* menu_UnLockFish;
	MenuItemImage* menu_AddGun;
	MenuItemImage* menu_SubGun;
	//
	MenuItemImage* menu_AddBringGold;
	MenuItemImage* menu_SubBringGold;

	//

	MenuItemImage* menu_setting;
	MenuItemImage* menu_help;
	MenuItemImage* menu_exit;

	LabelTTF* m_userName;
	LabelTTF* m_userGold;
	//LabelAtlas* m_userGold;
	LayerColor* m_layerBottom;

	Node* bottomNode;//底部用户信息节点

	Menu* m_menu;
	bool isShow;
	Vec2 m_ptLastPos;	//保存上一次点击位置，自动发炮时使用
	bool m_isShowSetting;	//是否显示菜单
	int m_nMenuShowTime;
	int m_current_bullet_mulriple;
	int m_current_bullet_kind;
	bool m_bLockScreen;
	AccountLayer* m_accountLayer;
	AwardLayer*	  m_awardLayer;
	static LrbyClientView *								m_pLrbyClientView;					// 对象指针
	LabelTTF*		 labelMessageTxt;
	std::string	 m_strMessage;
	bool		 m_isEnergy;				//是否是能量炮

	//锁定管理
	LockFishManager* m_LockFishManager;

	//创建对象池
	NetPool *m_netPool;
	//创建鱼网
	void addNet(Vec2 pt, int nNetStatue);

	//播放打中鱼音效
	void playCatchEffect(int nRand,int nFishKind);
	//打中鱼的金币
	void playCatchGold(const Vec2 pos,const int gold, const int chairId);
	void showMassGold();
	void removeMassGold(Node* sender, Node* gold);
public:
	Vector<BulletSprite* > m_vecBullet[BY_GAME_PLAYER];
	Sprite* spWave;
	NodeGrid* m_nodegrid;
	Sprite*	m_spGameBG;
	String	strBg;
public:
	CMD_C_TimeStamp m_TimeStamp;//时间戳
	CannonNode* cannon[BY_GAME_PLAYER];		//炮台
	int m_fireBulletNum[BY_GAME_PLAYER];        //每个玩家发射子弹数
	int m_nCurChair;
	Sprite* spScene;
	NS_lrbyFish::CMD_S_SwitchScene* switch_scene;
	Sprite* m_spCannonION[BY_GAME_PLAYER];		//能量炮台动画
	Sprite* m_lockFishIcon[BY_GAME_PLAYER];		//锁定鱼icon
	void setSuccess(bool isSuccess);
private:
	bool m_bGameConfig;
	//ExchangeGoldLayer* m_pExchangeGold;
	Sound *m_sound;
	float  m_dt;
	std::int8_t m_nBgIndex;
	std::int8_t m_nBgSoundIndex;

	ui::ImageView*									m_imageFace;			//中间背景
	bool	m_isFirst;			//第一次进入
	bool    m_isNext;			//是否可以播放下一条消息
	bool	m_isSuccess;		//是否成功兑换
	bool	m_isShowEntity;		//是否显示碎片
};

#endif // _LKPY_GAMECLIENTVIEW_H__