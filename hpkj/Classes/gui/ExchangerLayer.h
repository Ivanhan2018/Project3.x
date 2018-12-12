#ifndef __Game__ExchangerLayer__
#define __Game__ExchangerLayer__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "PromptBox.h"
USING_NS_CC;
USING_NS_CC_EXT;

enum ExchangeButtonTag
{
	ExchangeButton_Start = 1
};


#define ExchangeTableFileName "Sprite_ExchangeTableBk"
class ExchangerLayer :public Layer, public TableViewDelegate,
	public TableViewDataSource
{

private:
	bool cancelOrNot;
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	float tableGapX;
	float tableGapY;
	int itemNum;
	//∂“ªª
	TableView *exchangeTable;
	//π∫¬Ú
	int currentType;
	bool buyViewVisible;
	Sprite *buyBk;
	Sprite *buyText;
	LabelTTF *buyArticleType1;						//¿‡–Õ±Í«©
	Sprite *buyArticleTypeBk;							//ŒÔ∆∑¿‡–Õµ◊øÚ
	LabelTTF *buyArticleType2;						//ŒÔ∆∑¿‡–Õ
	LabelTTF *buyArticleNumLabel;						// ˝¡ø±Í«©
    //	EditBox *buyArticleNum;							//π∫¬Ú ˝¡ø ‰»ÎøÚ
    LabelTTF *buyArticleNum;							//π∫¬Ú ˝¡ø ‰»ÎøÚ
    Sprite *buyArticleNumBk;
    LabelTTF *contactLabel;						//联系方式标签
	EditBox *contactInput;							//联系方式输入框
	Menu *buyMenu;									//π∫¬Ú»∑∂®»°œ˚≤Àµ•
	PromptBox* promptBox;
private:
	void initView();
public:
	ExchangerLayer();
	~ExchangerLayer();
	CREATE_FUNC(ExchangerLayer);
	virtual bool init();
	void dismiss(Object *obj);
	void setBuyViewVisible(bool flag);
	void pressExchangeButton(Object *obj);
	void pressOk(Object *obj);
	void pressCancel(Object *obj);
    void pressExchange(int id);
	void cancelHttpRequest(Object *obj);
    void onHttpRequestCompleted(Node *sender, void *data);
    bool isNumber(const char *buffer);
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);


public:
};
#endif  //__Game__WarningLayler__