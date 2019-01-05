#include "FriendSystemLayer.h"
#include "DBHandler.h"
#include "PromptBox.h"
#include "GlobalDef.h"
#include "EntityMgr.h"

FriendSystemLayer::FriendSystemLayer()
{
	friendStatusTable = NULL;
	myFriendTable = NULL;
	friendData = NULL;
	friendIndex = -1;
	currentFriendBk = NULL;
}

FriendSystemLayer::~FriendSystemLayer()
{
	if(friendStatusTable!=NULL)
		friendStatusTable->release();
	if(myFriendTable!=NULL)
		myFriendTable->release();
	if(friendData != NULL){
		friendData->release();
	}
}

FriendSystemLayer *FriendSystemLayer::create()
{
	Array *normalName = Array::create();
	Array *selectedName = Array::create();
	normalName->addObject(String::create("Tab_FriendStatus1.png"));
	selectedName->addObject(String::create("Tab_FriendStatus2.png"));
	normalName->addObject(String::create("Tab_MyFriend1.png"));
	selectedName->addObject(String::create("Tab_MyFriend2.png"));
	normalName->addObject(String::create("Tab_AddFriend1.png"));
	selectedName->addObject(String::create("Tab_AddFriend2.png"));
	FriendSystemLayer *tabLayer = FriendSystemLayer::createWithTabItems(normalName,selectedName);
	return tabLayer;
}

FriendSystemLayer *FriendSystemLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	FriendSystemLayer *tabLayer = new FriendSystemLayer();
	if (tabLayer && tabLayer->initWithTabItems(itemNormalName, itemSelectedName))
	{
		tabLayer->initView();
		tabLayer->autorelease();
		return tabLayer;
	}
	else
	{
		CC_SAFE_DELETE(tabLayer);
		return NULL;
	}
}

void FriendSystemLayer::initView()
{
    currentIndex = 0;
	winSize = Director::getInstance()->getWinSize();
	//��ȡ�ҵĺ�������
	this->getMyFriendData();
	Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	tableViewCellSize = tempSprite->getContentSize();
	//���Ѷ�̬ҳ��
	cocos2d::Size tableViewSize;
	tableViewSize.width = tempSprite->getContentSize().width;
	tableViewSize.height = 370;
	FriendTableHelper *helper = new FriendTableHelper();
	friendStatusTable = TableView::create(helper, tableViewSize);
	friendStatusTable->setDelegate(helper);
	friendStatusTable->setAnchorPoint(Vec2(0.5, 0.5));
	friendStatusTable->setPosition(Vec2(15, 10));
	friendStatusTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	friendStatusTable->setDirection(ScrollView::Direction::VERTICAL);
	friendStatusTable->retain();
	friendStatusTable->setTag(TableTag_FriendStatus);
	this->addChild(friendStatusTable, 1);
	//this->setTouchEnabled(true);
	//�ҵĺ���ҳ��

	//�û�����
	float startX = 345;
	float startY = 350.0f;
	float gapY = 50;
	Color3B textColor = ccc3(255, 192, 0);
	string pTextUserName="";
	m_pUserName = LabelTTF::create(String::createWithFormat("%s%s",ConfigMgr::instance()->text("t82"),pTextUserName.c_str())->getCString(),"",22);
	m_pUserName->setColor(textColor);
	m_pUserName->setAnchorPoint(Vec2(0,0));
	m_pUserName->setPosition(Vec2(startX,startY));
	this->addChild(m_pUserName,2);

	//�û��Ա�
	Sprite *pButtonBJ = Sprite::create("dt_warning_8.png");
	const char *pTextUserSex;
	//if(g_GlobalUnits.getGolbalUserData()->cbGender == 1)
	//{
	//	pTextUserSex = String::createWithFormat
	//		("%s%s",ConfigMgr::instance()->text("t83"),ConfigMgr::instance()->text("t150"))->getCString();
	//}
	//else
	//{
	//	pTextUserSex = String::createWithFormat
	//		("%s%s",ConfigMgr::instance()->text("t83"),ConfigMgr::instance()->text("t151"))->getCString();
	//}
	pTextUserSex = String::createWithFormat("%s",ConfigMgr::instance()->text("t83"))->getCString();
	m_pUserSex = LabelTTF::create(pTextUserSex,"HelveticaNeue-Bold", 22);
	m_pUserSex->setColor(textColor);
	m_pUserSex->setAnchorPoint(Vec2(0,0));
	m_pUserSex->setPosition(Vec2(startX,startY-gapY));
	this->addChild(m_pUserSex,2);

	//�û����
	//const char *pTextUserGold = String::createWithFormat
	//	("%s%d",ConfigMgr::instance()->text("t85"),g_GlobalUnits.getGolbalUserData()->lScore)->getCString();
	const char *pTextUserGold = String::createWithFormat
		("%s",ConfigMgr::instance()->text("t85"))->getCString();
	pUserGold = LabelTTF::create(pTextUserGold,"HelveticaNeue-Bold", 22);
	pUserGold->setColor(textColor);
	pUserGold->setAnchorPoint(Vec2(0,0));
	pUserGold->setPosition(Vec2(startX,startY-2*gapY));
	this->addChild(pUserGold,2);

	//�û�ս��
	//const char *pTextUserRecord = String::createWithFormat
	//	("%s%d%s/%d%s",ConfigMgr::instance()->text("t88"),g_GlobalUnits.getGolbalUserData()->lWinCount,ConfigMgr::instance()->text("t165"),g_GlobalUnits.getGolbalUserData()->lLostCount,ConfigMgr::instance()->text("t166"))->getCString();
	const char *pTextUserRecord = String::createWithFormat
		("%s%s%s/%s%s",ConfigMgr::instance()->text("t88")," ",ConfigMgr::instance()->text("t165")," ",ConfigMgr::instance()->text("t166"))->getCString();
	pUserRecord = LabelTTF::create(pTextUserRecord,"HelveticaNeue-Bold", 22);
	pUserRecord->setColor(textColor);
	pUserRecord->setAnchorPoint(Vec2(0,0));
	pUserRecord->setPosition(Vec2(startX,startY-3*gapY));
	this->addChild(pUserRecord,2);

	//�û�ʤ��
	const char *pTextUserWinning;
	//if(g_GlobalUnits.getGolbalUserData()->lTotalCount == 0)
	//{
	//	pTextUserWinning = String::createWithFormat
	//		("%s%d%%",ConfigMgr::instance()->text("t89"),0)->getCString();
	//}
	//else
	//{
	//	pTextUserWinning = String::createWithFormat
	//		("%s%.2f%%",ConfigMgr::instance()->text("t89"),((float)g_GlobalUnits.getGolbalUserData()->lWinCount/(float)g_GlobalUnits.getGolbalUserData()->lTotalCount)*100)->getCString();
	//}
	//pTextUserWinning = String::createWithFormat
	//	("%s%d%%",ConfigMgr::instance()->text("t89"),0)->getCString();
	pTextUserWinning = String::createWithFormat
		("%s",ConfigMgr::instance()->text("t89"))->getCString();
	pUserWinning = LabelTTF::create(pTextUserWinning,"HelveticaNeue-Bold", 22);
	pUserWinning->setColor(textColor);
	pUserWinning->setAnchorPoint(Vec2(0,0));
	pUserWinning->setPosition(Vec2(startX,startY-4*gapY));
	this->addChild(pUserWinning,2);

	Sprite *sendItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendSend1.png"));
	Sprite *sendItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendSend2.png"));
	Sprite *deleteItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendDelete1.png"));
	Sprite *deleteItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendDelete2.png"));
	Sprite *chatItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendChat1.png"));
	Sprite *chatItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendChat2.png"));
	MenuItemSprite *sendItem = MenuItemSprite::create(sendItemNormalImage,sendItemSelectedImage,CC_CALLBACK_1(FriendSystemLayer::sendButton,this));
	MenuItemSprite *deleteItem = MenuItemSprite::create(deleteItemNormalImage,deleteItemSelectedImage,CC_CALLBACK_1(FriendSystemLayer::deleteButton,this));
	MenuItemSprite *chatItem = MenuItemSprite::create(chatItemNormalImage,chatItemSelectedImage,CC_CALLBACK_1(FriendSystemLayer::chatButton,this));
	myFriendOperaMenu = Menu::create(sendItem,deleteItem,chatItem,NULL);	
	myFriendOperaMenu->setPosition(Vec2(555, 42));	
	myFriendOperaMenu->alignItemsHorizontallyWithPadding(4);//�������з�ʽ��������20
	this->addChild(myFriendOperaMenu,2);

	tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend3.png"));
	tableViewSize.width = tempSprite->getContentSize().width;
	tableViewSize.height = 370;
	myFriendTable = TableView::create(this, tableViewSize);
	myFriendTable->setDelegate(this);
	myFriendTable->setAnchorPoint(Vec2(0.5, 0.5));
	myFriendTable->setPosition(Vec2(15, 10));
	myFriendTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	myFriendTable->setDirection(ScrollView::Direction::VERTICAL);
	myFriendTable->retain();
	myFriendTable->setTag(TableTag_MyFriend);
	this->addChild(myFriendTable, 1);

	myFriendBk = Sprite::create("Bk_Friend.png");
	myFriendBk->setAnchorPoint(Vec2(1,0));
	myFriendBk->setPosition(Vec2(800-3, 1));
	this->addChild(myFriendBk,1);	

	m_pUserName->setVisible(false);		
	m_pUserSex->setVisible(false);		
	pUserGold->setVisible(false);						
	pUserRecord->setVisible(false);				
	pUserWinning->setVisible(false);
	myFriendTable->setVisible(false);
	myFriendBk->setVisible(false);
	myFriendOperaMenu->setVisible(false);

	//��Ӻ���
	//����ID
	float textWidth = 500;
	addFriendID = LabelTTF::create(ConfigMgr::instance()->text("t557"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	addFriendID->setColor(ccc3(255,255,255));
	addFriendID->setAnchorPoint(Vec2(0,0.5f));
	addFriendID->setPosition(Vec2(50,268));
	this->addChild(addFriendID, 1);
	//���������
	friendAccount = EditBox::create(cocos2d::Size(389, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_Notice2.png")));
	friendAccount->setPosition(Vec2(233, 268));
	friendAccount->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	friendAccount->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	friendAccount->setAnchorPoint(Vec2(0,0.5f));
	friendAccount->setFontColor(ccc3(0,0,0));
	friendAccount->setFont("",30);
	friendAccount->setPlaceHolder(ConfigMgr::instance()->text("t557"));
	this->addChild(friendAccount, 1);
	//��Ӱ�ť
	Sprite * getCoinsNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendAdd1.png"));
	Sprite * getCoinsSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendAdd2.png"));
	Sprite * getCoinsDisSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_FriendAdd2.png"));
	getCoinsDisSprite->setColor(ccc3(100,100,100));
	MenuItemSprite *getCoinsMenuItem = MenuItemSprite::create(getCoinsNormalSprite, 
		getCoinsSelectSprite,  CC_CALLBACK_1(FriendSystemLayer::pressAddButton,this));
	addButton = Menu::create(getCoinsMenuItem,NULL);
	addButton->setAnchorPoint(Vec2(0,0));
	addButton->setPosition(Vec2(368, 175));
	this->addChild(addButton, 1);

	addFriendID->setVisible(false);
	friendAccount->setVisible(false);
	addButton->setVisible(false);
    
    if(friendAccount->isVisible() == false){
        friendAccount->setPositionY(friendAccount->getPositionY()-1000);
    }
	this->setKeypadEnabled(true);
	//setTouchEnabled(true);
}

void FriendSystemLayer::getMyFriendData()
{
    if (friendData != NULL) {
        friendData->release();
    }
	friendData = Array::create();
	friendData->retain();
	DBHandler *handler = new DBHandler();
	//char sql[800] = {0};
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	int rv = sqlite3_prepare(DB, "select * from FriendInfo", 100, &stmt, NULL);
	rv = sqlite3_step(stmt);
	while(rv == SQLITE_ROW)
	{
		DWORD dwUserID = sqlite3_column_int(stmt, 0);
		WORD wFaceID = sqlite3_column_int(stmt, 1);
		BYTE cbGender = sqlite3_column_int(stmt, 2);
		BYTE cbMemberOrder = sqlite3_column_int(stmt, 3);
		LONG lScore = sqlite3_column_int(stmt, 4);
		LONG lWinCount = sqlite3_column_int(stmt, 5);
		LONG lLostCount = sqlite3_column_int(stmt, 6);
		LONG lDrawCount = sqlite3_column_int(stmt, 7);
		LONG lFleeCount = sqlite3_column_int(stmt, 8);
		LONG lExperience = sqlite3_column_int(stmt, 9);
		char szNickName[NAME_LEN];
		memcpy(szNickName, sqlite3_column_text(stmt, 10), NAME_LEN);
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create(dwUserID), "dwUserID");
		dictionary->setObject(Integer::create(wFaceID), "wFaceID");
		dictionary->setObject(Integer::create(cbGender), "cbGender");
		dictionary->setObject(Integer::create(cbMemberOrder), "cbMemberOrder");
		dictionary->setObject(Integer::create(lScore), "lScore");
		dictionary->setObject(Integer::create(lWinCount), "lWinCount");
		dictionary->setObject(Integer::create(lLostCount), "lLostCount");
		dictionary->setObject(Integer::create(lDrawCount), "lDrawCount");
		dictionary->setObject(Integer::create(lFleeCount), "lFleeCount");
		dictionary->setObject(Integer::create(lWinCount+lLostCount+lDrawCount+lFleeCount), "lTotalCount");
		dictionary->setObject(Integer::create(lExperience), "lExperience");
		dictionary->setObject(String::create(szNickName), "szNickName");
		friendData->addObject(dictionary);
		rv = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	//if(friendData>0)
	//{
	//	friendIndex = 0;
	//}
}

void FriendSystemLayer::addMyFriendData(DWORD dwUserID)
{
	DBHandler *handler = new DBHandler();
	char sql[200] = {0};
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	sprintf(sql, "select * from FriendInfo where dwUserID=%lu", dwUserID);
	int rv = sqlite3_prepare(DB, sql, 200, &stmt, NULL);
	rv = sqlite3_step(stmt);
	if(rv == SQLITE_ROW)
	{
		DWORD dwUserID = sqlite3_column_int(stmt, 0);
		WORD wFaceID = sqlite3_column_int(stmt, 1);
		BYTE cbGender = sqlite3_column_int(stmt, 2);
		BYTE cbMemberOrder = sqlite3_column_int(stmt, 3);
		LONG lScore = sqlite3_column_int(stmt, 4);
		LONG lWinCount = sqlite3_column_int(stmt, 5);
		LONG lLostCount = sqlite3_column_int(stmt, 6);
		LONG lDrawCount = sqlite3_column_int(stmt, 7);
		LONG lFleeCount = sqlite3_column_int(stmt, 8);
		LONG lExperience = sqlite3_column_int(stmt, 9);
		char szNickName[NAME_LEN];
		memcpy(szNickName, sqlite3_column_text(stmt, 10), NAME_LEN);
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create(dwUserID), "dwUserID");
		dictionary->setObject(Integer::create(wFaceID), "wFaceID");
		dictionary->setObject(Integer::create(cbGender), "cbGender");
		dictionary->setObject(Integer::create(cbMemberOrder), "cbMemberOrder");
		dictionary->setObject(Integer::create(lScore), "lScore");
		dictionary->setObject(Integer::create(lWinCount), "lWinCount");
		dictionary->setObject(Integer::create(lLostCount), "lLostCount");
		dictionary->setObject(Integer::create(lDrawCount), "lDrawCount");
		dictionary->setObject(Integer::create(lFleeCount), "lFleeCount");
		dictionary->setObject(Integer::create(lWinCount+lLostCount+lDrawCount+lFleeCount), "lTotalCount");
		dictionary->setObject(Integer::create(lExperience), "lExperience");
		dictionary->setObject(String::create(szNickName), "szNickName");
		bool hasInsert = false;
		int i = 0;
		for(i = 0; i < friendData->count() ; i++)
		{
			Dictionary *dic = (Dictionary *)friendData->objectAtIndex(i);
			DWORD userID = ((Integer *)dic->objectForKey("dwUserID"))->getValue();
			if(dwUserID<userID)
			{
				friendData->insertObject(dictionary, i);
				hasInsert = true;
				break;
			}
		}
		if(hasInsert == false)
		{
			friendData->insertObject(dictionary, i);
		}
		rv = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	myFriendTable->reloadData();
}

void FriendSystemLayer::showFriendInfo(int index)
{
	friendIndex = index;
	Dictionary *dic = (Dictionary *)friendData->objectAtIndex(index);
	String *szNickName = (String *)dic->objectForKey("szNickName");
	Integer *cbGender = (Integer *)dic->objectForKey("cbGender");
	Integer *lScore = (Integer *)dic->objectForKey("lScore");
	Integer *lWinCount = (Integer *)dic->objectForKey("lWinCount");
	Integer *lLostCount = (Integer *)dic->objectForKey("lLostCount");
	Integer *lTotalCount = (Integer *)dic->objectForKey("lTotalCount");

	m_pUserName->setString(String::createWithFormat("%s%s",ConfigMgr::instance()->text("t82"),szNickName->getCString())->getCString());		

	const char *pTextUserSex;
	if(cbGender->getValue() == 1)
	{
		pTextUserSex = String::createWithFormat
			("%s%s",ConfigMgr::instance()->text("t83"),ConfigMgr::instance()->text("t150"))->getCString();
	}
	else
	{
		pTextUserSex = String::createWithFormat
			("%s%s",ConfigMgr::instance()->text("t83"),ConfigMgr::instance()->text("t151"))->getCString();
	}
	m_pUserSex->setString(pTextUserSex);

	const char *pTextUserGold = String::createWithFormat
		("%s%d",ConfigMgr::instance()->text("t85"),lScore->getValue())->getCString();
	pUserGold->setString(pTextUserGold);

	const char *pTextUserRecord = String::createWithFormat
		("%s%d%s/%d%s",ConfigMgr::instance()->text("t88"),lWinCount->getValue(),ConfigMgr::instance()->text("t165"),lLostCount->getValue(),ConfigMgr::instance()->text("t166"))->getCString();
	pUserRecord->setString(pTextUserRecord);

	const char *pTextUserWinning;
	if(lTotalCount->getValue() == 0)
	{
		pTextUserWinning = String::createWithFormat
			("%s%d%%",ConfigMgr::instance()->text("t89"),0)->getCString();
	}
	else
	{
		pTextUserWinning = String::createWithFormat
			("%s%.2f%%",ConfigMgr::instance()->text("t89"),((float)lWinCount->getValue()/(float)lTotalCount->getValue())*100)->getCString();
	}
	pUserWinning->setString(pTextUserWinning);
}

void FriendSystemLayer::doSomething()
{
	CCLOG("FriendSystemLayer::doSomething");
	if(currentItem == TabLayerBaseTag){
		friendStatusTable->setVisible(true);
		myFriendTable->setVisible(false);
		myFriendOperaMenu->setVisible(false);
		myFriendBk->setVisible(false);
		addFriendID->setVisible(false);
        if(friendAccount->isVisible() == true){
            friendAccount->setPositionY(friendAccount->getPositionY()-1000);
            friendAccount->setVisible(false);
        }
		addButton->setVisible(false);

		m_pUserName->setVisible(false);		
		m_pUserSex->setVisible(false);		
		pUserGold->setVisible(false);						
		pUserRecord->setVisible(false);				
		pUserWinning->setVisible(false);				
	}else if(currentItem == TabLayerBaseTag+1){
		friendStatusTable->setVisible(false);
		myFriendTable->setVisible(true);
		myFriendOperaMenu->setVisible(true);
		myFriendBk->setVisible(true);

		m_pUserName->setVisible(true);		
		m_pUserSex->setVisible(true);		
		pUserGold->setVisible(true);						
		pUserRecord->setVisible(true);				
		pUserWinning->setVisible(true);

        addFriendID->setVisible(false);
        if(friendAccount->isVisible() == true){
            friendAccount->setPositionY(friendAccount->getPositionY()-1000);
            friendAccount->setVisible(false);
        }
		addButton->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+2){
		friendStatusTable->setVisible(false);
		myFriendTable->setVisible(false);
		myFriendOperaMenu->setVisible(false);
		myFriendBk->setVisible(false);
        addFriendID->setVisible(true);
        if(friendAccount->isVisible() == false){
            friendAccount->setPositionY(friendAccount->getPositionY()+1000);
            friendAccount->setVisible(true);
        }
		addButton->setVisible(true);

		m_pUserName->setVisible(false);		
		m_pUserSex->setVisible(false);		
		pUserGold->setVisible(false);						
		pUserRecord->setVisible(false);				
		pUserWinning->setVisible(false);
	}
}

void FriendSystemLayer::sendButton(Object *obj)
{
    if(friendData->count()==0)
    {
        return;
    }
	Dictionary *dictionary = (Dictionary *)friendData->objectAtIndex(currentIndex);
	Integer *userID = (Integer *)dictionary->objectForKey("dwUserID");
	DWORD id = userID->getValue();
	EntityMgr::instance()->getDispatch()->SendPacketWithSendScore(String::createWithFormat("%lu", id)->getCString(), 1000, EntityMgr::instance()->getDispatch()->m_szPassword, 3);
    PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
    string promptText = ConfigMgr::instance()->text("t1009");
	String *szNickName = (String *)dictionary->objectForKey("szNickName");
    promptText+=szNickName->getCString();
    promptText+=ConfigMgr::instance()->text("t1010");
    promptBox->setPromptText(promptText.c_str());
   this->addChild(promptBox,3);
}

void FriendSystemLayer::deleteButton(Object *obj)
{
    if(friendData->count()==0)
    {
        return;
    }
    Dictionary *dictionary = (Dictionary *)friendData->objectAtIndex(currentIndex);
	Integer *userID = (Integer *)dictionary->objectForKey("dwUserID");
	DWORD id = userID->getValue();
    DBHandler *handler = new DBHandler();
    String *sql = String::createWithFormat("delete from FriendInfo where dwUserID=%lu", id);
    handler->execute(sql->getCString());
    this->getMyFriendData();
	myFriendTable->reloadData();
}

void FriendSystemLayer::chatButton(Object *obj)
{

}

bool isNumber(const char *buffer)
{
    bool bRet = true;
    int index = 0;
    while (buffer[index]!=0) {
        if(!(buffer[index]>'0'&&buffer[index]<'9')){
            bRet = false;
            break;
        }
        index++;
    }
    return bRet;
}

void FriendSystemLayer::pressAddButton(Object *obj)
{
//    bool bRet = isNumber(friendAccount->getText());
    
	DBHandler *handler = new DBHandler();
	string szNickName = friendAccount->getText();
	char sql[800] = {0};
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	sprintf(sql, "select * from UserInfo where nickname=\'%s\'", szNickName.c_str());
	int rv = sqlite3_prepare(DB, sql, 200, &stmt, NULL);
	rv = sqlite3_step(stmt);
	if(rv != SQLITE_ROW)
	{
		sqlite3_finalize(stmt);
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "select * from UserInfo where dwUserID=\'%s\'", szNickName.c_str());
        rv = sqlite3_prepare(DB, sql, 200, &stmt, NULL);
        rv = sqlite3_step(stmt);
        if(rv != SQLITE_ROW)
        {
            sqlite3_finalize(stmt);
		PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(ConfigMgr::instance()->text("t580"));
		this->addChild(promptBox,1);
            return;
        }
	}

	DWORD userID = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	handler = new DBHandler();
	bool existBefore = handler->isRecordExisted("FriendInfo", userID);
	if(existBefore == false){
		sprintf(sql, "insert into FriendInfo(dwUserID,wFaceID,cbGender,cbMemberOrder,lScore,dwWinCount,dwLostCount,dwDrawCount,dwFleeCount,dwExperience,nickname) select %lu,0,0,0,0,0,0,0,0,0,'' where not exists(select * from FriendInfo where dwUserID=%lu)", userID, userID);
		handler->execute(sql);
	}
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "INSERT OR REPLACE INTO FriendInfo(dwUserID,wFaceID,cbGender,cbMemberOrder,lScore,dwWinCount,dwLostCount,dwDrawCount,dwFleeCount,dwExperience,nickname) SELECT UserInfo.dwUserID,UserInfo.wFaceID,UserInfo.cbGender,UserInfo.cbMemberOrder,UserInfo.lScore,UserInfo.dwWinCount,UserInfo.dwLostCount,UserInfo.dwDrawCount,UserInfo.dwFleeCount,UserInfo.dwExperience,UserInfo.nickname FROM UserInfo,FriendInfo WHERE UserInfo.dwUserID = FriendInfo.dwUserID and UserInfo.dwUserID=%lu", userID);
	handler->execute(sql);

	bool existAfter = handler->isRecordExisted("FriendInfo", userID);
	if(existAfter == true){
		if(existBefore == false)
		{
			this->addMyFriendData(userID);
			PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
			promptBox->setPromptText(ConfigMgr::instance()->text("t579"));
			this->addChild(promptBox,1);
			bool bRet = handler->isRecordExisted("FriendStatus", userID);
			if(bRet == false)
			{
				memset(sql, 0, sizeof(sql));
				string str = ConfigMgr::instance()->text("t581");
				sprintf(sql, "insert into FriendStatus(dwUserID,status) values (%lu,\'%s %s\')", userID, szNickName.c_str(),str.c_str());
				handler->execute(sql);
			}
		}else
		{
		PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(ConfigMgr::instance()->text("t588"));
		this->addChild(promptBox,1);
		}
	}else{
		PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(ConfigMgr::instance()->text("t580"));
		this->addChild(promptBox,1);
	}
}

/**��Ԫ���С
������table��Ԫ������ı��
*/
cocos2d::Size FriendSystemLayer::cellSizeForTable(TableView *table)
{
	Sprite *tempSprite;
	tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend3.png"));
	cocos2d::Size size = tempSprite->getContentSize();
	size.width+=5;
	return size;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t FriendSystemLayer::numberOfCellsInTableView(TableView *table)
{
	return friendData->count();
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *FriendSystemLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cocos2d::Size screenSize = Director::getInstance()->getWinSize();
	Vec2 pos = cell->getPosition();
	Sprite *bk;
	bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend3.png"));

	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(0,2.5f));
	cell->addChild(bk);
	if(index == 0)
	{
		if(currentFriendBk!=NULL)
		{
			currentFriendBk->removeFromParentAndCleanup(true);
		}
		currentFriendBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend2.png"));
		currentFriendBk->setAnchorPoint(Vec2(0,0));
		currentFriendBk->setPosition(bk->getPosition());
		cell->addChild(currentFriendBk);
		this->showFriendInfo(0);
	}

	Dictionary *dic = (Dictionary *)friendData->objectAtIndex(index);
	String *nameCCStr = (String *)dic->objectForKey("szNickName");
	int cbGender = ((Integer *)dic->objectForKey("cbGender"))->getValue();
	int wFaceID = ((Integer *)dic->objectForKey("wFaceID"))->getValue();

	const char* pSZPhonoName;
	if(cbGender == 1)
	{
		if(wFaceID > 1)
		{
			wFaceID = 0;
		}
		pSZPhonoName = String::createWithFormat("dt_register_10.png")->getCString();
	}
	else
	{
		if(wFaceID > 1)
		{
			wFaceID = 0;
		}
		pSZPhonoName = String::createWithFormat("dt_register_8.png")->getCString();
	}
	Sprite *m_pPlayerPhoto;						//�û�ͷ��
	m_pPlayerPhoto = Sprite::create(pSZPhonoName);
	m_pPlayerPhoto->setAnchorPoint(Vec2(0,0.5));
	m_pPlayerPhoto->setPosition(Vec2(5,tableViewCellSize.height/2));
	cell->addChild(m_pPlayerPhoto,1);

	LabelTTF *nameLabel = LabelTTF::create(nameCCStr->getCString(), "", 30);
	nameLabel->setColor(ccc3(255,0,0));
	nameLabel->setAnchorPoint(Vec2(0,0.5));
	nameLabel->setPosition(Vec2(80, tableViewCellSize.height/2));
	cell->addChild(nameLabel, 1);
	cell->setTag(index);
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void FriendSystemLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
	if(currentFriendBk!=NULL)
	{
		currentFriendBk->removeFromParentAndCleanup(true);
	}
	currentFriendBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend2.png"));
	currentFriendBk->setAnchorPoint(Vec2(0,0));
	currentFriendBk->setPosition(Vec2(0,2.5f));
	cell->addChild(currentFriendBk);
    currentIndex = cell->getTag();
	this->showFriendInfo(cell->getTag());
}

void FriendSystemLayer::scrollViewDidScroll(ScrollView* view)
{

}
void FriendSystemLayer::scrollViewDidZoom(ScrollView* view)
{

}


FriendTableHelper::FriendTableHelper()
{
	friendStatusData = NULL;
	this->getFriendStatus();
}

FriendTableHelper::~FriendTableHelper()
{
	if(friendStatusData!=NULL)
		friendStatusData->release();
}

void FriendTableHelper::getFriendStatus()
{
	friendStatusData = Array::create();
	friendStatusData->retain();
	DBHandler *handler = new DBHandler();
	//char sql[800] = {0};
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	int rv = sqlite3_prepare(DB, "select * from FriendStatus", 100, &stmt, NULL);
	rv = sqlite3_step(stmt);
	while(rv == SQLITE_ROW)
	{
		char status[300] = {0};
		memcpy(status, sqlite3_column_text(stmt, 1), 300);
		friendStatusData->addObject(String::createWithFormat("%s", status));
		rv = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
}

/**��Ԫ���С
������table��Ԫ������ı��
*/
cocos2d::Size FriendTableHelper::cellSizeForTable(TableView *table)
{
	Sprite *tempSprite;
	tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	cocos2d::Size size = tempSprite->getContentSize();
	size.width+=5;
	return size;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t FriendTableHelper::numberOfCellsInTableView(TableView *table)
{
	return friendStatusData->count();
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *FriendTableHelper::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	char name[32];
	sprintf(name, "%d line", index);
	Sprite *bk;

	bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Friend1.png"));
	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(0,2.5f));
	cell->addChild(bk);

	String *status = (String *)friendStatusData->objectAtIndex(index);
	LabelTTF *nameLabel = LabelTTF::create(status->getCString(), "", 30);
	nameLabel->setColor(ccc3(255,0,0));
	nameLabel->setAnchorPoint(Vec2(0,0.5f));
	nameLabel->setPosition(Vec2(10, bk->getContentSize().height/2+2.5f));
	Vec2 pos = cell->getPosition();
	cell->addChild(nameLabel);
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void FriendTableHelper::tableCellTouched(TableView *table, TableViewCell *cell)
{

}

void FriendTableHelper::scrollViewDidScroll(ScrollView* view)
{

}
void FriendTableHelper::scrollViewDidZoom(ScrollView* view)
{

}