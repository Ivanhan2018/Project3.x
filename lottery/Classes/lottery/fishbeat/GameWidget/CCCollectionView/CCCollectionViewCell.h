//
//  CCCollectionViewCell.h
//  CCCollectionView
//
//  Created by zysun on 14-3-16.
//
//

#ifndef __CCCollectionView__CCCollectionViewCell__
#define __CCCollectionView__CCCollectionViewCell__

#include "extensions/ExtensionMacros.h"
#include "cocos2d.h"

USING_NS_CC;

class CollectionViewCell: public Node
{
public:
    CollectionViewCell();
    virtual ~CollectionViewCell();
    
	virtual bool init();
	CREATE_FUNC(CollectionViewCell);
    
    ssize_t getIdx() const;
    void setIdx(ssize_t uIdx);
    void reset();
	void getCellRect(Rect &rect);
	void addChildrenMenuItem(MenuItem *pItem, int zOrder);
	MenuItem *getChildrenMenuItm();
	void removeAllCellChildren();
	
	void onCellTouched();
	void onCellHighlight();
	void onCellUnhighlight();
private:
    ssize_t _idx;
	MenuItem *_pItem;
};

#endif /* defined(__CCCollectionView__CCCollectionViewCell__) */
