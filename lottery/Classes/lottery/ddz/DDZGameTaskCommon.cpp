#include "DDZGameTaskCommon.h"
#include "GlobalUnits.h"
#include "TaskInfo.h"
#include "DDZDDZRes.h"
#include "DBHandler.h"

int DDZtaskNum[3] = {50,100,500};
int DDZcoinNum[3] = {10000,20000,100000};
bool DDZGameTaskCommon::init()
{
	bool ret=false;
	do 
	{
		Layer::init();

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

		CCSize size=CCDirector::sharedDirector()->getWinSize();
		m_taskBk=Sprite::createWithSpriteFrameName(DDZtexture_name::s_task_k);
		m_taskBk->setAnchorPoint(ccp(0.5,0.5f));
		m_taskBk->setPosition(ccp(size.width*0.5,size.height*0.5));

		Dictionary * strings = Dictionary::createWithContentsOfFile("ddz_strings.xml");
		const char* buff1=((CCString*)strings->objectForKey("font_task"))->getCString();					//任务
		LabelTTF* label=LabelTTF::create(buff1,"黑体",20);
		label->setColor(ccc3(173,118,0));
		label->setAnchorPoint(ccp(0.5f,1.0f));
		label->setPosition(ccp(m_taskBk->getContentSize().width*0.5,m_taskBk->getContentSize().height-30));
		m_taskBk->addChild(label);

		m_tableView =TableView::create(this,cocos2d::Size(450,240));
		m_tableView->setPosition(ccp(50,30));
		//m_tableView->setDirection(kCCScrollViewDirectionVertical);

		m_tableView->setDelegate(this);  
		//m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);  
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

bool DDZGameTaskCommon::onTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
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

void DDZGameTaskCommon::onTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{

}

void DDZGameTaskCommon::onTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{

}

void DDZGameTaskCommon::onTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{

}

void DDZGameTaskCommon::runOutOrIn(bool isVisible)
{
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	m_isOut=!m_isOut; 
	m_tableView->reloadData();
	this->stopAllActions();
	this->runAction(MoveTo::create(0.2f,m_isOut?ccp(this->getPositionX(),size.height*1.0):ccp(this->getPositionX(),size.height*0.00)));
}
void DDZGameTaskCommon::onEnter()
{
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min)(),true);
	Layer::onEnter();
}

void DDZGameTaskCommon::onExit()
{
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
}

cocos2d::CCSize DDZGameTaskCommon::cellSizeForTable(TableView *table )
{
	return cocos2d::Size(100,50);  
}

TableViewCell* DDZGameTaskCommon::tableCellAtIndex(TableView *table, ssize_t idx )
{
	Dictionary * strings = Dictionary::createWithContentsOfFile("xml/strings.xml");
	CCString* strbuff=NULL;

	if (idx>=3)
	{
		strbuff=((CCString*)strings->objectForKey(CCString::createWithFormat("xyx_task%d",idx)->getCString()));	
	}
	else
	{
		const char* buff1=((CCString*)strings->objectForKey(CCString::createWithFormat("xyx_task%d",0)->getCString()))->getCString();							//完成
		const char* buff2=((CCString*)strings->objectForKey(CCString::createWithFormat("xyx_task%d",1)->getCString()))->getCString();							//局任务

		const char* buff3=((CCString*)strings->objectForKey(CCString::createWithFormat("task%d_jl",0)->getCString()))->getCString();								//奖励
		const char* buff4=((CCString*)strings->objectForKey(CCString::createWithFormat("task%d_jl",1)->getCString()))->getCString();								//金币

		int count=DDZtaskNum[idx];
		int num=totalCount;

		strbuff=CCString::createWithFormat("%d%s%d%s      %s%d%s  %d/%d",idx+1,buff1,DDZtaskNum[idx],buff2,
			buff3,DDZcoinNum[idx],buff4,num>count?count:num,count);

		//strbuff=CCString::createWithFormat("%d%s%d%s    %s%d%s  %d/%d",idx+1,buff1,TaskInfo::ShareTaskInfo()->m_nTaskConfig[idx*2],buff2,
		//	buff3,TaskInfo::ShareTaskInfo()->m_nTaskConfig[idx*2+1],buff4,num>count?count:num,count);

	}
	TableViewCell *pCell = table->dequeueCell();
	if (!pCell) {
		pCell = new TableViewCell();
		pCell->autorelease();
		Sprite *pSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_task_d);
		pSprite->setAnchorPoint(Vec2::ZERO);
		pSprite->setPosition(Vec2::ZERO);
		pCell->addChild(pSprite);
		pCell->setTag(idx);

		LabelTTF *pLabel = LabelTTF::create(strbuff->getCString(), "黑体", 20.0);
		pLabel->setAnchorPoint(ccp(0.5,0.5));
		pLabel->setPosition(ccp(pSprite->getContentSize().width*0.5,pSprite->getContentSize().height*0.5));
		pLabel->setTag(123);
		pCell->addChild(pLabel);
		pLabel->setColor(ccc3(173,118,0));
	}
	else
	{
		LabelTTF *pLabel = (LabelTTF*)pCell->getChildByTag(123);
		pLabel->setString(strbuff->getCString());
		pCell->setTag(idx);
	}

	return pCell; 
}

ssize_t DDZGameTaskCommon::numberOfCellsInTableView(TableView *table )
{
	return 4;
}

void DDZGameTaskCommon::tableCellTouched(TableView* table,TableViewCell* cell )
{
	CCLOG("tableCellTouched\n"); 
	// 	CCfaceAndVioce* _data = CCfaceAndVioce::create(msgToLogic_face);
	// 	_data->m_Type =2;
	// 	_data->m_KeyID = cell->getTag();
	// 	NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_data);
	// 	if (s_face) s_face->runOutOrIn();
}

char DDZGameTaskCommon::m_taskBUff[1024];

