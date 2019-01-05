#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "LrbyGameConfig.h"

class CGameScene : public cocos2d::Scene
{
	cocos2d::Sprite*			m_WaitDistribute;
	static CGameScene *								m_pGameScene;					// 对象指针
public:
	//static CGameScene* create();
	CGameScene(void);
	~CGameScene(void);
	virtual bool init();
	static Scene *createScene();
	CREATE_FUNC(CGameScene);
	static CGameScene* GetInstance() { return m_pGameScene; }
	void StartGame();
	void OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void anycLoadPicture(float dt);
	// 房间消息
	void onGameSinkUpdate(cocos2d::Ref *pRef);
	void onSliderCallBack(cocos2d::Ref* sender, cocos2d::ui::SliderEventType type);
	void loadingTextureCallBack(cocos2d::Texture2D* texture);

	int m_nNumOfLoad;
	//滑动条
	cocos2d::ui::Slider*		m_sldLoad;

	void MessageBox(const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag=0);

private:
	void sendEnterGameMsg(float dt);
};

#endif // !_GAMESCENE_H_
