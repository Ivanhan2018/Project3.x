
#include "QYPageTurn3D.h"
#include "2d/CCGrid.h"
#include "2d/CCNodeGrid.h"
#include "utility.h"

NS_CC_BEGIN

QYPageTurn3D::QYPageTurn3D()
	:m_pNodeGrid(NULL)
	,m_fMoveActionTime(-1)
	,m_fActAngle(0.f)
	,m_fDestAngle(0.0f*3.14f/180.0f)
{
	m_ePoingType = Type_Button_Left;
	m_kTouchPos = Vec2(0,0);
	m_fWidth = 150;
	m_fHeight = 200;

	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(QYPageTurn3D::updata),this,0.0f,false);
}

QYPageTurn3D::~QYPageTurn3D()
{
	m_pGrid3D->release();
	cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(QYPageTurn3D::updata),this);
}
QYPageTurn3D* QYPageTurn3D::create(NodeGrid* _gridNodeTarget, const Size& gridSize)
{
    QYPageTurn3D *action = new QYPageTurn3D();
	action->init(_gridNodeTarget,gridSize);
    return action;
}
void QYPageTurn3D::init(NodeGrid* _gridNodeTarget, const Size& gridSize)
{
	m_kGridSize = gridSize;
	m_pNodeGrid = _gridNodeTarget;

	Rect kRect = _gridNodeTarget->getGridRect();
	kRect.size.width *= 2.f;
	m_pGrid3D = Grid3D::create(gridSize,kRect);
	m_pGrid3D->setNeedDepthTestForBlit(true);
	m_pGrid3D->setActive(true);
	m_pGrid3D->retain();
	_gridNodeTarget->setGrid(m_pGrid3D);

	m_fWidth = 150;
	m_fHeight = 200;

	m_ePoingType = Type_Button_Left;
	m_kTouchPos = Vec2(0,0);

	OnActionPoint();
}
void QYPageTurn3D::restCard()
{
	m_kTouchPos = Vec2(0,0);
	m_ePoingType = Type_Button_Left;
	m_fMoveActionTime = -1;
	m_kMoveEndPos = Vec2(0,0);
	m_kMoveEndDestPos = Vec2(0,0);
	m_kMoveTouchPos = Vec2(0,0);
	m_kMoveTouchDestPos = Vec2(0,0);
	OnActionPoint();
}

float QYPageTurn3D::getWidth()
{
	return m_fWidth;

}
float QYPageTurn3D::getHeight()
{
	return m_fHeight;
}
void QYPageTurn3D::setSize(float fWidth,float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}
Vec2 QYPageTurn3D::changePosToNode(Vec2 kPos)
{
	kPos -= m_kMoveEndPos;
	kPos = utility::getAnglePos(Vec2(m_fWidth/2.f,m_fHeight/2.f),kPos,-m_fActAngle);
	kPos += m_kMoveEndPos;
	return kPos;
}
void QYPageTurn3D::setTouchPos(Vec2 kPos)
{
	if (IsInMoveAction())
	{
		return;
	}
	kPos = changePosToNode(kPos);
	m_fMoveActionTime = -1;
	m_kMoveEndPos = Vec2(0,0);
	m_kMoveTouchDestPos = kPos;
	if (m_kMoveTouchDestPos.x < 1)
	{
		m_kMoveTouchDestPos.x = 1;
	}

	if (m_kMoveTouchDestPos.x > m_fWidth-1)
	{
		m_kMoveTouchDestPos.x = m_fWidth-1;
	}
	if (m_kMoveTouchDestPos.y < 1)
	{
		m_kMoveTouchDestPos.y = 1;
	}

	if (m_kMoveTouchDestPos.y > m_fHeight-1)
	{
		m_kMoveTouchDestPos.y = m_fHeight-1;
	}

	if (m_kMoveTouchDestPos.x < m_fWidth/2 && m_kMoveTouchDestPos.y < m_fHeight/2)
	{
		m_ePoingType = Type_Button_Left;
	}
	if (m_kMoveTouchDestPos.x > m_fWidth/2 && m_kMoveTouchDestPos.y > m_fHeight/2)
	{
		m_ePoingType = Type_Top_Right;
	}
	if (m_kMoveTouchDestPos.x > m_fWidth/2 && m_kMoveTouchDestPos.y < m_fHeight/2)
	{
		m_ePoingType = Type_Button_Right;
	}
	if (m_kMoveTouchDestPos.x < m_fWidth/2 && m_kMoveTouchDestPos.y > m_fHeight/2)
	{
		m_ePoingType = Type_Top_Left;
	}
	m_kTouchPos = getPosByPoint(m_ePoingType);
	//OnActionPoint();
}
void QYPageTurn3D::setMoveToPos(Vec2 kPos,float fOffect)
{
	if (IsInMoveAction())
	{
		return;
	}
	kPos = changePosToNode(kPos);
	int iTouchEfftetX = m_fWidth/2*fOffect;
	int iTouchEfftetY = m_fHeight/2*fOffect;
	m_kMoveTouchDestPos = kPos;
	if (m_kMoveTouchDestPos.x < 1)
	{
		m_kMoveTouchDestPos.x = 1;
	}
	if (m_kMoveTouchDestPos.x > m_fWidth-1)
	{
		m_kMoveTouchDestPos.x = m_fWidth-1;
	}
	if (m_kMoveTouchDestPos.y < 1)
	{
		m_kMoveTouchDestPos.y = 1;
	}
	if (m_kMoveTouchDestPos.y > m_fHeight-1)
	{
		m_kMoveTouchDestPos.y = m_fHeight-1;
	}
	if (m_kMoveTouchDestPos.x < iTouchEfftetX)
	{
		if (m_ePoingType == Type_Button_Right
			||m_ePoingType == Type_Top_Right)
		{
			OnStartAction(Type_Left);
		}
	}
	if (m_kMoveTouchDestPos.x > m_fWidth-iTouchEfftetX)
	{
		if (m_ePoingType == Type_Button_Left
			||m_ePoingType == Type_Top_Left)
		{
			OnStartAction(Type_Right);
		}
	}
	if (m_kMoveTouchDestPos.y < iTouchEfftetY)
	{
		if (m_ePoingType == Type_Top_Left
			||m_ePoingType == Type_Top_Right)
		{
			OnStartAction(Type_Button);
		}
	}
	if (m_kMoveTouchDestPos.y > m_fHeight-iTouchEfftetY)
	{
		if (m_ePoingType == Type_Button_Left
			||m_ePoingType == Type_Button_Right)
		{
			OnStartAction(Type_Top);
		}
	}

	OnActionPoint();
}
void QYPageTurn3D::setEndPos(Vec2 kPos)
{
	if (IsInMoveAction())
	{
		return;
	}
	setMoveToPos(kPos,1);
}
Vec2 QYPageTurn3D::getPosByPoint(PointType kType)
{
	if (kType == Type_Button_Left)
	{
		return Vec2(0,0);
	}
	if (kType == Type_Button_Right)
	{
		return Vec2(m_fWidth,0);
	}
	if (kType == Type_Top_Left)
	{
		return Vec2(0,m_fHeight);
	}
	if (kType == Type_Top_Right)
	{
		return Vec2(m_fWidth,m_fHeight);
	}
	return Vec2(0,0);
}
void QYPageTurn3D::setCallFun(std::function<void(cocos2d::Vec2,float,bool)> pFun)
{
	m_pCallPos = pFun;
}
void QYPageTurn3D::setEndCallFun(std::function<void(void)> pFun)
{
	m_pEndCall = pFun;
}
void QYPageTurn3D::OpenDirect()
{
	if (IsInMoveAction())
	{
		return;
	}
	if (m_ePoingType == Type_Top_Left || m_ePoingType == Type_Button_Left)
	{
		OnStartAction(Type_Right);
	}
	if (m_ePoingType == Type_Top_Right || m_ePoingType == Type_Button_Right)
	{
		OnStartAction(Type_Left);
	}
}
void QYPageTurn3D::setAngle(float fAngle,bool bDirect)
{
	float fTmepPai = 3.14159f;
	m_fDestAngle = fAngle/180.f*fTmepPai;
	if (m_fActAngle > fTmepPai*2)
	{
		m_fActAngle -= fTmepPai*2;
	}
	if (m_fActAngle > m_fDestAngle)
	{
		m_fDestAngle += fTmepPai*2.f;
	}
	if (bDirect)
	{
		m_fActAngle = m_fDestAngle;
	}
}

float cocos2d::QYPageTurn3D::getAngle()
{
	float fTmepPai = 3.14159f;
	float fAngle = m_fDestAngle/fTmepPai*180.f;
	return fAngle;
}

void QYPageTurn3D::OnStartAction(TouchType eTouchType)
{
	m_kMoveTouchPos = m_kTouchPos;
	m_kMoveEndPos = Vec2(0,0);
	m_kMoveEndDestPos = Vec2(0,0);
	if (eTouchType == Type_Button)
	{
		m_kMoveEndDestPos.y = m_fHeight;
		if (m_ePoingType == Type_Top_Left)
		{
			m_kMoveTouchDestPos = Vec2(1,-m_fHeight);
		}
		else if (m_ePoingType == Type_Top_Right)
		{
			m_kMoveTouchDestPos = Vec2(m_fWidth-1,-m_fHeight);
		}
		else
		{
			return;
		}
	}
	if (eTouchType == Type_Top)
	{
		m_kMoveEndDestPos.y = -m_fHeight;
		if (m_ePoingType == Type_Button_Left)
		{
			m_kMoveTouchDestPos = Vec2(1.0f,m_fHeight*2);
		}
		else if (m_ePoingType == Type_Button_Right)
		{
			m_kMoveTouchDestPos = Vec2(m_fWidth-1.0f,m_fHeight*2);
		}
		else
		{
			return;
		}
	}
	if (eTouchType == Type_Left)
	{
		m_kMoveEndDestPos.x = m_fWidth;
		if (m_ePoingType == Type_Top_Right)
		{
			m_kMoveTouchDestPos = Vec2(-m_fWidth,m_fHeight-1);
		}
		else if (m_ePoingType == Type_Button_Right)
		{
			m_kMoveTouchDestPos = Vec2(-m_fWidth,1);
		}
		else
		{
			return;
		}
	}
	if (eTouchType == Type_Right)
	{
		m_kMoveEndDestPos.x = -m_fWidth;
		if (m_ePoingType == Type_Top_Left)
		{
			m_kMoveTouchDestPos = Vec2(m_fWidth*2,m_fHeight-1);
		}
		else if (m_ePoingType == Type_Button_Left)
		{
			m_kMoveTouchDestPos = Vec2(m_fWidth*2,1); 
		}
		else
		{
			return;
		}
	}
	m_fMoveActionTime = 0;
	m_fDurTime = 0.5f;
}

float QYPageTurn3D::distance(Vec2 a,Vec2 b,Vec2 p)
{
	return ((p.x-a.x)*(b.y-a.y)-(b.x-a.x)*(p.y-a.y))/sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));
}
void QYPageTurn3D::OnActionPoint()
{
	Vec2 kLogicPoint = m_kTouchPos;
	if (m_ePoingType == Type_Top_Right)
	{
		kLogicPoint.x = m_fWidth - kLogicPoint.x;
		kLogicPoint.y = m_fHeight - kLogicPoint.y;
	}
	if (m_ePoingType == Type_Top_Left)
	{
		kLogicPoint.y = m_fHeight - kLogicPoint.y;
	}
	if (m_ePoingType == Type_Button_Right)
	{
		kLogicPoint.x = m_fWidth - kLogicPoint.x;
	}
	float fDestDistance = sqrtf(kLogicPoint.x*kLogicPoint.x+kLogicPoint.y*kLogicPoint.y);
	float fAngle = 0.0f;
	if (fDestDistance > 0.001f)
	{
		fAngle = asinf(utility::Abs(kLogicPoint.y)/fDestDistance);
	}
	
	
	Vec2 a = Vec2(0,0.1);
	if (sinf(fAngle) >= 0.000001f)
	{
		a.y = fDestDistance/sinf(fAngle)/2;
	}
	Vec2 b(tanf(fAngle)*a.y,0);

	bool bChangeIdex = true;
	if (m_ePoingType == Type_Top_Right)
	{
		a.x = m_fWidth;
		a.y = m_fHeight - a.y;

		b.x = m_fWidth - b.x;
		b.y = m_fHeight;

		bChangeIdex = false;
	}

	if (m_ePoingType == Type_Top_Left)
	{
		a.y = m_fHeight - a.y;

		b.y = m_fHeight;

		bChangeIdex = false;
	}
	if (m_ePoingType == Type_Button_Right)
	{
		a.x = m_fWidth;

		b.x = m_fWidth - b.x;
	}

	if (m_pCallPos)
	{
		if (m_ePoingType == Type_Top_Right || m_ePoingType == Type_Button_Left)
		{
			m_pCallPos((m_kTouchPos+getPosByPoint(m_ePoingType))/2+m_kMoveEndPos,90.0f-CC_RADIANS_TO_DEGREES(fAngle),true);
		}
		else
		{
			m_pCallPos((m_kTouchPos+getPosByPoint(m_ePoingType))/2+m_kMoveEndPos,90.0f+CC_RADIANS_TO_DEGREES(fAngle),true);
		}
	}

	cocos2d::Node* pFrontNode = m_pNodeGrid->getChildByTag(1);
	cocos2d::Node* pBackNode = m_pNodeGrid->getChildByTag(2);
	if (pFrontNode && pBackNode)
	{
		if (bChangeIdex)
		{
			pFrontNode->setPosition(m_fWidth/2-m_pNodeGrid->getPosition().x+m_fWidth,m_fHeight/2-m_pNodeGrid->getPosition().y);
			pBackNode->setPosition(m_fWidth/2-m_pNodeGrid->getPosition().x,m_fHeight/2-m_pNodeGrid->getPosition().y);
		}
		else
		{
			pFrontNode->setPosition(m_fWidth/2-m_pNodeGrid->getPosition().x,m_fHeight/2-m_pNodeGrid->getPosition().y);
			pBackNode->setPosition(m_fWidth/2-m_pNodeGrid->getPosition().x+m_fWidth,m_fHeight/2-m_pNodeGrid->getPosition().y);
		}
	}

	float fXDistance = m_fWidth/m_kGridSize.width*2.0f;
	float fYDistance = m_fHeight/m_kGridSize.height;
	float fMinDistance = sqrtf(fXDistance*fXDistance + fYDistance*fYDistance)/2;

    
    for (int i = 0; i <= m_kGridSize.width; ++i)
    {
        for (int j = 0; j <= m_kGridSize.height; ++j)
		{
			bool bNext = false;
			bool bTurn = false;
			float fPosX = i*fXDistance;
			if (fPosX > m_fWidth)
			{
				fPosX = m_fWidth - (fPosX - m_fWidth);
				bNext = true;
			}
			float fPosY = j*fYDistance;
			float fPosZ = 0.1f;
			if (bNext)
			{
				fPosZ = 0.0;
			}
			float fPosDistance =distance(a,b,Vec2(fPosX,fPosY));
			if (m_ePoingType == Type_Button_Left && utility::fAbs(fDestDistance) > 20)
			{
				if ((fPosDistance > 0))
				{
					bTurn = true;
					if (fPosDistance < fMinDistance)
					{
						fPosX += cosf(fAngle)*utility::Abs(fPosDistance);
						fPosY += sinf(fAngle)*utility::Abs(fPosDistance);
					}
					else
					{
						fPosX += cosf(fAngle)*utility::Abs(fPosDistance)*2;
						fPosY += sinf(fAngle)*utility::Abs(fPosDistance)*2;
					}
				}
				else if(fPosDistance > -fMinDistance)
				{
					fPosX -= cosf(fAngle)*utility::Abs(fPosDistance);
					fPosY -= sinf(fAngle)*utility::Abs(fPosDistance);
					if (fPosX < 0)
					{
						fPosX = 0;
					}
					if (fPosY < 0)
					{
						fPosY = 0;
					}
				}
			}
			if (m_ePoingType == Type_Top_Right && utility::fAbs(fDestDistance) > 20)
			{
				if ((fPosDistance > 0))
				{
					bTurn = true;
					if (fPosDistance < fMinDistance)
					{
						fPosX -= cosf(fAngle)*utility::Abs(fPosDistance);
						fPosY -= sinf(fAngle)*utility::Abs(fPosDistance);
					}
					else
					{
						fPosX -= cosf(fAngle)*utility::Abs(fPosDistance)*2;
						fPosY -= sinf(fAngle)*utility::Abs(fPosDistance)*2;
					}
				}
				else if(fPosDistance > -fMinDistance)
				{
					fPosX += cosf(fAngle)*utility::Abs(fPosDistance);
					fPosY += sinf(fAngle)*utility::Abs(fPosDistance);
					if (fPosX > m_fWidth)
					{
						fPosX = m_fWidth;
					}
					if (fPosY > m_fHeight)
					{
						fPosY = m_fHeight;
					}
				}
			}

			if (m_ePoingType == Type_Top_Left && utility::fAbs(fDestDistance) > 20)
			{
				if ((fPosDistance < 0))
				{
					bTurn = true;
					if (fPosDistance > -fMinDistance)
					{
						fPosX += cosf(fAngle)*utility::Abs(fPosDistance);
						fPosY -= sinf(fAngle)*utility::Abs(fPosDistance);
					}
					else
					{
						fPosX += cosf(fAngle)*utility::Abs(fPosDistance)*2;
						fPosY -= sinf(fAngle)*utility::Abs(fPosDistance)*2;
					}
				}
				else if(fPosDistance < fMinDistance)
				{
					fPosX -= cosf(fAngle)*utility::Abs(fPosDistance);
					fPosY += sinf(fAngle)*utility::Abs(fPosDistance);
					if (fPosX < 0)
					{
						fPosX = 0;
					}
					if (fPosY > m_fHeight)
					{
						fPosY = m_fHeight;
					}
				}
			}
			if (m_ePoingType == Type_Button_Right && utility::fAbs(fDestDistance) > 20)
			{
				if ((fPosDistance < 0))
				{
					bTurn = true;
					if (fPosDistance > -fMinDistance)
					{
						fPosX -= cosf(fAngle)*utility::Abs(fPosDistance);
						fPosY += sinf(fAngle)*utility::Abs(fPosDistance);
					}
					else
					{
						fPosX -= cosf(fAngle)*utility::Abs(fPosDistance)*2;
						fPosY += sinf(fAngle)*utility::Abs(fPosDistance)*2;
					}
				}
				else if(fPosDistance < fMinDistance)
				{
					fPosX += cosf(fAngle)*utility::Abs(fPosDistance);
					fPosY -= sinf(fAngle)*utility::Abs(fPosDistance);
					if (fPosX > m_fWidth)
					{
						fPosX = m_fWidth;
					}
					if (fPosY < 0)
					{
						fPosY = 0;
					}
				}
			}
			fPosX += m_kMoveEndPos.x;
			fPosY += m_kMoveEndPos.y;

			cocos2d::Vec2 kTempPos = utility::getAnglePos(cocos2d::Vec2(m_fWidth/2.0f,m_fHeight/2.0f),cocos2d::Vec2(fPosX,fPosY),m_fActAngle);

			if (bTurn)
			{
				fPosZ = 0.2f;
				if (bNext)
				{
					fPosZ += 0.1;
				}
			}
			Vec3 kPos(kTempPos.x,kTempPos.y,0);
			kPos = kPos + m_pNodeGrid->getPosition3D();
			kPos.z = fPosZ+1.5f;
			if (bChangeIdex)
			{
				m_pGrid3D->setVertex(Vec2(m_kGridSize.width-i,m_kGridSize.height-j), kPos);
			}
			else
			{
				m_pGrid3D->setVertex(Vec2(i, j), kPos);
			}
            
        }
    }
}
bool QYPageTurn3D::IsInMoveAction(bool bEnd)
{
	if (utility::fAbs(m_fActAngle - m_fDestAngle) > 0.1f)
	{
		return true;
	}
	if (bEnd && m_fMoveActionTime >= m_fDurTime)
	{
		return false;
	}
	return m_fMoveActionTime >= 0;
}
void QYPageTurn3D::updata(float fTime)
{
	bool bActive = false;
	if (!utility::MoveTo(m_kTouchPos,m_kMoveTouchDestPos,m_kTouchPos,1000.0f*fTime))
	{
		bActive = true;
	}
	if (!utility::MoveTo(m_fActAngle,m_fDestAngle,m_fActAngle,10.0f*fTime))
	{
		restCard();
		bActive = true;
	}
	if (IsInMoveAction(true))
	{
		m_fMoveActionTime += fTime;
		if (m_fMoveActionTime >= m_fDurTime)
		{
			m_fMoveActionTime = m_fDurTime;
		}
		m_kMoveEndPos = m_kMoveEndDestPos*m_fMoveActionTime/m_fDurTime;
		m_kTouchPos = m_kMoveTouchPos + (m_kMoveTouchDestPos - m_kMoveTouchPos)*m_fMoveActionTime/m_fDurTime;
		OnActionPoint();
		if (m_fMoveActionTime >= m_fDurTime)
		{
			m_pCallPos(Vec2(0,0),0,false);
			if (m_pEndCall)
			{
				m_pEndCall();
			}
		}
		bActive = false;
	}
	if (bActive)
	{
		OnActionPoint();
	}
}

NS_CC_END
