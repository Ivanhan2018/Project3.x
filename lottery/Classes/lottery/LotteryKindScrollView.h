#ifndef __Game__LotteryKindScrollView__
#define __Game__LotteryKindScrollView__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

# ifndef DELTA
# define DELTA 10
# endif
#define LOTTERY_KIND_SPRITE_TAG			0
#define GAME_KIND_SPRITE_TAG			100
#define PHY_KIND_SPRITE_TAG				200
#define INVALID_TAG					-100
#define MAX_GAME				7

USING_NS_CC;
USING_NS_CC_EXT;

static int gameSortList[MAX_GAME] = {3,4,5,6,2,1,7};
static int gameKindIdList[MAX_GAME] = {KindId_DZPK,KindId_DDZ,KindId_BJL,KindId_FISH,KindId_PPC,KindId_BRNN,KindId_SGLY};

class LotteryKindScrollView : public Layer,TableViewDataSource,TableViewDelegate 
{
    
public:
    LotteryKindScrollView();
    ~LotteryKindScrollView();
    virtual bool init();
    static Scene* scene();
    void pressKindButton(int tag);
    void connectGameRoomServer(int tag);//by hxh
    void connectGameRoomServerEx(int tag,int serverId);//by hxh
    CREATE_FUNC(LotteryKindScrollView);

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//触摸事件 计算的是点击的是那个子页
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	//每一项的高度和宽度
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	//生成列表的每一项内容
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	//一共多少项
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	//刷新
	void refreshLayer();
public:
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *pTouch, Event *pEvent);
   
	//itype 0为游戏 1为彩票 2为体育
	void resetTable(const char* preName,const int count,const int iType);

private:
	int selectedSpriteTag;
	std::vector<int> m_Data;
	int m_Count;
	int m_iType;
	string m_name;
	TableView* m_table;
	TableViewCell* pCell;
	Vec2 start_pos;
};
#endif  //__Game__HelpScrollView__