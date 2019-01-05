#ifndef __LotteryMailBox__
#define __LotteryMailBox__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryMailBox: public Layer, public TableViewDelegate,
	public TableViewDataSource
{
public:
	LotteryMailBox();
	~LotteryMailBox();
	CREATE_FUNC(LotteryMailBox);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(extension::ScrollView* view);
	virtual void scrollViewDidZoom(extension::ScrollView* view);	

	//刷新
	void refreshLayer();

	void displayIndex(const int index);
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	void toNotice(Object *obj);
	void toMail(Object *obj);
	//
	void toInbox(Object *obj);
	void toOutBox(Object *obj);
	void toSendBox(Object *obj);

	//上一页下一页
	void toPre(Object *obj);
	void toNext(Object *obj);

	//删除回复
	void toDel(Object *obj);
	void toReply(Object *obj);

	void getInboxMessageRet(Object *obj);
	void chkInboxMessageRet(Object *obj);
	void getXJUserInfoRet(Object *obj);
	void sendMessageRet(Object *obj);
	void getMessageCountRet(Object *obj);
	void deleteMessageRet(Object *obj);

	//公告返回
	//void getNoticeRet(Object *obj);

	const char* getUtfString(const char* tempStr);

	//查询站内信
	void queryMessageBox();
	//查询人 模糊查找
	void queryMember(const char* name = "");

	//显示下级
	void showSubMember();
	//显示收件人
	void showSendMember();
	//显示主题
	void showSendTopic();

	//刷新layer
	void refreshList(Layer *lyScl);

	void selectButton(Object *obj);

	cocos2d::Size winSize;
	string username;
	DOUBLE money;
	float cellHeight;
	float cellNum;
	TableView *userInfoTable;

	//列表条数
	int row_count;
	Array* m_arrContent;

	//查询所需参数
	int m_iBoxType;
	int m_iPage;
	int m_iPageCount;
	int m_iPageNum;
	int m_iCurRow;
	//上级
	int m_top_id;
	int m_user_page_count;
	int m_user_page_cur;

	bool m_isPosting;
	//发件箱参数
	int m_iCurRevType;

	//弹出的下级人名显示 不一定会在列表里面 这里修改为存储个人相关所有信息
	cocos2d::Vector<Dictionary *> vec_NameId;

	//名字 因为不能一次获取完  这个列表仅用来保存当前返回的值
	Array* m_arrUserInfo;

	//发送次数
	int m_i_sendtimes;
};

#endif