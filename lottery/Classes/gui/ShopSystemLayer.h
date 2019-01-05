#ifndef __Game__ShopSystemLayer__
#define __Game__ShopSystemLayer__

#include "TabLayer.h"
#include "Define.h"
#include "PromptBox.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

enum ShopSystemTableTag
{
	ShopTableTag_Coins = 1,
	ShopTableTag_Article
};

enum ShopSystemBuyButtonTag
{
	BuyButton_Coins = 100,
	BuyButton_Article = 200
};


#define ShopCoinTableFileName "Sprite_ShopCoinTableBk"
#define BuyCoinButtonFileName "Sprite_ShopBuyCoins"
#define ShopArticleTableFileName "Sprite_ShopArticleTableBk"

class ArticleTableHelper:public TableViewDelegate,
	public TableViewDataSource,public Layer
{ 
private:
	Object *father;
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	float tableGapX;
	float tableGapY;
	int articleTableItemsNum;
public:
	ArticleTableHelper(Object *fath);
	~ArticleTableHelper();
	void buyButton(Object *obj);
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
};

class ShopSystemLayer : public TabLayer,public TableViewDelegate,
	public TableViewDataSource
{

private:
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	float tableGapX;
	float tableGapY;
	int coinsTableItemsNum;
	Array *friendStatusData;
	Array *myFriendList;
	Sprite *maskSprite2;
	Sprite *maskSprite3;
	//���?
	TableView *coinsTable;
	//����
	TableView *articleTable;
	//���չ�
	SCORE score;
	SCORE bankScore;
	Sprite *bankBk;
	LabelTTF *scoreLabel;
	LabelTTF *bankScoreLabel;
	Menu *bankMenu;
	//����
	bool buyViewVisible;
	bool depositViewVisible;
	bool withdrawViewVisible;
	bool sendViewVisible;
	Sprite *buyBk;
	Sprite *buyText;
	LabelTTF *buyArticleType1;						//���ͱ�ǩ
	Sprite *buyArticleTypeBk;							//��Ʒ���͵׿�
	LabelTTF *buyArticleType2;						//��Ʒ����
	LabelTTF *buyArticleNumLabel;						//������ǩ
	EditBox *buyArticleNum;							//�������������?
	Menu *buyMenu;									//����ȷ��ȡ���˵�
	int objTag;
	//���?
	Sprite *depositBk;
	Sprite *depositText;
	LabelTTF *depositNumLabel;						//������ǩ
	EditBox *depositNum;								//�������������?
	LabelTTF *depositPasswordLabel;					//������?
	EditBox *depositPassword;							//���������?
	Menu *depositMenu;								//ȷ��ȡ���˵�
	//ȡ��
	Sprite *withdrawBk;
	Sprite *withdrawText;
	LabelTTF *withdrawNumLabel;						//������ǩ
	EditBox *withdrawNum;								//�������������?
	LabelTTF *withdrawPasswordLabel;					//������?
	EditBox *withdrawPassword;							//���������?
	Menu *withdrawMenu;								//ȷ��ȡ���˵�
	//ת��
	Sprite *transferBk;
	Sprite *transferText;
	LabelTTF *transferNickname;						//�ǳƱ�ǩ
	EditBox *nickname;								//�ǳ������?
	LabelTTF *transferNumLabel;						//������ǩ
	EditBox *transferNum;								//�������������?
	LabelTTF *transferPasswordLabel;					//������?
	EditBox *transferPassword;							//���������?
	Menu *transferMenu;								//ȷ��ȡ���˵�

	//���Կ�
	PromptBox *promptBox;
private:
	void initView();
public:
	ShopSystemLayer();
	~ShopSystemLayer();
	static ShopSystemLayer *create();
	static ShopSystemLayer *createWithTabItems(Array *itemNormalName, Array *itemSelectedName);
	virtual void doSomething();
	void refreshView(Object *pObject);
    void buyGoodsInfoRet(Object *pObject);
    void queryBankInfo();
    void updateMoney(float dt);
	void setBuyViewVisible(bool flag);
	void setDepositViewVisible(bool flag);
	void setWithdrawViewVisible(bool flag);
	void setSendViewVisible(bool flag);
	void buyButton(Object *obj);
	void depositButton(Object *obj);
	void withdrawButton(Object *obj);
	void sendButton(Object *obj);
	void pressOk(Object *obj);
	void pressCancel(Object *obj);

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);


public:
};
#endif  //__Game__WarningLayler__