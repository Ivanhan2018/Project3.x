#include <math.h>
#include "RoomLayer.h"
#include "LotteryKindScrollView.h"
#include "MovingLabelLayer.h"
#include "CMD_GameServer.h"
#include "SceneView.h"

using namespace cocos2d;

float myabs(float a)
{
    return a>0?a:-a;
}

LotteryKindScrollView::LotteryKindScrollView()
{
    
}
LotteryKindScrollView::~LotteryKindScrollView()
{

}
Scene* LotteryKindScrollView::scene()
{
    Scene * scene = NULL;
    do
    {
        scene = Scene::create();
        CC_BREAK_IF(! scene);
        LotteryKindScrollView *layer = LotteryKindScrollView::create();
        CC_BREAK_IF(! layer);
        scene->addChild(layer);
    } while (0);
    return scene;
}
bool LotteryKindScrollView::init()
{
    bool bRet = false;
    do
    {
        if (!Layer::init())
        {
            return false;
        }

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(LotteryKindScrollView::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(LotteryKindScrollView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(LotteryKindScrollView::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(LotteryKindScrollView::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_Count = 0;
		selectedSpriteTag = INVALID_TAG;
		pCell = NULL;
		start_pos = Vec2(0,0);
		
		m_table = TableView::create(this, cocos2d::Size(SCREEN_WIDTH, 604));	
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

//彩种ID：1--开始
//游戏ID：101，102，103....
void LotteryKindScrollView::pressKindButton(int tag)
{
    playButtonSound();
    int pressedGameKind = 0;
	if(tag - GAME_KIND_SPRITE_TAG == 3)//3 百家乐
	{
		bool ret=pMsgDispatch->connectGameServer(KindId_BJL);
		if (!ret)
		{
			PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
			this->addChild(box,20);
			return;
		}
		pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
		pMsgDispatch->SendPacketWithEnterRoomPageCount(6);
		pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
		pMsgDispatch->setStartType(true);	
	} else if(tag - GAME_KIND_SPRITE_TAG == 4) //4 捕鱼
	{
		bool ret=pMsgDispatch->connectGameServer(GameId_dzpk);
		if (!ret)
		{
			PromptBox* box = PromptBox::PromptBoxWith(ccp(427,240),mPromptTypeServerShut);
			this->addChild(box,20);
			return;
		}
		pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
		pMsgDispatch->SendPacketWithEnterRoomPageCount(1);
		pMsgDispatch->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
		pMsgDispatch->setStartType(true);
		//Director::getInstance()->replaceScene(DZPKLayer::scene());
	}
	else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
        Director::getInstance()->getRunningScene()->addChild(layer,255);	
    }
}

void LotteryKindScrollView::onEnter()
{
    Layer::onEnter();
}

void LotteryKindScrollView::onExit()
{
    Layer::onExit();
}

bool LotteryKindScrollView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
       if(pTouch->getLocation().y > 150 + 604 || pTouch->getLocation().y < 150) return false;	

	start_pos = pTouch->getLocation();

    return true;
}
void LotteryKindScrollView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
   
}

void LotteryKindScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_table == NULL || selectedSpriteTag != INVALID_TAG) return;

	  Vec2 pos = pTouch->getLocation();	

	  if(pos.getDistance(start_pos) > 10) return;
		
	  int tempCount = m_Count / 4 + 1;

	  for(int cell_id = 0;cell_id < tempCount;cell_id ++)
	  {
			TableViewCell* tempCell = m_table->cellAtIndex(cell_id);
			if (tempCell == NULL) continue;	
			
			Vec2 pos1 = tempCell->convertToNodeSpace(pos);
			bool isFind = false;
			for(int i = 0;i < 4;i ++)
			{
				int idx = cell_id * 4 + i; //从1开始
				idx = m_Data.at(idx);
				Sprite* sptCell = (Sprite* )tempCell->getChildByTag(idx + LOTTERY_KIND_SPRITE_TAG);
				if(sptCell == NULL) continue;		
				
				if (sptCell->boundingBox().containsPoint(pos1))   
				{
					selectedSpriteTag = idx + LOTTERY_KIND_SPRITE_TAG;//避免连续点击
					sptCell->setScale(0.9f);
					sptCell->runAction(ScaleTo::create(0.2f,1.0f));
					pressKindButton(selectedSpriteTag);
					selectedSpriteTag = INVALID_TAG;					
					isFind = true;						
					break;					
				}
			}
			if(isFind) break;
	  }	
}

void LotteryKindScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

//itype 0为游戏 1为彩票 2为体育
void LotteryKindScrollView::resetTable(const char* preName,const int count,const int iType)
{

	m_Data.clear();

	m_Count = count;
	m_name = preName;	

	int table_idx = GAME_KIND_SPRITE_TAG;
	if(iType == 1) table_idx = LOTTERY_KIND_SPRITE_TAG;
	if(iType == 2) table_idx = PHY_KIND_SPRITE_TAG;

	for(int i = 0;i < m_Count;i ++)
	{
		m_Data.push_back(i + 1 + table_idx); //从一开始
	}

	if(m_table != NULL)
		m_table->reloadData();
}

void LotteryKindScrollView::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryKindScrollView::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryKindScrollView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

cocos2d::Size LotteryKindScrollView::cellSizeForTable(TableView *table)
{
	cocos2d::Size size = cocos2d::Size(SCREEN_WIDTH, 201); //高度定死
	return size;
}

TableViewCell* LotteryKindScrollView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell* cell = new TableViewCell();

	if(m_Data.size() < 1) return cell;

	for(int i = 0 ; i < 4 ; i ++)
	{		
		int data_index = idx * 4 + i + 1;
		string nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
		int kindId = m_Data.at(data_index - 1);		

		Sprite *kindSprite = Sprite::createWithSpriteFrame(spriteFrame(nameStr));
		kindSprite->setPosition(Vec2(SCREEN_WIDTH * (i * 2 + 1) / 8, 100));
		kindSprite->setTag(LOTTERY_KIND_SPRITE_TAG+kindId);
		cell->addChild(kindSprite);		
	}

	return cell;
}

ssize_t LotteryKindScrollView::numberOfCellsInTableView(TableView *table)
{
	int result = m_Count / 4;
	int tempLast = m_Count % 4; //除4有余数，则加1
	if (tempLast != 0) result ++;
	return result;
}