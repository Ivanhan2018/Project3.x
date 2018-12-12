//
//  MatchError.cpp
//  Game
//
//  Created by ÂÞÃô on 13-7-10.
//
//

#include "RuningBar.h"
#include "ConfigMgr.h"

RuningBar::RuningBar()
{
    
}

RuningBar::~RuningBar()
{
    
}

bool RuningBar::init()
{
    if(!Layer::init())
    {
        return false;
    }
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
	this->setContentSize(cocos2d::Size(winSize.width * 0.53,20));
	this->setPosition(Vec2(winSize.width * 0.24,winSize.height * 0.77 - 3));
	labelRunHole = LabelTTF::create(pConfigMgr->text("t404"),"",15);
	labelRunHole->setColor(ccc3(255,255,255));
	labelRunHole->setPosition(Vec2(winSize.width * 1.3,10));
	this->addChild(labelRunHole);
	MoveTo* moveTo = MoveTo::create(20.0f,Vec2(-winSize.width * 0.4,labelRunHole->getPositionY()));
	Sequence* actionSeq = Sequence::create(moveTo,CallFuncN::create(this,callfuncN_selector(RuningBar::reSertRunHolePosition)),NULL);
	RepeatForever* actionFor = RepeatForever::create(actionSeq);
	labelRunHole->runAction(actionFor);
    return true;
}

void RuningBar::reSertRunHolePosition(Node* node)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	labelRunHole->setPosition(Vec2(winSize.width * 1.4,10));
}

//void RuningBar::visit()
//{
//	Vec2 screenPos = this->convertToWorldSpace(Vec2::ZERO);
//
//	float scaleX = this->getScaleX();
//	float scaleY = this->getScaleY();
//
//	for (Node *p = m_pParent; p != NULL; p = p->getParent()) {
//		scaleX *= p->getScaleX();
//		scaleY *= p->getScaleY();
//	}
//
//	Rect frame =CCRectMake(screenPos.x, screenPos.y, m_obContentSize.width*scaleX, m_obContentSize.height*scaleY);
//
//	glEnable(GL_SCISSOR_TEST);
//	Director::getInstance()->getOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
//	
//	//Layer::visit();
//	glDisable(GL_SCISSOR_TEST);
//}
