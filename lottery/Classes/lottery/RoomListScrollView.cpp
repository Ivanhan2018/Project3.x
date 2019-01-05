#include "RoomListScrollView.h"
#include "MyNSString.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "MyConfig.h"//by hxh

USING_NS_CC;
using namespace cocos2d;
using namespace ui;

RoomListScrollView::RoomListScrollView():
	m_idx(-1),
	m_Count(25),
	m_length(2),
	m_itemHeigth(352),
	m_itemWidth(530),
	m_pandingX(5),
	m_pandingY(15)
{
    
}

RoomListScrollView::~RoomListScrollView()
{

}
bool RoomListScrollView::init(int idx)
{
    bool bRet = false;
    do
    {
        if (!Layer::init())
        {
            return false;
        }

		this->setContentSize(cocos2d::Size(m_itemWidth, m_itemHeigth * 2 + m_pandingY * 2));

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(RoomListScrollView::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(RoomListScrollView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(RoomListScrollView::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(RoomListScrollView::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#if 1
		m_idx=idx;
		int KindId=RoomLayer::getKindId(m_idx);
		m_RoomList=EntityMgr::instance()->getDispatch()->getGameServerList(KindId);
		m_Count=m_RoomList.size();
#endif		
		m_table = TableView::create(this, cocos2d::Size(m_itemWidth, m_itemHeigth * 2 + m_pandingY * 2));	
		m_table->setDelegate(this);
		m_table->setDirection(TableView::Direction::VERTICAL);
		m_table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		this->addChild(m_table, 0);
		m_table->reloadData();

		setTouchEnabled(true);

        bRet = true;
    } while (0);
    
    return bRet;
}

void RoomListScrollView::onEnter()
{
    Layer::onEnter();
    //
    //Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);
}

void RoomListScrollView::onExit()
{
    //
    Layer::onExit();
}

bool RoomListScrollView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}
void RoomListScrollView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
   
}

void RoomListScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	
}

void RoomListScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

void RoomListScrollView::scrollViewDidScroll(ScrollView* view)
{

}

void RoomListScrollView::scrollViewDidZoom(ScrollView* view)
{

}

void RoomListScrollView::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

cocos2d::Size RoomListScrollView::cellSizeForTable(TableView *table)
{
	cocos2d::Size size = cocos2d::Size(m_itemWidth, m_itemHeigth + m_pandingY); //高度定死
	return size;
}

TableViewCell* RoomListScrollView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	MyConfig& myConfig=MyConfig::Instance();
	int platform=(myConfig.m_platform>=0 && myConfig.m_platform<MAX_PLATFORM_NUM?myConfig.m_platform:0);

	TableViewCell* cell = new TableViewCell();

	for(int i = 0 ; i < m_length; i ++)
	{	
		
		int data_index = idx * m_length + i + 1;//data_index <= m_Count
		if (data_index > m_Count)
		{
			break;
		}

		int column = data_index % m_length;//列

		int errorX = 0;
		int errorY = -10;
		if (column == 1)
		{
			errorX = -12;
			errorY = -8;
		}
		else if (column == 2)
		{
			errorX = 0;
			errorY = -10;
		}
		else if (column == 0)
		{
			errorX = 10;
			errorY = -5;
		}

		tagGameServer &server=m_RoomList.at(data_index-1);
		
		Sprite *kindSprite = Sprite::create("room.png");
		kindSprite->setPosition(Vec2(m_itemWidth * (i * 2 + 1) / (m_length * 2) + errorX, m_itemHeigth / 2.0f + errorY));
		cell->addChild(kindSprite);	

		char strServerName[1000] = {0};
	    MyNSString::wc2mbs(server.szServerName,32,strServerName);

		float fArr[4]={25,20,20,20};
		string strArr[4];
		strArr[0]= strServerName;
		strArr[1]= String::createWithFormat("最低底注:%.2lf",server.lServerScore)->getCString();
		strArr[2]= String::createWithFormat("进入限制:%.2lf",server.lMinServerScore)->getCString();
		strArr[3]= String::createWithFormat("在线人数:%d",server.dwOnLineCount)->getCString();
		for(int j=0;j<4;j++)
		{
			auto txt = LabelTTF::create("","Arial",fArr[j]);
			string str = strArr[j];
			if(j>0){
			   MyNSString::GBKToUTF8(str);
			}
			txt->setString(str.c_str());
			txt->setColor(platform==0?ccc3(0,0,0):ccc3(255,255,255));
			txt->setPosition(Vec2(m_itemWidth * (i * 2 + 1) / (m_length * 2) + errorX, m_itemHeigth * 0.12-27*j+95));
			cell->addChild(txt);
		}

		Sprite *pNormalButton = Sprite::createWithSpriteFrame(spriteFrame("EnterGame.png"));
		Sprite *pSelectButton = Sprite::createWithSpriteFrame(spriteFrame("EnterGame_2.png"));
		// 进入游戏
		MenuItemSprite *pItemSprite = MenuItemSprite::create(pNormalButton,pSelectButton,[=](Ref* ){
			__Array *arr=__Array::create();
			arr->addObject(Integer::create(m_idx));
			arr->addObject(Integer::create(server.wServerID));
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATEDOWNLOAD,arr);
		});
		Menu *pButton = Menu::create(pItemSprite,NULL);
		pButton->setPosition(kindSprite->getPosition()+Vec2(0,-220));
		cell->addChild(pButton);
	}

	return cell;
}

ssize_t RoomListScrollView::numberOfCellsInTableView(TableView *table)
{
	
	//返回行数
	int result = m_Count / m_length;
	int tempLast = m_Count % m_length; //除2有余数，则加1
	if (tempLast != 0) result ++;
	return result;
	
	//return m_Count;
}
