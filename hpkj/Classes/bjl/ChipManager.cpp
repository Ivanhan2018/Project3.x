/***************************************
 筹码管理类
 ***************************************/
#include "ChipManager.h"
#include "CocosUnits.h"
#include "BJLRes.h"
#include "OperalayerBJL.h"
#include "MyConfig.h"
using namespace cocos2d;

extern Rect chipPosRect1[8];

float chipWidth = 40;
float chipHeight = 35;

void addRandSeedTimeSprite()
{
	timeval psv;
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec/1000;
	srand(rand_seed);
}

Vec2 randomPosInRect(int index)
{
	if(index<0||index>7)
	{
		return Vec2(0,0);
	}
	addRandSeedTimeSprite();
	Rect rect = chipPosRect1[index];
	Vec2 pos;
	pos.x = rect.origin.x+rand()%((int)(rect.size.width-chipWidth));
	pos.y = rect.origin.y+rand()%((int)(rect.size.height-chipHeight));
	return pos;
}

ChipManager::ChipManager()
{

}

ChipManager::~ChipManager()
{
	if (ChipManagerArray != NULL)
	{
		ChipManagerArray->release();
		ChipManagerArray = NULL;
	}	
}

ChipManager *ChipManager::create(){
	ChipManager *chipManager = new ChipManager();
	if (chipManager&&chipManager->initTotalChipValue())
	{
		chipManager->autorelease();
		return chipManager;
	}	
	CC_SAFE_DELETE(chipManager);
	return NULL;
}

bool ChipManager::initTotalChipValue()
{
	bool bRet = false;
	do 
	{
		// WinSize是设计分辨率的大小
		WinSize = Director::getInstance()->getWinSize();
		MyConfig& myConfig=MyConfig::Instance();
		for (int i = 0;i<8;i++)
		{
			m_pTotalChipValue[i] = CCLabelAtlas::create("0",texture_name::s_ChipMagerNumber,13,17,'0');
			m_pTotalChipValue[i]->setAnchorPoint(Vec2::ZERO);
			m_pSpriteNunber[i] = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_SpriteNunber));
			m_pSpriteNunber[i]->setAnchorPoint(Vec2::ZERO);
			m_pTotalChipValue[i]->setPosition(ccp(myConfig.m_ChipManagerNunberPos[i][0]+40,myConfig.m_ChipManagerNunberPos[i][1]));
			m_pSpriteNunber[i]->setPosition(ccp(myConfig.m_ChipManagerNunberPos[i][0],myConfig.m_ChipManagerNunberPos[i][1]));
			//CCLOG("Pos%d=\"%f,%f\" ",i,m_pSpriteNunber[i]->getPositionX(),m_pSpriteNunber[i]->getPositionY());
			// 总注是指该下注区域内所有玩家的下注额总和
			m_pTotalChipValue[i]->setVisible(false);
			m_pSpriteNunber[i]->setVisible(false);
		    this->addChild(m_pTotalChipValue[i],1);
			this->addChild(m_pSpriteNunber[i],1);
			ChipManagerArray = Array::create();
			ChipManagerArray->retain();
		}
		bRet = true;
	} while (0);

	return bRet;
}

void ChipManager::initGetChipManager()
{
	for (int i = 0; i < 8; i++)
	{
		m_pTotalChipValue[i]->setString(String::createWithFormat("%d",0)->getCString());
	}
}
void ChipManager::showTotalChipValue()
{
	for (int i = 0;i<8;i++)
	{
		m_pTotalChipValue[i]->setVisible(true);
		m_pSpriteNunber[i]->setVisible(true);
	}
}

void ChipManager::dissRemoveChuoValue()
{
	for (int i = 0;i<8;i++)
	{
		m_pTotalChipValue[i]->setVisible(false);
		m_pSpriteNunber[i]->setVisible(false);

	}
	//判断条件删除时,最好能够逆向遍历删除,这样不会漏掉任何元素
	for (int i = 0 ; i < ChipManagerArray->count() ; i++)
	{
		Node *node = (Node *)ChipManagerArray->objectAtIndex(i);
		node->removeFromParentAndCleanup(true);
	}
	CCLOG("ChipManagerArray has %d element", ChipManagerArray->count());
	ChipManagerArray->removeAllObjects();
}
void ChipManager::showGetChipManager( BYTE cbBetArea,LONGLONG lBetScore )
{
	for (int i = 0; i < 8; i++)
	{	
		if (cbBetArea == i)
		{
			m_pTotalChipValue[i]->setVisible(true);
			m_pSpriteNunber[i]->setVisible(true);
			m_pTotalChipValue[i]->setString(String::createWithFormat("%lld",lBetScore)->getCString());
		}
	}
}

void ChipManager::showChipManagerSprite( BYTE cbBetArea, LONGLONG lBetScore )
{
	cocos2d::Size	 WinthSize = Director::getInstance()->getWinSize();
	char Stringing[150];
	LONGLONG m_lBetScore = 0;
	m_lBetScore =lBetScore;
	int CiNumber=0;//次数
	int index=CHIP_SCORE_ENUM(m_lBetScore);
	MyEnumItem x=(MyEnumItem)(index>-1?index:m_pCKPX_100EnumItem);
	Sprite *m_pSpriteChipValue = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(x)));	
	m_pSpriteChipValue->setAnchorPoint(Vec2::ZERO);
	m_pSpriteChipValue->setScale(0.5);
	m_pSpriteChipValue->setPosition(randomPosInRect(cbBetArea));
	this->addChild(m_pSpriteChipValue);
	ChipManagerArray->addObject(m_pSpriteChipValue);
}

