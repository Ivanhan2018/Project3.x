#ifndef __Game__PlayerInfoLayer__
#define __Game__PlayerInfoLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include "cocos-ext.h"
#include "TabLayer.h"
#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
USING_NS_CC_EXT;

#define PHOTO_TABLE_CELL_BUTTON 1
#define SPRITE_MEDAL_L "Sprite_MedalL"
#define SPRITE_MEDAL_A "Sprite_MedalA"
#define MEDAL_NUM 79
#define MEDAL_TABLE_COLUMN 4
#define ARTICLE_NUM 9
#define ARTICLE_TABLE_COLUMN 5


class MedalTableHelper:public Node, public TableViewDelegate,
	public TableViewDataSource
{ 
private:
	Object *father;
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	float tableGapX;
	float tableGapY;
	int articleTableItemsNum;
	int tableTag;
public:
	MedalTableHelper(Object *fath, int tag);
	~MedalTableHelper();
	//void showMedalInfo(Object *obj);

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
};

class PlayerInfoLayer : public TabLayer,public TableViewDelegate,
	public TableViewDataSource
{
private:

	Menu *pCloseButton;
	Menu *pMenuPlayer;
	Menu *pMenu1;
	Menu *pMenu2;
	Menu* mianfeiMenu;
public:
	PlayerInfoLayer();
	~PlayerInfoLayer();
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	//CREATE_FUNC(PlayerInfoLayer);
	static PlayerInfoLayer *create();
	static PlayerInfoLayer *createWithTabItems(Array *itemNormalName, Array *itemSelectedName);
	virtual void doSomething();
	void pressChangePhoto(Object *pObject);

	void showMedalInfo(Object *obj, Control::EventType event);
		 virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	 virtual cocos2d::Size cellSizeForTable(TableView *table);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
	    virtual void scrollViewDidScroll(ScrollView* view);
    virtual void scrollViewDidZoom(ScrollView* view);
public:
    void updateUserPacket(float dt);
    void changePassWord(Object* obj);
	void changeUserName(Object *obj);
	//void saveChange(Object *obj);
	void changeUserPhoto(Object *obj);
	void dismiss(Object *obj);
	/**��ֹ�ý�������а�ť����**/
	void forbidButtom();
	/**�򿪸ý�������а�ť����**/
	void permitButtom();

	void onSureModifyNickName(Object* obj);
	void onModifyNickNameResult(Object* obj);
	void onModifyTouxiangResult(Object* obj);
	void setTouxiangModifyResult(Object* obj);
	void removeTouxiangLayer(Object* obj);

	void toFreeLayer(Object* obj);

	void rechange(Object* obj);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//�ֻ����ذ�ť
	bool isLegal(string str);
	void reloadArticleData();
	void updateArticle(Object *obj);

private:
	char* m_szModifyNickName;
	EditBox * m_pNickNameInput;
	bool m_bInputNickName;
	const char* m_szMidfyNickNameSave;

	int m_nGenterResult;
	int m_nHeadResult;
	//�û���Ϣ
	int photoNum;
	cocos2d::Size tableViewCellSize;
	Sprite *m_pPlayerPhoto;						//�û�ͷ��
	Sprite *photoBk;								//ͷ�񱳾�
	LabelTTF *m_pUserName;						//�û�����
	LabelTTF *m_pUserSex;							//�û��Ա�
	LabelTTF *pUserGold;							//�û����
	LabelTTF *pUserRecord;						//�û�ս��
	LabelTTF *pUserWinning;						//�û�ʤ��
	TableView *photoTable;						//ѡ��ͷ��
    Menu *changePassMenu;						//ѡ��ͷ��
	//ѫ��
	TableView *medalTable;						//ѫ��
	LabelTTF *labelMedalInfo;
	//����
	TableView *goodsTable;
	Sprite *goodsBk;
	MedalTableHelper *helper1;
};
#endif  //__Game__PlayerInfoLayer__