#include "RoomLayer.h"
#include "LotteryKindScrollView.h"
#include "ChongQingGambleLayer.h"
#include <math.h>
#include "GuangDong11Xuan5Layer.h"
#include "bjpk10Layer.h"
#include "FC3dpl3Layer.h"
#include "LotterySceneControl.h"
#include "MovingLabelLayer.h"
#include "CMD_GameServer.h"
#include "LotteryGameResult.h"
#include "LotteryGambleLayer.h"
#include "XingYunGambleLayer.h"
#include "BJK8GambleLayer.h"
#include "LiuHeGambleLayer.h"
#include "RoomListLayer.h"//by hxh
#include "RoomListScrollView.h"//by hxh
#include "MyConfig.h"//by hxh
//bjl
#include "SceneView.h"//by hxh
//by
//dzpk
#ifdef  USE_DZPK
#include "DZPKLayer.h"
#endif
//dzz
#ifdef  USE_DDZ
#include "DDZSceneControl.h"
#endif
//fish
#ifdef USE_FISHBEAT
#include "GameScene.h"
#endif // USE_FISHBEAT

#include "VersionControl.h"


using namespace cocos2d;

float myabs(float a)
{
	return a>0?a:-a;
}

LotteryKindScrollView::LotteryKindScrollView()
{
	m_table = nullptr;
}
LotteryKindScrollView::~LotteryKindScrollView()
{
	if(m_table != nullptr)
	{
		m_table->release();
	}
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
		m_iType = 0;

		m_table = TableView::create(this, cocos2d::Size(SCREEN_WIDTH, 604));	
		m_table->setDelegate(this);
		m_table->setDirection(TableView::Direction::VERTICAL);
		m_table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN); 
		m_table->retain();
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
	//playButtonSound();
	int pressedGameKind =  tag-LOTTERY_KIND_SPRITE_TAG;

	if(pressedGameKind == CZChongQingSSC||
		pressedGameKind == CZXinJiangSSC||
		pressedGameKind == CZJiangXiSSC ||
		pressedGameKind == CZ_FENFEN_CAI||
		pressedGameKind == CZ_WUFEN_CAI||
		pressedGameKind == CZ_TIANJIN_SSC||
		pressedGameKind == CZ_KOREA_FEN_CAI||
		pressedGameKind == CZ_JILI_2FEN_CAI ||
		pressedGameKind == CZ_TENCENT_FEN_CAI ||
		pressedGameKind == CZ_QQ_FEN_CAI ||
		pressedGameKind == CZ_CANADA_FEN_CAI||
		pressedGameKind == CZ_BEIJING_5FEN_CAI)        
	{
		ChongQingGambleLayer *layer = ChongQingGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZGD11Xuan5||pressedGameKind == CZSD11Xuan5|| pressedGameKind == CZJX11Xuan5 || pressedGameKind == CZ_HLJ11Xuan5)
	{
		GuangDong11Xuan5Layer *layer = GuangDong11Xuan5Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ_BJPK10)
	{
		bjpk10Layer *layer = bjpk10Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ3D || pressedGameKind == CZPailie3)
	{
		FC3DPL3Layer *layer = FC3DPL3Layer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ_SEVENSTAR)
	{
		LotteryGambleLayer *layer = LotteryGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}else if (pressedGameKind == CZ_LUCKY_28)
	{
		XingYunGambleLayer *layer = XingYunGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZKUAILE8)
	{
		BJK8GambleLayer *layer = BJK8GambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if (pressedGameKind == CZ_HK6HeCai)
	{
		LiuHeGambleLayer *layer = LiuHeGambleLayer::create();
		Scene *scene = Scene::create();
		scene->addChild(layer);
		layer->setGameKind((CaiZhong)pressedGameKind);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
	else if(tag - GAME_KIND_SPRITE_TAG >= 1 && tag - GAME_KIND_SPRITE_TAG <= MAX_GAME)
	{
		MyConfig &myConfig=MyConfig::Instance();
		int platform=(myConfig.m_platform>=0 && myConfig.m_platform<MAX_PLATFORM_NUM?myConfig.m_platform:0);
		int idx=tag - GAME_KIND_SPRITE_TAG - 1;
		int enable=myConfig.m_enable[platform][idx];
		if(enable==1)
		{
#ifdef USE_ROOM_LIST
			int idx=tag-GAME_KIND_SPRITE_TAG-1;
			RoomListLayer *layer = RoomListLayer::create(idx);
			//layer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			layer->setPosition(Vec2((SCREEN_WIDTH - layer->getContentSize().width) / 2, (SCREEN_HEIGHT - layer->getContentSize().height) / 2));
			this->getParent()->addChild(layer,EHALLLayerZOrder_RoomList);
#else
			int idx=tag-GAME_KIND_SPRITE_TAG-1;
			__Array *arr=__Array::create();
			arr->addObject(Integer::create(idx));
			arr->addObject(Integer::create(0));
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATEDOWNLOAD,arr);
#endif		    
		}
		else
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
			Director::sharedDirector()->getRunningScene()->addChild(layer,255);			
		}
	}
	else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t900"), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 4));
		Director::sharedDirector()->getRunningScene()->addChild(layer,255);	
	}

}

void LotteryKindScrollView::connectGameRoomServer(int tag)
{
	connectGameRoomServerEx(tag,0);
}

void LotteryKindScrollView::connectGameRoomServerEx(int tag,int serverId)
{
	int idx=tag-GAME_KIND_SPRITE_TAG-1;
	switch(idx)
	{
	case 0:
#ifdef USE_DZPK
		{
			bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_DZPK,serverId);
			if (!ret)
			{
				MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("Mytext.xml","t27"), ccp(SCREEN_WIDTH/2,SCREEN_WIDTH/2));
				this->addChild(layer,20);
				return;
			}
			EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
			EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(1);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
			EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
			EntityMgr::instance()->getDispatch()->setStartType(true);
			//Director::getInstance()->replaceScene(DZPKLayer::scene());
		}
#endif
		break;
	case 1:
#ifdef  USE_DDZ
		{
			bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_DDZ,serverId);
			if (!ret)
			{
				MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("Mytext.xml","t27"), ccp(SCREEN_WIDTH/2,SCREEN_WIDTH/2));
				this->addChild(layer,20);
				return;
			}
			EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
			EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(1);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
			EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
			EntityMgr::instance()->getDispatch()->setStartType(true);
			//Director::getInstance()->replaceScene(DZPKLayer::scene());
		}
#endif
		break;
	case 2:
#ifdef USE_BJL
		{
			bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_BJL,serverId);
			if (!ret)
			{
				MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("Mytext.xml","t27"), ccp(SCREEN_WIDTH/2,SCREEN_WIDTH/2));
				this->addChild(layer,20);			
				return;
			}
			EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART);
			EntityMgr::instance()->getDispatch()->SendPacketWithEnterRoomPageCount(6);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
			EntityMgr::instance()->getDispatch()->setBehaviorFlag(BEHAVIOR_LOGON_NORMAL);
			EntityMgr::instance()->getDispatch()->setStartType(true);	
		}
#endif
		break;
	case 3:
#ifdef USE_FISHBEAT
		{
			EntityMgr::instance()->getDispatch()->setGameKindId(KindId_FISH);
			EntityMgr::instance()->getDispatch()->setServerId(serverId);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
		}
#endif
		break;
	case 4:
#ifdef USE_PPC
		{

			//bool ret=EntityMgr::instance()->getDispatch()->connectGameServer(KindId_PPC);
			//if (!ret)
			//{
			//	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("Mytext.xml","t27"), ccp(SCREEN_WIDTH/2,SCREEN_WIDTH/2));
			//	this->addChild(layer,20);			
			//	return;
			//}
			EntityMgr::instance()->getDispatch()->setGameKindId(KindId_PPC);
			EntityMgr::instance()->getDispatch()->setServerId(serverId);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
		}
#endif
		break;
	case 5:
#ifdef USE_BRNN
		{
			EntityMgr::instance()->getDispatch()->setGameKindId(KindId_BRNN);
			EntityMgr::instance()->getDispatch()->setServerId(serverId);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
		}
#endif
		break;
	case 6:
#ifdef USE_SGLY
		{
			EntityMgr::instance()->getDispatch()->setGameKindId(KindId_SGLY);
			EntityMgr::instance()->getDispatch()->setServerId(serverId);
			NotificationCenter::sharedNotificationCenter()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
		}
#endif
		break;

	default:
		break;

	}
}

void LotteryKindScrollView::onEnter()
{
	Layer::onEnter();
	//
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);
}

void LotteryKindScrollView::onExit()
{
	//
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
			//if(idx > LOTTERY_COUNT - 1) break;
			if (idx > m_Count - 1) break;
			idx = m_Data.at(idx);
			Sprite* sptCell = (Sprite* )tempCell->getChildByTag(idx + LOTTERY_KIND_SPRITE_TAG);
			if(sptCell == NULL) continue;		

			if (sptCell->boundingBox().containsPoint(pos1))   
			{
				//点击声音
				playButtonSound();

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
	m_iType = iType;

	int table_idx = GAME_KIND_SPRITE_TAG;
	if(iType == 2)
	{
		table_idx = PHY_KIND_SPRITE_TAG;
	}

	if(iType > 0)  //其它也暂时没有用到，这里先不管体育 暂时当作彩票处理
	{
		table_idx = LOTTERY_KIND_SPRITE_TAG;

		static int vec_ssc[14] = {1,2,4,11,12,20,23,24,25,26,28,29,30,31};
		static int vec_11x5[5] = {6,7,8,9,16};
		static int vec_klc[4] = {10,13,14,17};
		static int vec_3d[3] = {3,5,15};

		//重庆彩是默认彩种，这里必须做特殊处理
		int iChongQingCai = 0;

		for(int i = 0,j = 0;i < m_Count;i ++)
		{					
			int tempInt = MyBetNumber::getInstance()->getCaiZhong(j + 1);

			//CCLOG("i = %d,j + 1 = %d,tempint =%d",i,j,tempInt);
			if(tempInt == 1) iChongQingCai ++;
			//重庆彩出现多次后，删除
			if(iChongQingCai > 1) break;			
#ifdef VER_369
			bool isContained = false;
			int k = 0;

			switch (iType)
			{
			case 1:
				for(k = 0;k < 14;k ++)
				{
					if(tempInt == vec_ssc[k])
					{
						isContained = true;
						break;
					}
				}
				break;
			case 2:
				for(k = 0;k < 5;k ++)
				{
					if(tempInt == vec_11x5[k])
					{
						isContained = true;
						break;
					}
				}
				break;
			case 3:
				for(k = 0;k < 4;k ++)
				{
					if(tempInt == vec_klc[k])
					{
						isContained = true;
						break;
					}
				}
				break;
			case 4:
				for(k = 0;k < 3;k ++)
				{
					if(tempInt == vec_3d[k])
					{
						isContained = true;
						break;
					}
				}
				break;
			default:
				break;
			}
			if(!isContained)
			{
				tempInt = 0;
			}
#endif

			if(tempInt == 0) 
			{
				j ++;i --;
				continue;
			}
			m_Data.push_back(tempInt + table_idx); //从一开始
			j ++;
		}
	}else
	{
		//重新排序		
		for(int i = 0;i < m_Count;i ++)
		{
			m_Data.push_back(gameSortList[i] + table_idx);
		}
	}

#ifdef VER_369

	m_table->setViewSize(cocos2d::Size(SCREEN_WIDTH, 420));

#endif

	//重新定义大小
	m_Count = m_Data.size();

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
	//int tag = cell->getIdx();
	//if (tag < 0) return;	
	//
	//pCell = cell;
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

		if (data_index > m_Count) break;

		int kindId = m_Data.at(data_index - 1);	

		string nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();

		if(data_index > 20) //彩票只显示前18个彩种
		{			
			nameStr = String::createWithFormat("%s%02d_2.png",m_name.c_str(),data_index)->getCString();
		}	

		if (m_iType == 0) //游戏
		{
			data_index = gameSortList[data_index - 1];
			nameStr = String::createWithFormat("%s%02d_2.png",m_name.c_str(),data_index)->getCString();
#ifdef USE_DZPK
			if(data_index == 1)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif // DZPK
#ifdef USE_DDZ
			if(data_index == 2)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif // 
#ifdef USE_FISHBEAT
			if(data_index == 4)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif // 
#ifdef USE_BJL
			if(data_index == 3)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif
#ifdef USE_PPC
			if(data_index == 5)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif
#ifdef USE_BRNN
			if(data_index == 6)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif
#ifdef USE_SGLY
			if(data_index == 7)
				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
#endif
//#ifdef USE_ZJH
//			if(data_index == 8)
//				nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),data_index)->getCString();
//#endif
		} else
		{
			nameStr = String::createWithFormat("%s%02d.png",m_name.c_str(),kindId)->getCString();
		}

		Sprite *kindSprite = Sprite::createWithSpriteFrame(spriteFrame(nameStr));
		if(kindSprite)
		{
			kindSprite->setPosition(Vec2(SCREEN_WIDTH * (i * 2 + 1) / 8, 100));
			kindSprite->setTag(LOTTERY_KIND_SPRITE_TAG+kindId);
			cell->addChild(kindSprite);	
		}
		else
		{
			CCLOG("类型图片: %s不见了", nameStr.c_str());
		}
#ifdef VER_369
		if(kindSprite)
			kindSprite->setPositionX((SCREEN_WIDTH - 78) * (i * 2 + 1) / 8 + 40);		
#endif
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