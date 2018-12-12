#include "ScrollAdd.h"
#include "Define.h"
#include "CocosUnits.h"

ScrollAdd::ScrollAdd()
{

}

ScrollAdd::~ScrollAdd()
{	
	this->unschedule(schedule_selector(ScrollAdd::refresh));
}

bool ScrollAdd::init()
{
	if(!Layer::create())
	{
		return false;
	}

	initData();
	initView();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ScrollAdd::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(ScrollAdd::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(ScrollAdd::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(ScrollAdd::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ScrollAdd::initData()
{
	pData.clear(); 
	pData.push_back("beark4.png");
	pData.push_back("beark5.png");
	pData.push_back("beark6.png");

	m_start_pos = Vec2(0,0);
	m_cur_index = 0;
}

void ScrollAdd::initView()
{
	pSprite = Sprite::createWithSpriteFrame(spriteFrame("beark4.png"));
	pSprite->setPosition(Vec2(0,0));
	pSprite->setAnchorPoint(Vec2(0,0));
	this->addChild(pSprite);	

	this->schedule(schedule_selector(ScrollAdd::refresh),5.0f);
}

bool ScrollAdd::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 temp = pTouch->getLocation();
	//LOG("%f,%f", temp.x, temp.y);

	if (temp.y > SCREEN_HEIGHT - 156 || temp.y < SCREEN_HEIGHT - 156 - 286) return false;	

	m_start_pos = temp;
	return true;
}

void ScrollAdd::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 temp = pTouch->getLocation();

	if(temp.x - m_start_pos.x > 50) changeFrame(false);
	if(temp.x - m_start_pos.x < -50) changeFrame(true);
}

void ScrollAdd::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void ScrollAdd::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

void ScrollAdd::changeFrame(bool isLeft)
{
	if(isLeft) //往左滑
	{
		m_cur_index += 3;
		m_cur_index --;
		m_cur_index %= 3;
	} else
	{
		m_cur_index ++;
		m_cur_index %= 3;
	}

	std::string tempStr = pData.at(m_cur_index);

	pSprite->setSpriteFrame(spriteFrame(tempStr.c_str()));
}

void ScrollAdd::refresh(float delta)
{
	changeFrame(false);
}

void ScrollAdd::scrollViewDidScroll(ScrollView* view)
{

}

void ScrollAdd::scrollViewDidZoom(ScrollView* view)
{

}

//触摸事件 计算的是点击的是那个子页
void ScrollAdd::tableCellTouched(TableView* table, TableViewCell* cell)
{

}
//每一项的高度和宽度
cocos2d::Size ScrollAdd::cellSizeForTable(TableView *table)
{
	cocos2d::Size size = cocos2d::Size(SCREEN_WIDTH, 288);
	return size;
}
//生成列表的每一项内容
TableViewCell* ScrollAdd::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell* cell = new TableViewCell();	

	if (pData.size() < 1) return cell;
	
	std::string tempStr = pData.at(idx);

	Sprite* sptAdd = Sprite::createWithSpriteFrame(spriteFrame(tempStr.c_str()));
	sptAdd->setAnchorPoint(Vec2(0,0));
	sptAdd->setPosition(Vec2(0, 0));
	cell->addChild(sptAdd);

	return cell;
}
//一共多少项
ssize_t ScrollAdd::numberOfCellsInTableView(TableView *table)
{
	return 3;
}

void ScrollAdd::onEnter()
{
	Layer::onEnter();
}

void ScrollAdd::onExit()
{
	Layer::onExit();
}