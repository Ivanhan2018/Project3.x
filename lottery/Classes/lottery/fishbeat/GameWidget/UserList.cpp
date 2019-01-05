#include "UserList.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include "UserItem.h"



USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

UserList::UserList(void)
{
}
UserList::~UserList(void)
{
}
bool UserList::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	Widget* UI = GUIReader::getInstance()->widgetFromJsonFile("ComLayer/UserList_1/UserList_1.json");
	this->addChild(UI,10);
		 
	m_close = (ui::Button*)Helper::seekWidgetByName(UI,"Button_7");
	m_close->setTag(em_OP_UserList_exit);

	m_listView = (ListView*)Helper::seekWidgetByName(UI,"ListView_5");
	
	return true;
}

void UserList::addItem(std::int32_t userID)
{		
	for(int i=0;i<1;i++)
	{
		Layout* layout = Layout::create();
		layout->setContentSize(cocos2d::Size(550,230));
		int n = 0;
		int x = 0;
		while (n!=3)
		{
			//CUserItem* pUserItem=	pTableView->GetClientUserItem(i*3+n);
			CUserItem * pUserItem = NULL;
			n++;
			if(pUserItem==NULL)
				continue;

			Button* lu_Bg = Button::create("ComLayer/UserList_1/ul_ubg.png","","");
			lu_Bg->setTag(i*3+n-1);
			lu_Bg->setPosition(Vec2(x*522+14,7));
			lu_Bg->setAnchorPoint(Vec2::ZERO);
			layout->addChild(lu_Bg);

			Sprite* ul_user = Sprite::create("ComLayer/UserList_1/ul_user.png");
			ul_user->setPosition(Vec2(30,30));
			ul_user->setAnchorPoint(Vec2::ZERO);
			lu_Bg->addChild(ul_user);
				
			Label* tNickName = Label::create();//Label::createWithTTF(StringUtils::format("%s",buf), "SimHei", 50);
			tNickName->setWidth(320);
			tNickName->setHeight(48);
			tNickName->setSystemFontSize(48);
			tNickName->setString("");
			tNickName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			tNickName->setPosition(200,130);
			tNickName->setColor(Color3B(255,255,255));
			lu_Bg->addChild(tNickName);

			Sprite* ul_jet = Sprite::create("ComLayer/UserList_1/ul_jet.png");
			ul_jet->setPosition(Vec2(200,40));
			ul_jet->setAnchorPoint(Vec2::ZERO);
			lu_Bg->addChild(ul_jet);

			Label* tUserScore = Label::create();//Label::createWithTTF(StringUtils::format("%s",buf), "SimHei", 50);
			tUserScore->setName("score_text");
			tUserScore->setSystemFontSize(40);
			tUserScore->setString(StringUtils::format("%lli",pUserItem->getUserGold()));
			tUserScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			tUserScore->setPosition(260,58);
			tUserScore->setColor(Color3B(255,255,255));
			lu_Bg->addChild(tUserScore);
			x++;
		}
		if(x==0)
			continue;
		m_listView->pushBackCustomItem(layout);
	}
}
void UserList::onCallback(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	m_close->addTouchEventListener(target,selector);
}

void UserList::removeAllItem()
{
	m_listView->removeAllItems();
}

void UserList::setUserScore(unsigned short nChairID, long long score)
{
	if (m_listView != nullptr)
	{
		const auto& arrayRootChildren = m_listView->getItems();
		for (auto& subWidget : arrayRootChildren)
		{
			const auto& arraylu_BgChildren = subWidget->getChildren();
			for (auto& subBg : arraylu_BgChildren)
			{
				if (subBg->getTag() == nChairID)
				{
					Label* text = dynamic_cast<Label*>(subBg->getChildByName("score_text"));
					if (text != nullptr)
					{
						text->setString(StringUtils::format("%lli", score));
					}
				}
			}
		}
	}
}
