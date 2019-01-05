#include "RoomBottom.h"
#include "RoomPlayer.h"
//#include "RoomChip.h"
#include "RoomInfo.h"
//#include "RoomCardAction.h"
#include "Sound.h"
#include "PlayerModel.h"
#include "Players.h"
//#include "RoomScene.h"
//#include "RoomProgress.h"
//#include "RoomRule.h"
//#include "TigerMachineLayer.h"

bool RoomBottom::init()
{
	 if (!Layer::init())
    {
        return false;
    }

	//老虎机
	Scale9Sprite* tiger_machine_normal = Scale9Sprite::createWithSpriteFrameName("tiger_machine.png");
	Scale9Sprite* tiger_machine_selected = Scale9Sprite::createWithSpriteFrameName("tiger_machine1.png");
	Scale9Sprite* tiger_machine_highLight = Scale9Sprite::createWithSpriteFrameName("tiger_machine1.png");
	Scale9Sprite* tiger_machine_disabled = Scale9Sprite::createWithSpriteFrameName("tiger_machine1.png");

	CCSize tiger_machine_size = tiger_machine_normal->getContentSize();
	ControlButton* tiger_machine = ControlButton::create(tiger_machine_normal);
	tiger_machine->setPreferredSize(tiger_machine_size);

	tiger_machine->setZoomOnTouchDown(false);
	tiger_machine->setBackgroundSpriteForState(tiger_machine_normal,Control::State::NORMAL);
	tiger_machine->setBackgroundSpriteForState(tiger_machine_selected,Control::State::SELECTED);
	tiger_machine->setBackgroundSpriteForState(tiger_machine_highLight,Control::State::HIGH_LIGHTED);
	tiger_machine->setBackgroundSpriteForState(tiger_machine_disabled,Control::State::DISABLED);
	tiger_machine->setAnchorPoint(ccp(0.5f,0.5f));
	tiger_machine->setPosition(ccp(108,30));
    tiger_machine->setTag(300);
	this->addChild(tiger_machine,1);
	tiger_machine->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::tigerMachineCallback), Control::EventType::TOUCH_DOWN);

	//牌形组合查看
	Scale9Sprite* icon_card_normal = Scale9Sprite::createWithSpriteFrameName("card_default.png");
	Scale9Sprite* icon_card_selected = Scale9Sprite::createWithSpriteFrameName("card_default1.png");
	Scale9Sprite* icon_card_highLight = Scale9Sprite::createWithSpriteFrameName("card_default1.png");
	Scale9Sprite* icon_card_disabled = Scale9Sprite::createWithSpriteFrameName("card_default1.png");

	CCSize icon_card_size = icon_card_normal->getContentSize();
	ControlButton* icon_card = ControlButton::create(icon_card_normal);
	icon_card->setPreferredSize(icon_card_size);

	icon_card->setZoomOnTouchDown(false);
	icon_card->setBackgroundSpriteForState(icon_card_normal,Control::State::NORMAL);
	icon_card->setBackgroundSpriteForState(icon_card_selected,Control::State::SELECTED);
	icon_card->setBackgroundSpriteForState(icon_card_highLight,Control::State::HIGH_LIGHTED);
	icon_card->setBackgroundSpriteForState(icon_card_disabled,Control::State::DISABLED);
	icon_card->setAnchorPoint(ccp(0.5f,0.5f));
	icon_card->setPosition(ccp(28,30));
    icon_card->setTag(400);
	this->addChild(icon_card,1);
	icon_card->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::icon_cardCallback), Control::EventType::TOUCH_DOWN);

	return true;
		
}



//更新自己头像
void RoomBottom::updateMyPlayerUI(int seatid)
{
    show();
    
    int mySeatID = 0;//RoomScene::getMySeatID();
    if (seatid!=mySeatID)
    {
        return;
    }
     
	for (int i=0;i<11;i++)
	{
		if (this->getChildByTag(i))
		{
			this->removeChildByTag(i,true);
		}
	}
    
    //如果不在游戏中
    if (!RoomInfo::getInstance()->isGamming)
        return;
 
    if (mySeatID==-1)
    {
        CCLabelTTF *info = CCLabelTTF::create("你正在观战，请选择空位进行下一轮","Arial-BoldMT",20);
        info->setAnchorPoint(ccp(0.5f,0.5f));
        info->setPosition(ccp(560,33));
        info->setTag(10);
        this->addChild(info,2);
        return;
    }
    //else if (13 == RoomScene::getRff()[mySeatID].player_status || 3 == RoomScene::getRff()[mySeatID].player_status
    //         || 0 == RoomScene::getRff()[mySeatID].player_status)
    //{
    //    return;
    //}
	else
	{	
		//if ( 10 == RoomScene::getRff()[mySeatID].player_status ) //自己在操作中
		{			
            ////是否有自动操作
            //if (RoomScene::getRff()[mySeatID].auto_cmd != 0)
            {
                //if (RoomScene::getRff()[mySeatID].auto_cmd ==6 )//弃牌
                //{
                //    //updateStatus(3);
                //    send_player_action(2004,0);
                //}
				//else if (RoomScene::getRff()[mySeatID].auto_cmd ==7 )//看或弃牌
    //            {
    //                if (RoomScene::getRff()[mySeatID].min_bet==0)
    //                {
    //                    //updateStatus(1);
    //                    send_player_action(2003,0);
    //                }else{
    //                    //updateStatus(3);
    //                    send_player_action(2004,0);
    //                }
    //              
    //            }
				//else if (RoomScene::getRff()[mySeatID].auto_cmd ==8 )
    //            {
    //                if (RoomScene::getRff()[mySeatID].min_bet==0)
    //                {
    //                    //updateStatus(1);
    //                    send_player_action(2003,0);
    //                }
				//	else{
    //                    //updateStatus(2);
    //                    send_player_action(2002,RoomScene::getRff()[mySeatID].min_bet);
    //                }
    //            }
            }
            
		
			//if (RoomScene::getRff()[mySeatID].min_bet==0)
			{
					//看牌
					Scale9Sprite* seeCard_normal = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
					Scale9Sprite* seeCard_selected = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
					Scale9Sprite* seeCard_highLight = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
					Scale9Sprite* seeCard_disabled = Scale9Sprite::createWithSpriteFrameName("room_action_btn2.png");

					seeCard_highLight->setColor(ccc3(131,131,131));
					CCSize seeCard_size = seeCard_normal->getContentSize();
					CCLabelTTF *see_label = CCLabelTTF::create("看牌","Arial-BoldMT",24);
					ControlButton* seeCard = ControlButton::create(see_label,seeCard_normal);
					seeCard->setPreferredSize(seeCard_size);

					seeCard->setZoomOnTouchDown(false);
					seeCard->setBackgroundSpriteForState(seeCard_normal,Control::State::NORMAL);
					seeCard->setBackgroundSpriteForState(seeCard_selected,Control::State::SELECTED);
					seeCard->setBackgroundSpriteForState(seeCard_highLight,Control::State::HIGH_LIGHTED);
					seeCard->setBackgroundSpriteForState(seeCard_disabled,Control::State::DISABLED);
					seeCard->setAnchorPoint(ccp(0.5f,0.5f));
					seeCard->setPosition(ccp(247,27));
					seeCard->setTag(1);
					this->addChild(seeCard,1);
					seeCard->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback), Control::EventType::TOUCH_DOWN);

			}
	        //else
			{
					//跟注
					Scale9Sprite* followGold_normal = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
					Scale9Sprite* followGold_selected = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
					Scale9Sprite* followGold_highLight = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
					Scale9Sprite* followGold_disabled = Scale9Sprite::createWithSpriteFrameName("room_action_btn2.png");

					followGold_highLight->setColor(ccc3(131,131,131));
					CCSize followGold_size = followGold_normal->getContentSize();
					CCLabelTTF *follow_label = CCLabelTTF::create("跟注","Arial-BoldMT",24);
					ControlButton* followGold = ControlButton::create(follow_label,followGold_normal);
					followGold->setPreferredSize(followGold_size);

					followGold->setZoomOnTouchDown(false);
					followGold->setBackgroundSpriteForState(followGold_normal,Control::State::NORMAL);
					followGold->setBackgroundSpriteForState(followGold_selected,Control::State::SELECTED);
					followGold->setBackgroundSpriteForState(followGold_highLight,Control::State::HIGH_LIGHTED);
					followGold->setBackgroundSpriteForState(followGold_disabled,Control::State::DISABLED);
					followGold->setAnchorPoint(ccp(0.5f,0.5f));
					followGold->setPosition(ccp(247,27));
					followGold->setTag(2);
					this->addChild(followGold,1);
					followGold->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback),Control::EventType::TOUCH_DOWN);
			}
			//弃牌
			Scale9Sprite* Discard_normal = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* Discard_selected = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* Discard_highLight = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* Discard_disabled = Scale9Sprite::createWithSpriteFrameName("room_action_btn2.png");

			Discard_highLight->setColor(ccc3(131,131,131));
			CCSize Discard_size = Discard_normal->getContentSize();
			CCLabelTTF *dis_label = CCLabelTTF::create("弃牌","Arial-BoldMT",24);
			ControlButton* Discard = ControlButton::create(dis_label,Discard_normal);
			Discard->setPreferredSize(Discard_size);

			Discard->setZoomOnTouchDown(false);
			Discard->setBackgroundSpriteForState(Discard_normal,Control::State::NORMAL);
			Discard->setBackgroundSpriteForState(Discard_selected,Control::State::SELECTED);
			Discard->setBackgroundSpriteForState(Discard_highLight,Control::State::HIGH_LIGHTED);
			Discard->setBackgroundSpriteForState(Discard_disabled,Control::State::DISABLED);
			Discard->setAnchorPoint(ccp(0.5f,0.5f));
			Discard->setPosition(ccp(405,27));
			Discard->setTag(3);
			this->addChild(Discard,1);
			Discard->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback),Control::EventType::TOUCH_DOWN);

			//加注
			Scale9Sprite* addBtn_normal = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* addBtn_selected = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* addBtn_highLight = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* addBtn_disabled = Scale9Sprite::createWithSpriteFrameName("room_action_btn2.png");

			addBtn_highLight->setColor(ccc3(131,131,131));
			CCSize addBtn_size = addBtn_normal->getContentSize();
			CCLabelTTF *add_label = CCLabelTTF::create("加注","Arial-BoldMT",24);
			ControlButton* addBtn = ControlButton::create(add_label,addBtn_normal);
			addBtn->setPreferredSize(addBtn_size);

			addBtn->setZoomOnTouchDown(false);
			addBtn->setBackgroundSpriteForState(addBtn_normal,Control::State::NORMAL);
			addBtn->setBackgroundSpriteForState(addBtn_selected,Control::State::SELECTED);
			addBtn->setBackgroundSpriteForState(addBtn_highLight,Control::State::HIGH_LIGHTED);
			addBtn->setBackgroundSpriteForState(addBtn_disabled,Control::State::DISABLED);
            addBtn->setTitleColorForState(ccc3(179,160,160),Control::State::DISABLED);
			addBtn->setAnchorPoint(ccp(0.5f,0.5f));
			addBtn->setPosition(ccp(405+158,27));
			addBtn->setTag(4);
			this->addChild(addBtn,1);	
			addBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback),Control::EventType::TOUCH_DOWN);
            //if (RoomScene::getRff()[mySeatID].own_gold<RoomScene::getRff()[mySeatID].min_raise ) {
            //    addBtn->setColor(ccc3(131,131,131));
            //    addBtn->setEnabled(false);
            //}

			//全下
			Scale9Sprite* AllBtn_normal = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* AllBtn_selected = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* AllBtn_highLight = Scale9Sprite::createWithSpriteFrameName("room_action_btn1.png");
			Scale9Sprite* AllBtn_disabled = Scale9Sprite::createWithSpriteFrameName("room_action_btn2.png");

			AllBtn_highLight->setColor(ccc3(131,131,131));
			CCSize AllBtn_size = AllBtn_normal->getContentSize();
			CCLabelTTF *all_label = CCLabelTTF::create("全下","Arial-BoldMT",24);
			ControlButton* AllBtn = ControlButton::create(all_label,AllBtn_normal);
			AllBtn->setPreferredSize(AllBtn_size);

			AllBtn->setZoomOnTouchDown(false);
			AllBtn->setBackgroundSpriteForState(AllBtn_normal,Control::State::NORMAL);
			AllBtn->setBackgroundSpriteForState(AllBtn_selected,Control::State::SELECTED);
			AllBtn->setBackgroundSpriteForState(AllBtn_highLight,Control::State::HIGH_LIGHTED);
			AllBtn->setBackgroundSpriteForState(AllBtn_disabled,Control::State::DISABLED);
			AllBtn->setAnchorPoint(ccp(0.5f,0.5f));
			AllBtn->setPosition(ccp(563+158,27));
			AllBtn->setTag(5);
			this->addChild(AllBtn,1);	
			AllBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback),Control::EventType::TOUCH_DOWN);

			////玩家筹码不够时
			//if  (RoomScene::getRff()[mySeatID].own_gold <  RoomScene::getRff()[mySeatID].min_bet)
			//{
			//	this->removeChildByTag(1,true);
			//	this->removeChildByTag(4,true);
			//	Discard->setPosition(ccp(247,27));
			//	AllBtn->setPosition(ccp(405,27));
			//}
		}
		//else //自己不在操作中
		{
			//若是第一轮，则为自动弃牌按钮
			if (true/* && RoomScene::getRff()[mySeatID].current_round == 1*/)
			{
				Scale9Sprite* auto_disbtn_normal = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
				Scale9Sprite* auto_disbtn_selected = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
				Scale9Sprite* auto_disbtn_highLight = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
				Scale9Sprite* auto_disbtn_disabled = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");

				auto_disbtn_highLight->setColor(ccc3(131,131,131));
				CCSize auto_disbtn_size = auto_disbtn_normal->getContentSize();
				ControlButton* auto_disbtn = ControlButton::create(auto_disbtn_normal);
				auto_disbtn->setPreferredSize(auto_disbtn_size);

				auto_disbtn->setZoomOnTouchDown(false);
				auto_disbtn->setBackgroundSpriteForState(auto_disbtn_normal,Control::State::NORMAL);
				auto_disbtn->setBackgroundSpriteForState(auto_disbtn_selected,Control::State::SELECTED);
				auto_disbtn->setBackgroundSpriteForState(auto_disbtn_highLight,Control::State::HIGH_LIGHTED);
				auto_disbtn->setBackgroundSpriteForState(auto_disbtn_disabled,Control::State::DISABLED);
				auto_disbtn->setAnchorPoint(ccp(0.5f,0.5f));
				auto_disbtn->setPosition(ccp(405+158,27));
				auto_disbtn->setTag(6);
				this->addChild(auto_disbtn,1);
				auto_disbtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback),Control::EventType::TOUCH_DOWN);

                //selected frame
                CCSprite *tag_frame = CCSprite::createWithSpriteFrameName("player_com_btn_frame.png");
                tag_frame->setAnchorPoint(ccp(0.5f,0.5f));
                tag_frame->setPosition(ccp(auto_disbtn_size.width-25,auto_disbtn_size.height/2));
                auto_disbtn->addChild(tag_frame);
                
				//selected tag
				selected_tag1 = CCSprite::createWithSpriteFrameName("player_com_btn_selected.png");
				selected_tag1->setAnchorPoint(ccp(0.5f,0.5f));
				selected_tag1->setPosition(ccp(auto_disbtn_size.width-20,auto_disbtn_size.height/2+4));
				selected_tag1->setVisible(false);
				auto_disbtn->addChild(selected_tag1);
				//if (RoomScene::getRff()[mySeatID].auto_cmd == 6)  //玩家选择了自动弃牌
				//{
				//	selected_tag1->setVisible(true);
				//}

				CCLabelTTF *auto_dis_label = CCLabelTTF::create("弃牌","Arial-BoldMT",24);
				auto_dis_label->setAnchorPoint(ccp(0.5f,0.5f));
				auto_dis_label->setPosition(ccp(65,auto_disbtn_size.height/2));
				auto_disbtn->addChild(auto_dis_label);
			}else//看或弃牌按钮
			{
				Scale9Sprite* auto_seeBtn_normal = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
				Scale9Sprite* auto_seeBtn_selected = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
				Scale9Sprite* auto_seeBtn_highLight = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
				Scale9Sprite* auto_seeBtn_disabled = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");

				auto_seeBtn_highLight->setColor(ccc3(131,131,131));
				CCSize auto_seeBtn_size = auto_seeBtn_normal->getContentSize();
				ControlButton* auto_seeBtn = ControlButton::create(auto_seeBtn_normal);
				auto_seeBtn->setPreferredSize(auto_seeBtn_size);

				auto_seeBtn->setZoomOnTouchDown(false);
				auto_seeBtn->setBackgroundSpriteForState(auto_seeBtn_normal,Control::State::NORMAL);
				auto_seeBtn->setBackgroundSpriteForState(auto_seeBtn_selected,Control::State::SELECTED);
				auto_seeBtn->setBackgroundSpriteForState(auto_seeBtn_highLight,Control::State::HIGH_LIGHTED);
				auto_seeBtn->setBackgroundSpriteForState(auto_seeBtn_disabled,Control::State::DISABLED);
				auto_seeBtn->setAnchorPoint(ccp(0.5f,0.5f));
				auto_seeBtn->setPosition(ccp(405+158,27));
				auto_seeBtn->setTag(7);
				this->addChild(auto_seeBtn,1);
				auto_seeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback),Control::EventType::TOUCH_DOWN);

                //selected frame
                CCSprite *tag_frame = CCSprite::createWithSpriteFrameName("player_com_btn_frame.png");
                tag_frame->setAnchorPoint(ccp(0.5f,0.5f));
                tag_frame->setPosition(ccp(auto_seeBtn_size.width-25,auto_seeBtn_size.height/2));
                auto_seeBtn->addChild(tag_frame);
                
				//selected tag
				selected_tag1 = CCSprite::createWithSpriteFrameName("player_com_btn_selected.png");
				selected_tag1->setAnchorPoint(ccp(0.5f,0.5f));
				selected_tag1->setPosition(ccp(auto_seeBtn_size.width-20,auto_seeBtn_size.height/2+4));
				selected_tag1->setVisible(false);
				auto_seeBtn->addChild(selected_tag1);
				//if ( RoomScene::getRff()[mySeatID].auto_cmd == 7 ) //玩家选择了看或弃牌
				//{
				//	selected_tag1->setVisible(true);  
				//}

				CCLabelTTF *auto_see_label = CCLabelTTF::create("看或弃牌","Arial-BoldMT",24);
				auto_see_label->setAnchorPoint(ccp(0.5f,0.5f));
				auto_see_label->setPosition(ccp(60,auto_seeBtn_size.height/2));
				auto_seeBtn->addChild(auto_see_label);
			}
			
			//跟任何注按钮
			Scale9Sprite* auto_follow_normal = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
			Scale9Sprite* auto_follow_selected = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
			Scale9Sprite* auto_follow_highLight = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");
			Scale9Sprite* auto_follow_disabled = Scale9Sprite::createWithSpriteFrameName("player_com_btn_long.png");

			auto_follow_highLight->setColor(ccc3(131,131,131));
			CCSize auto_follow_size = auto_follow_normal->getContentSize();
			ControlButton* auto_follow = ControlButton::create(auto_follow_normal);
			auto_follow->setPreferredSize(auto_follow_size);

			auto_follow->setZoomOnTouchDown(false);
			auto_follow->setBackgroundSpriteForState(auto_follow_normal,Control::State::NORMAL);
			auto_follow->setBackgroundSpriteForState(auto_follow_selected,Control::State::SELECTED);
			auto_follow->setBackgroundSpriteForState(auto_follow_highLight,Control::State::HIGH_LIGHTED);
			auto_follow->setBackgroundSpriteForState(auto_follow_disabled,Control::State::DISABLED);
			auto_follow->setAnchorPoint(ccp(0.5f,0.5f));
			auto_follow->setPosition(ccp(563+158,27));
			auto_follow->setTag(8);
			this->addChild(auto_follow,1);
			auto_follow->addTargetWithActionForControlEvents(this, cccontrol_selector(RoomBottom::ctrlBtnCallback), Control::EventType::TOUCH_DOWN);
            
            //selected frame
            CCSprite *tag_frame = CCSprite::createWithSpriteFrameName("player_com_btn_frame.png");
            tag_frame->setAnchorPoint(ccp(0.5f,0.5f));
            tag_frame->setPosition(ccp(auto_follow_size.width-25,auto_follow_size.height/2));
            auto_follow->addChild(tag_frame);
            
			//selected tag
			selected_tag2 = CCSprite::createWithSpriteFrameName("player_com_btn_selected.png");
			selected_tag2->setAnchorPoint(ccp(0.5f,0.5f));
			selected_tag2->setPosition(ccp(auto_follow_size.width-20,auto_follow_size.height/2+4));
			selected_tag2->setVisible(false);
			auto_follow->addChild(selected_tag2);
			//if ( RoomScene::getRff()[mySeatID].auto_cmd == 8 ) //玩家选择了“跟任何注”
			//{
			//	selected_tag2->setVisible(true);  
			//}

			CCLabelTTF *auto_see_label = CCLabelTTF::create("跟任何注","Arial-BoldMT",24);
			auto_see_label->setAnchorPoint(ccp(0.5f,0.5f));
			auto_see_label->setPosition(ccp(60,auto_follow_size.height/2));
			auto_follow->addChild(auto_see_label);
		}

	}
}

void RoomBottom::tigerMachineCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
    
    CCPoint originPoint=CCDirector::sharedDirector()->getVisibleOrigin();
 //   TigerMachineLayer* tigerMachineLayer = TigerMachineLayer::create();
	//tigerMachineLayer->setAnchorPoint(CCPointZero);
	//tigerMachineLayer->setPosition(originPoint);
 //   this->getParent()->addChild(tigerMachineLayer,100);

}

void RoomBottom::icon_cardCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
     CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
     CCPoint originPoint = CCDirector::sharedDirector()->getVisibleOrigin();
     
     //RoomRule *roomRule = RoomRule::create();
     //roomRule->setAnchorPoint(ccp(0,0));
     //roomRule->setPosition(ccp(originPoint.x,originPoint.y));
     //roomRule->setTag(200);
     //scene->addChild(roomRule);

     Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
}


void RoomBottom::ctrlBtnCallback(CCObject *sender,cocos2d::extension::Control::EventType controlEvent)
{
	 ControlButton* ctrl_btn = (ControlButton*)sender;
	 int btn_tag = ctrl_btn->getTag();
	 //1.看牌 2.跟注 3.弃牌 4.加注 5.全下 6.自动弃牌 7.看或弃牌 8.跟任何注

     int mySeatID = 0;//RoomScene::getMySeatID();
     if ( 0== btn_tag )
     {
         //checkMyInfo();
     }else if ( 1== btn_tag ){
 
         updateStatus(btn_tag);
         send_player_action(2003,0);
         
         Sound::getInstance()->playEffect("sound/sound_kanpai.ogg");
	 }else if (2 == btn_tag ){
    
         //send_player_action(2002,RoomScene::getRff()[mySeatID].min_bet);
         updateStatus(btn_tag);
	 }else if (3 == btn_tag){

		 updateStatus(btn_tag);
         send_player_action(2004,0);
         
         Sound::getInstance()->playEffect("sound/sound_qipai.ogg");
	 }else if (4 == btn_tag){
         
         hide();
         
         //RoomProgress *roomProgress = RoomProgress::create();
         //roomProgress->setPosition(ccp(0,0));
         //roomProgress->setTag(9);
         //this->addChild(roomProgress);
	 }else if (5 == btn_tag){

         //send_player_action(2001,RoomScene::getRff()[mySeatID].own_gold);
		 updateStatus(btn_tag);
         
         Sound::getInstance()->playEffect("sound/sound_quanya.ogg");
	 }else if (6 == btn_tag){
         
		 selected_tag2->setVisible(false);
		 if (selected_tag1->isVisible())
		 {
			 selected_tag1->setVisible(false);             
             //RoomScene::getRff()[mySeatID].auto_cmd = 0;
		 }else
		 {
			 selected_tag1->setVisible(true);
             //RoomScene::getRff()[mySeatID].auto_cmd = 6;
		 }
         
         Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
	 }else if (7 == btn_tag){
         
		selected_tag2->setVisible(false);
		if (selected_tag1->isVisible())
		{
			 selected_tag1->setVisible(false);            
             //RoomScene::getRff()[mySeatID].auto_cmd = 0;
		}else 
		{
			 selected_tag1->setVisible(true);
             //RoomScene::getRff()[mySeatID].auto_cmd = 7;
		}
         
         Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
	 }else if (8 == btn_tag){
         
		selected_tag1->setVisible(false);
		if (selected_tag2->isVisible())
		{
			 selected_tag2->setVisible(false);
             //RoomScene::getRff()[mySeatID].auto_cmd = 0;
		}else 
		{
			 selected_tag2->setVisible(true);
             //RoomScene::getRff()[mySeatID].auto_cmd = 8;
		}
         
         Sound::getInstance()->playEffect("sound/sound_dianji.ogg");
	 }
}


void RoomBottom::send_player_action(int idx,int bet)
{
    //if (RoomScene::getActionMutex())
    //    return;
    //RoomScene::setActionMutex(true);
    int mySeatID =0;//RoomScene::getMySeatID();
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomChip *roomChip = dynamic_cast<RoomChip*>(scene->getChildByTag(10)->getChildByTag(3));
    //int send_bet = Players::getInstance()->getOthersMostChip(bet+RoomScene::getRff()[mySeatID].bet);
    //if (roomChip){
    //    if (idx==2001 || idx==2002){
    //        RoomScene::getRff()[mySeatID].own_gold -= send_bet;
    //        RoomScene::getRff()[mySeatID].own_gold += RoomScene::getRff()[mySeatID].bet;
    //        roomChip->sendBet(mySeatID,send_bet);
    //    }
    //}
    
    //JPacket jpacket;
    //jpacket.val["cmd"] = CMD_PLAYER_ACTION;
    //jpacket.val["bet"] = send_bet;
    //jpacket.val["ac"] = idx;
    //jpacket.end();
    //CCTcpClient::getInstance()->send_data(jpacket.tostring());
}

 
void RoomBottom::timer_bar_scheduler(float dt)
{

}


void RoomBottom::checkMyInfo()
{
    for (int i=0;i<50;i++)
    {
        if ( Players::getInstance()[i].seatid==0/*RoomScene::getMySeatID()*/)
        {
            PlayerModel::getInstance()->name = Players::getInstance()[i].name;
            PlayerModel::getInstance()->sex = Players::getInstance()[i].sex;
            PlayerModel::getInstance()->vlevel = Players::getInstance()[i].exp;  //need modify
            PlayerModel::getInstance()->money = Players::getInstance()[i].money;
            PlayerModel::getInstance()->rmb = Players::getInstance()[i].rmb;
            PlayerModel::getInstance()->total_win = Players::getInstance()[i].total_win;
            PlayerModel::getInstance()->total_board = Players::getInstance()[i].total_board;
            PlayerModel::getInstance()->max_ship =  Players::getInstance()[i].max_chip;
            
            for (int j=0;j<5;j++)
            {
                PlayerModel::getInstance()->best_cards[j] = Players::getInstance()[i].best_cards[j];
            }

            break;
        }
    }
}

void RoomBottom::updateStatus(int status)
{
    this->unscheduleAllSelectors();

    int mySeatID = 0;//RoomScene::getMySeatID();
    //RoomScene::getRff()[mySeatID].player_status = status;
    //RoomScene::getRff()[mySeatID].bborsb = 0;
    //RoomScene::getRff()[mySeatID].auto_cmd = 0;
    
    updateMyPlayerUI(mySeatID);
    //drawAction
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if (!scene->getChildByTag(10))
    {
        return;
    }
    //RoomCardAction *roomCardAction = dynamic_cast<RoomCardAction*>(scene->getChildByTag(10)->getChildByTag(2));
    RoomOtherPlayer *roomOtherPlayer = dynamic_cast<RoomOtherPlayer*>(scene->getChildByTag(10)->getChildByTag(0));
    //if (roomCardAction){
    //    roomCardAction->stopMyCardAction(); //终止手中牌的震动动画
    //    if (status==3){//弃牌，则播放弃牌动画
    //        roomCardAction->drawAnimate(mySeatID);
    //    }
    //}
    if (roomOtherPlayer){
        roomOtherPlayer->updateOtherPlayerUI(mySeatID);
    }
}

int RoomBottom::getNextPlayerSeatidOfGaming(int seatid)
{
	int nextSeatid;
	int index=0;
	for (int i=0;i<9;i++)
	{
		//if ( RoomScene::getRff()[i].player_status<=11 && RoomScene::getRff()[i].player_status!=3 )
		//{
		//	index++;
		//}
	}
	if ( 1==index )
	{
		return -1;
	}
	

	for (int i=seatid+1;i<9;i++)
	{
		//if ( RoomScene::getRff()[i].player_status<=11 && RoomScene::getRff()[i].player_status!=3 )
		//{
		//	nextSeatid = i;
		//	return nextSeatid;
		//}
	}
	for (int i=0;i<seatid;i++)
	{
		//if ( RoomScene::getRff()[i].player_status<=11 && RoomScene::getRff()[i].player_status!=3 )
		//{
		//	nextSeatid = i;
		//	return nextSeatid;
		//}
		
	}
	return -1;
}

void RoomBottom::hide()
{
    for (int i=1;i<6;i++)
    {
        if (this->getChildByTag(i))
        {
            this->getChildByTag(i)->setVisible(false);
        }
    }
    do{
        CC_BREAK_IF(!getChildByTag(300));
        CC_BREAK_IF(!getChildByTag(400));
        getChildByTag(300)->setVisible(false);
        getChildByTag(400)->setVisible(false);
    }while(0);
}

void RoomBottom::show()
{
    for (int i=1;i<6;i++)
    {
        if (this->getChildByTag(i))
        {
            this->getChildByTag(i)->setVisible(true);
        }
    }
    do{
        CC_BREAK_IF(!getChildByTag(300));
        CC_BREAK_IF(!getChildByTag(400));
        getChildByTag(300)->setVisible(true);
        getChildByTag(400)->setVisible(true);
    }while(0);
}


