#include "DZPKChip.h"
#include "DDZRes.h"
#include "DZPKTools.h"
#include "CocosUnits.h"

DZPKChip::DZPKChip()
{
}

DZPKChip::~DZPKChip()
{
}

bool DZPKChip::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void DZPKChip::initUI()
{
	m_pChipImg = Node::create();
	addChild(m_pChipImg);

	m_pChipNum = Label::create();
	this->addChild(m_pChipNum);
	m_pChipNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pChipNum->setSystemFontSize(20);
	m_pChipNum->setPosition(Vec2(25, 0));
}

void DZPKChip::changeChip(LONGLONG llValue)
{
	changeChipImg(llValue);
	changeChipNum(llValue);
}

void DZPKChip::changeChipImg(LONGLONG llValue)
{
	m_pChipImg->removeAllChildren();
	if (llValue <= 0)
	{
		return;
	}
	int nChipCount = getChipGroupCount(llValue);
	vector<int> vChipGroup = getChipGroup(llValue);

	for (int i = 0;i < nChipCount; i++)
	{
		char cTexture[32] = "";
		sprintf(cTexture, texture_name::s_dzchip, *(vChipGroup.begin()+i));
		Sprite* pChip = Sprite::createWithSpriteFrameName(cTexture);
		m_pChipImg->addChild(pChip);
		pChip->setPosition(Vec2(0, i * 2));
	}
}

void DZPKChip::changeChipNum(LONGLONG llValue)
{
	if (llValue <= 0)
	{
		m_pChipNum->setVisible(false);
		return;
	}
	m_pChipNum->setVisible(true);
	if (llValue >= 10000)
	{
		char szBuf[100] = ""; 
		double  jsdbhfvs=  (double)llValue / 10000;
		sprintf(szBuf, "%0.2f%s", jsdbhfvs, DZPK_loadText("t10"));
		m_pChipNum->setString(szBuf);
	}
	else
	{
		m_pChipNum->setString(addCommaToNumber(llValue).c_str());
	}
}

int DZPKChip::getChipGroupCount(LONGLONG llValue)
{
	if (llValue==1)
		return 1;
	else if (llValue<=3)
		return 2;
	else if (llValue==4)
		return 3;
	else if (llValue<=9)
		return 3;
	else if (llValue<=19)
		return 3;
	else if (llValue<=24)
		return 4;
	else if (llValue<=49)
		return 5;
	else if (llValue<=99)
		return 6;
	else if (llValue<=199)
		return 4;
	else if (llValue<=399)
		return 5;
	else if (llValue<=499)
		return 6;
	else if (llValue<=999)
		return 4;
	else if (llValue<=1999)
		return 5;
	else if (llValue<=3999)
		return 6;
	else if (llValue<=4999)
		return 4;
	else if (llValue<=9999)
		return 5;
	else if (llValue<=19999)
		return 6;
	else if (llValue<=39999)
		return 4;
	else if (llValue<=79999)
		return 5;
	else if (llValue<=99999)
		return 6;
	else if (llValue<=499999)
		return 4;
	else if (llValue<=999999)
		return 5;
	else if (llValue<=1999999)
		return 6;
	else if (llValue<=3999999)
		return 4;
	else if (llValue<=7999999)
		return 5;
	else if (llValue>=8000000)
		return 6;
	else
		return 0;
}

vector<int> DZPKChip::getChipGroup(LONGLONG llValue)
{
	std::vector<int> type;
	type.clear();
	if (llValue==1)
	{
		type.push_back(1);
	}
	else if (llValue<=3)
	{
		for (int i=0;i<2;i++)
		{
			type.push_back(1);
		}
	}
	else if (llValue==4)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(1);
		}
	}
	else if (llValue<=9)
	{
		for (int i=0;i<2;i++)
		{
			type.push_back(1);
		}
		type.push_back(2);
	}
	else if (llValue<=19)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(1);
		}
		type.push_back(2);
	}
	else if (llValue<=24)
	{
		for (int i=0;i<4;i++)
		{
			type.push_back(1);
		}
		type.push_back(2);
	}
	else if (llValue<=49)
	{
		for (int i=0;i<5;i++)
		{
			type.push_back(1);
		}
		type.push_back(2);
	}
	else if (llValue<=99)
	{
		for (int i=0;i<6;i++)
		{
			type.push_back(2);
		}
	}
	else if (llValue<=199)
	{ 
		for (int i=0;i<3;i++)
		{
			type.push_back(1);
		}
		type.push_back(3);
	}
	else if (llValue<=399)
	{
		for (int i=0;i<4;i++)
		{
			type.push_back(1);
		}
		type.push_back(3);
	}
	else if (llValue<=499)
	{
		for (int i=0;i<5;i++)
		{
			type.push_back(1);
		}
		type.push_back(3);
	}
	else if (llValue<=999)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(2);
		}
		type.push_back(3);
	}
	else if (llValue<=1999)
	{
		for (int i=0;i<4;i++)
		{
			type.push_back(2);
		}
		type.push_back(3);
	}
	else if (llValue<=3999)
	{
		for (int i=0;i<6;i++)
		{
			type.push_back(3);
		}
	}
	else if (llValue<=4999)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(2);
		}
		type.push_back(4);
	}
	else if (llValue<=9999)
	{
		for (int i=0;i<4;i++)
		{
			type.push_back(2);
		}
		type.push_back(4);
	}
	else if (llValue<=19999)
	{
		for (int i=0;i<5;i++)
		{
			type.push_back(2);
		}
		type.push_back(4);
	}
	else if (llValue<=39999)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(3);
		}
		type.push_back(4);
	}
	else if (llValue<=49999)
	{
		for (int i=0;i<4;i++)
		{
			type.push_back(3);
		}
		type.push_back(4);
	}
	else if (llValue<=99999)
	{
		for (int i=0;i<6;i++)
		{
			type.push_back(4);
		}
	}
	else if (llValue<=499999)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(3);
		}
		type.push_back(5);
	}
	else if (llValue<=999999)
	{
		for (int i=0;i<4;i++)
		{
			type.push_back(3);
		}
		type.push_back(5);
	}
	else if (llValue<=1999999)
	{
		for (int i=0;i<5;i++)
		{
			type.push_back(3);
		}
		type.push_back(5);
	}
	else if (llValue<=3999999)
	{
		for (int i=0;i<3;i++)
		{
			type.push_back(4);
		}
		type.push_back(5);
	}
	else if (llValue<=7999999)
	{ 
		for (int i=0;i<4;i++)
		{
			type.push_back(4);
		}
		type.push_back(5);
	}
	else if (llValue>=8000000)
	{
		for (int i=0;i<6;i++)
		{
			type.push_back(5);
		}
	}
	
	return type;
}