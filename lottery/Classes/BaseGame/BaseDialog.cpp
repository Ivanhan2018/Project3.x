#include "BaseDialog.h"

BaseDialog::BaseDialog()
{
}

BaseDialog::~BaseDialog()
{
}

bool BaseDialog::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->onTouchBegan = CC_CALLBACK_2(BaseDialog::onTouchBegan, this);
		m_pListener->onTouchMoved = CC_CALLBACK_2(BaseDialog::onTouchMoved, this);
		m_pListener->onTouchCancelled = CC_CALLBACK_2(BaseDialog::onTouchCancelled, this);
		m_pListener->onTouchEnded = CC_CALLBACK_2(BaseDialog::onTouchEnded, this);
		m_pListener->setSwallowTouches(true);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pListener, this);
		
		m_sVSize = Director::getInstance()->getVisibleSize();

		bRet = true;
	} while (0);

	return bRet;
}

bool BaseDialog::onTouchBegan(Touch *touch, Event *unused_event)
{
	log("BaseDialog::onTouchBegan");
	return true;
}

void BaseDialog::onTouchMoved(Touch *touch, Event *unused_event)
{
	log("BaseDialog::onTouchMoved");
}

void BaseDialog::onTouchCancelled(Touch *touch, Event *unused_event)
{
	log("BaseDialog::onTouchCancelled");
}

void BaseDialog::onTouchEnded(Touch *touch, Event *unused_event)
{
	log("BaseDialog::onTouchEnded");
}

void BaseDialog::closed()
{
	removeFromParent();
}
