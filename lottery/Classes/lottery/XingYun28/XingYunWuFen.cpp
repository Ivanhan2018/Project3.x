#include "HelpLayer.h"
#include "RoomLayer.h"
#include "XingYunWuFen.h"
#include "BetLimit.h"
#include "MovingLabelLayer.h"
#include "comm.h"
#include "VersionControl.h"

using namespace cocos2d;

XingYunWuFen::XingYunWuFen()
{

}

XingYunWuFen::~XingYunWuFen()
{

}

Scene* XingYunWuFen::scene()
{
	Scene * scene = NULL;
	do 
	{
		scene = Scene::create();
		CC_BREAK_IF(! scene);
		XingYunWuFen *layer = XingYunWuFen::create();
		CC_BREAK_IF(! layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool XingYunWuFen::init()
{
	bool bRet = false;
	do 
	{
		if (!Layer::init())
		{
			return false;
		}

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(XingYunWuFen::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(XingYunWuFen::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(XingYunWuFen::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(XingYunWuFen::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		m_node = Node::create();
		m_node->setPosition(Vec2(0,0));
		this->addChild(m_node,2);

		fontSize = 27;
		pt = Vec2(240, SCREEN_HEIGHT - 162 - 70);	

		memset(ballStatus,0,sizeof(ballStatus));

		//界面
		initBetArea();

		bRet = true;
	} while (0);

	return bRet;
}

void XingYunWuFen::initBetArea()
{
	pt.y = pt.y-areaGapY-areaBkHeight / 2;	

	this->initHeBalls(Vec2(ballStartX, pt.y), W_AREA_BALL_TAG);	

	pt.y -= 320 + 5 + 80;//4 * 80

	if(pt.y>0)
	{
		m_endPos = Vec2(pt.x,0);
	}else{
		m_endPos=Vec2(pt.x,-pt.y);
	}	
}

void XingYunWuFen::initHeBalls(Vec2 startPos, int startTag)
{
	SpriteFrame *ballSpriteFrame = spriteFrame(HE_IMG);
	SpriteFrame *ballSpriteFrame2 = spriteFrame("gamble_02.png");	

	Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
	Texture2D *ballTexture2D2 = ballSpriteFrame2->getTexture();
	Vec2 pos = startPos;
	static char* ballValue[] = {"959.18","324.75","164.34","97.41","65.73","46.5","35.42","27.12","21.91","17.7","15.64","14.06","13.49","13.01",
								"13.14","13.29","14.27","15.44","17.99","21.64","27.55","34.88","47.07","64.94","98.61","162.38","328.55","974.81"};
	for (int i = 0; i < 28;){
		//球
		Sprite *ballSprite = NULL;
		if(i > 100) //< 12
		{
			ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballTexture2D2, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		} else
		{
			ballSprite = Sprite::createWithSpriteFrame(
				SpriteFrame::createWithTexture(ballTexture2D, Rect(i * ballRadius,0,ballRadius,ballRadius)));
		}

		ballSprite->setPosition(Vec2(pos.x + (ballRadius + ballGapX)*(i % 7), pos.y));
		ballSprite->setTag(startTag + i);
		m_node->addChild(ballSprite);

		Sprite* ballBack = Sprite::createWithSpriteFrame(spriteFrame("dt_tz_bs_01.png"));
		ballBack->setPosition(Vec2(ballSprite->getPositionX(), ballSprite->getPositionY() - 55));
		m_node->addChild(ballBack);
		//文字
		LabelTTF *ltfBall = LabelTTF::create(ballValue[i],"",18);
		ltfBall->setPosition(Vec2(ballSprite->getPositionX(), ballSprite->getPositionY() - 55));		
		ltfBall->setFontFillColor(ccColor3B::BLACK);
		ltfBall->setDimensions(cocos2d::Size(70,18));
		m_node->addChild(ltfBall);
		i++;
		if (i % 7 == 0){
			pos.x = startPos.x;
			pos.y -= (ballRadius + areaGapY) + 20;
		}
	}

	//按钮
	for(int j = 0;j < 10;j++)
	{
		Sprite* sptBall = Sprite::createWithSpriteFrame(spriteFrame(String::createWithFormat("bt_tz_bs_jian_normal%d.png",51 + j)->getCString()));
		Vec2 tempV = Vec2(SCREEN_WIDTH / 2 - sptBall->getContentSize().width / 2 + (j % 2) * sptBall->getContentSize().width, pos.y);
		if(j == 2) tempV.x -= sptBall->getContentSize().width;
		if(j == 5) tempV.x += sptBall->getContentSize().width;
		sptBall->setPosition(tempV);
		sptBall->setTag(51 + j);
		m_node->addChild(sptBall);
		if(j < 2 || j > 4) 
		pos.y -= (sptBall->getContentSize().height) * (j % 2);
	}
}

void XingYunWuFen::onEnter()
{
	Layer::onEnter();	
}

void XingYunWuFen::onExit()
{
	//
	Layer::onExit();
}

bool XingYunWuFen::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (pTouch->getLocation().y < 280 || pTouch->getLocation().y > 1080)  //1280-162-30
	{		
		return false;
	}
		
	//清空数组
	m_vecPot.clear();
	m_bDrag = false;

	return true;
}
void XingYunWuFen::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = pTouch->getLocation();	
	//重写加速滑动
	m_bDrag = true; 

	m_end = m_node->getPosition().y + pTouch->getDelta().y;

	if(m_end<0){
		m_node->setPositionY(0);
		m_end = 0;
	} else
		if (m_end>m_endPos.y)
		{
			m_node->setPositionY(m_endPos.y);
			m_end = m_endPos.y;
		} else
		{
			m_node->setPositionY(m_end);	
			//加入点	
			m_vecPot.push_back(pTouch->getLocation()); 			
		}
}

//手指触摸离开
void XingYunWuFen::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if(m_bDrag && m_vecPot.size() > 5)
	{
		m_bDrag = false;
		int iVecSize = m_vecPot.size(); 
		if(iVecSize < 1) return;
		float fYspeed = m_vecPot[iVecSize - 1].y;

		if(iVecSize < 2) return;

		fYspeed = fYspeed - m_vecPot[iVecSize - 2].y; 	

		while (abs(fYspeed) > 0.1)  
		{  
			Vec2 potNew = m_node->getPosition() + Vec2(0, fYspeed);  
			auto moveBy = MoveBy::create(0.5f, Vec2(0, fYspeed * 20)); 

			//检测滚动的位置
			m_node->runAction(Sequence::create(moveBy,CallFunc::create([=](){
				if(m_node->getPositionY() < 0)
				{
					m_node->setPositionY(0);					
				} else
					if (m_node->getPositionY() > m_endPos.y)
					{
						m_node->setPositionY(m_endPos.y);						
					}

			}),nullptr));				

			fYspeed *= 0.2;  //设置垂直摩擦系数
		}  

		return;
	}

	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	touchPoint = m_node->convertToNodeSpace(touchPoint);
	endPoint = touchPoint;
	//开始检测球
	int ballNum = 28;		

	for(int i = 0 ; i < ballNum ; i++)
	{
		Sprite *ballSprite = (Sprite *)m_node->getChildByTag(W_AREA_BALL_TAG+i);
		if(ballSprite == NULL) continue;
		Rect rect = ballSprite->boundingBox();
		if(rect.containsPoint(endPoint)){	

			ballStatus[i] = 1-ballStatus[i];

			SpriteFrame *ballSpriteFrame;

			if(i > 100) // < 12
			{
				ballSpriteFrame = spriteFrame("gamble_02.png");
			}else
			{
				ballSpriteFrame = spriteFrame(HE_IMG);
			}

			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			if(ballStatus[i] == 0){					
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(i*ballRadius,0,ballRadius,ballRadius)));				
			}else{					
				ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,
					Rect(i*ballRadius,ballRadius,ballRadius,ballRadius)));					
			}			
		} else
		{
			ballStatus[i] = 0;

			SpriteFrame *ballSpriteFrame;

			if(i > 100) // < 12
			{
				ballSpriteFrame = spriteFrame("gamble_02.png");
			}else
			{
				ballSpriteFrame = spriteFrame(HE_IMG);
			}

			Texture2D *ballTexture2D = ballSpriteFrame->getTexture();
			ballSprite->setSpriteFrame(SpriteFrame::createWithTexture(ballTexture2D,Rect(i*ballRadius,0,ballRadius,ballRadius)));	
		}
	}
	
	//开始检测按钮
	for(int j = 0;j < 10;j++)
	{
		Sprite* sptBall = (Sprite *)m_node->getChildByTag(51 + j);
		if(sptBall)
		{
			if(sptBall->boundingBox().containsPoint(endPoint))
			{
				ballStatus[28 + j] = 1-ballStatus[28 + j];
				sptBall->setSpriteFrame(spriteFrame(String::createWithFormat("bt_tz_bs_jian_pressed%d.png",51 + j)->getCString()));
			} else
			{
				ballStatus[28 + j] = 0;
				sptBall->setSpriteFrame(spriteFrame(String::createWithFormat("bt_tz_bs_jian_normal%d.png",51 + j)->getCString()));
			}
		}		
	}
}

//手指触摸取消
void XingYunWuFen::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

String *XingYunWuFen::getBetNumber()
{
	String* result = String::create("");
	betZhuShu = 0;  //实际上是玩法id

	for(int i = 0;i < 40;i ++)
	{
		if(ballStatus[i])
		{			
			if(i < 28) 
			{
				betZhuShu = i + 1;
				result = String::createWithFormat("%02d|",tagXingYun28[i]);
			}else
			{					
				if((i > 27 && i < 30) || (i > 33 && i < 36))
				{
					betZhuShu = 29;//大小单双
				}else
				if(i > 29 && i < 34)
				{
					betZhuShu = 30;//趣味
				}else
				if(i > 35)
				{
					betZhuShu = 31;//极值
				}
				result = String::createWithFormat("%d|",tagXingYun28[i]);
			}
			break;
		}
	}

	return result;
}