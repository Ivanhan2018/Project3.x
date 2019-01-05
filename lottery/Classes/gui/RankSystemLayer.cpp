#include "RankSystemLayer.h"
#include "JsonHelper.h"
#include "EntityMgr.h"

#include "network/HttpClient.h"

RankSystemLayer::RankSystemLayer()
{
	richTable = NULL;
	heroTable = NULL;
	luckTable = NULL;
	teacherTable = NULL;
	charmTable = NULL;
}

RankSystemLayer::~RankSystemLayer()
{

}

void RankSystemLayer::OnExit()
{
	if(richTable!=NULL)
		richTable->release();
	if(heroTable!=NULL)
		heroTable->release();
	if(luckTable!=NULL)
		luckTable->release();
	if(teacherTable!=NULL)
		teacherTable->release();
	if(charmTable!=NULL)
		charmTable->release();
}

RankSystemLayer *RankSystemLayer::create()
{
	Array *normalName = Array::create();
	Array *selectedName = Array::create();
	normalName->addObject(String::create("Tab_RichBoard1.png"));
	selectedName->addObject(String::create("Tab_RichBoard2.png"));
	normalName->addObject(String::create("Tab_HeroBoard1.png"));
	selectedName->addObject(String::create("Tab_HeroBoard2.png"));
	normalName->addObject(String::create("Tab_LuckBoard1.png"));
	selectedName->addObject(String::create("Tab_LuckBoard2.png"));
	normalName->addObject(String::create("Tab_TeacherBoard1.png"));
	selectedName->addObject(String::create("Tab_TeacherBoard2.png"));
	normalName->addObject(String::create("Tab_CharmBoard1.png"));
	selectedName->addObject(String::create("Tab_CharmBoard2.png"));
	return RankSystemLayer::createWithTabItems(normalName, selectedName);
}

RankSystemLayer *RankSystemLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	RankSystemLayer *tabLayer = new RankSystemLayer();
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

void RankSystemLayer::initView()
{
	richRankText = ConfigMgr::instance()->text("t578");
	heroRankText = heroRankText;
	luckRankText = heroRankText;
	teacherRankText = heroRankText;
	charmRankText = heroRankText;

	float gapX = 11;
	float gapY = 10;
	winSize = Director::getInstance()->getWinSize();
	winSize.width -= gapX*2;

	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = 225;

	myRankBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board6.png"));
	tableItemSize.width = winSize.width;
	tableItemSize.height = myRankBk->getContentSize().height;
	//�ҵ����б���ͼ
	myRankBk->setAnchorPoint(Vec2(0.5,0.5));
	myRankBk->setScaleX(winSize.width/myRankBk->getContentSize().width);
	myRankBk->setScaleY(0.75f);
	myRankBk->setPosition(Vec2(400, tableViewSize.height+gapY+tableItemSize.height*0.75f*0.5f));
	this->addChild(myRankBk,1);	

	float textWidth = 200;
	myRankLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t560"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentRight);
	myRankLabel1->setColor(ccc3(255,255,255));
	myRankLabel1->setAnchorPoint(Vec2(1,0.5f));
	myRankLabel1->setPosition(Vec2(myRankBk->getPositionX(), myRankBk->getPositionY()-1));
	this->addChild(myRankLabel1, 1);

	myRankLabel2 = LabelTTF::create(richRankText.c_str(),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	myRankLabel2->setColor(ccc3(255,255,255));
	myRankLabel2->setAnchorPoint(Vec2(0,0.5f));
	myRankLabel2->setPosition(myRankLabel1->getPosition());
	this->addChild(myRankLabel2, 1);
	//�����Ϸ�ͼƬ
	rankBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board5.png"));
	rankBk->setAnchorPoint(Vec2(0.5,0.5));
	rankBk->setPosition(Vec2(400, tableViewSize.height+gapY+tableItemSize.height*0.75f+rankBk->getContentSize().height/2-30));
	this->addChild(rankBk,1);	
	float value = 21;
	myRankFieldLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t561"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	myRankFieldLabel1->setColor(ccc3(255,255,255));
	myRankFieldLabel1->setPosition(Vec2(118, rankBk->getPositionY()-value));
	this->addChild(myRankFieldLabel1, 1);

	myRankFieldLabel2 = LabelTTF::create(ConfigMgr::instance()->text("t562"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	myRankFieldLabel2->setColor(ccc3(255,255,255));
	myRankFieldLabel2->setPosition(Vec2(244+146, myRankFieldLabel1->getPositionY()));
	this->addChild(myRankFieldLabel2, 1);

	myRankFieldLabel3 = LabelTTF::create(ConfigMgr::instance()->text("t563"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	myRankFieldLabel3->setColor(ccc3(255,255,255));
	myRankFieldLabel3->setPosition(Vec2(428, myRankFieldLabel1->getPositionY()));
//	this->addChild(myRankFieldLabel3, 1);

	myRankFieldLabel4 = LabelTTF::create(ConfigMgr::instance()->text("t564"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	myRankFieldLabel4->setColor(ccc3(255,255,255));
	myRankFieldLabel4->setPosition(Vec2(624, myRankFieldLabel1->getPositionY()));
	this->addChild(myRankFieldLabel4, 1);
	
	RankTableHelper *richTableHelper;
	RankTableHelper *heroTableHelper;
	RankTableHelper *luckTableHelper;
	RankTableHelper *teacherTableHelper;
	RankTableHelper *charmTableHelper;
	//�Ƹ���
	richTableHelper = new RankTableHelper(this, 8, RankTableTag_Rich);
	richTable = TableView::create(richTableHelper, tableViewSize);
	richTable->setDelegate(richTableHelper);
	richTable->setAnchorPoint(Vec2(0.5, 0.5));
	richTable->setPosition(Vec2(gapX, gapY));
	richTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	richTable->setDirection(ScrollView::Direction::VERTICAL);
	richTable->retain();
	this->addChild(richTable, 1);
	//Ӣ�۰�
	heroTableHelper = new RankTableHelper(this, 8, RankTableTag_Hero);
	heroTable = TableView::create(heroTableHelper, tableViewSize);
	heroTable->setDelegate(heroTableHelper);
	heroTable->setAnchorPoint(Vec2(0.5, 0.5));
	heroTable->setPosition(Vec2(gapX, gapY));
	heroTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	heroTable->setDirection(ScrollView::Direction::VERTICAL);
	heroTable->retain();
	this->addChild(heroTable, 1);
	heroTable->setVisible(false);
	//���˰�
	luckTableHelper = new RankTableHelper(this, 10, RankTableTag_Luck);
	luckTable = TableView::create(luckTableHelper, tableViewSize);
	luckTable->setDelegate(luckTableHelper);
	luckTable->setAnchorPoint(Vec2(0.5, 0.5));
	luckTable->setPosition(Vec2(gapX, gapY));
	luckTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	luckTable->setDirection(ScrollView::Direction::VERTICAL);
	luckTable->retain();
	this->addChild(luckTable, 1);
	luckTable->setVisible(false);
	//��ʦ��
	teacherTableHelper = new RankTableHelper(this, 12, RankTableTag_Teacher);
	teacherTable = TableView::create(teacherTableHelper, tableViewSize);
	teacherTable->setDelegate(teacherTableHelper);
	teacherTable->setAnchorPoint(Vec2(0.5, 0.5));
	teacherTable->setPosition(Vec2(gapX, gapY));
	teacherTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	teacherTable->setDirection(ScrollView::Direction::VERTICAL);
	teacherTable->retain();
	this->addChild(teacherTable, 1);
	teacherTable->setVisible(false);
	//������
	charmTableHelper = new RankTableHelper(this, 14, RankTableTag_Charm);
	charmTable = TableView::create(charmTableHelper, tableViewSize);
	charmTable->setDelegate(charmTableHelper);
	charmTable->setAnchorPoint(Vec2(0.5, 0.5));
	charmTable->setPosition(Vec2(gapX, gapY));
	charmTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	charmTable->setDirection(ScrollView::Direction::VERTICAL);
	charmTable->retain();
	this->addChild(charmTable, 1);
	charmTable->setVisible(false);


	richTable->reloadData();
	heroTable->reloadData();
	luckTable->reloadData();
	teacherTable->reloadData();
	charmTable->reloadData();

	this->setKeypadEnabled(true);
	//this->setTouchEnabled(true);
	//setTouchEnabled(true);
	flag = false;
}

void RankSystemLayer::doSomethingBeforeClosed()
{
	flag = true;
}

void RankSystemLayer::doSomething()
{
	CCLOG("RankSystemLayer::doSomething");
	if(currentItem == TabLayerBaseTag){
		myRankFieldLabel4->setString(ConfigMgr::instance()->text("t564"));
		richTable->setVisible(true);
		heroTable->setVisible(false);
		luckTable->setVisible(false);
		teacherTable->setVisible(false);
		charmTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+1){
		myRankFieldLabel4->setString(ConfigMgr::instance()->text("t565"));
		richTable->setVisible(false);
		heroTable->setVisible(true);
		luckTable->setVisible(false);
		teacherTable->setVisible(false);
		charmTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+2){
		myRankFieldLabel4->setString(ConfigMgr::instance()->text("t566"));
		richTable->setVisible(false);
		heroTable->setVisible(false);
		luckTable->setVisible(true);
		teacherTable->setVisible(false);
		charmTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+3){
		myRankFieldLabel4->setString(ConfigMgr::instance()->text("t567"));
		richTable->setVisible(false);
		heroTable->setVisible(false);
		luckTable->setVisible(false);
		teacherTable->setVisible(true);
		charmTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+4){
		myRankFieldLabel4->setString(ConfigMgr::instance()->text("t568"));
		richTable->setVisible(false);
		heroTable->setVisible(false);
		luckTable->setVisible(false);
		teacherTable->setVisible(false);
		charmTable->setVisible(true);
	}
	if(currentItem == TabLayerBaseTag){
		myRankLabel2->setString(richRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+1){
		myRankLabel2->setString(heroRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+2){
		myRankLabel2->setString(luckRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+3){
		myRankLabel2->setString(teacherRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+4){
		myRankLabel2->setString(charmRankText.c_str());
	}
}

void RankSystemLayer::setMyRankText(string text, RankTableTag tag)
{
	if(flag == true){
		return;
	}
	if(tag == RankTableTag_Rich)
	{
		richRankText = text;
	}else if(tag == RankTableTag_Hero)
	{
		heroRankText = text;
	}else if(tag == RankTableTag_Luck)
	{
		luckRankText = text;
	}else if(tag == RankTableTag_Teacher)
	{
		teacherRankText = text;
	}else if(tag == RankTableTag_Charm)
	{
		charmRankText = text;
	}
	if(currentItem == TabLayerBaseTag){
		myRankLabel2->setString(richRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+1){
		myRankLabel2->setString(heroRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+2){
		myRankLabel2->setString(luckRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+3){
		myRankLabel2->setString(teacherRankText.c_str());
	}else if(currentItem == TabLayerBaseTag+4){
		myRankLabel2->setString(charmRankText.c_str());
	}
}

void RankSystemLayer::reloadData(RankTableTag tag)
{
	if(flag == true){
		return;
	}
	if(tag == RankTableTag_Rich)
	{
		richTable->reloadData();
	}else if(tag == RankTableTag_Hero)
	{
		heroTable->reloadData();
	}else if(tag == RankTableTag_Luck)
	{
		luckTable->reloadData();
	}else if(tag == RankTableTag_Teacher)
	{
		teacherTable->reloadData();
	}else if(tag == RankTableTag_Charm)
	{
		charmTable->reloadData();
	}
}

void RankSystemLayer::sendButton(Object *obj)
{

}

void RankSystemLayer::deleteButton(Object *obj)
{

}

void RankSystemLayer::chatButton(Object *obj)
{

}

void RankSystemLayer::pressAddButton(Object *obj)
{

}
/**��Ԫ���С
������table��Ԫ������ı��
*/
cocos2d::Size RankSystemLayer::cellSizeForTable(TableView *table)
{
	return tableItemSize;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t RankSystemLayer::numberOfCellsInTableView(TableView *table)
{
	int size;
	size = 20;
	return size;
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *RankSystemLayer::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	//����ͼ
	Sprite *bk;
	if(index%2 == 1){
		bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board6.png"));
	}else{
		bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board7.png"));
	}
	bk->setScaleX(winSize.width/bk->getContentSize().width);
	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(0,0));
	cell->addChild(bk);
	//����
	Sprite *awardSprite;
	if(index == 0){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board1.png"));
	}else if(index == 1){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board2.png"));
	}else if(index == 2){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board3.png"));
	}else{
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board4.png"));
	}
	awardSprite->setAnchorPoint(Vec2(0,0.5f));
	awardSprite->setPosition(Vec2(17,tableItemSize.height/2));
	cell->addChild(awardSprite);

	char str[5];
	sprintf(str, "%d", index+1);
	float textWidth = 100;
	LabelTTF *rankLabel = LabelTTF::create(str,"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	rankLabel->setColor(ccc3(255,255,255));
	rankLabel->setAnchorPoint(Vec2(0,0.5f));
	rankLabel->setPosition(Vec2(95, tableItemSize.height/2));
	cell->addChild(rankLabel);

	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void RankSystemLayer::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void RankSystemLayer::scrollViewDidScroll(ScrollView* view)
{

}

void RankSystemLayer::scrollViewDidZoom(ScrollView* view)
{

}

/**��Ԫ���С
������table��Ԫ������ı��
*/

void RankTableHelper::cancelHttpRequest(Object *obj)
{
	cancelOrNot = true;
}

RankTableHelper::RankTableHelper(RankSystemLayer *fath, int num, RankTableTag tag)
{
	cancelOrNot = false;
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(RankTableHelper::cancelHttpRequest),"CancelHttpRequest", NULL);
	
	nicknameArray = Array::create();
	nicknameArray->retain();
	field4ValueArray = Array::create();
	field4ValueArray->retain();
	float gapX = 11;
	float gapY = 10;
	father = fath;
	winSize = Director::getInstance()->getWinSize();
	winSize.width -= gapX*2;
	Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board6.png"));
	tableItemSize.height = tempSprite->getContentSize().height;
	tableItemSize.width = winSize.width;
	itemsNum = num;
	tableTag = tag;
	url = "http://117.18.5.229:999/mobile/GetWealthRank.aspx?uid=";
	string urlExtra = "&tid=";
	char str[33];
	sprintf(str, "%lu", EntityMgr::instance()->getDispatch()->m_dwUserID);
	string userId = string(str);
	if(tag == RankTableTag_Rich)
	{
		string id = "1";
		url = url + userId + urlExtra + id;
	}else if(tag == RankTableTag_Hero)
	{
		string id = "4";
		url = url + userId + urlExtra + id;
	}else if(tag == RankTableTag_Luck)
	{
		string id = "2";
		url = url + userId + urlExtra + id;
	}else if(tag == RankTableTag_Teacher)
	{
		string id = "5";
		url = url + userId + urlExtra + id;
	}else if(tag == RankTableTag_Charm)
	{
		string id = "3";
		url = url + userId + urlExtra + id;
	}
	network::HttpRequest* request = new network::HttpRequest();//����request����,����new�����Ķ�����ʹ��autorelease()��ԭ�����  
	request->setUrl(url.c_str());//����url  
	request->setRequestType(network::HttpRequest::Type::GET);//��������ʽ  
	request->setResponseCallback(this, callfuncND_selector(RankTableHelper::onHttpRequestCompleted));//���ǻص�����ͻص�����  
	request->setTag("GET test1");//�����û���ʶ������ͨ��response��ȡ  
	httpClient = network::HttpClient::getInstance();
	httpClient->send(request);//ʹ��network::HttpClient����ʵ��������request  
	request->release();//����release() 

}

RankTableHelper::~RankTableHelper()
{
	if(nicknameArray!=NULL)
		nicknameArray->release();
	if(field4ValueArray!=NULL)
		field4ValueArray->release();
	NotificationCenter::getInstance()->removeObserver(this, "CancelHttpRequest");
}


void RankTableHelper::onHttpRequestCompleted(Node *sender, void *data)  
{  
	if(cancelOrNot)
	{
		return;
	}
	network::HttpResponse *response = (network::HttpResponse*)data;  

	if (!response)  
	{  
		return;  
	}  

	// ��ȡ��Ӧrequest���ַ�����ʶ  
	if (0 != strlen(response->getHttpRequest()->getTag()))   
	{  
		CCLOG("%s completed", response->getHttpRequest()->getTag());  
	}  
	//��ȡ���ش��룬����200��404��  
	int statusCode = response->getResponseCode();  
	char statusString[64] = {};  
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());  
	//m_labelStatusCode->setString(statusString);  
	CCLOG("response code: %d", statusCode);  

	if (!response->isSucceed())   
	{  
		CCLOG("response failed");  
		CCLOG("error buffer: %s", response->getErrorBuffer());//���Ե���getErrorBuffer()����ȡ����ԭ��  
		return;  
	}  

	// dump data  
	std::vector<char> *buffer = response->getResponseData();//������ȡ���յ�������  
	char dataBuffer[32768]; 
	int dataSize = buffer->size();

	if(dataSize<=0)
	{
		return;
	}

	for (unsigned int i = 0; i < dataSize; i++)  
	{  
		dataBuffer[i] = (*buffer)[i];  
	}   
	int index = 0;
	Json::Value rootValue = JsonHelper::getJsonFromData(dataBuffer, dataSize);
	int tempSize = rootValue.size();
	Json::Value rootValue1 = rootValue["Table"];
	tempSize = rootValue1.size();
	Json::Value value;
	value = rootValue1["GemeInfoTable"];

	int size = value.size();
	for(Json::ValueIterator it = value.begin() ; it!=value.end() ; it++)
	{
		Json::Value childValue = *it;
		string nicknameStr = childValue["NickName"].asString();
		if(nicknameStr == "")
		{
			nicknameStr = childValue["nickname"].asString();
		}
		string field4Str;
		if(tableTag == RankTableTag_Rich)
		{
			field4Str = childValue["score"].asString();
		}else if(tableTag == RankTableTag_Hero)
		{
			field4Str = childValue["WinCount"].asString();
		}else if(tableTag == RankTableTag_Luck)
		{
			field4Str = childValue["money"].asString();
		}else if(tableTag == RankTableTag_Teacher)
		{
			field4Str = childValue["Spreadercount"].asString();
		}else if(tableTag == RankTableTag_Charm)
		{
			field4Str = childValue["LoveLiness"].asString();
		}
		this->addTableData(nicknameStr, field4Str);
	}
	index = 0;
	rootValue1 = rootValue["Table1"];
	value = rootValue1["GemeInfoTable1"];

	size = value.size();
	Json::Value childValue = value[index];
	string rankText = childValue["rows"].asString();

	if(rankText == "")
	{
		father->setMyRankText(ConfigMgr::instance()->text("t578"), tableTag);
	}
	else
	{
		father->setMyRankText(childValue["rows"].asString().c_str(), tableTag);
	}
	father->reloadData(tableTag);
} 

void RankTableHelper::addTableData(string nickname, string field4)
{
	if(nicknameArray == NULL){
		nicknameArray = Array::create();
	}
	if(field4ValueArray == NULL){
		field4ValueArray = Array::create();
	}
	String *nickCCStr = String::create(nickname);
	String *field4CCStr = String::create(field4);
	nicknameArray->addObject(nickCCStr);
	field4ValueArray->addObject(field4CCStr);
}

cocos2d::Size RankTableHelper::cellSizeForTable(TableView *table)
{
	Sprite *tempSprite;
	tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board6.png"));
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = tempSprite->getContentSize().height;
	return size;
}

/**����е�Ԫ��ĸ���
������table��Ԫ������ı��
*/
ssize_t RankTableHelper::numberOfCellsInTableView(TableView *table)
{
	return nicknameArray->count();
}

/**��Ԫ���ʼ��
������table��Ԫ������ı��
*/
TableViewCell *RankTableHelper::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	//����ͼ
	Sprite *bk;
	if(index%2 == 1){
		bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board6.png"));
	}else{
		bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board7.png"));
	}
	bk->setScaleX(winSize.width/bk->getContentSize().width);
	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(0,0));
	cell->addChild(bk);
	//����
	Sprite *awardSprite;
	if(index == 0){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board1.png"));
	}else if(index == 1){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board2.png"));
	}else if(index == 2){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board3.png"));
	}else{
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board4.png"));
	}
	awardSprite->setAnchorPoint(Vec2(0,0.5f));
	awardSprite->setPosition(Vec2(17,tableItemSize.height/2));
	cell->addChild(awardSprite);

	char str[5];
	sprintf(str, "%d", index+1);
	float textWidth = 100;
	LabelTTF *rankLabel = LabelTTF::create(str,"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	rankLabel->setColor(ccc3(255,255,255));
	rankLabel->setAnchorPoint(Vec2(0,0.5f));
	rankLabel->setPosition(Vec2(95, tableItemSize.height/2));
	cell->addChild(rankLabel);

	textWidth = 180;
	LabelTTF *nicknameLabel;

	if(index > nicknameArray->count()){
		nicknameLabel =  LabelTTF::create("","",20,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	}else{
		String *nicknameCCStr = (String *)nicknameArray->objectAtIndex(index);
		nicknameLabel = LabelTTF::create(nicknameCCStr->getCString(),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	}
	nicknameLabel->setColor(ccc3(255,255,255));
	nicknameLabel->setAnchorPoint(Vec2(0.5f,0.5f));
	nicknameLabel->setPosition(Vec2(244+146, tableItemSize.height/2));
	cell->addChild(nicknameLabel);

	LabelTTF *field4Label;
	if(index > field4ValueArray->count()){
		field4Label =  LabelTTF::create("","",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	}else{
		String *field4CCStr = (String *)field4ValueArray->objectAtIndex(index);
		field4Label = LabelTTF::create(field4CCStr->getCString(),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	}
	field4Label->setColor(ccc3(255,255,255));
	field4Label->setAnchorPoint(Vec2(0,0.5f));
	field4Label->setPosition(Vec2(600, tableItemSize.height/2));
	cell->addChild(field4Label);

	LabelTTF *chengHaoLabel;
	if(index > field4ValueArray->count()){
		chengHaoLabel =  LabelTTF::create("","",20,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	}else{
		chengHaoLabel = LabelTTF::create(ConfigMgr::instance()->text("t10"),"",30,cocos2d::Size(textWidth,0),kCCTextAlignmentCenter);
	}
	chengHaoLabel->setColor(ccc3(255,255,255));
	chengHaoLabel->setAnchorPoint(Vec2(0.5f,0.5f));
	chengHaoLabel->setPosition(Vec2(428, tableItemSize.height/2));
//	cell->addChild(chengHaoLabel);
	return cell;  
}

/**�����Ԫ��
������table����Ԫ������ı��
cell����Ԫ��
*/
void RankTableHelper::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void RankTableHelper::scrollViewDidScroll(ScrollView* view)
{

}
void RankTableHelper::scrollViewDidZoom(ScrollView* view)
{

}
