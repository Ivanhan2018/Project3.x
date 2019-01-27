#include "DDZVoiceAndFace.h"
#include "DDZRes.h"
#include "cocos-ext.h"
#include "DDZSceneData.h"
static DDZVoiceAndFace* s_face=NULL;
/////////////////////////////////////////////////DDZCC_scrollView
bool DDZCC_scrollView::init()
{
	//   显示显示的区域
	m_scrollView =ScrollView::create(cocos2d::Size(260,280),NULL);
	m_scrollView->setAnchorPoint(Vec2::ZERO);
	m_scrollView->setPosition(Vec2::ZERO);
	m_scrollView->setTouchEnabled(false);
	//m_scrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
	m_scrollView->setBounceable(true);
	m_scrollView->setDelegate(this);
	this->addChild(m_scrollView);
	return true;
}

void DDZCC_scrollView::addViewNode( Node* _Node,cocos2d::Size _size)
{
	m_scrollView->setContainer(_Node);
	m_scrollView->setContentSize(_size);
}

void DDZCC_scrollView::scrollViewDidScroll( ScrollView* view )
{

}



/////////////////////////////////////////////////DDZCC_listView
bool DDZCC_listView::init()
{
	do
	{
		//CC_BREAK_IF( !Layer::init() );
		cocos2d::Size size =Director::getInstance()->getWinSize();
		m_tableView =TableView::create(this,cocos2d::Size(220,300));
		m_tableView->setPosition(ccp(0,0));
		//m_tableView->setDirection(kCCScrollViewDirectionVertical);


		m_tableView->setDelegate(this);  
		//m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);  
		m_tableView->reloadData();// add  
		this->addChild(m_tableView, 1);  
		m_tableView->setTouchEnabled(false);
		//setTouchEnabled(false);
	} while (0);
	return true;
}

cocos2d::Size DDZCC_listView::cellSizeForTable( TableView *table )
{
	return cocos2d::Size(100,50);
}

TableViewCell* DDZCC_listView::tableCellAtIndex( TableView *table, ssize_t idx )
{
	Dictionary * strings = Dictionary::createWithContentsOfFile("xml/strings.xml");
	const char* buff=((CCString*)strings->objectForKey(CCString::createWithFormat("wav%d",idx)->getCString()))->getCString();
	TableViewCell *pCell = table->dequeueCell();
	if (!pCell) {
		pCell = new TableViewCell();
		pCell->autorelease();
		Sprite *pSprite = Sprite::createWithSpriteFrameName(DDZtexture_name::s_yyk);
		pSprite->setAnchorPoint(Vec2::ZERO);
		pSprite->setPosition(Vec2::ZERO);
		pCell->addChild(pSprite);
		pCell->setTag(idx);

		LabelTTF *pLabel = LabelTTF::create(buff, "", 14.0);
		pLabel->setAnchorPoint(ccp(0.5,0.5));
		pLabel->setPosition(ccp(pSprite->getContentSize().width*0.5,pSprite->getContentSize().height*0.5));
		pLabel->setTag(123);
		pCell->addChild(pLabel);
		pLabel->setColor(ccc3(173,118,0));
	}
	else
	{
		LabelTTF *pLabel = (LabelTTF*)pCell->getChildByTag(123);
		pLabel->setString(buff);
		pCell->setTag(idx);
	}

	return pCell; 
}

ssize_t DDZCC_listView::numberOfCellsInTableView(TableView *table )
{
	return 6;
}

void DDZCC_listView::tableCellTouched(TableView* table,TableViewCell* cell )
{
	CCLOG("tableCellTouched\n"); 
	DDZCCfaceAndVioce* _data = DDZCCfaceAndVioce::create(DDZmsgToLogic_face);
	_data->m_Type =2;
	_data->m_KeyID = cell->getTag();
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_data);
	if (s_face) s_face->runOutOrIn();
}


bool DDZVoiceAndFace::init()
{
	bool ret =false;
	do 
	{
		cocos2d::Size WinSize =Director::getInstance()->getWinSize();

		m_BackSpt =Sprite::createWithSpriteFrameName(DDZtexture_name::s_faceBg);
		m_BackSpt->setPosition(ccp(WinSize.width*0.5,WinSize.height*0.5));
		this->addChild(m_BackSpt);
		
// 		m_VoiceSlide= Sprite::createWithSpriteFrameName(DDZtexture_name::s_xlt);
// 		m_VoiceSlide->setPosition(ccp(TEXBACK_WIDTH*0.97,TEXBACK_HEIGHT*0.5));
// 		m_BackSpt->addChild(m_VoiceSlide);
				
		//--表情设置
		m_FaceNode =Node::create();
		m_faceCount=0;
		setFaceElement(0,DDZtexture_name::s_faceCool);
		setFaceElement(1,DDZtexture_name::s_faceHan);
		setFaceElement(2,DDZtexture_name::s_faceHanlen);
		setFaceElement(3,DDZtexture_name::s_faceKu);
		setFaceElement(4,DDZtexture_name::s_faceNu);
		setFaceElement(5,DDZtexture_name::s_faceNue);
		setFaceElement(6,DDZtexture_name::s_faceQiu);
		setFaceElement(7,DDZtexture_name::s_faceTu);
		setFaceElement(8,DDZtexture_name::s_faceXiao);
		setFaceElement(9,DDZtexture_name::s_faceYun);

		m_scrollView=DDZCC_scrollView::create();
		m_scrollView->addViewNode(m_FaceNode,cocos2d::Size(250,/*m_faceCount/Face_H_Count*68)*/360));			//--此处主要我们画图是从上往下画，控件排布是下往上排
		m_scrollView->setPosition(35,32);
		m_BackSpt->addChild(m_scrollView);

		//--语言设置
		m_VoiceNode=DDZCC_listView::create();
		m_VoiceNode->setPosition(ccp(TEXBACK_WIDTH*0.67,TEXBACK_HEIGHT*0.11));
		m_BackSpt->addChild(m_VoiceNode);

		//setTouchEnabled(true);
		//setTouchMode(kCCTouchesOneByOne);
		m_isOut =true;
		s_face=this;
		this->setPosition(ccp(this->getPositionX(),WinSize.height*1.0));
		ret =true;
	} while (0);
	return ret;
}

void DDZVoiceAndFace::setFaceElement( int _type,const char * _name )
{
	cocos2d::Size size=cocos2d::Size(TEXBACK_WIDTH,TEXBACK_HEIGHT);
	size.width=size.width*0.5;
	Vec2 basept =ccp(size.width*0.2,size.height*0.2);
	Sprite * _spt=Sprite::createWithSpriteFrameName(_name);
	_spt->setPosition(ccp((m_faceCount%Face_W_Count)*size.width*(0.9f/Face_W_Count)+basept.x,
	basept.y+(m_faceCount/Face_W_Count)*size.height*(0.9/Face_H_Count)));
	_spt->setTag(_type);
	m_FaceNode->addChild(_spt,_type);
	m_faceCount++;
}

bool DDZVoiceAndFace::onTouchBegan(Touch *pTouch,Event *pEvent)
{
	Vec2 pt =pTouch->getLocation();
	if (m_isOut==true)return false;

	if (!m_BackSpt->boundingBox().containsPoint(pt))
	{
		runOutOrIn();
		return true;
	}
	m_TouchBeganPos=pTouch->getLocation();
	m_VoiceNode->m_tableView->ccTouchBegan(pTouch,pEvent);
	m_scrollView->m_scrollView->ccTouchBegan(pTouch,pEvent);
	return true;
}

void DDZVoiceAndFace::onTouchMoved(Touch *pTouch,Event *pEvent)
{
	m_VoiceNode->m_tableView->ccTouchMoved(pTouch,pEvent);
	m_scrollView->m_scrollView->ccTouchMoved(pTouch,pEvent);
}

void DDZVoiceAndFace::onTouchEnded(Touch *pTouch,Event *pEvent)
{
	Object* pObj=NULL;
	Vec2 pt =pTouch->getLocation();
	m_VoiceNode->m_tableView->ccTouchEnded(pTouch,pEvent);
	m_scrollView->m_scrollView->ccTouchEnded(pTouch,pEvent);
	if(ccpLength(ccp(pt.x-m_TouchBeganPos.x,pt.y-m_TouchBeganPos.y))<=10)
	{
		//--点击表情
		//Array_FOREACH(m_FaceNode->getChildren(),pObj)
		for(int i = 0;i < m_FaceNode->getChildrenCount() - 1;i ++)
		{
			Sprite* pNode=(Sprite*)pObj;
			Vec2 _Nodept=m_FaceNode->convertToNodeSpace(pt);
			if (pNode->boundingBox().containsPoint(_Nodept))
			{
				DDZCCfaceAndVioce* _data = DDZCCfaceAndVioce::create(DDZmsgToLogic_face);
				_data->m_Type =1;
				_data->m_KeyID = pNode->getTag();
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOLOGIC,_data);
				runOutOrIn();
				return ;
			}
		}
		//--点击语音在list里面处理
	}
}

void DDZVoiceAndFace::onTouchCancelled(Touch *pTouch,Event *pEvent)
{

}

void DDZVoiceAndFace::runOutOrIn(bool isVisible)
{
	cocos2d::Size size=Director::getInstance()->getWinSize();
	m_isOut=!m_isOut; 
	this->stopAllActions();
	this->runAction(MoveTo::create(0.2f,m_isOut?ccp(this->getPositionX(),size.height*1.0):ccp(this->getPositionX(),size.height*0.00)));
}

void DDZVoiceAndFace::onEnter()
{
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min)(),true);
	Layer::onEnter();
}

void DDZVoiceAndFace::onExit()
{
	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
}

