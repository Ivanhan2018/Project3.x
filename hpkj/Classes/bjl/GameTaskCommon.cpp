#include "GameTaskCommon.h"
#include "globalUnits/GlobalUnits.h"
#include "gui/TaskInfo.h"
#include "BJLRes.h"
#include "DBHandler.h"
#include "CocosUnits.h"
#include "MyConfig.h"

int taskNum[3] = {50,100,500};
int coinNum[3] = {10000,20000,100000};
bool GameTaskCommon::init()
{
	bool ret=false;
	do 
	{
		Layer::init();

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameTaskCommon::onTouchBegan,this);
		listener->onTouchEnded = CC_CALLBACK_2(GameTaskCommon::onTouchEnded,this);
		listener->onTouchMoved = CC_CALLBACK_2(GameTaskCommon::onTouchMoved,this);
		listener->onTouchCancelled = CC_CALLBACK_2(GameTaskCommon::onTouchCancelled, this);

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		DBHandler *handler = DBHandler::sharedDBHandler();
		sqlite3 *DB = handler->getDB();
		sqlite3_stmt *stmt;
		char sql[200]={0};
		sprintf(sql, "select * from UserInfo where dwUserID=%ld",pMsgDispatch->m_dwUserID);
		int rv = sqlite3_prepare(DB, sql, 100, &stmt, NULL);
		rv = sqlite3_step(stmt);
		if(rv == SQLITE_ROW)
		{
			LONG lWinCount = sqlite3_column_int(stmt, 5);
			LONG lLostCount = sqlite3_column_int(stmt, 6);
			LONG lDrawCount = sqlite3_column_int(stmt, 7);
			LONG lFleeCount = sqlite3_column_int(stmt, 8);
			totalCount=lWinCount+lLostCount+lDrawCount+lFleeCount;
		}else
		{
			totalCount = 0;
		}
		sqlite3_finalize(stmt);

		MyConfig& myConfig=MyConfig::Instance();

		cocos2d::Size size=Director::getInstance()->getWinSize();
		m_taskBk=Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_bankerlist));
		m_taskBk->setAnchorPoint(ccp(0.5,0.5f));
		m_taskBk->setPosition(ccp(size.width*0.5,size.height*0.5));

		// 创建垂直TableView，其中垂直填充顺序是TopDown
		m_tableView =TableView::create(this,cocos2d::Size(myConfig.m_ViewSize[0],myConfig.m_ViewSize[1]));

		m_tableView->setPosition(ccp(myConfig.m_ViewPos[0],myConfig.m_ViewPos[1]));
		m_tableView->setAnchorPoint(Vec2::ZERO);
		m_tableView->setDelegate(this);
		m_tableView->reloadData();  
		m_tableView->setTouchEnabled(true);
		m_taskBk->addChild(m_tableView, 1);  
		this->addChild(m_taskBk);
		this->setPosition(ccp(this->getPositionX(),size.height*1.0));
		memset(m_taskBUff,0,sizeof(m_taskBUff));
		m_isOut =true;
		ret =true;
	} while (0);
	return true;
}

bool GameTaskCommon::onTouchBegan(Touch *pTouch,Event *pEvent)
{
	Vec2 pt =pTouch->getLocation();
	if (m_isOut==true)return false;
	if (!m_taskBk->boundingBox().containsPoint(pt))
	{
		runOutOrIn();
		return true;
	}
	return true;
}

void GameTaskCommon::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void GameTaskCommon::onTouchEnded(Touch *pTouch,Event *pEvent)
{

}

void GameTaskCommon::onTouchCancelled(Touch *pTouch,Event *pEvent)
{

}

void GameTaskCommon::runOutOrIn(bool isVisible)
{
	cocos2d::Size size=Director::getInstance()->getWinSize();
	m_isOut=!m_isOut; 
	m_tableView->reloadData();
	this->stopAllActions();
	this->runAction(MoveTo::create(0.2f,m_isOut?ccp(this->getPositionX(),size.height*1.0):ccp(this->getPositionX(),size.height*0.00)));
}
void GameTaskCommon::onEnter()
{
	Layer::onEnter();
}

void GameTaskCommon::onExit()
{
	Layer::onExit();
}

cocos2d::Size GameTaskCommon::cellSizeForTable( TableView *table )
{
	MyConfig& myConfig=MyConfig::Instance();
	return cocos2d::Size(myConfig.m_CellSize[0],myConfig.m_CellSize[1]);
}

TableViewCell* GameTaskCommon::tableCellAtIndex( TableView *table, ssize_t idx )
{
	MyConfig& myConfig=MyConfig::Instance();
	CCDictionary * strings = CCDictionary::createWithContentsOfFile("bjl/xml/strings.xml");//
	String* strbuff=NULL;
	TableViewCell *pCell = table->dequeueCell();
	if (!pCell) {
		pCell = new TableViewCell();
		pCell->autorelease();

		//创建单元格
		int Num=m_ApplyBankerList.size();
	    int i=idx;
		DWORD dwUserID=m_ApplyBankerList[i];
		CUserItem* pUserItem=(CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(dwUserID);
		if (pUserItem)
		{
			string strNickName=pUserItem->getUserNickName();
			LabelTTF *pLabel0 = LabelTTF::create("", "Arial", 20.0);//昵称
			pLabel0->setString(strNickName);
			pLabel0->setPosition(ccp(myConfig.m_LblPos[0][0],myConfig.m_LblPos[0][1]));
			pLabel0->setTag(i+100*0);
			pCell->addChild(pLabel0);
			pLabel0->setColor(ccc3(173,118,0));

			String *strbuff=String::createWithFormat("%d",pUserItem->getUserGold());
			LabelTTF *pLabel1 = LabelTTF::create("","Arial", 20.0);//金币
			pLabel1->setString(strbuff->getCString());
			pLabel1->setPosition(ccp(myConfig.m_LblPos[1][0],myConfig.m_LblPos[1][1]));
			pLabel1->setTag(i+100*1);
			pCell->addChild(pLabel1);
			pLabel1->setColor(ccc3(173,118,0));
		}
	}
	else
	{
		pCell->setTag(idx);
	}

	return pCell; 
}

ssize_t GameTaskCommon::numberOfCellsInTableView( TableView *table )
{
	return m_ApplyBankerList.size();
}

void GameTaskCommon::tableCellTouched( TableView* table, TableViewCell* cell )
{
	CCLOG("tableCellTouched\n"); 
}

void GameTaskCommon::setData(const vector<DWORD>&PlayerList)
{
	m_ApplyBankerList.clear();
	vector<DWORD>::const_iterator iter=PlayerList.begin();
	for(;iter!=PlayerList.end();++iter)
	{
		m_ApplyBankerList.push_back((DWORD)*iter);
	}
	//加载数据
	m_tableView->reloadData();
}

bool GameTaskCommon::IsInApplyBankerList(DWORD dwUserID)
{
	bool bIsApply=false;
	vector<DWORD>::const_iterator iter=m_ApplyBankerList.begin();
	for(;iter!=m_ApplyBankerList.end();++iter)
	{
		if(*iter==dwUserID)
		{
		   bIsApply=true;
		   break;
		}
	}
	return bIsApply;
}

char GameTaskCommon::m_taskBUff[1024];

