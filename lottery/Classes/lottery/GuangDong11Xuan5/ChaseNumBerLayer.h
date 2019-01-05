#ifndef _ChaseNumBerLayer_H
#define _ChaseNumBerLayer_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "GuangDong11Xuan5WuFen.h"
#include "Define.h"
#include "LotteryGlobal.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
enum ChaseNutagType
{ 
	ChaseNuZhuiHaowuTag = 0,                                   //
	ChaseNuZhuiHaoShiwuTag,
	ChaseNuZhuiHaoerShiTag,
	ChaseNuZhuiHaoerShiwuTag,                                           //
};
class ChaseNumBerLayer : public Layer, public TableViewDelegate,
	public TableViewDataSource, public EditBoxDelegate
{
public:
	ChaseNumBerLayer();
	~ChaseNumBerLayer();
	CREATE_FUNC(ChaseNumBerLayer);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);	

	virtual void editBoxEditingDidBegin(EditBox* editBox){};
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text) {};
	virtual void editBoxReturn(EditBox* editBox){};
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	void setting(Object* obj);
	//transaction menu
	void recharge(Object *obj);
	void withdraw(Object* obj);
	//record menu
	void betRecord(Object *obj);
	void touZhuContinueRet(Object *obj);
	void ZhuiHaoQiShuRecord(Object *obj);
	void contiRecord(Object *obj);
	void changeKind(Object *obj);//
	void XuanZeQiShu(Object *obj);
	void IsEndeBeiTou(Object *obj);
	void IsEndeZhuiHao(Object *obj);
	void XuanZeZhuiHaoQiShu(Object *obj);
	void TouZhuZhuiHaoButton(Object *obj);
	void ZhuiHaoCloseButton(Object *obj);
	void ShowZhuiHaoMen();
	void dissZhuiHaoMen();

	void IsFendanHourRet(Object *obj);

	cocos2d::Size winSize;
	TableView *touZhuTable;
	//开奖记录
	EditBox *tempItemInpu;
	LabelTTF *gameTypeLabel;
	LabelTTF *gameKindLabel;
	LabelTTF *qiShuLabel;
	LabelTTF *haoMaLabel;
	LabelTTF *timeLabel;
	LabelTTF *TouZhuJinEChaseNumBerLabel;
	Sprite *gap1;
	LabelTTF *gameTypeLabe2;
	LabelTTF *gameKindLabe2;
	LabelTTF *qiShuLabe2;
	LabelTTF *haoMaLabe2;
	LabelTTF *timeLabe2;
	LabelTTF *tempIttf;
	Sprite *gap2;
	EditBox *beilvInput;
	EditBox *CtratebeilvInput;
	MenuItemSprite *XuanZeQiShuItem;
	MenuItemSprite *StramItem;
	MenuItemSprite *CloseItem;
	MenuItemSprite *betRecordItem;
	MenuItemSprite *GetRecordItem;
	MenuItemSprite *ButtonCodeRecordItem;
	Menu *pCloseButton;
	Menu *recordMenu;
	Menu *recordMenu2;
	Menu *middleMenu;
	Menu *StramMenu2;
	Menu *ZhuiHaoQiShuQiShMenu;
	bool ZhuiHaoQiShuBool;
	bool ZhuiHao11Xuan5KIsShowQiShu;
	int ZhuiHaoBool;  //中奖后是否停止追号，0是 1否
	int BeiTouBool;   //是否倍投，1正在倍投 0否
	int zhuiHaoQiShu;  //追号期数
	float cellHeight;
	int BaoCunQiShu;  //保存选择期数
	int Mkind;
	float nMoshiNumber;
	float TouZhuJinENumber;
	int betZhuiHaoZhuShu;
	int nMoshi;
	int ZhuiHao;
	int QuanJunBeiShu;   //保存当前设置的倍数，设为全局变量
	DWORD GDselectedItemTagNumber;  //当前期数
	BYTE chaseStatus[MAX_QIHAO];			//勾选框的状态，0位未选中，1为选中,默认为0
	int chaseInputStatus[MAX_QIHAO];    //输入框的值	默认为1
	float chaseInputMonreny[MAX_QIHAO];    //金钱的值	默认为1
	bool ZhuiHaoBeiShu;
	Integer *gameKyTe;            //彩票种类
	Integer *gameKind;            //游戏种类
	Integer *betZhuShu;           //注数
	Integer *nMoshiteger;         //模式
	Float *ZhangHuYuENumber;      //账户余额
	String *betNumber;            //彩票数字
	    /**********/
    Node *m_TableNode;
    Sprite *fandianTableBk;
    TableView *fandianTable;
    cocos2d::Size tableViewSize;
    float tableCellWidth;
    float tableCellHeight;
    float tableCellNum;
    int fandianStatus;              //0，1，2分别表示不可见，点击设置倍数，设置倍数，默认为0

};

#endif // __HELLOWORLD_SCENE_H__