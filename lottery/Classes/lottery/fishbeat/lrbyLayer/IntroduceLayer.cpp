#include "IntroduceLayer.h"
#include "FishKindScrollView.h"


USING_NS_CC;
using namespace cocostudio;
using namespace ui;


IntroduceLayer::IntroduceLayer()
{
	m_btnQuit = nullptr;
}

IntroduceLayer::~IntroduceLayer()
{
}

bool IntroduceLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	LayerColor* color = LayerColor::create(ccc4(0, 0, 0, 120));
	color->setContentSize(cocos2d::Size(1280, 720));
	this->addChild(color);
	this->setContentSize(color->getContentSize());
	
	auto bg = ui::ImageView::create("709/Introduce/BackGround_Big.png");
	
	bg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(bg);

	m_btnQuit = ui::Button::create("709/Introduce/btn_quit.png","","");
	m_btnQuit->setPosition(Vec2(bg->getContentSize().width - m_btnQuit->getContentSize().width / 2 - 20, bg->getContentSize().height - m_btnQuit->getContentSize().height / 2 - 20));
	m_btnQuit->addTouchEventListener(this,SEL_TouchEvent(&IntroduceLayer::buttonQuit));
	bg->addChild(m_btnQuit, 100);

	
	FishKindScrollView* list = FishKindScrollView::create();
	list->setPosition(Vec2(bg->getContentSize().width / 2 - list->getContentSize().width / 2, bg->getContentSize().height / 2 - list->getContentSize().height / 2 - 10));
	bg->addChild(list);

	return true;
}

void IntroduceLayer::buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	this->removeFromParentAndCleanup(true);
}

IntroduceItem::IntroduceItem()
{
	m_rankingbg = nullptr;
	m_txtSub1 = nullptr;
	m_txtSub2 = nullptr;
	m_txtSub3 = nullptr;
	m_fishbg1 = nullptr;
	m_fishbg2 = nullptr;
	m_fishbg3 = nullptr;
}

IntroduceItem::~IntroduceItem()
{

}

IntroduceItem* IntroduceItem::create(int nRow)
{
	IntroduceItem* layout = new IntroduceItem();
	if (layout && layout->init(nRow))
	{
		layout->autorelease();
		return layout;
	}
	CC_SAFE_DELETE(layout);
	return nullptr;
}

bool IntroduceItem::init(int nRow)
{
	if (!Layout::init())
		return false;
	this->setAnchorPoint(Vec2::ZERO);

	m_rankingbg = ui::ImageView::create("709/Introduce/BackGround_Small.png");
	m_rankingbg->setPosition(Vec2(310,100));
	this->addChild(m_rankingbg);

	m_fishbg1 = ui::ImageView::create(CCString::createWithFormat("709/Introduce/fish%d.png", 3 * nRow  + 1)->getCString());
	m_fishbg1->setPosition(Vec2(95,100));
	this->addChild(m_fishbg1);

	m_fishbg2 = ui::ImageView::create(CCString::createWithFormat("709/Introduce/fish%d.png", 3 * nRow  + 2)->getCString());
	m_fishbg2->setPosition(Vec2(310,100));
	this->addChild(m_fishbg2);

	m_fishbg3 = ui::ImageView::create(CCString::createWithFormat("709/Introduce/fish%d.png", 3 * nRow  + 3)->getCString());
	m_fishbg3->setPosition(Vec2(525,100));
	this->addChild(m_fishbg3);

	if (nRow == 0)
	{
		m_txtSub1= ui::Text::create("X 2","Arial",25);
		m_txtSub2= ui::Text::create("X 2","Arial",25);
		m_txtSub3= ui::Text::create("X 3","Arial",25);
	}
	else if (nRow == 1)
	{
		m_txtSub1= ui::Text::create("X 4","Arial",25);
		m_txtSub2= ui::Text::create("X 5","Arial",25);
		m_txtSub3= ui::Text::create("X 6","Arial",25);
	}
	else if (nRow == 2)
	{
		m_txtSub1= ui::Text::create("X 7","Arial",25);
		m_txtSub2= ui::Text::create("X 8","Arial",25);
		m_txtSub3= ui::Text::create("X 9","Arial",25);
	}
	else if (nRow == 3)
	{
		m_txtSub1= ui::Text::create("X 10","Arial",25);
		m_txtSub2= ui::Text::create("X 12","Arial",25);
		m_txtSub3= ui::Text::create("X 15","Arial",25);
	}
	else if (nRow == 4)
	{
		m_txtSub1= ui::Text::create("X 18","Arial",25);
		m_txtSub2= ui::Text::create("X 20","Arial",25);
		m_txtSub3= ui::Text::create("X 25","Arial",25);
	}
	else if (nRow == 5)
	{
		m_txtSub1= ui::Text::create("X 30","Arial",25);
		m_txtSub2= ui::Text::create("X 35","Arial",25);
		m_txtSub3= ui::Text::create("40-120","Arial",25);
	}
	else if (nRow == 6)
	{
		m_txtSub1= ui::Text::create("120-300","Arial",25);
		m_txtSub2= ui::Text::create("X 320","Arial",25);
		m_txtSub3= ui::Text::create("40-300","Arial",25);
	}
	else if (nRow == 7)
	{
		m_txtSub1= ui::Text::create("","Arial",25);
		m_txtSub2= ui::Text::create("","Arial",25);
		m_txtSub3= ui::Text::create("","Arial",25);

		m_txtSub1->setString("");
		m_txtSub2->setString("");
		m_txtSub3->setString("");

	}

	m_txtSub1->setPosition(Vec2(95,30));
	this->addChild(m_txtSub1);

	m_txtSub2->setPosition(Vec2(310,30));
	this->addChild(m_txtSub2);

	m_txtSub3->setPosition(Vec2(525,30));
	this->addChild(m_txtSub3);

	return true;
}

