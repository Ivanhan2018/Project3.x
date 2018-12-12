#ifndef __Game__GuessCoinSide__
#define __Game__GuessCoinSide__

#include "cocos2d.h"
using namespace cocos2d;

class GuessCoinSide : public Layer
{
private:
	Sprite * pBJ2;
	Sprite *coinSprite;
	bool canRotate;
	SCORE score;
	SCORE betScore;
	int betSide;
	LabelTTF *scoreLabel;
	LabelTTF *betScoreLabel;
	LabelTTF *betInfoLabel;
    SCORE scoreEarned;
    bool coinSideRet;
public:
	GuessCoinSide();
	~GuessCoinSide();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(GuessCoinSide);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
public:
	void dismiss(Object *obj);
	void initView();
    void initLabel();
    void updateMoney(float dt);
	//void onDiscount(Object* obj);
	//void onBecomeVIP(Object* obj);
	void showHeadFrame();
	void showTailFrame();
	void toZero();
	void pressCancel(Object *obj);
	void pressChip(Object *obj);
	void pressConfirm(Object* obj);
	void betHead(Object* obj);
	void betTail(Object* obj);
	void coinRotate(int tag);
	void coinRotateEnd(Node *obj);
	void addRandSeed();
	void queryBankInfo();
	void refreshView(Object *pObject);
    void guessCoinRet(Object *pObject);
    void refreshScoreLabel();
};
#endif  //__Game__GessCoinSide__