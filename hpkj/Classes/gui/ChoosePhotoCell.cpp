#include "ChoosePhotoCell.h"
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "ChoosePhotoScrollLayer.h"
#include "ChoosePlayerPhotoLayer.h"
#include "PromptBox.h"
#include "PlayerInfoLayer.h"
#include "GroupSprite.h"
#include "MovingLabelLayer.h"
ChoosePhotoCell::ChoosePhotoCell(void){

}
ChoosePhotoCell::~ChoosePhotoCell(void){

}
ChoosePhotoCell* ChoosePhotoCell::PhotoCellWith(int type){
	ChoosePhotoCell* photoCell = new ChoosePhotoCell();
	if(photoCell)
	{
		photoCell->initPhotoCellWith(type);
		photoCell->autorelease();
		return photoCell;
	}
	CC_SAFE_DELETE(photoCell);
	return NULL;
}
bool ChoosePhotoCell::initPhotoCellWith(int type){
	    m_nType = type;
		winSize = Director::getInstance()->getWinSize();

		Sprite *pNormalPlayerBG = Sprite::createWithSpriteFrame(spriteFrame("dt_player_bj.png"));
		/*Sprite *pSelectPlayerBG = Sprite::createWithSpriteFrame(spriteFrame("dt_player_bj.png"));
		MenuItemSprite *pPlayerButtom = MenuItemSprite::create(pNormalPlayerBG,pSelectPlayerBG,CC_CALLBACK_1(ChoosePhotoCell::choosePhoto));
		Menu *pMenu = Menu::create(pPlayerButtom,NULL);*/
		pNormalPlayerBG->setPosition(Vec2(82.0f,77.0f));
		this->addChild(pNormalPlayerBG);

		const char *pStr;
		if(m_nType < 100)
		{
			if(m_nType >= 1)
			{
				pStr = String::createWithFormat("dt_player_1.png")->getCString();
			}
			else
			{
				/**Í¼ÏñÎªÅ®£¬±àÂëÎª0**/
				pStr = String::createWithFormat("dt_player_%d.png",m_nType)->getCString();
			}
		}
		else
		{
			if(m_nType >= 101)
			{
				pStr = String::createWithFormat("dt_player_101.png")->getCString();
			}
			else
			{
				/**Í¼ÏñÎªÄÐ£¬±àÂëÎª100**/
				pStr = String::createWithFormat("dt_player_%d.png",m_nType)->getCString();
			}
		}
		pPhotoBJ1 = Sprite::createWithSpriteFrame(spriteFrame(pStr));
		
		pPhotoBJ1->setPosition(Vec2(82.0f,80.0f));
		this->addChild(pPhotoBJ1,2);	
        
	    Sprite *pChooseKuang0 = Sprite::createWithSpriteFrame(spriteFrame("dt_playinfo_4.png"));
		Sprite *pChooseKuang1 = Sprite::createWithSpriteFrame(spriteFrame("dt_playinfo_4.png"));
		MenuItemSprite *pChooseKuang=MenuItemSprite::create(pChooseKuang0,pChooseKuang1,CC_CALLBACK_1(ChoosePhotoCell::choosePhoto,this));
		pMenu = Menu::create(pChooseKuang,NULL);
		pMenu->setPosition(Vec2(82.0f+48.0f,25.0f));
		this->addChild(pMenu,2);

		pChoose = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_0.png"));
		pChoose->setPosition(Vec2(78.0f+48.0f,27.0f));
		pChoose->setVisible(false);
		this->addChild(pChoose,3);

		/*layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t29"),Vec2(82.0f,80.0f));
		this->addChild(layer,10);*/

         if(type==0||type==100){
			pPhotoBJ1->setScale(0.5f);
			pMenu->setVisible(true);
		 }else{
          pMenu->setVisible(false);
		 }
	    return true;
}
void ChoosePhotoCell::setCellIdx(int idx){
	m_choose = idx;
	const char *pStr;
	if(m_choose < 100)
	{
		if(m_choose >= 1)
		{
			pStr = String::createWithFormat("dt_player_1.png")->getCString();
		}
		else
		{
			pStr = String::createWithFormat("dt_player_%d.png",m_choose)->getCString();
		}
	}
	else
	{
		if(m_choose >= 101)
		{
			pStr = String::createWithFormat("dt_player_101.png")->getCString();
		}
		else
		{
			pStr = String::createWithFormat("dt_player_%d.png",m_choose)->getCString();
		}
	}
	
	
	pPhotoBJ1->setSpriteFrame(spriteFrame(pStr));
	if(m_choose==0||m_choose==100){
		pPhotoBJ1->setScale(0.5f);
	}
	if(m_choose==0||m_choose==100){
		pPhotoBJ1->setScale(0.5f);	
		pMenu->setVisible(true);
	}else{
		pMenu->setVisible(false);
	}
	

}
void ChoosePhotoCell:: choosePhoto(Object* obj){
	playButtonSound();
	CCLOG("m_choose==%d",m_choose);
	pChoose->setVisible(true);
	
	
	CCActionInterval * delaytime = DelayTime::create(0.2f);
	CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(ChoosePhotoCell::callbackC));
	FiniteTimeAction *pAction = Sequence::create(delaytime,funcall,NULL);
	this->runAction(pAction);


	

}
void ChoosePhotoCell::callbackC(){
	if(pMsgDispatch->connectLoginServer())
	{
		int nhead;
		int nGente;
		if(m_choose >= 100)
		{
			nhead = m_choose - 100;
			nGente = 1;
		}
		else
		{
			nhead = m_choose;
			nGente = 2;
		}
		pMsgDispatch->modifyTouxiang(nhead,nGente);
		Integer* pNickNameResult = Integer::create(nGente*10+nhead);
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATETOUXIANGNUM,pNickNameResult);

	}
	else
	{
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATASEVERSHUT,NULL);
	}
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATETOUXIANGLAYER,NULL);

	//pLayer1->setVisible(false);
	
}
