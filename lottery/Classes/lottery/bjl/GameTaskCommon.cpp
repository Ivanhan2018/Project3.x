#include "GameTaskCommon.h"
#include "GlobalUnits.h"
#include "TaskInfo.h"
#include "DDZRes.h"
#include "DBHandler.h"
#include "CocosUnits.h"//by hxh
#include "MyConfig.h"//by hxh
#include "ToolKit.h"

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
		sprintf(sql, "select * from UserInfo where dwUserID=%ld",EntityMgr::instance()->getDispatch()->m_dwUserID);
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
		//m_taskBk=Sprite::createWithSpriteFrameName(texture_name::s_task_k);
		m_taskBk=Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_bankerlist));//<by hxh>
		m_taskBk->setAnchorPoint(ccp(0.5,0.5f));
		m_taskBk->setPosition(ccp(size.width*0.5,size.height*0.5));
#if 0
		Dictionary * strings = Dictionary::createWithContentsOfFile("strings.xml");//by hxh
		const char* buff1=((String*)strings->objectForKey("font_task"))->getCString();					//任务
		LabelTTF* label=LabelTTF::create(buff1,"Arial",20);//黑体修改成Arial<by hxh>
		label->setColor(ccc3(173,118,0));
		label->setAnchorPoint(ccp(0.5f,1.0f));
		label->setPosition(ccp(m_taskBk->getContentSize().width*0.5,m_taskBk->getContentSize().height-30));
		m_taskBk->addChild(label);
#endif
		// 创建垂直TableView，其中垂直填充顺序是TopDown<by hxh>
		m_tableView =TableView::create(this,cocos2d::Size(myConfig.m_ViewSize[0],myConfig.m_ViewSize[1]));

		m_tableView->setPosition(ccp(myConfig.m_ViewPos[0],myConfig.m_ViewPos[1]));
		//m_tableView->setDirection(kCCScrollViewDirectionVertical);
		m_tableView->setAnchorPoint(Vec2::ZERO);//<by hxh>
		m_tableView->setDelegate(this);
		//m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);//从上向下渲染单元格<by hxh>  
		m_tableView->reloadData();  
		m_tableView->setTouchEnabled(true);
		m_taskBk->addChild(m_tableView, 1);  
		this->addChild(m_taskBk);
		this->setPosition(ccp(this->getPositionX(),size.height*1.0));
		memset(m_taskBUff,0,sizeof(m_taskBUff));
		//setTouchMode(kCCTouchesOneByOne);
		//setTouchEnabled(true);
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
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min)(),true);
	Layer::onEnter();
}

void GameTaskCommon::onExit()
{
	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
}

cocos2d::Size GameTaskCommon::cellSizeForTable( TableView *table )
{
	MyConfig& myConfig=MyConfig::Instance();
	return cocos2d::Size(myConfig.m_CellSize[0],myConfig.m_CellSize[1]);
}

TableViewCell* GameTaskCommon::tableCellAtIndex( TableView *table, ssize_t idx )
{
	String* strbuff=NULL;
#if 0
	Dictionary * strings = Dictionary::createWithContentsOfFile("strings.xml");//by hxh
	if (idx>=3)
	{
		strbuff=((String*)strings->objectForKey(String::createWithFormat("xyx_task%d",idx)->getCString()));	
	}
	else
	{
		const char* buff1=((String*)strings->objectForKey(String::createWithFormat("xyx_task%d",0)->getCString()))->getCString();							//完成
		const char* buff2=((String*)strings->objectForKey(String::createWithFormat("xyx_task%d",1)->getCString()))->getCString();							//局任务

		const char* buff3=((String*)strings->objectForKey(String::createWithFormat("task%d_jl",0)->getCString()))->getCString();								//奖励
		const char* buff4=((String*)strings->objectForKey(String::createWithFormat("task%d_jl",1)->getCString()))->getCString();								//金币

		int count=taskNum[idx];
		int num=totalCount;

		strbuff=String::createWithFormat("%d%s%d%s      %s%d%s  %d/%d",idx+1,buff1,taskNum[idx],buff2,
			buff3,coinNum[idx],buff4,num>count?count:num,count);

		//strbuff=String::createWithFormat("%d%s%d%s    %s%d%s  %d/%d",idx+1,buff1,TaskInfo::ShareTaskInfo()->m_nTaskConfig[idx*2],buff2,
		//	buff3,TaskInfo::ShareTaskInfo()->m_nTaskConfig[idx*2+1],buff4,num>count?count:num,count);

	}
#endif
	TableViewCell *pCell = table->dequeueCell();
	if (!pCell) {
		pCell = new TableViewCell();
		pCell->autorelease();
		setCellData(pCell,idx);
	}
	else
	{
		pCell->setTag(idx);
		pCell->removeAllChildren();
		setCellData(pCell,idx);
	}

	return pCell; 
}

ssize_t GameTaskCommon::numberOfCellsInTableView( TableView *table )
{
	//MyConfig& myConfig=MyConfig::Instance();
	//return myConfig.m_NumberOfCells;
	return m_ApplyBankerList.size();
}

void GameTaskCommon::tableCellTouched( TableView* table, TableViewCell* cell )
{
	CCLOG("tableCellTouched\n"); 
	// 	CCfaceAndVioce* _data = CCfaceAndVioce::create(msgToLogic_face);
	// 	_data->m_Type =2;
	// 	_data->m_KeyID = cell->getTag();
	// 	NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_data);
	// 	if (s_face) s_face->runOutOrIn();
}

void GameTaskCommon::setCellData(TableViewCell *pCell,int i)
{
	MyConfig& myConfig=MyConfig::Instance();
	//创建单元格
	int Num=m_ApplyBankerList.size();
	//int i=idx;
	DWORD dwUserID=m_ApplyBankerList[i];
	auto selfUserID = EntityMgr::instance()->roomFrame()->getMeUserDate()->dwUserID;
	CUserItem* pUserItem=(CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(dwUserID);
	if (pUserItem)
	{
		string strNickName=pUserItem->getUserNickName();
		if (selfUserID != dwUserID)
		{
			strNickName = ToolKit::dealStringOfPrivacy(strNickName);
		}

		//strNickName="abcdef123456";
		LabelTTF *pLabel0 = LabelTTF::create("", "Arial", 20.0);//昵称
		pLabel0->setString(strNickName);
		pLabel0->setAnchorPoint(ccp(0,0.5));
		pLabel0->setPosition(ccp(myConfig.m_LblPos[0][0],myConfig.m_LblPos[0][1]));
		pLabel0->setTag(i+100*0);
		pCell->addChild(pLabel0);
		pLabel0->setColor(ccc3(173,118,0));

		LONGLONG lScore=pUserItem->getUserGold();
		String *strbuff=String::createWithFormat("%lld",lScore);
		LabelTTF *pLabel1 = LabelTTF::create("","Arial", 20.0);//金币
		pLabel1->setString(strbuff->getCString());
		pLabel1->setAnchorPoint(ccp(0,0.5));
		pLabel1->setPosition(ccp(myConfig.m_LblPos[1][0],myConfig.m_LblPos[1][1]));
		pLabel1->setTag(i+100*1);
		pCell->addChild(pLabel1);
		pLabel1->setColor(ccc3(173,118,0));

		//		// 测试用	
		//#ifdef USE_TEST_TABLEVIEW
		//			String *strbuff2=String::createWithFormat("%d",idx);
		//			LabelTTF *pLabel2 = LabelTTF::create(strbuff2->getCString(),"Arial", 20.0);//行号
		//			pLabel2->setAnchorPoint(ccp(0.5,0.5));
		//			pLabel2->setPosition(ccp(myConfig.m_LblPos[1][0]+100,myConfig.m_LblPos[1][1]));
		//			pLabel2->setTag(i+100*2);
		//			pCell->addChild(pLabel2);
		//			pLabel2->setColor(ccc3(173,118,0));
		//#endif
	}
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

