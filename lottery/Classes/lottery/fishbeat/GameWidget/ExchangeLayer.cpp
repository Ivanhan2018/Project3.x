#include "ExchangeLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "Define.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

ExchangeLayer::ExchangeLayer(void)
{
	m_strAccounts = "";
	m_strPassword = "";
	m_strNickname = "";
	m_uiWidget = nullptr;
	m_layoutA = nullptr;
	m_layoutB = nullptr;
	m_layoutC = nullptr;
	m_ExOkA = nullptr;
	m_ExOkB =nullptr;
	m_close=nullptr;
	m_PicImgA = nullptr;
	m_PicImgB = nullptr;
	m_TextCoin = nullptr;
	m_TextTicket = nullptr;
	SjA = nullptr;
	SjB = nullptr;
	Dz = nullptr;
	Xm = nullptr;
	m_ProgressA = nullptr;
	m_ProgressB = nullptr;

	for (int x = 0; x < 6; x++)
	{
		First_[x]=nullptr;
	} 
	for (int x = 0; x < 4; x++)
	{
		Second_[x]=nullptr;
	} 
	for (int i = 0; i < 8; i++)
	{
		m_TextNum[i] = nullptr;
	}

	m_nFeeNeedTicket[0] = 600000;
	m_nFeeNeedTicket[1] = 1000000;
	m_nFeeNeedTicket[2] = 20000000;

	m_nFlowNeedTicket[0] = 100000;
	m_nFlowNeedTicket[1] = 180000;
	m_nFlowNeedTicket[2] = 350000;
}

ExchangeLayer::~ExchangeLayer(void)
{

}

ExchangeLayer * ExchangeLayer::create()
{
	ExchangeLayer *ret = new ExchangeLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}
bool ExchangeLayer::init()
{
	if ( !Layer::init() )
		return false;

	m_uiWidget = GUIReader::getInstance()->widgetFromJsonFile("ExchangeLayer/ExchangeLayer.ExportJson");
	this->addChild(m_uiWidget);

	m_layoutA =(Layout*)Helper::seekWidgetByName(m_uiWidget,"Panel_ExchangeA");
	m_layoutB = (Layout*)Helper::seekWidgetByName(m_uiWidget,"Panel_ExchangeB");
	m_layoutC = (Layout*)Helper::seekWidgetByName(m_uiWidget,"Panel_ExchangeC");
	m_ExOkA =(Button*)Helper::seekWidgetByName(m_uiWidget,"Btn_OKA");
	m_ExOkB =(Button*)Helper::seekWidgetByName(m_uiWidget,"Btn_OKB");
	m_close = (Button*)Helper::seekWidgetByName(m_uiWidget,"Btn_close");
	m_PicImgA = (ImageView*)Helper::seekWidgetByName(m_uiWidget,"Image_IconA");
	m_PicImgB = (ImageView*)Helper::seekWidgetByName(m_uiWidget,"Image_IconB");

	//进度条
 	m_ProgressA = (LoadingBar*)Helper::seekWidgetByName(m_uiWidget, "Ex_Tel_ProgressBar");
 	m_ProgressB = (LoadingBar*)Helper::seekWidgetByName(m_uiWidget, "Ex_Flow_ProgressBar");

	SjA =(TextField*)Helper::seekWidgetByName(m_uiWidget,"PhoneA");
	SjB =(TextField*)Helper::seekWidgetByName(m_uiWidget,"PhoneB");
	Dz =(TextField*)Helper::seekWidgetByName(m_uiWidget,"Addr");
	Xm =(TextField*)Helper::seekWidgetByName(m_uiWidget,"Name");

	m_TextCoin =(Text*)Helper::seekWidgetByName(m_uiWidget,"Label_coin");
	m_TextTicket =(Text*)Helper::seekWidgetByName(m_uiWidget,"Label_ticket");

	//m_TextCoin->setString(StringUtils::format("%lli",100));
	//m_TextTicket->setString(StringUtils::format("%d",CGlobalUserInfo::GetInstance()->GetEntityResult()->dwTicketCount));
	m_TextCoin->setString(StringUtils::format("%lli",10000));
	m_TextTicket->setString(StringUtils::format("%d",100));

	m_close->setTag(0);
	m_close->setScale(1.5f);
	m_ExOkA->addTouchEventListener(this,SEL_TouchEvent(&ExchangeLayer::ExOk));
	m_ExOkB->addTouchEventListener(this,SEL_TouchEvent(&ExchangeLayer::ExOk));

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_TextNum[2*i + j] = (Text*)Helper::seekWidgetByName(m_uiWidget, StringUtils::format("Ex_Label_%d_%d", i, j));
		}
	}
	
	m_TextNum[1]->setString("10");
	m_TextNum[3]->setString("10");
	m_TextNum[5]->setString("10");
	m_TextNum[7]->setString("10");

	SjA->setTag(1);
	SjB->setTag(1);
	Dz->setTag(2);
	Xm->setTag(3);
	//绑定回调
	SjA->addEventListenerTextField(this,SEL_TextFieldEvent(&ExchangeLayer::OnTextFieldCallBack));
	SjB->addEventListenerTextField(this,SEL_TextFieldEvent(&ExchangeLayer::OnTextFieldCallBack));
	Dz->addEventListenerTextField(this,SEL_TextFieldEvent(&ExchangeLayer::OnTextFieldCallBack));
	Xm->addEventListenerTextField(this,SEL_TextFieldEvent(&ExchangeLayer::OnTextFieldCallBack));

	string strName;
	for (int i=0; i<10;++i)
	{
		strName = StringUtils::format("Ex_Btn_%d",i);
		First_[i] = dynamic_cast<Button*>(Helper::seekWidgetByName(m_uiWidget,strName));
		First_[i]->setTag(i);
		First_[i]->addTouchEventListener(this,SEL_TouchEvent(&ExchangeLayer::OnRechargeCallBack));
	}
	return true;
}



void ExchangeLayer::OnRechargeCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	log("%d",tag);
	//下层触摸
	//SetBtnEnabled(false);
	if (tag >= 0 && tag <= 5)
	{ 
		m_ExOkA->setTag(tag + 32);
		m_PicImgA->loadTexture(StringUtils::format("ExchangeLayer/Dh_0_%d.png",tag));
		m_layoutA->setVisible(true);
		m_layoutB->setVisible(false);
		m_layoutC->setVisible(false);
	}	
	if (tag >= 6 && tag <= 9)
	{
		m_ExOkB->setTag(tag + 32);
		m_PicImgB->loadTexture(StringUtils::format("Dh_0_%d.png",tag),Widget::TextureResType::PLIST);
		m_layoutA->setVisible(false);
		m_layoutB->setVisible(true);
		m_layoutC->setVisible(false);
	}
}
void ExchangeLayer::ExOk(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{

}

void ExchangeLayer::OnTextFieldCallBack(cocos2d::Ref* sender, cocos2d::ui::TextFiledEventType type)
{
	TextField* mTextField = dynamic_cast<TextField*>(sender);
	int tag = mTextField->getTag();
	switch (type)
	{
	case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME: //开始编辑事件
		{
			if (tag==1)
			{
				m_strAccounts = "";
				mTextField->setText("");
			}
			else if (tag==2)
			{
				m_strPassword = "";
				mTextField->setText("");
			}
			else if (tag==3)
			{
				m_strNickname = "";
				mTextField->setText("");
			}
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME://如果是结束编辑事件
		{
			std::string strText = mTextField->getStringValue().c_str();
			if (tag==1)
			{
				char szMobileNum[12]={0};
				if (strText.length()>11)
					strText[11]=0;

				for (int i=0; i<strText.length(); ++i)
				{
					if (strText[i]<'0' || strText[i]>'9' || strText[0]=='0')
					{
						strText[i]=0;
						break;
					}
				}
				mTextField->setText("");
				mTextField->setText(strText);
				m_strAccounts = mTextField->getStringValue().c_str();
			}
			else if (tag==2)
			{
				char szMobileNum[129]={0};
				if (strText.length()>128)
					strText[128]=0;
			
				//mTextField->setTextAreaSize(cocos2d::Size(380,24));
				mTextField->ignoreContentAdaptWithSize(false);
				((Label*)(mTextField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
				mTextField->setContentSize(cocos2d::Size(400,64));
				mTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
				mTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
				mTextField->setText(strText);
				m_strPassword = mTextField->getStringValue().c_str();
			}
			else if (tag==3)
			{
				char szMobileNum[17]={0};
				if (strText.length()>16)
					strText[16]=0;
				mTextField->setText("");
				mTextField->setText(strText);
				m_strNickname = mTextField->getStringValue().c_str();
			}
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT://正在输入事件
		//判断是在哪个输入框操作结束	
		{
			std::string strText = mTextField->getStringValue().c_str();
			if (tag==1)
			{
				char szMobileNum[12]={0};
				if (strText.length()>11)
					strText[11]=0;

				for (int i=0; i<strText.length(); ++i)
				{
					if (strText[i]<'0' || strText[i]>'9' || strText[0]=='0')
					{
						strText[i]=0;
						break;
					}
				}
				mTextField->setText("");
				mTextField->setText(strText);
				m_strAccounts = mTextField->getStringValue().c_str();
			}
			else if (tag==2)
			{
				char szMobileNum[129]={0};
				if (strText.length()>128)
					strText[128]=0;

				//mTextField->setTextAreaSize(cocos2d::Size(380,24));
				mTextField->ignoreContentAdaptWithSize(false);
				((Label*)(mTextField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
				mTextField->setContentSize(cocos2d::Size(400,40));
				mTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
				mTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
				mTextField->setText(strText);
				m_strPassword = mTextField->getStringValue().c_str();
			}
			else if (tag==3)
			{
				char szMobileNum[17]={0};
				if (strText.length()>16)
					strText[16]=0;
				mTextField->setText("");
				mTextField->setText(strText);
				m_strNickname = mTextField->getStringValue().c_str();
			}
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_DELETE_BACKWARD://输入的时候删除事件	
		break;
	default:
		break;
	}
}
void ExchangeLayer::SetBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::SEL_TouchEvent selector)
{
	m_close->addTouchEventListener(sender,selector);
}

//void ExchangeLayer::SetBtnEnabled(bool enabled)
//{
//	//m_close->setEnabled(enabled);
//	for (int x = 0; x < 2; x++)
//	{
//		for (int y = 0; y < 5; y++)
//		{
//			btnStore[x][y]->setVisible(enabled);
//		}
//	} 
//}
//
//ExchangeItem::ExchangeItem()
//{
//	m_ExchangBG = nullptr;
//}
//
//ExchangeItem::~ExchangeItem()
//{
//
//}
//
//ExchangeItem* ExchangeItem::create(int nNum)
//{
//	ExchangeItem* layout = new ExchangeItem();
//	if (layout && layout->init(nNum))
//	{
//		layout->autorelease();
//		return layout;
//	}
//	CC_SAFE_DELETE(layout);
//	return nullptr;
//}
//
//bool ExchangeItem::init(int nNum)
//{
//	if (!Layout::init())
//		return false;
//	this->setAnchorPoint(Vec2::ZERO);
//
//	m_ExchangBG = ui::ImageView::create("background.png");
//	m_ExchangBG->setPosition(Vec2(310,100));
//	this->addChild(m_ExchangBG);
//}
//
//
//
void ExchangeLayer::updateView()
{
	//m_TextTicket->setString(StringUtils::format("%d",CGlobalUserInfo::GetInstance()->GetEntityResult()->dwTicketCount));
}

void ExchangeLayer::updateEntity(int nTicketCount, int nIpadCount, int nMp3Count, int nWhiteCount, int nZippoCount, int nIpadNeed, int nMp3Need, int nWhiteNeed, int nZippoNeed)
{
	float fPercentA = 0;// CGlobalUserInfo::GetInstance()->GetEntityResult()->dwTicketCount / 2250000.0f;
	float fPercentB = 0;//CGlobalUserInfo::GetInstance()->GetEntityResult()->dwTicketCount / 400000.0f;
	m_ProgressA->setPercent(fPercentA * 100);
	m_ProgressB->setPercent(fPercentB * 100);

	m_TextTicket->setString(StringUtils::format("%d", nTicketCount));
	m_TextNum[0]->setString(StringUtils::format("%d", nIpadCount));
	m_TextNum[1]->setString(StringUtils::format("%d", nIpadNeed));
	m_TextNum[2]->setString(StringUtils::format("%d", nMp3Count));
	m_TextNum[3]->setString(StringUtils::format("%d", nMp3Need));
	m_TextNum[4]->setString(StringUtils::format("%d", nWhiteCount));
	m_TextNum[5]->setString(StringUtils::format("%d", nWhiteNeed));
	m_TextNum[6]->setString(StringUtils::format("%d", nZippoCount));
	m_TextNum[7]->setString(StringUtils::format("%d", nZippoNeed));

	//兑换话费展现
	if (nTicketCount < m_nFeeNeedTicket[0])
	{
		First_[0]->setEnabled(false);
		First_[1]->setEnabled(false);
		First_[2]->setEnabled(false);
	}
	else if (nTicketCount < m_nFeeNeedTicket[1])
	{
		First_[0]->setScale(1.2f);
		First_[1]->setEnabled(false);
		First_[2]->setEnabled(false);
	}
	else if (nTicketCount < m_nFeeNeedTicket[3])
	{
		First_[0]->setScale(1.2f);
		First_[1]->setScale(1.2f);
		First_[2]->setEnabled(false);
	}
	else
	{
		First_[0]->setScale(1.2f);
		First_[1]->setScale(1.2f);
		First_[2]->setEnabled(1.2f);
	}
	
	//兑换流量展现
	if (nTicketCount < m_nFlowNeedTicket[0])
	{
		First_[3]->setEnabled(false);
		First_[4]->setEnabled(false);
		First_[5]->setEnabled(false);
	}
	else if (nTicketCount < m_nFlowNeedTicket[1])
	{
		First_[3]->setScale(1.2f);
		First_[4]->setEnabled(false);
		First_[5]->setEnabled(false);
	}
	else if (nTicketCount < m_nFlowNeedTicket[3])
	{
		First_[3]->setScale(1.2f);
		First_[4]->setScale(1.2f);
		First_[5]->setEnabled(false);
	}
	else
	{
		First_[3]->setScale(1.2f);
		First_[4]->setScale(1.2f);
		First_[5]->setEnabled(1.2f);
	}
	//兑换实物展现
	if (nIpadCount < nIpadNeed)
	{
		First_[6]->setEnabled(false);
	}
	if (nMp3Count < nMp3Need)
	{
		First_[7]->setEnabled(false);
	}
	if (nWhiteCount < nWhiteNeed)
	{
		First_[8]->setEnabled(false);
	}
	if (nZippoCount < nZippoNeed)
	{
		First_[9]->setEnabled(false);
	}
}

void ExchangeLayer::updateEntity()
{
	//DWORD dwTicketCount = CGlobalUserInfo::GetInstance()->GetEntityResult()->dwTicketCount;
	//int nIpadCount = CGlobalUserInfo::GetInstance()->GetEntityResult()->nIpadCount;
	//int nMp3Count = CGlobalUserInfo::GetInstance()->GetEntityResult()->nMp3Count;
	//int nWhiteCount = CGlobalUserInfo::GetInstance()->GetEntityResult()->nWhiteCount;
	//int nZippoCount = CGlobalUserInfo::GetInstance()->GetEntityResult()-> nZippoCount;
	//int nIpadNeed = CGlobalUserInfo::GetInstance()->GetEntityResult()->nIpadNeed;
	//int nMp3Need = CGlobalUserInfo::GetInstance()->GetEntityResult()->nMp3Need;
	//int nWhiteNeed  = CGlobalUserInfo::GetInstance()->GetEntityResult()->nWhiteNeed;
	//int nZippoNeed  = CGlobalUserInfo::GetInstance()->GetEntityResult()->nZippoNeed;
	DWORD dwTicketCount = 1000;
	int nIpadCount = 1000;
	int nMp3Count = 1000;
	int nWhiteCount = 1000;
	int nZippoCount = 1000;
	int nIpadNeed = 1000;
	int nMp3Need = 1000;
	int nWhiteNeed  = 1000;
	int nZippoNeed  = 1000;

	float fPercentA = dwTicketCount / 2250000.0f;
	float fPercentB = dwTicketCount / 400000.0f;
	m_ProgressA->setPercent(fPercentA * 100);
	m_ProgressB->setPercent(fPercentB * 100);

	m_TextTicket->setString(StringUtils::format("%d", dwTicketCount));
	m_TextNum[0]->setString(StringUtils::format("%d", nIpadCount));
	m_TextNum[1]->setString(StringUtils::format("%d", nIpadNeed));
	m_TextNum[2]->setString(StringUtils::format("%d", nMp3Count));
	m_TextNum[3]->setString(StringUtils::format("%d", nMp3Need));
	m_TextNum[4]->setString(StringUtils::format("%d", nWhiteCount));
	m_TextNum[5]->setString(StringUtils::format("%d", nWhiteNeed));
	m_TextNum[6]->setString(StringUtils::format("%d", nZippoCount));
	m_TextNum[7]->setString(StringUtils::format("%d", nZippoNeed));

	//兑换话费展现
	if (dwTicketCount < m_nFeeNeedTicket[0])
	{
		First_[0]->setEnabled(false);
		First_[1]->setEnabled(false);
		First_[2]->setEnabled(false);
	}
	else if (dwTicketCount < m_nFeeNeedTicket[1])
	{
		First_[0]->setScale(1.2f);
		First_[1]->setEnabled(false);
		First_[2]->setEnabled(false);
	}
	else if (dwTicketCount < m_nFeeNeedTicket[3])
	{
		First_[0]->setScale(1.2f);
		First_[1]->setScale(1.2f);
		First_[2]->setEnabled(false);
	}
	else
	{
		First_[0]->setScale(1.2f);
		First_[1]->setScale(1.2f);
		First_[2]->setEnabled(1.2f);
	}

	//兑换流量展现
	if (dwTicketCount < m_nFlowNeedTicket[0])
	{
		First_[3]->setEnabled(false);
		First_[4]->setEnabled(false);
		First_[5]->setEnabled(false);
	}
	else if (dwTicketCount < m_nFlowNeedTicket[1])
	{
		First_[3]->setScale(1.2f);
		First_[4]->setEnabled(false);
		First_[5]->setEnabled(false);
	}
	else if (dwTicketCount < m_nFlowNeedTicket[3])
	{
		First_[3]->setScale(1.2f);
		First_[4]->setScale(1.2f);
		First_[5]->setEnabled(false);
	}
	else
	{
		First_[3]->setScale(1.2f);
		First_[4]->setScale(1.2f);
		First_[5]->setEnabled(1.2f);
	}
	//兑换实物展现
	if (nIpadCount < nIpadNeed)
	{
		First_[6]->setEnabled(false);
	}
	if (nMp3Count < nMp3Need)
	{
		First_[7]->setEnabled(false);
	}
	if (nWhiteCount < nWhiteNeed)
	{
		First_[8]->setEnabled(false);
	}
	if (nZippoCount < nZippoNeed)
	{
		First_[9]->setEnabled(false);
	}
}
