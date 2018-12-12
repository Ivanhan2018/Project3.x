#include "ExchangerLayer.h"
#include "RoomLayer.h"
#include "CocosUnits.h"
#include "SceneControl.h"
#include "JsonHelper.h"
#include "network/HttpClient.h"


ExchangerLayer::ExchangerLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ExchangerLayer::cancelHttpRequest),"CancelHttpRequest", NULL);
	cancelOrNot = false;
	exchangeTable = NULL;
}

ExchangerLayer::~ExchangerLayer()
{
	NotificationCenter::getInstance()->removeObserver(this, "CancelHttpRequest");
	if(exchangeTable!=NULL)
		exchangeTable->release();
}

void ExchangerLayer::cancelHttpRequest(Object *obj)
{
	cancelOrNot = true;
}

bool ExchangerLayer::init()
{
	bool bRet = false;
	do 
	{
		winSize = Director::getInstance()->getWinSize();
		ccColor3B whiteColor = ccc3(255,255,255);
		ccColor3B blackColor = ccc3(0,0,0);
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);

		Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ExchangeTableBk1.png"));
		tableItemSize = tempSprite->getContentSize();
		tableGapX = (winSize.width-tableItemSize.width*3)/4;
		tableGapY = 4;
		itemNum = 5;

		Sprite * pBJ  = Sprite::create("Bk_Exchange1.png");
		pBJ->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBJ,0);

		Sprite * exchangeSprite  = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Exchange3.png"));
		exchangeSprite->setAnchorPoint(Vec2(0,0.5f));
		exchangeSprite->setPosition(Vec2(0,390));
		this->addChild(exchangeSprite,2);

		/**πÿ±’∞¥≈•**/
		Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pButtonClose = MenuItemSprite::create
			(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(ExchangerLayer::dismiss,this));
		pbuttonSelectClose->setScale(0.9f);
		pbuttonSelectClose->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pButton0 = Menu::create(pButtonClose,NULL);
		pButton0->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93)); 
		this->addChild(pButton0,2);

		//Ω±“
		cocos2d::Size tableViewSize;
		tableViewSize.width = winSize.width;
		tableViewSize.height = 336;
		exchangeTable = TableView::create(this, tableViewSize);
		exchangeTable->setDelegate(this);
		exchangeTable->setAnchorPoint(Vec2(0.5, 0.5));
		exchangeTable->setPosition(Vec2(0, 0));
		exchangeTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		exchangeTable->setDirection(ScrollView::Direction::VERTICAL);
		exchangeTable->retain();
		this->addChild(exchangeTable, 1);

		//π∫¬Ú
		buyBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyBk.png"));
		buyBk->setAnchorPoint(Vec2(0.5f,0));
		buyBk->setPosition(Vec2(winSize.width/2,88));
		this->addChild(buyBk,2);

		buyText = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Exchange1.png"));
		buyText->setPosition(Vec2(winSize.width/2,340));
		this->addChild(buyText,2);

        float textWidth = 100;
		float startY = 288;
		float gapY = 56;
		//类型标签
		buyArticleType1 = LabelTTF::create(pConfigMgr->text("t558"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		buyArticleType1->setColor(whiteColor);
		buyArticleType1->setPosition(Vec2(241,startY));
		this->addChild(buyArticleType1,2);
		//物品类型底框
		buyArticleTypeBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png"));
		buyArticleTypeBk->setAnchorPoint(Vec2(0,0.5f));
		buyArticleTypeBk->setPosition(Vec2(284,startY));
		buyArticleTypeBk->setAnchorPoint(Vec2(0,0.5F));
		this->addChild(buyArticleTypeBk,2);
		//物品类型
		textWidth = 400;
		buyArticleType2 = LabelTTF::create(pConfigMgr->text("t558"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		buyArticleType2->setColor(blackColor);
		buyArticleType2->setAnchorPoint(Vec2(0,0.5f));
		buyArticleType2->setPosition(Vec2(290,startY));
		this->addChild(buyArticleType2,2);
        
		startY -= gapY;
		//数量标签
		textWidth = 100;
		buyArticleNumLabel = LabelTTF::create(pConfigMgr->text("t559"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		buyArticleNumLabel->setColor(whiteColor);
		buyArticleNumLabel->setPosition(Vec2(241,startY));
		this->addChild(buyArticleNumLabel,2);
        
		//购买数量输入框
		textWidth = 400;
        buyArticleNumBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png"));
        buyArticleNumBk->setPosition(Vec2(284, startY));
        buyArticleNumBk->setAnchorPoint(Vec2(0,0.5f));
       this->addChild(buyArticleNumBk, 2);
        buyArticleNum = LabelTTF::create("1", "", 30);
        buyArticleNum->setPosition(Vec2(295, startY));
        buyArticleNum->setColor(ccc3(0,0,0));
        
//		buyArticleNum = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
//		buyArticleNum->setPosition(Vec2(284, startY));
//		buyArticleNum->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
//		buyArticleNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
//		buyArticleNum->setAnchorPoint(Vec2(0,0.5f));
//		buyArticleNum->setFontColor(blackColor);
//		buyArticleNum->setFontSize(20);
//        buyArticleNum->setText("1");
		this->addChild(buyArticleNum,2);
		
		startY -= gapY;
		//联系方式标签
		textWidth = 100;
		contactLabel = LabelTTF::create(pConfigMgr->text("t1000"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
		contactLabel->setColor(whiteColor);
		contactLabel->setPosition(Vec2(241,startY));
		this->addChild(contactLabel,2);
        
		//联系方式输入框
		textWidth = 400;
		contactInput = EditBox::create(cocos2d::Size(268, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
		contactInput->setPosition(Vec2(284, startY));
		contactInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		contactInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		contactInput->setAnchorPoint(Vec2(0,0.5f));
		contactInput->setFontColor(blackColor);
		contactInput->setFontSize(20);
		this->addChild(contactInput,2);
        
		//购买确定取消菜单
		Sprite * okNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk1.png"));
		Sprite * okSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyOk2.png"));
		Sprite * cancelNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel1.png"));
		Sprite * cancelSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyCancel2.png"));
		MenuItemSprite *okMenuItem = MenuItemSprite::create(okNormalSprite,
                                                                okSelectSprite,  CC_CALLBACK_1(ExchangerLayer::pressOk,this));
		MenuItemSprite *canceldrawMenuItem = MenuItemSprite::create(cancelNormalSprite,
                                                                        cancelSelectSprite,  CC_CALLBACK_1(ExchangerLayer::pressCancel,this));
		buyMenu = Menu::create(okMenuItem,canceldrawMenuItem,NULL);
		buyMenu->setPosition(Vec2(winSize.width * 0.5, 126));
		buyMenu->alignItemsHorizontallyWithPadding(20);
		this->addChild(buyMenu,2);
		this->setBuyViewVisible(false);
        
		this->setKeypadEnabled(true);
		//this->setTouchEnabled(true);
		bRet = true;
	}while(0);
	return bRet;
}

void ExchangerLayer::dismiss(Object *obj){
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void ExchangerLayer::setBuyViewVisible(bool flag)
{
	buyViewVisible = flag;
	buyBk->setVisible(flag);
	buyText->setVisible(flag);
	buyArticleType1->setVisible(flag);
	buyArticleTypeBk->setVisible(flag);
	buyArticleType2->setVisible(flag);
	buyArticleNumLabel->setVisible(flag);
	buyArticleNum->setVisible(flag);
    buyArticleNumBk->setVisible(flag);
	contactLabel->setVisible(flag);
	contactInput->setVisible(flag);
//	buyArticleNum->setText("");
	contactInput->setText("");
	buyMenu->setVisible(flag);
}

void ExchangerLayer::pressExchangeButton(Object *obj)
{
	if(buyViewVisible == true)
	{
		return;
	}
	currentType = ((Node *)obj)->getTag();
	for(int i = 0 ; i < itemNum ; i++){
		if(currentType == ExchangeButton_Start+i)
		{
			buyArticleType2->setString(pConfigMgr->text(String::createWithFormat("t%d", 589+i)->getCString()));
		}
		if(currentType<3){
			contactLabel->setString(pConfigMgr->text("t1000"));
		}else{
			contactLabel->setString(pConfigMgr->text("t1001"));
		}
	}
	this->setBuyViewVisible(true);
}

void ExchangerLayer::pressOk(Object *obj)
{
	//this->setBuyViewVisible(false);
	for(int i = 0 ; i < itemNum ; i++){
		if(currentType == ExchangeButton_Start+i)
		{
			
		}
	}
//    string str1 = buyArticleNum->getText();
    string str1 = "1";
    string str2 = contactInput->getText();
    if(str1!=""&&str2!="")
    {
        if(this->isNumber(str1.c_str()) == false){
            promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
            promptBox->setPromptText(pConfigMgr->text("t1007"));
           this->addChild(promptBox,3);
        }else if(this->isNumber(str2.c_str()) == false){
            promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
            promptBox->setPromptText(pConfigMgr->text("t1008"));
           this->addChild(promptBox,3);
        }else{
        this->pressExchange(currentType);
        }
    }else{
        promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
        promptBox->setPromptText(pConfigMgr->text("t1005"));
       this->addChild(promptBox,3);
    }
}

bool ExchangerLayer::isNumber(const char *buffer)
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

void ExchangerLayer::pressCancel(Object *obj)
{
	this->setBuyViewVisible(false);
}

/**µ•‘™∏Ò¥Û–°
≤Œ ˝£∫tableµ•‘™∏ÒÀ˘ Ùµƒ±Ì∏Ò
*/
cocos2d::Size ExchangerLayer::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(winSize.width, tableItemSize.height+tableGapY);
}

/**±Ì∏Ò÷–µ•‘™∏Òµƒ∏ˆ ˝
≤Œ ˝£∫tableµ•‘™∏ÒÀ˘ Ùµƒ±Ì∏Ò
*/
ssize_t ExchangerLayer::numberOfCellsInTableView(TableView *table)
{
	int size;
	size = itemNum/3;
	if(itemNum%3!=0)
	{
		size++;
	}
	return size;
}

/**µ•‘™∏Ò≥ı ºªØ
≤Œ ˝£∫tableµ•‘™∏ÒÀ˘ Ùµƒ±Ì∏Ò
*/
TableViewCell *ExchangerLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	char name[32];
	float startX = tableItemSize.width/2+tableGapX;
	float startY = tableItemSize.height/2+tableGapY;
	for(int i = 1 ; i < 4&&(((int)index*3+i)<=itemNum) ; i++)
	{
		std::sprintf(name, "%s%d.png", ExchangeTableFileName, (int)index*3+i);
		Sprite *cellBk = Sprite::createWithSpriteFrame(spriteFrame(name));
		cellBk->setPosition(Vec2(startX, startY));
		cell->addChild(cellBk);

		memset(name, 0, sizeof(name));	
		Sprite * buyNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ExchangeButton1.png"));
		Sprite * buySelectSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_ExchangeButton2.png"));
		MenuItemSprite *buyMenuItem = MenuItemSprite::create(buyNormalSprite, 
			buySelectSprite,  CC_CALLBACK_1(ExchangerLayer::pressExchangeButton,this));
		buyMenuItem->setTag(ExchangeButton_Start+((int)index*3+i-1));
		Menu *buyMenu = Menu::create(buyMenuItem,NULL);
		buyMenu->setPosition(Vec2(startX+tableItemSize.width/2-buyNormalSprite->getContentSize().width/2-8,
			startY-tableItemSize.height/2+buyNormalSprite->getContentSize().height/2+8));
		cell->addChild(buyMenu);
		startX+=tableItemSize.width+tableGapX;
	}
	return cell;  
}

/**µ„ª˜µ•‘™∏Ò
≤Œ ˝£∫table£¨µ•‘™∏ÒÀ˘ Ùµƒ±Ì∏Ò
cell£¨µ•‘™∏Ò
*/
void ExchangerLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void ExchangerLayer::scrollViewDidScroll(ScrollView* view)
{

}
void ExchangerLayer::scrollViewDidZoom(ScrollView* view)
{

}

//兑换
void ExchangerLayer::pressExchange(int id)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	char str[33];
	std::sprintf(str, "%lu", pMsgDispatch->m_dwUserID);
	string userId = string(str);
    promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
    promptBox->setPromptText(pConfigMgr->text("t556"));
   this->addChild(promptBox,3);
    string url = "http://117.18.5.229:999/mobile/ExCharge.aspx?filter=";
    url = url+userId;
    url+="&typid=";
    url+=String::createWithFormat("%d",id)->getCString();
    url=url+"&counts=";
    //    url+=buyArticleNum->getText();
    url+="1";
    url+="&uinfo=";
    url+=contactInput->getText();
    url+="&dt=";
    url+=(rand()%1000);
	this->setBuyViewVisible(false);
    network::HttpRequest* request = new network::HttpRequest();//¥¥Ω®request∂‘œÛ,’‚¿Ônew≥ˆ¿¥µƒ∂‘œÛ≤ªƒ‹ π”√autorelease()£¨‘≠“Ú∫Û ˆ
    request->setUrl(url.c_str());//…Ë÷√url
    request->setRequestType(network::HttpRequest::Type::GET);//…Ë÷√«Î«Û∑Ω Ω
    request->setResponseCallback(this, callfuncND_selector(ExchangerLayer::onHttpRequestCompleted));//’‚ «ªÿµ˜∂‘œÛ∫Õªÿµ˜∫Ø ˝
    request->setTag("GET test1");//…Ë÷√”√ªß±Í ∂£¨ø…“‘Õ®π˝responseªÒ»°
		network::HttpClient::getInstance()->send(request);// π”√network::HttpClientπ≤œÌ µ¿˝¿¥∑¢ÀÕrequest
    request->release();//µ˜”√release()
}

void ExchangerLayer::onHttpRequestCompleted(Node *sender, void *data)
{
    //return;
	if(cancelOrNot)
	{
		return;
	}
	network::HttpResponse *response = (network::HttpResponse*)data;
    
	if (!response)
	{
		return;
	}
    
	// ªÒ»°∂‘”¶requestµƒ◊÷∑˚¥Æ±Í ∂
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s completed", response->getHttpRequest()->getTag());
	}
	//ªÒ»°∑µªÿ¥˙¬Î£¨±»»Á200°¢404µ»
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	std::sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	//m_labelStatusCode->setString(statusString);
	CCLOG("response code: %d", statusCode);
    
	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer: %s", response->getErrorBuffer());//ø…“‘µ˜”√getErrorBuffer()¿¥ªÒ»°¥ÌŒÛ‘≠“Ú
		return;
	}
    
	// dump data
	std::vector<char> *buffer = response->getResponseData();//”√¿¥ªÒ»°Ω” ’µΩµƒ ˝æ›
	char dataBuffer[255] = {0};
	int dataSize = buffer->size();
    
	if(dataSize<=0)
	{
        return;
	}
    
	for (unsigned int i = 0; i < dataSize; i++)
	{
		dataBuffer[i] = (*buffer)[i];
	}
	
	if(promptBox == NULL){
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
		promptBox->setPromptText(dataBuffer);
		this->addChild(promptBox,3);
	}else{
		promptBox->setPromptText(dataBuffer);
	}
}