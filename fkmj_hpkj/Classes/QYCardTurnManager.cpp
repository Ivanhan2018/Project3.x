#include "QYCardTurnManager.h"
#include "GameLib.h"
#include "WindowInfo.h"
#include "QYCardTurn.h"

USING_NS_CC;

FV_SINGLETON_STORAGE(QYCardTurnManager);

QYCardTurnManager::QYCardTurnManager()
	:m_pActCardTurn(NULL)
{
	cocos2d::Layer::init();
	WidgetScenceXMLparse kScence1("Script/CardTurnScence.xml",this);

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setSwallowsTouches(false);

	
	WidgetManager::addButtonCB("Button_OpenCardDirect0",this,button_selector(QYCardTurnManager::Button_OpenCardDirect0));
	WidgetManager::addButtonCB("Button_OpenCardDirect1",this,button_selector(QYCardTurnManager::Button_OpenCardDirect1));

	WidgetManager::addButtonCB("Button_Rotate",this,button_selector(QYCardTurnManager::Button_Rotate));
	
	m_bCardTurned[0] = false;
	m_bCardTurned[1] = false;

	m_bIsSelf = false;
}
QYCardTurnManager::~QYCardTurnManager()
{

}
void QYCardTurnManager::Button_OpenCardDirect0(cocos2d::Ref*,WidgetUserInfo*)
{
}
void QYCardTurnManager::Button_OpenCardDirect1(cocos2d::Ref*,WidgetUserInfo*)
{
}
void QYCardTurnManager::initCard()
{

	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"TurnCardRoot");

	Vec2 kSize;
	kSize.x = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"CardWidth"));
	kSize.y = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"CardHeight"));

	Vec2 kRealSize = WindowInfo::Instance().getRealPos(kSize);

	cocos2d::Node* pRootNode = getParent();
	while (pRootNode && pRootNode->getParent())
	{
		pRootNode = pRootNode->getParent();
	}

	m_pCardTurn1 = new QYCardTurn(kRealSize.x,kRealSize.y);
	pRootNode->addChild(m_pCardTurn1);
	WidgetManager::Instance().createWidget("TurnCardSkin1",m_pCardTurn1);
	m_pCardTurn1->setTurnPos(WindowInfo::Instance().getRealPos(WidgetFun::getChildWidget(this,"CardTurn1")->getPosition()));
	m_pCardTurn1->setTag(0);
	m_pCardTurn2 = new QYCardTurn(kRealSize.x,kRealSize.y);
	m_pCardTurn2->setTag(1);
	WidgetManager::Instance().createWidget("TurnCardSkin1",m_pCardTurn2);
	pRootNode->addChild(m_pCardTurn2);
	m_pCardTurn2->setTurnPos(WindowInfo::Instance().getRealPos(WidgetFun::getChildWidget(this,"CardTurn2")->getPosition()));

	m_pCardTurn1->getNodeFront()->setScaleX(kRealSize.x/kSize.x);
	m_pCardTurn1->getNodeFront()->setScaleY(kRealSize.y/kSize.y);
	m_pCardTurn1->getNodeBack()->setScaleX(kRealSize.x/kSize.x);
	m_pCardTurn1->getNodeBack()->setScaleY(kRealSize.y/kSize.y);

	m_pCardTurn2->getNodeFront()->setScaleX(kRealSize.x/kSize.x);
	m_pCardTurn2->getNodeFront()->setScaleY(kRealSize.y/kSize.y);
	m_pCardTurn2->getNodeBack()->setScaleX(kRealSize.x/kSize.x);
	m_pCardTurn2->getNodeBack()->setScaleY(kRealSize.y/kSize.y);

}
void QYCardTurnManager::showCard(bool bTouch,cocos2d::Node* pNode1Show,cocos2d::Node* pNode1Hide,
									cocos2d::Node* pNode2Show,cocos2d::Node* pNode2Hide)
{
	CCAssert(pNode1Show && pNode1Hide,"");
	m_bCardTurned[0] = false;
	m_bCardTurned[1] = false;
	m_pCardTurn1->getNodeFront()->removeAllChildren();
	m_pCardTurn1->getNodeBack()->removeAllChildren();
	m_pCardTurn2->getNodeFront()->removeAllChildren();
	m_pCardTurn2->getNodeBack()->removeAllChildren();

	m_pCardTurn1->getNodeFront()->addChild(pNode1Show);
	m_pCardTurn1->getNodeBack()->addChild(pNode1Hide);

	cocos2d::Vec2 kPos0 = WindowInfo::Instance().getRealPos(WidgetFun::getChildWidget(this,"CardTurn0")->getPosition());
	cocos2d::Vec2 kPos1 = WindowInfo::Instance().getRealPos(WidgetFun::getChildWidget(this,"CardTurn1")->getPosition());
	cocos2d::Vec2 kPos2 = WindowInfo::Instance().getRealPos(WidgetFun::getChildWidget(this,"CardTurn2")->getPosition());
	if (pNode2Show && pNode2Hide)
	{
		m_pCardTurn2->getNodeFront()->addChild(pNode2Show);
		m_pCardTurn2->getNodeBack()->addChild(pNode2Hide);

		m_pCardTurn1->setTurnPos(kPos1);
		m_pCardTurn2->setTurnPos(kPos2);
		m_pCardTurn2->setVisible(true);

		WidgetFun::setPos(m_pCardTurn1,"Img_RotateImg",kPos1);
		WidgetFun::setPos(m_pCardTurn2,"Img_RotateImg",kPos2);

		WidgetFun::setVisible(m_pCardTurn1,"Img_RotateImg",true);
		WidgetFun::setVisible(m_pCardTurn2,"Img_RotateImg",true);

		WidgetFun::setVisible(this,"CardTurn0",false);
		WidgetFun::setVisible(this,"CardTurn1",true);
		WidgetFun::setVisible(this,"CardTurn2",true);
	}
	else
	{
		m_pCardTurn1->setTurnPos(kPos0);
		m_pCardTurn2->setVisible(false);
		WidgetFun::setVisible(this,"CardTurn0",true);
		WidgetFun::setVisible(this,"CardTurn1",false);
		WidgetFun::setVisible(this,"CardTurn2",false);

		WidgetFun::setPos(m_pCardTurn1,"Img_RotateImg",kPos0);
		WidgetFun::setVisible(m_pCardTurn1,"Img_RotateImg",true);
		WidgetFun::setVisible(m_pCardTurn2,"Img_RotateImg",false);
	}
	setVisible(true);
	setTouchEnabled(bTouch);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->setPriority(_touchListener,-129);
	setSwallowsTouches(true);

	setAngle(0,true);
	m_pCardTurn1->show();
	m_pCardTurn2->show();

	if (!m_bIsSelf)
	{
		WidgetFun::setVisible(m_pCardTurn1,"Img_RotateImg",false);
		WidgetFun::setVisible(m_pCardTurn2,"Img_RotateImg",false);
	}
}
void QYCardTurnManager::HideCard()
{
	setVisible(false);
	setTouchEnabled(false);

	m_pCardTurn1->hide();
	m_pCardTurn2->hide();
}
bool QYCardTurnManager::GetTouchCardTurn(QYCardTurn* pCardTurn,cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kPosOffect = pCardTurn->getGridNodeTarget()->convertToNodeSpace(kPos);
	kPosOffect = pCardTurn->getPageTurn3D()->changePosToNode(kPosOffect);
	float fWidth = pCardTurn->getPageTurn3D()->getWidth();
	float fHeight = pCardTurn->getPageTurn3D()->getHeight();
	if (kPosOffect.x > 0 && kPosOffect.x < fWidth
		&& kPosOffect.y > 0 && kPosOffect.x < fHeight)
	{
		return true;
	}
	return false;
}
bool QYCardTurnManager::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (touch->getID() != 0)
	{
		return false;
	}
	onTouchesBegan(touch->getLocation());
	return true;
}

void QYCardTurnManager::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (touch->getID() != 0)
	{
		return;
	}
	onTouchesMoved(touch->getLocation());
}

void QYCardTurnManager::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (touch->getID() != 0)
	{
		return;
	}
	onTouchesEnded(touch->getLocation());
}

void QYCardTurnManager::onTouchCancelled(Touch *touch, Event *unused_event)
{
	if (touch->getID() != 0)
	{
		return;
	}
}
void QYCardTurnManager::onTouchesBegan(cocos2d::Vec2 kPos)
{
	m_bMoved = false;
	m_kBegainPos = kPos;
	m_pActCardTurn = NULL;
	if (GetTouchCardTurn(m_pCardTurn1,kPos))
	{
		m_pActCardTurn = m_pCardTurn1;

	}
	if (GetTouchCardTurn(m_pCardTurn2,kPos))
	{
		m_pActCardTurn = m_pCardTurn2;
	}
	if (m_pActCardTurn)
	{

		WidgetFun::setVisible(m_pActCardTurn,"Img_RotateImg",false);
	}
}
void QYCardTurnManager::onTouchesMoved(cocos2d::Vec2 kPos)
{
	if (!m_pActCardTurn)
	{
		if (GetTouchCardTurn(m_pCardTurn1,kPos))
		{
			m_pActCardTurn = m_pCardTurn1;
		}
		if (GetTouchCardTurn(m_pCardTurn2,kPos))
		{
			m_pActCardTurn = m_pCardTurn2;
		}
	}
	if (m_pActCardTurn)
	{
		if (!m_bMoved && m_kBegainPos.distance(kPos) < 10.0f)
		{
			return;
		}
		if (!m_bMoved)
		{
			cocos2d::Vec2 kTouchNodePos1 = m_pActCardTurn->getGridNodeTarget()->convertToNodeSpace(m_kBegainPos);
			m_pActCardTurn->getPageTurn3D()->setTouchPos(kTouchNodePos1);

			int iIdex1 = m_pActCardTurn->getTag();
			m_bCardTurned[m_pActCardTurn->getTag()] = true;
			if (m_pFun)
			{
				m_pFun(iIdex1,TYPE_BEGIN,kTouchNodePos1);
			}
		}

		cocos2d::Vec2 kTouchNodePos2 = m_pActCardTurn->getGridNodeTarget()->convertToNodeSpace(kPos);
		m_pActCardTurn->getPageTurn3D()->setMoveToPos(kTouchNodePos2);

		int iIdex2 = m_pActCardTurn->getTag();
		if (m_pFun)
		{
			m_pFun(iIdex2,TYPE_MOVE,kTouchNodePos2);
		}

		m_bMoved = true;
	}
}
void QYCardTurnManager::onTouchesEnded(cocos2d::Vec2 kPos)
{
	if (m_pActCardTurn)
	{
		int iIdex = m_pActCardTurn->getTag();
		if (m_bMoved)
		{
			cocos2d::Vec2 kTouchNodePos = m_pActCardTurn->getGridNodeTarget()->convertToNodeSpace(kPos);
			m_pActCardTurn->getPageTurn3D()->setEndPos(kTouchNodePos);

			if (m_pFun)
			{
				m_pFun(iIdex,TYPE_END,kTouchNodePos);
			}
		}

		if (!m_pActCardTurn->getPageTurn3D()->IsInMoveAction() && !m_bCardTurned[m_pActCardTurn->getTag()] && m_bIsSelf)
		{
			m_pActCardTurn->getPageTurn3D()->restCard();
			int fAngle = m_pActCardTurn->getPageTurn3D()->getAngle();
			fAngle = (fAngle+90)%360;
			m_pActCardTurn->getPageTurn3D()->setAngle(fAngle,false);

			if (m_pFun)
			{
				m_pFun(iIdex,TYPE_Rotate,Vec2(0,0));
			}
		}
	}
}
void QYCardTurnManager::onNetTouchAction(int iIdex,int iAction,cocos2d::Vec2 kPos)
{
	if (iIdex == 0)
	{
		m_pActCardTurn = m_pCardTurn1;
	}
	if (iIdex == 1)
	{
		m_pActCardTurn = m_pCardTurn2;
	}
	if (!m_pActCardTurn)
	{
		return;
	}
	if (iAction == TYPE_BEGIN)
	{
		m_pActCardTurn->getPageTurn3D()->setTouchPos(kPos);
	}
	if (iAction == TYPE_MOVE)
	{
		m_pActCardTurn->getPageTurn3D()->setMoveToPos(kPos);
	}
	if (iAction == TYPE_END)
	{
		m_pActCardTurn->getPageTurn3D()->setEndPos(kPos);
	}
	if (iAction == TYPE_Rotate)
	{
		m_pActCardTurn->getPageTurn3D()->restCard();
		int fAngle = m_pActCardTurn->getPageTurn3D()->getAngle();
		fAngle = (fAngle+90)%360;
		m_pActCardTurn->getPageTurn3D()->setAngle(fAngle,false);
	}
}
void QYCardTurnManager::setCallFun(std::function<void(int,int,cocos2d::Vec2)> pFun)
{
	m_pFun = pFun;
}

void QYCardTurnManager::AutoOpen()
{
	m_pCardTurn1->getPageTurn3D()->OpenDirect();
	m_pCardTurn2->getPageTurn3D()->OpenDirect();

	WidgetFun::setVisible(m_pCardTurn1,"Img_RotateImg",false);
	WidgetFun::setVisible(m_pCardTurn2,"Img_RotateImg",false);
}
void QYCardTurnManager::setAngle(float fAngle,bool bDirect)
{
	m_pCardTurn1->getPageTurn3D()->setAngle(fAngle,bDirect);
	m_pCardTurn2->getPageTurn3D()->setAngle(fAngle,bDirect);
}

void QYCardTurnManager::setEndCallBack(std::function<void(void)> pFun1,std::function<void(void)> pFun2)
{
	m_pCardTurn1->setEndCallFun(pFun1);
	m_pCardTurn2->setEndCallFun(pFun2);
}

bool QYCardTurnManager::isCardTurndEnd()
{
	if (WidgetFun::isWidgetVisble(this,"CardTurn0"))
	{
		return m_pCardTurn1->getIsTurnEnd();
	}
	else
	{
		return m_pCardTurn1->getIsTurnEnd() && m_pCardTurn2->getIsTurnEnd();
	}
	
}

void QYCardTurnManager::showOpenBtn(bool bShow)
{
	m_bIsSelf = bShow;
	WidgetFun::setVisible(this,"BJLOpenShadowInfo:BJL_OpenCard",bShow);
}

void QYCardTurnManager::showShadowInfo(bool bShow)
{
	if (bShow)
	{
		WidgetFun::runWidgetAction(this,"BJLOpenShadowInfo","ShowShadowInfo");
	}
	else
	{
		WidgetFun::runWidgetAction(this,"BJLOpenShadowInfo","HideShadowInfo");
	}
}

void QYCardTurnManager::setShadowInfoScale(float fScale)
{
	WidgetFun::setWidgetScale(this,"BJLOpenShadowInfo",fScale);
}

void QYCardTurnManager::setOpenPlayerName(const std::string& kName)
{
	WidgetFun::setText(this,"BJLOpenShadowInfo:LookPlayerName",utility::a_u8(kName));
}

void QYCardTurnManager::Button_Rotate(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
// 	std::string kIndex = WidgetFun::getUserInfoValue(pUserInfo,"TurndIndex");
// 	if (kIndex =="1")
// 	{
// 	}
// 	else if(kIndex =="2")
// 	{
// 		int fAngle = m_pCardTurn2->getPageTurn3D()->getAngle();
// 		fAngle = (fAngle+90)%360;
// 		m_pCardTurn2->getPageTurn3D()->setAngle(fAngle,false);
// 	}
}

