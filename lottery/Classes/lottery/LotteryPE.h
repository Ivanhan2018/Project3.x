#ifndef __LotteryPE__
#define __LotteryPE__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#define EDITBOX_TAG_PE 6950

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

enum PETypeID
{
	PE_peie,PE_fandian
};

class LotteryPE:public Layer, TableViewDelegate, TableViewDataSource
{

public:
	LotteryPE();
	~LotteryPE();	

	CREATE_FUNC(LotteryPE);
	virtual bool init();

	//count是配额个数
	void setUserID(const int userID);
	//配额
	void setPeiE(const double userPeie);

	//设置类型
	void setDisplayType(const int typeID);

	void tableCellTouched(TableView* table, TableViewCell* cell);
	cocos2d::Size cellSizeForTable(TableView *table);
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	ssize_t numberOfCellsInTableView(TableView *table);

	virtual void scrollViewDidScroll(ScrollView* view){}
	virtual void scrollViewDidZoom(ScrollView* view){}

private:

	void confirm(Object *obj);
	void back(Object *obj);
	//查询下级配额
	void xiaJiPeiERet(Object *obj);
	//设置下级配额
	void xiaJiPeiESetRet(Object *obj);

	//下级返点
	void xiaJiFanDian(Object *obj);
	//设置下级返点
	void xiaJiFanDianRet(Object *obj);

	//查询返点

	void initView(); 
	void initData();

	int m_userID;
	int m_typeID;
	int m_userPeie;
	String* m_userName;
	Double* m_fanDian;
	String* m_listFanDian;

	int m_peie[5];
	int m_count;
};

#endif