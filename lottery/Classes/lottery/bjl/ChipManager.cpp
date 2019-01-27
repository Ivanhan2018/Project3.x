/***************************************
 筹码管理类
 ***************************************/
#include "ChipManager.h"
#include "CocosUnits.h"
#include "BJLRes.h"
#include "OperalayerBJL.h"//<by hxh>
#include "MyConfig.h"//by hxh
using namespace cocos2d;

// by hxh 20160707
#if 0
Rect chipPosRect[8] = {
	Rect(0,180,212,116),
	Rect(225,180,350,80),
	Rect(586,180,212,116),
	Rect(0,120,200,60),
	Rect(590,120,200,60),
	Rect(217,108,367,68),
	Rect(0,298,224,70),
	Rect(576,298,224,70)
};
#else
extern Rect chipPosRect1[8];
#endif

// 83*72改为103*102
#if 1
float chipWidth=50;
float chipHeight=50;
#else
float chipWidth = 40;
float chipHeight = 35;
#endif

int addRandSeedTimeSprite()
{
	timeval psv;
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec/1000;
	srand(rand_seed);
	int iRet=(psv.tv_sec/3)%4;
	return iRet;
}

Vec2 randomPosInRect(int index)
{
	if(index<0||index>7)
	{
		return Vec2(0,0);
	}
	int iRet=addRandSeedTimeSprite();
	//Rect rect = chipPosRect1[index];//<by hxh>
	MyConfig& myConfig=MyConfig::Instance();
	Rect rect = Rect(myConfig.m_ChipPosRect[index][0],myConfig.m_ChipPosRect[index][1],myConfig.m_ChipPosRect[index][2],myConfig.m_ChipPosRect[index][3]);
	Vec2 pos;
#if 1
	int rw=0;
	int rh=0;
	int w=(int)(rect.size.width/2-chipWidth);
	int h=(int)(rect.size.height/2-chipHeight);
	
	rw=rand()%(w==0?1:w);
	rh=rand()%(h==0?1:h);

	CCLOG("aaaaaaaaa  %d, %d", rw, rh);

	switch( iRet ){
	case 0:  break;
	case 1:  rw=-rw;;break;
	case 2:  rw=-rw;rh=-rh;break;
	case 3:  rh=-rh;break;
	default: break;
	}
	pos.x = rect.origin.x+rect.size.width/2+rw;
	pos.y = rect.origin.y+rect.size.height/2+rh;
#else
	pos.x = rect.origin.x+rand()%((int)(rect.size.width-chipWidth));
	pos.y = rect.origin.y+rand()%((int)(rect.size.height-chipHeight));
#endif
	return pos;
}

Vec2 randomPosInRect(int index, int randX, int randY)
{
	if(index<0||index>7)
	{
		return Vec2(0,0);
	}
	//int iRet=addRandSeedTimeSprite();
	timeval psv;
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec/1000;
	//srand(rand_seed);
	int iRet=(psv.tv_sec/3)%4;
	//Rect rect = chipPosRect1[index];//<by hxh>
	MyConfig& myConfig=MyConfig::Instance();
	Rect rect = Rect(myConfig.m_ChipPosRect[index][0],myConfig.m_ChipPosRect[index][1],myConfig.m_ChipPosRect[index][2],myConfig.m_ChipPosRect[index][3]);
	Vec2 pos;
#if 1
	int rw=0;
	int rh=0;
	int w=(int)(rect.size.width/2-chipWidth);
	int h=(int)(rect.size.height/2-chipHeight);
	rw=randX%(w==0?1:w);
	rh=randY%(h==0?1:h);

	switch( iRet ){
	case 0:  break;
	case 1:  rw=-rw;;break;
	case 2:  rw=-rw;rh=-rh;break;
	case 3:  rh=-rh;break;
	default: break;
	}
	pos.x = rect.origin.x+rect.size.width/2+rw;
	pos.y = rect.origin.y+rect.size.height/2+rh;
#else
	pos.x = rect.origin.x+rand()%((int)(rect.size.width-chipWidth));
	pos.y = rect.origin.y+rand()%((int)(rect.size.height-chipHeight));
#endif
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
			m_pTotalChipValue[i] = LabelAtlas::create("0",texture_name::s_ChipMagerNumber,13,17,'0');//<by hxh>
			m_pTotalChipValue[i]->setAnchorPoint(Vec2::ZERO);
			m_pSpriteNunber[i] = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_SpriteNunber));
			m_pSpriteNunber[i]->setAnchorPoint(Vec2::ZERO);
			m_pTotalChipValue[i]->setPosition(ccp(myConfig.m_ChipManagerNunberPos[i][0]+40,myConfig.m_ChipManagerNunberPos[i][1]));
			m_pSpriteNunber[i]->setPosition(ccp(myConfig.m_ChipManagerNunberPos[i][0],myConfig.m_ChipManagerNunberPos[i][1]));
			//CCLOG("Pos%d=\"%f,%f\" ",i,m_pSpriteNunber[i]->getPositionX(),m_pSpriteNunber[i]->getPositionY());
			// 总注是指该下注区域内所有玩家的下注额总和<by hxh>
#if 1
			m_pTotalChipValue[i]->setVisible(false);
			m_pSpriteNunber[i]->setVisible(false);
#endif
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
	//m_lBetScore =(lBetScore/100);
	m_lBetScore =lBetScore;//by hxh 20160629
	int CiNumber=0;//次数
	// by hxh 20160707
#if 0
	sprintf(Stringing, "BJL_Cannger_%d.png",m_lBetScore);
	Sprite *m_pSpriteChipValue = Sprite::createWithSpriteFrame(spriteFrame(Stringing));
#else
	int index=CHIP_SCORE_ENUM(m_lBetScore);
	MyEnumItem x=(MyEnumItem)(index>-1?index:m_pCKPX_100EnumItem);
	Sprite *m_pSpriteChipValue = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(x)));
#endif	
	m_pSpriteChipValue->setAnchorPoint(Vec2::ZERO);
	m_pSpriteChipValue->setScale(0.5);
	//case 0:      //闲
	//case 1:    //平
	//case 2:    //庄
	//case 3:    //闲天王
	//case 4:    //庄天王 
	//case 5:    //同上平
	//case 6:    //闲对子
	//case 7:    //庄对子
	m_pSpriteChipValue->setPosition(randomPosInRect(cbBetArea));
	this->addChild(m_pSpriteChipValue);
	ChipManagerArray->addObject(m_pSpriteChipValue);

}

void ChipManager::showChipManagerSprite( BYTE cbBetArea, LONGLONG lBetScore, int randX, int randY)
{
	cocos2d::Size	 WinthSize = Director::getInstance()->getWinSize();
	char Stringing[150];
	LONGLONG m_lBetScore = 0;
	//m_lBetScore =(lBetScore/100);
	m_lBetScore =lBetScore;
	int CiNumber=0;//次数
#if 0
	sprintf(Stringing, "BJL_Cannger_%d.png",m_lBetScore);
	Sprite *m_pSpriteChipValue = Sprite::createWithSpriteFrame(spriteFrame(Stringing));
#else
	int index=CHIP_SCORE_ENUM(m_lBetScore);
	MyEnumItem x=(MyEnumItem)(index>-1?index:m_pCKPX_100EnumItem);
	Sprite *m_pSpriteChipValue = Sprite::createWithSpriteFrame(spriteFrame(BJL_Cannger_x_png(x)));
#endif	
	m_pSpriteChipValue->setAnchorPoint(Vec2::ZERO);
	m_pSpriteChipValue->setScale(0.5);
	//case 0:      //闲
	//case 1:    //平
	//case 2:    //庄
	//case 3:    //闲天王
	//case 4:    //庄天王 
	//case 5:    //同上平
	//case 6:    //闲对子
	//case 7:    //庄对子
	m_pSpriteChipValue->setPosition(randomPosInRect(cbBetArea,randX,randY));
	this->addChild(m_pSpriteChipValue);
	ChipManagerArray->addObject(m_pSpriteChipValue);

}

void ChipManager::showChipManangerSpriteForAll(BYTE cbBetArea, LONGLONG lBetScore)
{
	/*恢复对局时，显示下注界面筹码*/
	LONGLONG m_lBetScore = lBetScore;

	const LONGLONG s_llArr[7]={CHIP_SCORE_1,CHIP_SCORE_2,CHIP_SCORE_3,CHIP_SCORE_4,CHIP_SCORE_5,CHIP_SCORE_6,CHIP_SCORE_7};

	srand((unsigned)time(NULL));


	for (int i = 6; i >= 0; i--)
	{
		if (m_lBetScore == 0)
		{
			break;
		}
		
		int tempCount = m_lBetScore / s_llArr[i];

		if (tempCount <=0)
		{
			continue;
		}

		for (int j = 0; j < tempCount; j ++)
		{
			int rX = rand();
			int rY = rand();
			this->showChipManagerSprite(cbBetArea, s_llArr[i], rX, rY);
		}
		
		m_lBetScore = m_lBetScore % s_llArr[i];
		
	}
	
}

