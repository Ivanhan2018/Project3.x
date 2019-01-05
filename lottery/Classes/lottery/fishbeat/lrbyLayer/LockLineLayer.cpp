#include "LockLineLayer.h"
#include "math_aide.h"

USING_NS_CC;

LockLineLayer::LockLineLayer(void):
	m_vectorLine(NULL),
	line_space(65)
{
	m_vectorLine.clear();
}

LockLineLayer::~LockLineLayer(void)
{

}

bool LockLineLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

//创建对应个数的点
void LockLineLayer::initLineView(int num)
{
	int hasNum = m_vectorLine.size();
	int neadNum = num - hasNum;
	if (neadNum > 0)
	{
		for (int i = hasNum; i < num; i++)
		{
			auto lineNode = Sprite::create("709/LockView/lock_line_node.png");
			lineNode->setTag(i);
			if (i < 2)
			{
				lineNode->setVisible(false);
			}
			this->addChild(lineNode);
			m_vectorLine.pushBack(lineNode);
		}
	}
	else
	{
		for (int i = num; i< hasNum; i++)
		{

			Sprite* sp = m_vectorLine.at(i);
			//log("xin::........ remove lock node ..........  %d", i);
			int tag = sp->getTag();
			if (sp)
			{
				this->removeChild(sp);
			}
		}
		Vector<Sprite *>::iterator iter = m_vectorLine.begin();
		Sprite* item;
		while( iter != m_vectorLine.end())  
		{  
			item = *iter;
			if (item->getTag() >= num)
			{
				iter = m_vectorLine.erase(iter);
			}
			else
			{
				iter ++ ;  
			}
		}
		  
	}
}

//设置每个点的坐标
void LockLineLayer::initLineNodePositon()
{
	
	if (m_vectorLine.empty())
	{
		return;
	}
	float distance = MathAide::CalcDistance(m_start.x, m_start.y, m_end.x, m_end.y);
	float angle = MathAide::CalcAngle(m_start.x, m_start.y, m_end.y, m_end.y);
	float cos_value = fabs(m_start.x - m_end.x) / distance;
	float sin_value =  fabs(m_start.y - m_end.y) / distance;
	for (int i = 0; i < m_vectorLine.size(); i++)
	{
		int lengthX = 0;
		int lengthY = 0;
		if (m_start.x > m_end.x)
		{
			if (m_start.y > m_end.y)
			{//第3象限
				lengthX = m_start.x - (i * line_space * cos_value);
				lengthY = m_start.y - (i * line_space * sin_value);
			}
			else
			{//第二象限
				lengthX = m_start.x - (i * line_space * cos_value);
				lengthY = m_start.y + (i * line_space * sin_value);
			}
		}
		else
		{
			if (m_start.y > m_end.y)
			{//第4象限
				lengthX = m_start.x + (i * line_space * cos_value);
				lengthY = m_start.y - (i * line_space * sin_value);
			}
			else
			{//第一象限
				lengthX = m_start.x + (i * line_space * cos_value);
				lengthY = m_start.y + (i * line_space * sin_value);
			}
		}
		m_vectorLine.at(i)->setPosition(lengthX, lengthY);
	}
}

void LockLineLayer::setPointStartAndEnd(Vec2 startP, Vec2 endP)
{
	this->m_start = startP;
	this->m_end = endP;

	float tempDist = MathAide::CalcDistance(m_start.x, m_start.y, m_end.x, m_end.y);

	int distence = tempDist;

	int nodeCount = distence / line_space;//点的总个数
	initLineView(nodeCount);
	initLineNodePositon();
}