#ifndef __Game__PromptBox__
#define __Game__PromptBox__

#include "cocos2d.h"
#include "CocosUnits.h"
USING_NS_CC;


typedef enum
{
	mPromptTypeServerShut,
	mPromptTypeLoginFaild,
	mPromptTypePasswordFind,
	mPromptTypeGetChargeFaild,
	mPromptTypeErrorPhone,
	mPromptTypeTwoInputError,
	mPromptTypeErrorQQNumber,
	mPromptTypeErrorNickName,
	mPromptTypeSeverPasswordFaild,
	mPromptTypeLoginLegal,
	mPromptTypeMakeSureModifyNickName,
	mPromptTypeNosever,
	mPromptTypeGetError,
	mPromptTypeLackCoins,
	mPromptStartFaild,	
	mPromptVersionNew, 
	mPromptNickNameError,
	mPromptpasswordUsed,
	mPromptexitSelect,
	mPromptTypeServer,
	mPromptUsedCHINESE
} mPromptType;

class  PromptBox : public Layer 
{
public:
	static PromptBox* PromptBoxWith(Vec2 pos , mPromptType type);

	bool initPromptBox(Vec2 pos,mPromptType type);
	PromptBox();
	~PromptBox();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
	void close(Object *obj);
	void closePromptBox(Object * obj);
	void setPromptText(const char* szDes);
	void onActionDown(Node* obj);
	void okButton(Object* obj);
	void cannelButton(Object* obj);
	void onOverRegist(Object* obj);
	void setLevelAndX(int nLevel,int nXXX,bool isLack);
    void setBackgroundScale(float scale);
	void exitGame(Object* obj);
private:
	const char * getTheCharWithType(mPromptType type);
    Sprite * back;
	LabelTTF* m_pAlartWord;
	Menu * m_pBackMenu;
	bool m_bBackMenu;
	Menu* m_pOkMenu;
	bool m_bOkMenu;
	mPromptType m_nTypes;
	int m_nLevel;
	int m_nXXX;
	bool m_bIsLack;
};


#endif
