#include "FMGameItemLayer.h"
#include "Define.h"
#include <math.h>
#define PI acos(-1)

bool GameItemLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameItemLayer::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(GameItemLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameItemLayer::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	inFlag = false;
	_angle = 0.0;
	animationDuration = 0.3f;
	this->ignoreAnchorPointForPosition(false);
	_selectedItem = NULL;
	cocos2d::Size WinSize = WINSIZE; 
	this->setContentSize(WinSize/3*2);
	this->setAnchorPoint(Vec2(0.5f, 0.5f));

	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
	//setTouchEnabled(true);
	//this->setTouchEnabled(true);
	return true;

}
void GameItemLayer::addMenuItem(MenuItem *item)
{
	item->setPosition(this->getContentSize() / 2);
	this->addChild(item);
	_items.push_back(item);
	setUnitAngle(2 * PI / _items.size());
	reset();
	updatePositionWithAnimation();
	return;
}
void GameItemLayer::updatePosition()
{
	cocos2d::Size menuSize = getContentSize();
	float disY = menuSize.height / 8;
	float disX = menuSize.width / 3;
	for (int i = 0; i < _items.size(); i++)
	{
		float x = menuSize.width / 2 + disX*sin(i*_unitAngle+getAngle());
		float y = menuSize.height / 2 - disY*cos(i*_unitAngle + getAngle());
		_items.at(i)->setPosition(Vec2(x, y));
		_items.at(i)->setZOrder(-(int)y);
		//Opacity  129~255
		_items.at(i)->setOpacity(192 + 63 * cos(i*_unitAngle + getAngle()));
		_items.at(i)->setScale(0.75 + 0.25*cos(i*_unitAngle + getAngle()));
	}
	return;
}
void GameItemLayer::updatePositionWithAnimation()
{
	//��ֹͣ���п��ܴ��ڵĶ���
	for (int i = 0; i < _items.size(); i++)
		_items.at(i)->stopAllActions();
	cocos2d::Size menuSize = getContentSize();
	float disY = menuSize.height / 8;
	float disX = menuSize.width / 3;
	for (int i = 0; i < _items.size(); i++)
	{
		float x = menuSize.width / 2 + disX*sin(i*_unitAngle + getAngle());
		float y = menuSize.height / 2 - disY*cos(i*_unitAngle + getAngle());
		MoveTo *moveTo = MoveTo::create(animationDuration, Vec2(x, y));
		_items.at(i)->runAction(moveTo);
		//Opacity  129~255
		CCFadeTo *fadeTo =CCFadeTo::create(animationDuration, (192 + 63 * cos(i*_unitAngle + getAngle())));
		_items.at(i)->runAction(fadeTo);
		//���ű���  0.5~1
		ScaleTo *scaleTo = ScaleTo::create(animationDuration, 0.75 + 0.25*cos(i*_unitAngle + getAngle()));
		_items.at(i)->runAction(scaleTo);
		_items.at(i)->setZOrder(-(int)y);
	}
	scheduleOnce(schedule_selector(GameItemLayer::actionEndCallBack), animationDuration);
	return;
}
void GameItemLayer::reset()
{
	_angle = 0;
}
void GameItemLayer::setAngle(float angle)
{
	this->_angle = angle;
}
float GameItemLayer::getAngle()
{
	return _angle;
}
void GameItemLayer::setUnitAngle(float angle)
{
	_unitAngle = angle;
}
float GameItemLayer::getUnitAngle()
{
	return _unitAngle;
}

float GameItemLayer::disToAngle(float dis)
{
	float width = this->getContentSize().width / 2;
	return dis / width*getUnitAngle();
}

MenuItem * GameItemLayer::getCurrentItem()
{
	if (_items.size() == 0)
		return NULL;
	//����ʵ�ʼ�����0.1getAngle(),������ֹ���ȶ�ʧ
	int  index = (int)((2 * PI - getAngle()) / getUnitAngle()+0.1*getUnitAngle());
	index %= _items.size();
	return _items.at(index);
}
bool GameItemLayer::onTouchBegan(Touch *touch, Event *event)
{
	inFlag = false;
	//��ֹͣ���п��ܴ��ڵĶ���
	for (int i = 0; i < _items.size(); i++)
		_items.at(i)->stopAllActions();
	if (_selectedItem)
		_selectedItem->unselected();
	Vec2 position = this->convertToNodeSpace(touch->getLocation());
	cocos2d::Size size = this->getContentSize();
	Rect rect;
	if (_selectedItem){
		cocos2d::Size itemSize = _selectedItem->getContentSize();
		rect = Rect(size.width/2-itemSize.width/2, size.height/2-itemSize.height/2, itemSize.width, itemSize.height);
		if (rect.containsPoint(position)){
			inFlag = true;
		}
	}
	rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(position)){
		return true;
	}
	return false;
}
void GameItemLayer::onTouchEnded(Touch* touch, Event* event)
{
	float xDelta = touch->getLocation().x - touch->getStartLocation().x;
	rectify(xDelta>0);
	if (disToAngle(fabs(xDelta))<getUnitAngle() / 6&&_selectedItem){
		if(inFlag == true){
			_selectedItem->activate();
		}
	}
	updatePositionWithAnimation();
	return;
}
void GameItemLayer::onTouchMoved(Touch* touch, Event* event)
{
	float angle = disToAngle(touch->getDelta().x);
	setAngle(getAngle() + angle);
	updatePosition();
	return;
}

void GameItemLayer::rectify(bool forward){
	float angle = getAngle();
	while (angle<0)
		angle += PI * 2;
	while (angle>PI * 2)
		angle -= PI * 2;
	if(forward>0)
		angle = ((int)((angle + getUnitAngle() / 3*2) / getUnitAngle()))*getUnitAngle();
	else
		angle = ((int)((angle + getUnitAngle() / 3 ) / getUnitAngle()))*getUnitAngle();
	setAngle(angle);
}

void GameItemLayer::actionEndCallBack(float dx)
{
	_selectedItem = getCurrentItem();
//	if(_selectedItem)
//		_selectedItem->selected();
}

void GameItemLayer::setEnabled(bool flag)
{
	auto children = this->getChildren();
	if(children.size() == 0)
	{
		return;
	}
	for(int i = 0 ; i < children.size() ; i++)
	{
		MenuItem *item = (MenuItem *)children.at(i);
		item->setEnabled(flag);
	}
}