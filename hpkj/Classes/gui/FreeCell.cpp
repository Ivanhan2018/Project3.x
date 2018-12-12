#include "FreeCell.h"
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "FreeLayer.h"
#include "FreeScrollLayer.h"
#include "MovingLabelLayer.h"
#include "RoomLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

FreeCell::FreeCell(void){
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(FreeCell::setMenuEnable), MSG_UI_ANS_CANNELDOWNLOADGAME, NULL);
}
FreeCell::~FreeCell(void){
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_RUNBREAD);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_CHANGSHAMAJIADOWNLOAD);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_RUNFASTDOWNLOAD);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_THREETOONE);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_CANNELDOWNLOADGAME);
}
FreeCell *FreeCell::FreeCellWith(int type,bool isGet){
	FreeCell* Cell = new FreeCell();
	if(Cell)
	{
		Cell->initFreeCellWith(type,isGet);
		Cell->autorelease();
		return Cell;
	}
	CC_SAFE_DELETE(Cell);
	return NULL;
}
bool FreeCell::initFreeCellWith(int type,bool isGet){
	    m_nType = type;

		m_nIsget = isGet;

		if(m_nType == 0)
		{
			NotificationCenter::getInstance()->addObserver(
				this, callfuncO_selector(FreeCell::setPesent), MSG_UI_ANS_CHANGSHAMAJIADOWNLOAD, NULL);
		}
		if(m_nType == 1)
		{
			NotificationCenter::getInstance()->addObserver(
				this, callfuncO_selector(FreeCell::setPesent), MSG_UI_ANS_RUNFASTDOWNLOAD, NULL);
		}
		if(m_nType == 2)
		{
			NotificationCenter::getInstance()->addObserver(
				this, callfuncO_selector(FreeCell::setPesent), MSG_UI_ANS_RUNBREAD, NULL);
		}
		if(m_nType == 3)
		{
			NotificationCenter::getInstance()->addObserver(
				this, callfuncO_selector(FreeCell::setPesent), MSG_UI_ANS_THREETOONE, NULL);
		}

		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		Sprite *pBGLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_free_4.png"));
		pBGLeft->setPosition(Vec2(211.5f,45.0f));
		this->addChild(pBGLeft,0);
		Sprite *pBGRight = Sprite::createWithSpriteFrame(spriteFrame("dt_free_4.png"));
		pBGRight->setRotationY(180.0f);
		pBGRight->setPosition(Vec2(596.5f - 1.0f,45.0f));
		this->addChild(pBGRight,0);
		const char *pStr = String::createWithFormat("dt_free_1%d.png",m_nType)->getCString();
		pSprite = Sprite::createWithSpriteFrame(spriteFrame(pStr));
		pSprite->setPosition(Vec2(64.0f,45.0f));
		this->addChild(pSprite,1);


		/**添加进度条**/
		pJinDuTiao0 = Sprite::createWithSpriteFrame(spriteFrame("dt_free_14.png"));
		pJinDuTiao0->setAnchorPoint(Vec2(0,0.5f));
		pJinDuTiao0->setPosition(Vec2(148.0f,48.0f));
		this->addChild(pJinDuTiao0,1);

		pJinDuTiao1 = Sprite::createWithSpriteFrame(spriteFrame("dt_free_15.png"));
		pJinDuTiao = ProgressTimer::create(pJinDuTiao1);
		///**先设置进度条从左到右走**/
		pJinDuTiao->setMidpoint(Vec2(0.0f,0.0f));
		pJinDuTiao->setBarChangeRate(Vec2(1,0));
		pJinDuTiao->setType(ProgressTimer::Type::BAR);
		pJinDuTiao->setPercentage(0.0f);
		pJinDuTiao->setAnchorPoint(Vec2(0,0.5f));
		pJinDuTiao->setPosition(Vec2(148.0f,48.0f)); 
		this->addChild(pJinDuTiao,2);
		
	
		m_titile = LabelTTF::create("","",22);
		m_titile->setAnchorPoint(Vec2(0,0.5));
		m_titile->setPosition(Vec2(148.0f,72.0f));
		m_titile->setColor(ccc3(0,0,0));
		this->addChild(m_titile,1);

		m_text = LabelTTF::create("","",22);
		m_text->setAnchorPoint(Vec2(0,0.5));
		m_text->setPosition(Vec2(148.0f,23.0f));
		m_text->setColor(ccc3(0,0,0));
		this->addChild(m_text,1);
				
        
			
		if(!isGet)
		{
			pTextWaring = LabelTTF::create(pConfigMgr->text("t170"),"",22);
		}
		else
		{
			pTextWaring = LabelTTF::create(pConfigMgr->text("t459"),"",22);
		}
		pTextWaring->setPosition(Vec2(710.0f,72.0f));
		pTextWaring->setColor(ccc3(0,0,0));
		this->addChild(pTextWaring,1);
		Sprite *pNormalDownload;
		Sprite *pSelectDownload;
		Sprite* pDisDownload;

		bool bExit = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if(m_nType == 0)
		{
			bExit = isGameExit(3);
		}
		else if(m_nType == 1)
		{
			bExit = isGameExit(6);
		}
		else if(m_nType == 2)
		{
			bExit = isGameExit(2);
		}
		else if(m_nType == 3)
		{
			bExit = isGameExit(5);
		}
		CCLOG("ALREADY GET GAMEINFO");
#endif

		if(bExit)
		{
			if(isGet)
			{
				pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
				pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
				pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
				pJinDuTiao->setVisible(false);
				pJinDuTiao0->setVisible(false);
			}
			else
			{
				pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
				pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
				pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
				pJinDuTiao->setVisible(false);
				pJinDuTiao0->setVisible(false);
			}
		}
		else
		{
			pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
			pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
			pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
		}
		
		m_pMenuButton = MenuItemSprite::create(pNormalDownload,pSelectDownload,CC_CALLBACK_1(FreeCell::chooseFree,this));
	    pSelectDownload->setScale(0.9f);
		pSelectDownload->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pMenu = Menu::create(m_pMenuButton,NULL);
		pMenu->setPosition(Vec2(720.0f,30.0f));
		this->addChild(pMenu,1);

		

		//const char *str = String::createWithFormat("%s%d%s",pConfigMgr->text("t452"),0,"%")->getCString();
		//pTTF = LabelTTF::create("","",18);
		//pTTF->setColor(ccc3(0,255,0));
		//pTTF->setAnchorPoint(Vec2(0,0.5f));
		//pTTF->setPosition(Vec2(148.0f+pJinDuTiao0->getContentSize().width+20,48.0f));
		//this->addChild(pTTF,2);
		

		/**csmj**/
		 m_csmj_per = 0.0f;
		/**pdk**/
		 m_pdk_per=   0.0f;
		/**phz**/
		 m_phz_per=   0.0f;
		/**sdh**/
		 m_sdh_per=   0.0f;
		 
		 CCLOG("INIT OVER");
	return true;
}
void FreeCell::setCellIdx(int idx,bool isGet){
	m_choose = idx;
	m_nIsget = isGet;
	const char *pStr = String::createWithFormat("dt_free_1%d.png",m_choose)->getCString();
	pSprite->setSpriteFrame(spriteFrame(pStr));
	if(!m_nIsget)
	{
		pTextWaring->setString(pConfigMgr->text("t170"));
	}
	else
	{
		pTextWaring->setString(pConfigMgr->text("t459"));
	}

	bool bExit = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(m_choose == 0)
	{
		bExit = isGameExit(3);
	}
	else if(m_choose == 1)
	{
		bExit = isGameExit(6);
	}
	else if(m_choose == 2)
	{
		bExit = isGameExit(2);
	}
	else if(m_choose == 3)
	{
		bExit = isGameExit(5);
	}
#endif
	CCLOG("ALREADY GET GAMEINFO IN SET ID");
	switch(m_choose){
	case 0:
		{	
			pJinDuTiao->setPercentage(m_csmj_per);
			Sprite *pNormalDownload;
			Sprite *pSelectDownload;
			Sprite* pDisDownload;
			if(bExit)
			{
				if(!m_nIsget)
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
				else
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
			}
			else
			{
				pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
				pJinDuTiao->setSprite(pJinDuTiao1);
				pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
				pJinDuTiao->setVisible(true);
				pJinDuTiao0->setVisible(true);
			}
			m_pMenuButton->setNormalImage(pNormalDownload);
			m_pMenuButton->setSelectedImage(pSelectDownload);
			m_pMenuButton->setDisabledImage(pDisDownload);
			pSelectDownload->setScale(0.9f);
			pSelectDownload->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
			m_titile->setString(pConfigMgr->text("t171"));	
			m_text->setString(pConfigMgr->text("t175"));	
 			//pTTF->setString(String::createWithFormat("%s%d%s",pConfigMgr->text("t452"),m_csmj_per,"%")->getCString());	
			break;
		}
	case 1:
		{
			Sprite *pNormalDownload;
			Sprite *pSelectDownload;
			Sprite* pDisDownload;
			if(bExit)
			{
				if(!m_nIsget)
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
				else
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
			}
			else
			{
				pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
				pJinDuTiao->setSprite(pJinDuTiao1);
				pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
				pJinDuTiao->setVisible(true);
				pJinDuTiao0->setVisible(true);
			}
			m_pMenuButton->setNormalImage(pNormalDownload);
			m_pMenuButton->setSelectedImage(pSelectDownload);
			m_pMenuButton->setDisabledImage(pDisDownload);
			pSelectDownload->setScale(0.9f);
			pSelectDownload->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		    m_titile->setString(pConfigMgr->text("t174"));	
			m_text->setString(pConfigMgr->text("t178"));	
 		    //pTTF->setString(String::createWithFormat("%s%d%s",pConfigMgr->text("t452"),m_pdk_per,"%")->getCString());
		break;
		  }
	case 2:
		{
			Sprite *pNormalDownload;
			Sprite *pSelectDownload;
			Sprite* pDisDownload;
			if(bExit)
			{
				if(!m_nIsget)
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
				else
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
			}
			else
			{
				pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
				pJinDuTiao->setSprite(pJinDuTiao1);
				pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
				pJinDuTiao->setVisible(true);
				pJinDuTiao0->setVisible(true);
			}
			m_pMenuButton->setNormalImage(pNormalDownload);
			m_pMenuButton->setSelectedImage(pSelectDownload);
			m_pMenuButton->setDisabledImage(pDisDownload);
			pSelectDownload->setScale(0.9f);
			pSelectDownload->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
			m_titile ->setString(pConfigMgr->text("t173"));	
			m_text->setString(pConfigMgr->text("t177"));	
 			//pTTF->setString(String::createWithFormat("%s%d%s",pConfigMgr->text("t452"),m_phz_per,"%")->getCString());
			break;
		}

	case 3:
		{
			Sprite *pNormalDownload;
			Sprite *pSelectDownload;
			Sprite* pDisDownload;
			if(bExit)
			{
				if(!m_nIsget)
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_lingqu.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
				else
				{
					pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_2.png"));
					pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
					pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
					pJinDuTiao->setVisible(false);
					pJinDuTiao0->setVisible(false);
				}
			}
			else
			{
				pNormalDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pSelectDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pDisDownload = Sprite::createWithSpriteFrame(spriteFrame("dt_free_0.png"));
				pJinDuTiao1->setSpriteFrame(spriteFrame("dt_free_15.png"));
					pJinDuTiao->setSprite(pJinDuTiao1);
				pJinDuTiao->setPercentage(pJinDuTiao->getPercentage());
				pJinDuTiao->setVisible(true);
				pJinDuTiao0->setVisible(true);
			}
			m_pMenuButton->setNormalImage(pNormalDownload);
			m_pMenuButton->setSelectedImage(pSelectDownload);
			m_pMenuButton->setDisabledImage(pDisDownload);
			pSelectDownload->setScale(0.9f);
			pSelectDownload->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
			m_titile ->setString(pConfigMgr->text("t172"));	
			m_text->setString(pConfigMgr->text("t176"));	
 			//pTTF->setString(String::createWithFormat("%s%d%s",pConfigMgr->text("t452"),m_sdh_per,"%")->getCString());
			break;
		}

	default:
		{
			break;
		}

	}
	CCLOG("SET OVER");

}
void FreeCell:: chooseFree(Object* obj)
{
	playButtonSound();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(EntityMgr::instance()->m_nDownLoadPe == -1)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t458"),Vec2(427,240));
		this->getParent()->addChild(layer,50);
		return;
	}
	if(m_choose == 2)
	{
		if(isGameExit(2))
		{
			if(!m_nIsget)
			{
				pMsgDispatch->connectLoginServer();
				pMsgDispatch->getDownLoadAward(SeverID_PHZ,1);
				m_pMenuButton->setEnabled(false);
			}
			else
			{
				CCLOG("have gameID = 2");
				openAndriodGame("2");
			}
		}
		else
		{
			downloadGame("2");
			CCLOG("not have gameID = 2 start download");
			m_pMenuButton->setEnabled(false);
			EntityMgr::instance()->m_nDownLoadPe = -1;
		}
		
	}
	else if(m_choose == 0)
	{
		if(isGameExit(3))
		{
			if(!m_nIsget)
			{
				pMsgDispatch->connectLoginServer();
				pMsgDispatch->getDownLoadAward(SeverID_CSMJ,1);
				m_pMenuButton->setEnabled(false);
			}
			else
			{
				CCLOG("have gameID = 3");
				openAndriodGame("3");
			}
		}
		else
		{
			downloadGame("3");
			CCLOG("not have gameID = 3 start download");
			m_pMenuButton->setEnabled(false);
			EntityMgr::instance()->m_nDownLoadPe = -1;
		}
	}
	else if(m_choose == 1)
	{
		if(isGameExit(6))
		{
			if(!m_nIsget)
			{
				pMsgDispatch->connectLoginServer();
				pMsgDispatch->getDownLoadAward(SeverID_PDK,1);
				m_pMenuButton->setEnabled(false);
			}
			else
			{
				CCLOG("have gameID = 6");
				openAndriodGame("6");
			}
		}
		else
		{
			downloadGame("6");
			CCLOG("not have gameID = 6start download");
			m_pMenuButton->setEnabled(false);
			EntityMgr::instance()->m_nDownLoadPe = -1;
		}
	}
	else if(m_choose == 3)
	{
		if(isGameExit(5))
		{
			if(!m_nIsget)
			{
				pMsgDispatch->connectLoginServer();
				pMsgDispatch->getDownLoadAward(SeverID_SDH,1);
				m_pMenuButton->setEnabled(false);
			}
			else
			{
				CCLOG("have gameID = 5");
				openAndriodGame("5");
			}
		}
		else
		{
			downloadGame("5");
			CCLOG("not have gameID = 5start download");
			m_pMenuButton->setEnabled(false);
			EntityMgr::instance()->m_nDownLoadPe = -1;
		}
	}
	
#endif
}
void FreeCell::setPesent(Object* obj)
{
	Integer* inter = (Integer*)obj;
	pJinDuTiao->runAction(CCProgressTo::create(1.0f,inter->getValue()));
}

void FreeCell::setMenuEnable(Object* obj)
{
	if(EntityMgr::instance()->m_nDownLoadPe == 1)
	{
		m_pMenuButton->setEnabled(true);
	}
	else
	{
		m_pMenuButton->setEnabled(false);
	}
}
