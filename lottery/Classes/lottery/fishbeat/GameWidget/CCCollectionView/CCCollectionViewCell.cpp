//
//  CCCollectionViewCell.cpp
//  CCCollectionView
//
//  Created by lv on 14-3-15.
//
//

#include "CCCollectionViewCell.h"

CollectionViewCell::CollectionViewCell()
{
	_pItem = nullptr;
}

CollectionViewCell::~CollectionViewCell()
{
}
 bool CollectionViewCell::init()
 {
	 if (!Node::init())
	 {
		 return  false;
	 }
	 _pItem = nullptr;
	 return true;
 }

void CollectionViewCell::reset()
{
    _idx = -1;
}

ssize_t CollectionViewCell::getIdx() const
{
    return _idx;
}

void CollectionViewCell::setIdx(ssize_t idx)
{
    _idx = idx;
}

void CollectionViewCell::getCellRect(Rect &rect)
{
	if (getChildrenCount()>0)
	{
		cocos2d::Vector<Node*>::iterator _item=getChildren().begin();
		const cocos2d::Size _size=(*_item)->getContentSize();
		rect.setRect(0,0,_size.width,_size.height);
	}
	else
	{
		rect.setRect(0,0,0,0);
	}
}

void CollectionViewCell::addChildrenMenuItem(MenuItem *pItem, int zOrder)
{
	const cocos2d::Size _size = pItem->getContentSize();
	pItem->setAnchorPoint(Vec2(0.5f,0.5f));
	pItem->setPosition(Vec2(_size.width*0.5f, _size.height*0.5f));
	this->removeAllChildren();
	_pItem = pItem;
	this->addChild(pItem, zOrder);
}

MenuItem *CollectionViewCell::getChildrenMenuItm()
{
	return _pItem;
}

void CollectionViewCell::removeAllCellChildren()
{
	removeAllChildren();
	_pItem = nullptr;
}


void CollectionViewCell::onCellTouched()
{
	if (_pItem && _pItem->isEnabled())
	{
		_pItem->activate();	
	}
}

void CollectionViewCell::onCellHighlight()
{
	if (_pItem && _pItem->isEnabled())
	{
		_pItem->selected();
	}
}

void CollectionViewCell::onCellUnhighlight()
{
	if (_pItem)
	{
		_pItem->unselected();
	}
}