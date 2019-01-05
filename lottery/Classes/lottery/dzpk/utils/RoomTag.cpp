//
//  RoomTag.cpp
//  gamePoker
//
//  Created by xavier on 13-12-4.
//
//

#include "RoomTag.h"
#include "RoomScene.h"
#include "Sound.h"



bool RoomTag::init()
{
    if (!Layer::init())
    {
        return false;
    }
    removeAllChildrenWithCleanup(true);
    return true;
}


CCPoint RoomTag::getAnimatePos(int id)
{
    int x,y;
    CCSize WinSize = CCDirector::sharedDirector()->getVisibleSize();

    int X[9] = {
        WinSize.width/2+0,
        WinSize.width/2-173,
        WinSize.width/2-340,
        WinSize.width/2-340,
        WinSize.width/2-173,
        WinSize.width/2+173,
        WinSize.width/2+340,
        WinSize.width/2+340,
        WinSize.width/2+173,
    };
    int Y[9] = {
        WinSize.height/2-110-40,
        WinSize.height/2-110-40,
        WinSize.height/2-33-40,
        WinSize.height/2+107-40,
        WinSize.height/2+165-40,
        WinSize.height/2+165-40,
        WinSize.height/2+107-40,
        WinSize.height/2-33-40,
        WinSize.height/2-110-40,
    };
    
    
    int mySeatID = RoomScene::getMySeatID();
    if (mySeatID!=-1)
	{
        if (mySeatID==id)
        {
            x = X[0];
            y = Y[0];
        }else if ( mySeatID<id )
		{
			x=X[id-mySeatID];
			y=Y[id-mySeatID];
		}else
		{
			x=X[9-mySeatID+id];
			y=Y[9-mySeatID+id];
		}
	}
    else
    {
		x=X[id];
		y=Y[id];
    }
    
    return ccp(x,y);
}
CCPoint RoomTag::getChatTextPos(int id)
{
    int x,y;

    CCSize WinSize = CCDirector::sharedDirector()->getVisibleSize();
    int X[9] = {
        WinSize.width/2+0-0,
        WinSize.width/2-173-0,
        WinSize.width/2-340+0,
        WinSize.width/2-340+0,
        WinSize.width/2-173-0,
        WinSize.width/2+173+0,
        WinSize.width/2+340-0,
        WinSize.width/2+340-0,
        WinSize.width/2+173-0,
    };
    int Y[9] = {
        WinSize.height/2-110+55,
        WinSize.height/2-110+55,
        WinSize.height/2-33+30,
        WinSize.height/2+107-30,
        WinSize.height/2+165-30,
        WinSize.height/2+165-30,
        WinSize.height/2+107-30,
        WinSize.height/2-33+30,
        WinSize.height/2-110+55,
    };
    
    int mySeatID = RoomScene::getMySeatID();
    if (mySeatID!=-1)
	{
        if (mySeatID==id)
        {
            x = X[0];
            y = Y[0];
        }else if ( mySeatID<id )
		{
			x=X[id-mySeatID];
			y=Y[id-mySeatID];
		}else
		{
			x=X[9-mySeatID+id];
			y=Y[9-mySeatID+id];
		}
	}
    else
    {
		x=X[id];
		y=Y[id];
    }
    
    return ccp(x,y);
}
CCPoint RoomTag::getChatFacePos(int id)
{
    int x,y;
 
    CCSize WinSize = CCDirector::sharedDirector()->getVisibleSize();
    int X[9] = {
        WinSize.width/2+0,
        WinSize.width/2-173,
        WinSize.width/2-340,
        WinSize.width/2-340,
        WinSize.width/2-173,
        WinSize.width/2+173,
        WinSize.width/2+340,
        WinSize.width/2+340,
        WinSize.width/2+173,
    };
    int Y[9] = {
        WinSize.height/2-110,
        WinSize.height/2-110,
        WinSize.height/2-33,
        WinSize.height/2+107,
        WinSize.height/2+165,
        WinSize.height/2+165,
        WinSize.height/2+107,
        WinSize.height/2-33,
        WinSize.height/2-110,
    };
    
    int mySeatID = RoomScene::getMySeatID();
    if (mySeatID!=-1)
	{
        if (mySeatID==id)
        {
            x = X[0];
            y = Y[0];
        }else if ( mySeatID<id )
		{
			x=X[id-mySeatID];
			y=Y[id-mySeatID];
		}else
		{
			x=X[9-mySeatID+id];
			y=Y[9-mySeatID+id];
		}
	}
    else
    {
		x=X[id];
		y=Y[id];
    }
    
    return ccp(x,y);
}

CCPoint RoomTag::getBankerTagPos(int id)
{
    int x,y;
    
    CCSize WinSize = CCDirector::sharedDirector()->getVisibleSize();

    int X[9] = {
        WinSize.width/2+0-60,
        WinSize.width/2-173+60,
        WinSize.width/2-340+60,
        WinSize.width/2-340+70,
        WinSize.width/2-173-60,
        WinSize.width/2+173-60,
        WinSize.width/2+340-70,
        WinSize.width/2+340-60,
        WinSize.width/2+173+60,
    };
    int Y[9] = {
        WinSize.height/2-110+40,
        WinSize.height/2-110+40,
        WinSize.height/2-33+40,
        WinSize.height/2+107-0,
        WinSize.height/2+165-40,
        WinSize.height/2+165-40,
        WinSize.height/2+107-0,
        WinSize.height/2-33+40,
        WinSize.height/2-110+40,
    };
    
    int mySeatID = RoomScene::getMySeatID();
    if (mySeatID!=-1)
	{
        if (mySeatID==id)
        {
            x = X[0];
            y = Y[0];
        }else if ( mySeatID<id )
		{
			x=X[id-mySeatID];
			y=Y[id-mySeatID];
		}else
		{
			x=X[9-mySeatID+id];
			y=Y[9-mySeatID+id];
		}
	}
    else
    {
		x=X[id];
		y=Y[id];
    }
    
    return ccp(x,y);
}

int RoomTag::random(int x, int y)
{
    return x+rand()%(y-x+1);
}

void RoomTag::winAnimate(int seatid)
{
    if (seatid!=RoomScene::getMySeatID()) {
        return;
    }
    int tag = seatid;
    if (getChildByTag(tag) || seatid<0)
    {
        return;
    }
    
    CCPoint pos = getAnimatePos(seatid);
    pos.y+=140;
    
    CCSprite *wintag = CCSprite::createWithSpriteFrameName("room_win_label.png");
    wintag->setAnchorPoint(ccp(0.5f,0.5f));
    wintag->setPosition(pos);
    wintag->setOpacity(0);
    wintag->setTag(tag);
    this->addChild(wintag);
    wintag->runAction(CCFadeIn::create(0.3f));
    CCCallFuncN *callback = CCCallFuncN::create(this, callfuncN_selector(RoomTag::winTagCallBack));
    wintag->runAction(CCSequence::create(CCDelayTime::create(4.0f),CCFadeOut::create(1.0f),callback,NULL));
    
}

void RoomTag::playerChatText(int seatid,string str)
{
    string arr_path;
    int mySeatID = RoomScene::getMySeatID();
    bool flipX=false;
    bool flipY=false;
    CCPoint anchorPoint = ccp(0.5f,0);
    CCPoint arrPos = ccp(0,0);
    
    int index=0;
    if (mySeatID!=-1)
    {
        if (seatid>=mySeatID){
            index = seatid-mySeatID;
        }else{
            index = seatid+9-mySeatID;
        }
    }else{
        index=seatid;
    }

    if (index==0)
    {
        arr_path="chat_frame_2.png";
    }else if (index==1){
        arr_path="chat_frame_3.png";
    }else if (index==2){
        arr_path="chat_frame_3.png";
        anchorPoint = ccp(0,0);
    }else if (index==3){
        arr_path="chat_frame_3.png";
        flipY=true;
        anchorPoint = ccp(0,1);
    }else if (index==4){
        arr_path="chat_frame_3.png";
        flipX=true;
        flipY=true;
        anchorPoint = ccp(0.5f,1);
    }else if (index==5){
        arr_path="chat_frame_3.png";
        flipY=true;
        anchorPoint = ccp(0.5f,1);
    }else if (index==6){
        arr_path="chat_frame_3.png";
        flipX=true;
        flipY=true;
        anchorPoint = ccp(1,1);
    }else if (index==7){
        arr_path="chat_frame_3.png";
        flipX=true;
        anchorPoint = ccp(1,0);
    }else if (index==8){
        arr_path="chat_frame_3.png";
        flipX=true;
    }else{
        return;
    }
   
    if (getChildByTag(10+seatid))
    {
        removeChildByTag(10+seatid,true);
    }

    Scale9Sprite *chat_bg = Scale9Sprite::createWithSpriteFrameName("chat_frame_1.png");
    chat_bg->setPosition(getChatTextPos(seatid));
    chat_bg->setAnchorPoint(anchorPoint);
    chat_bg->setTag(10+seatid);
    this->addChild(chat_bg,9);

    CCLabelTTF *testLabel = CCLabelTTF::create(str.c_str(), "Arial-BoldMT", 17.0);
    int width = testLabel->getContentSize().width>180? 190:testLabel->getContentSize().width+10;
    
    CCLabelTTF *chat_text = CCLabelTTF::create(str.c_str(), "Arial-BoldMT", 17.0, CCSizeMake(180, 0), kCCTextAlignmentLeft);
    int height = chat_text->getContentSize().height+10;
    
    width = width<30? 30:width;
    height = height<30? 30:height;

    chat_bg->setContentSize(CCSizeMake(width,height));
    testLabel->setPosition(ccp(chat_bg->getContentSize().width/2,chat_bg->getContentSize().height/2));
    chat_text->setPosition(ccp(chat_bg->getContentSize().width/2,chat_bg->getContentSize().height/2));
    

    if (testLabel->getContentSize().width>180)
        chat_bg->addChild(chat_text);
    else
        chat_bg->addChild(testLabel);
    
    if (index==0){
        arrPos = ccp(chat_bg->getContentSize().width/2,-4);
    }else if (index==1){
        arrPos = ccp(chat_bg->getContentSize().width/2,-4);
    }else if (index==2){
        arrPos = ccp(13,-4);
    }else if (index==3){
        arrPos = ccp(13,4+chat_bg->getContentSize().height);
    }else if (index==4){
        arrPos = ccp(chat_bg->getContentSize().width/2,4+chat_bg->getContentSize().height);
    }else if (index==5){
        arrPos = ccp(chat_bg->getContentSize().width/2,4+chat_bg->getContentSize().height);
    }else if (index==6){
        arrPos = ccp(chat_bg->getContentSize().width-13,4+chat_bg->getContentSize().height);
    }else if (index==7){
        arrPos = ccp(chat_bg->getContentSize().width-13,-4);
    }else if (index==8){
        arrPos = ccp(chat_bg->getContentSize().width/2,-4);
    }
 
    CCSprite *chat_arr = CCSprite::createWithSpriteFrameName(arr_path.c_str());
    chat_arr->setAnchorPoint(ccp(0.5f,0.5f));
    chat_arr->setPosition(arrPos);
    chat_arr->setFlipX(flipX);
    chat_arr->setFlipY(flipY);
    chat_bg->addChild(chat_arr,10);

    
    chat_bg->runAction(CCSequence::create(CCDelayTime::create(4.0f),
                                          CCCallFuncND::create(this, callfuncND_selector(RoomTag::chatTextClear),(void*)(10+seatid)),NULL));
}
/*
void RoomTag::playerChatText(int seatid,string str)
{
    string bg_path;
    CCPoint label_pos;
    int mySeatID = RoomScene::getMySeatID();
    int per=0;
    bool flip=false;
    
    int index=0;
    if (mySeatID!=-1)
    {
        if (seatid>=mySeatID){
            index = seatid-mySeatID;
        }else{
            index = seatid+9-mySeatID;
        }
    }else{
        index=seatid;
    }
    
    if (index==0 || index==1 || index==8 )
    {
        per=3;
        bg_path="chat_frame_1.png";
        flip=false;
    }else if (index==2){
        per=3;
        bg_path="chat_frame_2.png";
        flip=true;
    }else if (index==3){
        per=15;
        bg_path="chat_frame_2.png";
        flip=false;
    }else if (index==4 || index==5){
        per=15;
        bg_path="chat_frame_1.png";
        flip=true;
    }else if (index==6){
        per=15;
        bg_path="chat_frame_3.png";
        flip=false;
    }else if (index==7){
        per=3;
        bg_path="chat_frame_3.png";
        flip=true;
    }else{
        return;
    }
    
    if (getChildByTag(10+seatid))
    {
        removeChildByTag(10+seatid,true);
    }
    
    CCSprite *chat_bg = CCSprite::createWithSpriteFrameName(bg_path.c_str());
    chat_bg->setAnchorPoint(ccp(0.5f,0.5f));
    chat_bg->setPosition(getChatTextPos(seatid));
    chat_bg->setFlipY(flip);
    chat_bg->setTag(10+seatid);
    this->addChild(chat_bg,10);
    
    CCSize size = chat_bg->getContentSize();
    
    CCScrollView *chat_scroll = CCScrollView::create(CCSizeMake(180, 22));
    chat_scroll->setBounceable(false);
    chat_scroll->setAnchorPoint(ccp(0,0));
    chat_scroll->setPosition(ccp(5,18-per));
    chat_bg->addChild(chat_scroll,1);
    
    label_pos = ccp(2,0);
    
    CCLabelTTF *chat_text = CCLabelTTF::create(str.c_str(), "Arial-BoldMT", 17.0, CCSizeMake(180, 0), kCCTextAlignmentLeft);
    chat_text->setAnchorPoint(ccp(0,1.0f));
    chat_text->setPosition(label_pos);
    chat_scroll->addChild(chat_text);
    
    CCLabelTTF *chat = CCLabelTTF::create("汉字", "Arial-BoldMT", 17.0, CCSizeMake(180, 0), kCCTextAlignmentLeft);
    float hz = chat->getContentSize().height;
    
    int lines = ceil(chat_text->getContentSize().height/hz) ;//(每行间距21个像素)
    chat_text->setPositionY((lines-1)*(-hz)-2);
    
    CCArray *array = CCArray::createWithCapacity(2*lines);
    for (int i=0;i!=lines-1;++i){
        array->addObject(CCDelayTime::create(2.0f));
        array->addObject(CCEaseSineIn::create(CCMoveBy::create(0.3f, ccp(0,hz))));
    }
    array->addObject(CCDelayTime::create(2.0f));
    array->addObject(CCCallFuncND::create(this, callfuncND_selector(RoomTag::chatTextClear),(void*)(10+seatid)));
    
    chat_text->runAction(CCSequence::create(array));
}
*/

void RoomTag::playerChatFace(int seatid,string str)
{
    if (getChildByTag(20+seatid))
    {
        removeChildByTag(10+seatid,true);
    }
    
    CCString *face_path = CCString::createWithFormat("rooms/face/game_face%s.png",str.c_str());
    CCSprite *face = CCSprite::create(face_path->getCString());
    face->setAnchorPoint(ccp(0.5f,0.5f));
    face->setPosition(getChatFacePos(seatid));
    face->setTag(20+seatid);
    this->addChild(face);
    
    CCCallFuncN *callback = CCCallFuncN::create(this, callfuncN_selector(RoomTag::chatCallBack));
    face->runAction(CCSequence::create(
                                       CCMoveBy::create(0.6,ccp(0,15)),
                                       CCMoveBy::create(0.5,ccp(0,-15)),
                                       CCMoveBy::create(0.6,ccp(0,15)),
                                       CCMoveBy::create(0.5,ccp(0,-15)),
                                  
                                       CCDelayTime::create(2.0f),
                                       callback,
                                       NULL
                                      ));
    
}

void RoomTag::addBankerTag(int seatid)
{
    if (seatid<0)
    {
        return;
    }

    for (int i=0;i<9;i++)
    {
        if (getChildByTag(30+i))
        {
            this->removeChildByTag(30+i,true);
        }
    }
 
    CCSprite *bankerTag = CCSprite::createWithSpriteFrameName("winer_tag.png");
    bankerTag->setAnchorPoint(ccp(0.5f,0.5f));
    bankerTag->setPosition(getBankerTagPos(seatid));
    bankerTag->setScale(0.8f);
    bankerTag->setTag(30+seatid);
    this->addChild(bankerTag);
}

void RoomTag::winTagCallBack(CCNode* pSender)
{
    int tag =pSender->getTag();
    this->removeChildByTag(tag, true);
}

void RoomTag::chatCallBack(CCNode* pSender)
{
    int tag =pSender->getTag();
    this->removeChildByTag(tag,true);
}


void RoomTag::chatTextClear(CCNode* pSender, void* data)
{
    int tag = (int)(data);
    do{
        CC_BREAK_IF(!getChildByTag(tag));
        removeChildByTag(tag, true);
    }while (0);
}

void RoomTag::bankerTagRemove(int seatid)
{
    if (getChildByTag(30+seatid))
    {
        this->removeChildByTag(30+seatid,true);
    }
}

void RoomTag::hideTag()
{
    for (int i=0;i<40;i++)
    {
        if (getChildByTag(i))
        {
            CCNode *node = (CCNode*)(getChildByTag(i));
            node->setVisible(false);
        }
    }
}

void RoomTag::showTag()
{
    for (int i=0;i<9;i++)
    {
        if (getChildByTag(i))
        {
            CCNode *node = (CCNode*)(getChildByTag(i));
            node->setPosition(getAnimatePos(i));
            node->setVisible(true);
        }
    }
    for (int i=10;i<19;i++)
    {
        if (getChildByTag(i))
        {
            CCNode *node = (CCNode*)(getChildByTag(i));
            node->setPosition(getChatTextPos(i-10));
            node->setVisible(true);
        }
    }
    for (int i=20;i<29;i++)
    {
        if (getChildByTag(i))
        {
            CCNode *node = (CCNode*)(getChildByTag(i));
            node->setPosition(getChatFacePos(i-20));
            node->setVisible(true);
        }
    }
    for (int i=30;i<39;i++)
    {
        if (getChildByTag(i))
        {
            CCNode *node = (CCNode*)(getChildByTag(i));
            node->setPosition(getBankerTagPos(i-30));
            node->setVisible(true);
        }
    }
}
