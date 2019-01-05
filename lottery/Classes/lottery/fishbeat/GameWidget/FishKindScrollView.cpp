#include "FishKindScrollView.h"

USING_NS_CC;
using namespace cocos2d;
using namespace ui;

FishKindScrollView::FishKindScrollView():
	m_Count(25),
	m_length(3),
	m_itemHeigth(181),
	m_itemWidth(620),
	m_pandingX(5),
	m_pandingY(15)
{
    
}
FishKindScrollView::~FishKindScrollView()
{

}
bool FishKindScrollView::init()
{
    bool bRet = false;
    do
    {
        if (!Layer::init())
        {
            return false;
        }

		this->setContentSize(cocos2d::Size(m_itemWidth, m_itemHeigth * 2 + m_pandingY * 2));

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(FishKindScrollView::onTouchBegan, this);	
		listener->onTouchEnded = CC_CALLBACK_2(FishKindScrollView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(FishKindScrollView::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(FishKindScrollView::onTouchCancelled, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
		
		m_table = TableView::create(this, cocos2d::Size(m_itemWidth, m_itemHeigth * 2 + m_pandingY * 2));	
		m_table->setDelegate(this);
		m_table->setDirection(TableView::Direction::VERTICAL);
		m_table->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		this->addChild(m_table, 0);

		m_table->reloadData();	
		setTouchEnabled(true);

        bRet = true;
    } while (0);
    
    return bRet;
}

void FishKindScrollView::onEnter()
{
    Layer::onEnter();
    //
    //Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this,-128);
}

void FishKindScrollView::onExit()
{
    //
    Layer::onExit();
}

bool FishKindScrollView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}
void FishKindScrollView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
   
}

void FishKindScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	
}

void FishKindScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

void FishKindScrollView::scrollViewDidScroll(ScrollView* view)
{

}

void FishKindScrollView::scrollViewDidZoom(ScrollView* view)
{

}

void FishKindScrollView::tableCellTouched(TableView* table, TableViewCell* cell)
{
}

cocos2d::Size FishKindScrollView::cellSizeForTable(TableView *table)
{
	cocos2d::Size size = cocos2d::Size(m_itemWidth, m_itemHeigth + m_pandingY); //高度定死
	return size;
}

TableViewCell* FishKindScrollView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell* cell = new TableViewCell();

	if ((idx + 1) * m_length <= m_Count)
	{
		auto bg = Sprite::create("709/Introduce/BackGround_Small.png");
		bg->setPosition(Vec2(m_itemWidth / 2.0f, m_itemHeigth / 2.0f));
		cell->addChild(bg);
	}

	for(int i = 0 ; i < m_length; i ++)
	{	
		
		int data_index = idx * m_length + i + 1;
		if (data_index > m_Count)
		{
			break;
		}

		string nameStr = String::createWithFormat("709/Introduce/fish%d.png", data_index)->getCString();

		if (data_index == m_Count)
		{
			nameStr = "709/Introduce/halo.png";
		}
		

		int column = data_index % m_length;//列

		int errorX = 0;
		int errorY = -10;
		if (column == 1)
		{
			errorX = -12;
			errorY = -8;
		}
		else if (column == 2)
		{
			errorX = 0;
			errorY = -10;
		}
		else if (column == 0)
		{
			errorX = 10;
			errorY = -5;
		}

		if ((idx + 1) * m_length > m_Count)
		{
			auto bg = Sprite::create("709/Introduce/BackGround_Small11.png");
			bg->setPosition(Vec2(m_itemWidth * (i * 2 + 1) / (m_length * 2) + errorX, m_itemHeigth / 2.0f));
			cell->addChild(bg);
		}

		Sprite *kindSprite = Sprite::create(nameStr);
		kindSprite->setPosition(Vec2(m_itemWidth * (i * 2 + 1) / (m_length * 2) + errorX, m_itemHeigth / 2.0f + errorY));
		cell->addChild(kindSprite);		

		auto txt = LabelTTF::create("X 200","Arial",25);
		setFishScore(data_index, txt);
		txt->setPosition(Vec2(m_itemWidth * (i * 2 + 1) / (m_length * 2) + errorX, m_itemHeigth * 0.12));
		cell->addChild(txt);
		
	}

	return cell;
}

ssize_t FishKindScrollView::numberOfCellsInTableView(TableView *table)
{
	
	//返回行数
	int result = m_Count / m_length;
	int tempLast = m_Count % m_length; //除4有余数，则加1
	if (tempLast != 0) result ++;
	return result;
	
	//return m_Count;
}

void FishKindScrollView::setFishScore(int index, LabelTTF* text)
{
	string name = "";
	switch (index)
	{
	case 1:
		name = "X 2";
		break;
	case 2:
		name = "X 2";
		break;
	case 3:
		name = "X 3";
		break;
	case 4:
		name = "X 4";
		break;
	case 5:
		name = "X 5";
		break;
	case 6: 
		name = "X 6";
		break;
	case 7:
		name = "X 7";
		break;
	case 8:
		name = "X 8";
		break;
	case 9:
		name = "X 9";
		break;
	case 10:
		name = "X 10";
		break;
	case 11:
		name = "X 12";
		break;
	case 12:
		name = "X 15";
		break;
	case 13:
		name = "X 18";
		break;
	case 14: 
		name = "X 20";
		break;
	case 15:
		name = "X 25";
		break;
	case 16:
		name = "X 30";
		break;
	case 17:
		name = "X 35";
		break;
	case 18:
		name = "40-120";
		break;
	case 19:
		name = "120-300";
		break;
	case 20:
		name = "X 320";
		break;
	case 21:
		name = "40-300";
		break;
	case 22: 
		name = "";
		break;
	case 23:
		name = "";
		break;
	case 24:
		name = "";
		break;
	case 25:
		name = "";
		break;
	default:
		break;
	}
	text->setString(name);


	/*
	if (index == 0)
	{
		m_txtSub1= LabelTTF::create("X 2","Arial",25);
		m_txtSub2= LabelTTF::create("X 2","Arial",25);
		m_txtSub3= LabelTTF::create("X 3","Arial",25);
	}
	else if (nRow == 1)
	{
		m_txtSub1= LabelTTF::create("X 4","Arial",25);
		m_txtSub2= LabelTTF::create("X 5","Arial",25);
		m_txtSub3= LabelTTF::create("X 6","Arial",25);
	}
	else if (nRow == 2)
	{
		m_txtSub1= LabelTTF::create("X 7","Arial",25);
		m_txtSub2= LabelTTF::create("X 8","Arial",25);
		m_txtSub3= LabelTTF::create("X 9","Arial",25);
	}
	else if (nRow == 3)
	{
		m_txtSub1= LabelTTF::create("X 10","Arial",25);
		m_txtSub2= LabelTTF::create("X 12","Arial",25);
		m_txtSub3= LabelTTF::create("X 15","Arial",25);
	}
	else if (nRow == 4)
	{
		m_txtSub1= LabelTTF::create("X 18","Arial",25);
		m_txtSub2= LabelTTF::create("X 20","Arial",25);
		m_txtSub3= LabelTTF::create("X 25","Arial",25);
	}
	else if (nRow == 5)
	{
		m_txtSub1= LabelTTF::create("X 30","Arial",25);
		m_txtSub2= LabelTTF::create("X 35","Arial",25);
		m_txtSub3= LabelTTF::create("40-120","Arial",25);
	}
	else if (nRow == 6)
	{
		m_txtSub1= LabelTTF::create("120-300","Arial",25);
		m_txtSub2= LabelTTF::create("X 320","Arial",25);
		m_txtSub3= LabelTTF::create("40-300","Arial",25);
	}
	else if (nRow == 7)
	{
		m_txtSub1= LabelTTF::create("","Arial",25);
		m_txtSub2= LabelTTF::create("","Arial",25);
		m_txtSub3= LabelTTF::create("","Arial",25);

		m_txtSub1->setString("");
		m_txtSub2->setString("");
		m_txtSub3->setString("");

	}
	int i = nRow * m_length;
	m_txtSub1->setPosition(Vec2(m_itemWidth * (i * 2 + 1) / (m_length * 2), m_itemHeigth * 0.2));
	node->addChild(m_txtSub1);

	m_txtSub2->setPosition(Vec2(m_itemWidth * ((i + 1) * 2 + 1) / (m_length * 2), m_itemHeigth * 0.3));
	node->addChild(m_txtSub2);

	m_txtSub3->setPosition(Vec2(m_itemWidth * ((i + 2) * 2 + 1) / (m_length * 2), m_itemHeigth * 0.25));
	node->addChild(m_txtSub3);
	*/

}