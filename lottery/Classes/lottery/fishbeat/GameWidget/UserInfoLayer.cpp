#include "UserInfoLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "VisibleRect.h"

#include "CMessageBox.h"
#include "GlobalDef.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

UserInfoLayer::UserInfoLayer(void) 
{
	m_Touchbg = nullptr;
	m_moreInfo = nullptr;
	m_ModifyPW = nullptr;
	m_backpack = nullptr;
	m_record = nullptr;
	for(int i=0;i<3;i++)
	{
		m_mpPW[i]=nullptr;
		m_mpBtnClear[i]=nullptr;
		m_szCinText[i]="";

		m_btn_bp[i]=nullptr;

		L_bp[i]=nullptr;
		if (i<2)
		{
			L_record[i]=nullptr;
		}
	}
	m_btnClose = nullptr;
	m_btnInfo = nullptr;
	m_btnRecord = nullptr;
	m_btnBackpack = nullptr;
	m_btnModifyPW = nullptr;

	m_faceID = nullptr;
	m_tf_Nickname = nullptr;
	m_txtGameID = nullptr;
	m_txtScore = nullptr;

	m_moreinfoPhone = nullptr;
	m_moreinfoEmail = nullptr;
	m_UnderWrite = nullptr;
	m_btnUnderWrite = nullptr;

	m_btn_get_record = nullptr;
	m_btn_game_record = nullptr;
	ListView0 = nullptr;
	ListView1 = nullptr;

	for(int i=0;i<2;i++)
	{
		for(int n=0;n<5;n++)
		{
			m_bpIme[i*5+n]=nullptr;
			m_cfIme[i*5+n]=nullptr;
			m_dfIme[i*5+n]=nullptr;
		}
	}
	for(int i=0;i<4;i++)
	{
		m_isUserInfoModify[i] = false;
	}

	m_mpBtnCel = nullptr;
	m_mpBtnSend = nullptr;
	m_bpTxtName = nullptr;
	m_bpTxtDescribe = nullptr;
	m_bpDestroy = nullptr;
	m_bpExercise = nullptr;
	m_wFaceID = INVALID_CHAIR;

	m_isEmailEditing = false;
	m_isNickNameEditing = false;
	m_isUnderWriteEditing = false;
}

UserInfoLayer::~UserInfoLayer(void)
{


}
bool UserInfoLayer::init()
{
	//登陆界面
	uiWidget = GUIReader::getInstance()->widgetFromJsonFile("UserInfo/UserInfo.ExportJson");
	this->addChild(uiWidget);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("face/face.plist","face/face.png");

	GetCocosUI();
	m_btnShare->setTag(em_Share);
	m_btnShare->addTouchEventListener(this,toucheventselector(UserInfoLayer::OnUserInfoSel));
	m_btn2VCode->setTag(em_2V_Code);
	m_btn2VCode->addTouchEventListener(this,toucheventselector(UserInfoLayer::OnUserInfoSel));
	m_btnCopyLink->setTag(em_CopyLink);
	m_btnCopyLink->addTouchEventListener(this,toucheventselector(UserInfoLayer::OnUserInfoSel));


	//复制没用到干掉
	(dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,"Image_127"))->setVisible(false));
	m_btnCopyLink->setVisible(false);

	m_Touchbg = dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,"Panel_14"));
	m_moreInfo = dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,"moreinfo"));
	m_ModifyPW = dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,"modifpw"));
	m_backpack = dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,"backpack"));
	m_record  = dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,"record"));
	for(int i=0;i<3;i++)
	{
		L_bp[i]=dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("L_bp%d",i)));
		if (i<2)
		{
		    L_record[i]=dynamic_cast<Layout*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("L_record%d",i)));
		}
	}
	m_btnClose = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"ui_btn_close"));
	m_btnInfo = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"ui_btn_userinfo"));
	m_btnRecord = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"ui_btn_record"));
	m_btnBackpack = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"ui_btn_knapsack"));
	m_btnModifyPW = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"ui_btn_modifPw"));

	m_faceID = dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,"img_face"));
	m_faceID->setTouchEnabled(true);
	m_faceID->addTouchEventListener(this, toucheventselector(UserInfoLayer::onBtnFaceTap));

	m_btnClose->setTag(0);
	m_btnInfo->setTag(em_UserInfo);
	m_btnRecord->setTag(em_Record);
	m_btnBackpack->setTag(em_Backpack);
	m_btnModifyPW->setTag(em_ModifyPW);

	m_btnInfo->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnRecord->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnBackpack->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnModifyPW->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnBackpack->setVisible(false);
	m_btnBackpack->setEnabled(false);
	//游戏记录暂时屏蔽掉
	m_btnRecord->setVisible(false);
	m_btnRecord->setEnabled(false);

	m_tf_Nickname = dynamic_cast<TextField*>(Helper::seekWidgetByName(uiWidget,"base_txt_nickname"));
	m_txtGameID = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"base_txt_gameid"));
	m_txtScore = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"base_txt_score"));
	m_txtAccount = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"base_txt_Account"));


	m_moreinfoPhone =  dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"moreinfo_phone"));
	m_moreinfoEmail =  dynamic_cast<TextField*>(Helper::seekWidgetByName(uiWidget,"moreinfo_email"));
	m_UnderWrite =  dynamic_cast<TextField*>(Helper::seekWidgetByName(uiWidget,"moreinfo_underwrite"));
	m_btnUnderWrite =  dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_underwrite_edit"));
	m_btnEmail =  dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_email_edit"));
	m_btnNickName =  dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_nickname_edit"));
	m_btnModifyOK =  dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"moreinfo_modify_ok"));

	


	m_tf_Nickname->setTextHorizontalAlignment(TextHAlignment::CENTER);
	m_tf_Nickname->setTag(em_NickNameTF);
	m_tf_Nickname->addEventListenerTextField(this,SEL_TextFieldEvent(&UserInfoLayer::OnTextFieldCallBack));
	m_btnUnderWrite->setTag(em_PerfectInfo);
	m_btnUnderWrite->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnEmail->setTag(em_EmailEdit);
	m_btnEmail->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnNickName->setTag(em_NickNameEdit);
	m_btnNickName->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));
	m_btnModifyOK->setTag(em_ModidySucceed);
	m_btnModifyOK->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnUserInfoSel));


	m_moreinfoPhone->setTag(emUserInfo_phone);
// 	m_moreinfoPhone->addEventListenerTextField(this,SEL_TextFieldEvent(&UserInfoLayer::OnTextFieldCallBack));
	m_moreinfoEmail->setTag(emUserInfo_Emai);
	m_moreinfoEmail->addEventListenerTextField(this,SEL_TextFieldEvent(&UserInfoLayer::OnTextFieldCallBack));

	
	m_moreinfoEmail->ignoreContentAdaptWithSize(false);
	((Label*)(m_moreinfoEmail->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
// 	m_moreinfoEmail->setContentSize(cocos2d::Size(200,25));
	m_moreinfoEmail->setTextHorizontalAlignment(TextHAlignment::CENTER);
	m_moreinfoEmail->setTextVerticalAlignment(TextVAlignment::CENTER);

	m_UnderWrite->setTag(emUserInfo_underWrite);
	m_UnderWrite->addEventListenerTextField(this,SEL_TextFieldEvent(&UserInfoLayer::OnTextFieldCallBack));

	m_UnderWrite->ignoreContentAdaptWithSize(false);
	((Label*)(m_UnderWrite->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
// 	((Label*)(m_UnderWrite->getVirtualRenderer()))->setDimensions(250,150);
// 	m_UnderWrite->setContentSize(cocos2d::Size(250,150));
	m_UnderWrite->setTextHorizontalAlignment(TextHAlignment::CENTER);
	m_UnderWrite->setTextVerticalAlignment(TextVAlignment::CENTER);

	for(int i=0;i<3;i++)
	{
		m_mpPW[i]=dynamic_cast<TextField*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("mp_pw%d",i)));
		m_mpPW[i]->setTag(emModifPW_PW0+i);
		m_mpPW[i]->addEventListenerTextField(this,SEL_TextFieldEvent(&UserInfoLayer::OnTextFieldCallBack));
		m_mpBtnClear[i]=dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("mp_btn_clear%d",i)));
		m_mpBtnClear[i]->setTag(em_ModifyPWClear0+i);
		m_mpBtnClear[i]->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));

		m_btn_bp[i]=dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("m_btn_bp%d",i)));
		m_btn_bp[i]->setTag(em_Backpack_btn_bp0+i);
		m_btn_bp[i]->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));
	}
	for(int i=0;i<2;i++)
	{
		for(int n=0;n<5;n++)
		{
			m_bpIme[i*5+n]=dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("p_%d_%d",i,n)));
			m_bpIme[i*5+n]->loadTexture("property/property_bg_nor.png");

			m_cfIme[i*5+n]=dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("b_%d_%d",i,n)));
			m_cfIme[i*5+n]->loadTexture("property/property_bg_nor.png");

			m_dfIme[i*5+n]=dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,StringUtils::format("k_%d_%d",i,n)));
			m_dfIme[i*5+n]->loadTexture("property/property_bg_nor.png");
		}
	}

	ListView0 = dynamic_cast<ListView*>(Helper::seekWidgetByName(uiWidget,"ListView0"));
    ListView1 = dynamic_cast<ListView*>(Helper::seekWidgetByName(uiWidget,"ListView1"));

	m_btn_get_record = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_get_record"));
    m_btn_game_record = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_game_record"));

	m_mpBtnCel = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"mp_btn_cel"));
	m_mpBtnSend = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"mp_btn_send"));

	m_bpTxtName = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"p_name"));
	m_bpTxtDescribe = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"p_describe"));
	m_bpDestroy = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_destroy"));
	m_bpExercise = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_exercise"));

	m_btn_get_record->setTag(em_Record_btn_get_record);
	m_btn_game_record->setTag(em_Record_btn_game_record);
	   
	m_mpBtnCel->setTag(em_ModifyPWCel);
	m_mpBtnSend->setTag(em_ModifyPWSend);
	m_bpDestroy->setTag(em_BackpackDestroy);
	m_bpExercise->setTag(em_BackpackSend);

	m_btn_get_record->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));
	m_btn_game_record->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));

	m_mpBtnCel->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));
	m_mpBtnSend->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));
	m_bpDestroy->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));
	m_bpExercise->addTouchEventListener(this,SEL_TouchEvent(&UserInfoLayer::OnBtnCallBack));


	m_imgMoreInfoBack = dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,"moreinfo_back"));
	m_imgPhoneNumBack = dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,"moreinfo_phone_back"));
	m_imgNickBack = dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,"Image_nick_back"));

	//邮箱没用到干掉
	(dynamic_cast<ImageView*>(Helper::seekWidgetByName(uiWidget,"Image_81_0"))->setVisible(false));
	m_btnEmail->setVisible(false);
	m_imgPhoneNumBack->setVisible(false);




	m_moreinfoEmail->setText("");
	m_UnderWrite->setText("");
	m_UnderWrite->setTouchAreaEnabled(false);
	m_moreinfoEmail->setTouchAreaEnabled(false);

	m_UnderWrite->setTouchSize(cocos2d::Size(290,160));
	m_moreinfoEmail->setTouchSize(cocos2d::Size(213,33));

	m_UnderWrite->setTouchEnabled(false);
	m_moreinfoEmail->setTouchEnabled(false);

// 	m_imgMoreInfoBack->setCascadeColorEnabled(false);
// 	m_imgPhoneNumBack->setCascadeColorEnabled(false);
// 	m_UnderWrite->setCascadeColorEnabled(false);
// 	m_moreinfoEmail->setCascadeColorEnabled(false);

   //没用到，先注释掉
	//PrintfShareLink();
	//Printf2VCode();


	m_btnInfo->setBright(false);
	m_btn_game_record->setBright(false);
	m_btn_bp[0]->setBright(false);
	updateView();
	return true;
}



void UserInfoLayer::GetCocosUI()
{
	m_txt_NickName = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"Label_nickname"));

	m_btnShare = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_share"));
	m_btn2VCode = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"img_2v_code"));
	m_btnCopyLink = dynamic_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btn_CopyLink"));

	m_txtShareLink = dynamic_cast<Text*>(Helper::seekWidgetByName(uiWidget,"txt_ShareLink"));


	m_btnShare->setVisible(false);
	m_btn2VCode->setVisible(false);
	//m_btnCopyLink->setVisible(false);
	m_txtShareLink->setVisible(false);
}




void UserInfoLayer::OnTextFieldCallBack(cocos2d::Ref* sender, cocos2d::ui::TextFiledEventType type)
{
	TextField* mTextField = dynamic_cast<TextField*>(sender);
	int tag = mTextField->getTag();
	switch (type)
	{
	case cocos2d::ui::TEXTFIELD_EVENT_ATTACH_WITH_IME: //开始编辑事件
		{
			if (tag==emUserInfo_phone)
			{
				m_szCinText[0] = "";
				mTextField->setText("");
			}
			else if (tag==emUserInfo_Emai)
			{
				m_szCinText[1] = "";
				mTextField->setText("");
			}
			else if (tag==emUserInfo_underWrite)
			{
				m_szCinText[2] = "";
				mTextField->setText("");
			}
			else if (tag==em_NickNameTF)
			{
				m_tf_Nickname->setText("");
			}
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_DETACH_WITH_IME://如果是结束编辑事件
		{
			std::string strText = mTextField->getStringValue().c_str();
			if (tag==emUserInfo_phone)
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
				m_szCinText[0] = mTextField->getStringValue().c_str();
			}
			else if (tag==emUserInfo_Emai)
			{
				char szMobileNum[129]={0};
				if (strText.length()>128)
					strText[128]=0;
// 				mTextField->ignoreContentAdaptWithSize(false);
// 				((Label*)(mTextField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
// 				mTextField->setContentSize(cocos2d::Size(200,25));
// 				mTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
// 				mTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
				mTextField->setText(strText);
				m_szCinText[1] = mTextField->getStringValue().c_str();
			}
			else if (tag==emUserInfo_underWrite)
			{
				char szMobileNum[33]={0};
				if (strText.length()>32)
					strText[32]=0;
// 				mTextField->ignoreContentAdaptWithSize(false);
// 				((Label*)(mTextField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
// 				mTextField->setContentSize(cocos2d::Size(290,25));
// 				mTextField->setTextHorizontalAlignment(TextHAlignment::LEFT);
// 				mTextField->setTextVerticalAlignment(TextVAlignment::TOP);
				mTextField->setText(strText);
				m_szCinText[2] = mTextField->getStringValue().c_str();
			}
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_INSERT_TEXT://正在输入事件
		//判断是在哪个输入框操作结束	
		{
			if (tag==emModifPW_PW0 || tag==emModifPW_PW1 || tag==emModifPW_PW2)
			{
				m_szCinText[tag-emModifPW_PW0] = mTextField->getStringValue().c_str();
				break;
			}

			std::string strText = mTextField->getStringValue().c_str();
			if (tag==emUserInfo_phone)
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
				m_szCinText[0] = mTextField->getStringValue().c_str();
			}
			else if (tag==emUserInfo_Emai)
			{
				char szMobileNum[34]={0};
				if (strText.length()>33)
					strText[33]=0;
// 				mTextField->ignoreContentAdaptWithSize(false);
// 				((Label*)(mTextField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
// 				mTextField->setContentSize(cocos2d::Size(200,25));
// 				mTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
// 				mTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
				mTextField->setText(strText);
				m_szCinText[1] = mTextField->getStringValue().c_str();
			}
			else if (tag==emUserInfo_underWrite)
			{
				char szMobileNum[33]={0};
				if (strText.length()>32)
					strText[32]=0;
// 				mTextField->ignoreContentAdaptWithSize(false);
// 				((Label*)(mTextField->getVirtualRenderer()))->setLineBreakWithoutSpace(true);
// 				mTextField->setContentSize(cocos2d::Size(250,150));
// 				mTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
// 				mTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
				mTextField->setText(strText);
				m_szCinText[2] = mTextField->getStringValue().c_str();
			}
		}
		break;
	case cocos2d::ui::TEXTFIELD_EVENT_DELETE_BACKWARD://输入的时候删除事件	
		break;
	default:
		break;
	}
}
void UserInfoLayer::SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	m_btnClose->addTouchEventListener(target,selector);
}

void UserInfoLayer::OnUserInfoSel(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	switch (tag)
	{
	case em_UserInfo:
		{
			m_moreInfo->setVisible(true);
			m_ModifyPW->setVisible(false);
			m_backpack->setVisible(false);
			m_record->setVisible(false);

			m_btnInfo->setBright(false);
			m_btnRecord->setBright(true);
			m_btnBackpack->setBright(true);
			m_btnModifyPW->setBright(true);
		}break;
	case em_Backpack:
		{
			m_moreInfo->setVisible(false);
			m_ModifyPW->setVisible(false);
			m_backpack->setVisible(true);
			m_record->setVisible(false);

			m_btnInfo->setBright(true);
			m_btnRecord->setBright(true);
			m_btnBackpack->setBright(false);
			m_btnModifyPW->setBright(true);

			m_btn_bp[0]->setBright(false);
			m_btn_bp[1]->setBright(true);
			m_btn_bp[2]->setBright(true);
			updateBackpack();
			updateBackpack1();
			updateBackpack2();
		}break;
	case em_ModifyPW:
		{
			m_moreInfo->setVisible(false);
			m_ModifyPW->setVisible(true);
			m_backpack->setVisible(false);
			m_record->setVisible(false);

			m_btnInfo->setBright(true);
			m_btnRecord->setBright(true);
			m_btnBackpack->setBright(true);
			m_btnModifyPW->setBright(false);
		}break;
	case em_Record:
		{
			m_moreInfo->setVisible(false);
			m_ModifyPW->setVisible(false);
			m_backpack->setVisible(false);
			m_record->setVisible(true);

			m_btnInfo->setBright(true);
			m_btnRecord->setBright(false);
			m_btnBackpack->setBright(true);
			m_btnModifyPW->setBright(true);

			m_btn_get_record->setBright(true);
			m_btn_game_record->setBright(false);
			//更新记录
			updateRecord();
		}break;
	case em_PerfectInfo:
		{
			m_isUnderWriteEditing = !m_isUnderWriteEditing;
			if (m_isUnderWriteEditing)
			{
				
				m_btnUnderWrite->loadTextureNormal("GG_modify_OK_b.png",TextureResType::PLIST);
				m_btnUnderWrite->loadTexturePressed("GG_modify_OK_d.png", TextureResType::PLIST);


// 				m_btnUnderWrite->setVisible(false);
// 				m_btnModifyOK->setVisible(true);

				m_imgMoreInfoBack->setColor(Color3B::WHITE);

				m_UnderWrite->setColor(Color3B::BLACK);

				m_UnderWrite->setTouchEnabled(true);
			}
			else
			{
				m_btnUnderWrite->loadTextureNormal("GG_user3.png",TextureResType::PLIST);
				m_btnUnderWrite->loadTexturePressed("GG_user1.png", TextureResType::PLIST);

				m_btnUnderWrite->setVisible(true);
				// 			m_btnModifyOK->setVisible(false);

				m_imgMoreInfoBack->setColor(Color3B::BLACK);

				m_UnderWrite->setColor(Color3B::WHITE);

				// 			m_UnderWrite->setTouchAreaEnabled(false);
				// 			m_moreinfoEmail->setTouchAreaEnabled(false);

				m_UnderWrite->setTouchEnabled(false);

				// 			m_szCinText[0] = m_moreinfoPhone->getStringValue().c_str();
				m_szCinText[2] = m_UnderWrite->getStringValue().c_str();
				
				m_isUserInfoModify[2] = true;

				// return LobbyScene::GetInstance()->MessageBox(CGameLobby::GetInstance()->readLanguageString("chage_view")->getCString(),nullptr);
			}
			


		}break;
	case em_ModidySucceed:
		{
		
			break;
		}
		break;
	case em_EmailEdit:
		{
			m_isEmailEditing = !m_isEmailEditing;
			if (m_isEmailEditing)
			{
				m_btnEmail->loadTextureNormal("GG_modify_OK_b.png",TextureResType::PLIST);
				m_btnEmail->loadTexturePressed("GG_modify_OK_d.png", TextureResType::PLIST);

				m_imgPhoneNumBack->setColor(Color3B::WHITE);
				m_moreinfoEmail->setColor(Color3B::BLACK);
				m_moreinfoEmail->setTouchEnabled(true);
			}
			else
			{
				m_btnEmail->loadTextureNormal("GG_user3.png",TextureResType::PLIST);
				m_btnEmail->loadTexturePressed("GG_user1.png", TextureResType::PLIST);

				m_imgPhoneNumBack->setColor(Color3B::BLACK);
				m_moreinfoEmail->setColor(Color3B::WHITE);
				m_moreinfoEmail->setTouchEnabled(false);

				m_isUserInfoModify[1] = true;

			}
		}
		break;
	case em_NickNameEdit:
		{
			m_isNickNameEditing = !m_isNickNameEditing;
			if (m_isNickNameEditing)
			{
				m_btnNickName->loadTextureNormal("GG_modify_OK_b.png",TextureResType::PLIST);
				m_btnNickName->loadTexturePressed("GG_modify_OK_d.png", TextureResType::PLIST);

				m_imgNickBack->setColor(Color3B::WHITE);
				m_tf_Nickname->setColor(Color3B::BLACK);
				m_txt_NickName->setColor(Color3B::BLACK);
				m_tf_Nickname->setTouchEnabled(true);

			}
			else
			{
				m_btnNickName->loadTextureNormal("GG_user3.png",TextureResType::PLIST);
				m_btnNickName->loadTexturePressed("GG_user1.png", TextureResType::PLIST);

				m_imgNickBack->setColor(Color3B::BLACK);
				m_tf_Nickname->setColor(Color3B::WHITE);
				m_txt_NickName->setColor(Color3B::WHITE);
				m_tf_Nickname->setTouchEnabled(false);

				m_szCinText[3] = m_tf_Nickname->getStringValue().c_str();				
			}
		}
		break;
	case em_2V_Code:
		{
			
		}
		break;
	case em_Share:
		{
//#if 1
//			char WebUrl[LEN_NICKNAME*3]={0};
//			TChar2Utf8(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->szAgencyWebURL,WebUrl,sizeof(WebUrl));
//
//			char account[LEN_NICKNAME*3]={0};
//			TChar2Utf8(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->szAccounts,account,sizeof(account));
//
//			char str_shareUrl[255];
//// 			sprintf(str_shareUrl, "%s/?prv=%s", WebUrl, account);
//			sprintf(str_shareUrl, "%s/MobileRegister.aspx?agentName=%s", WebUrl, account);
//
//			char str_shareImg[255];
//			sprintf(str_shareImg, "%s/images/sharelogo.jpg", WebUrl);
//
//			log("sharetext 1 = %s ", CGameLobby::GetInstance()->readLanguageString("ShareText001")->getCString());
//			log("sharetext 2 = %s ", CGameLobby::GetInstance()->readLanguageString("ShareText002")->getCString());
//
//			Dictionary *content = Dictionary::create();
//			content -> setObject(CCString::create(CGameLobby::GetInstance()->readLanguageString("ShareText002")->getCString()), "content");
//			content -> setObject(CCString::create(str_shareImg), "image");
//			content -> setObject(CCString::create(CGameLobby::GetInstance()->readLanguageString("ShareText001")->getCString()), "title");
//			content -> setObject(CCString::create(CGameLobby::GetInstance()->readLanguageString("ShareText001")->getCString()), "description");
//
//			/*content -> setObject(CCString::create("this is a test word"), "content");
//			content -> setObject(CCString::create("http://img0.bdstatic.com/img/image/shouye/systsy-11927417755.jpg"), "image");
//			content -> setObject(CCString::create("title"), "title");
//			content -> setObject(CCString::create("description"), "description");*/
//
//			content -> setObject(CCString::create(str_shareUrl), "url");
//			content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
//			content -> setObject(CCString::create(str_shareUrl), "siteUrl");
//			content -> setObject(CCString::create("ShareSDK"), "site");
//			content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
//			content -> setObject(CCString::create("extInfo"), "extInfo");
//
//			C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, NULL);
//#else
//
//			Dictionary *content = Dictionary::create();
//			content -> setObject(CCString::create("this is a test word"), "content");
//			content -> setObject(CCString::create("http://img0.bdstatic.com/img/image/shouye/systsy-11927417755.jpg"), "image");
//			content -> setObject(CCString::create("测试标题"), "title");
//			content -> setObject(CCString::create("测试描述"), "description");
//			content -> setObject(CCString::create("http://sharesdk.cn"), "url");
//			content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
//			content -> setObject(CCString::create("http://sharesdk.cn"), "siteUrl");
//			content -> setObject(CCString::create("ShareSDK"), "site");
//			content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
//			content -> setObject(CCString::create("extInfo"), "extInfo");
//
//			C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, UserShareResultHandler);
//#endif
		}
		break;
	case em_CopyLink:
		{
			
		}
		break;

	default:
	    break;
	}
}

// void UserShareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
// {
// 	switch (state) {
// 	case C2DXResponseStateSuccess:
// 		log("分享成功");
// 		break;
// 	case C2DXResponseStateFail:
// 		log("分享失败");
// 		break;
// 	default:
// 	    break;
// 	}
// }

void UserInfoLayer::OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	switch (tag)
	{
	case em_ModifyPWClear0:
	case em_ModifyPWClear1:
	case em_ModifyPWClear2:
		{
			m_mpPW[tag-em_ModifyPWClear0]->setText("");
			m_szCinText[tag-em_ModifyPWClear0]="";
		}break;
	case em_Backpack_btn_bp0:
		{
			L_bp[0]->setVisible(true);
			L_bp[1]->setVisible(false);
			L_bp[2]->setVisible(false);

			m_btn_bp[0]->setBright(false);
			m_btn_bp[1]->setBright(true);
			m_btn_bp[2]->setBright(true);
		}
		break;
	case em_Backpack_btn_bp1:
		{
			L_bp[0]->setVisible(false);
			L_bp[1]->setVisible(true);
			L_bp[2]->setVisible(false);

			m_btn_bp[0]->setBright(true);
			m_btn_bp[1]->setBright(false);
			m_btn_bp[2]->setBright(true);
		}
		break;
	case em_Backpack_btn_bp2:
		{
			L_bp[0]->setVisible(false);
			L_bp[1]->setVisible(false);
			L_bp[2]->setVisible(true);

			m_btn_bp[0]->setBright(true);
			m_btn_bp[1]->setBright(true);
			m_btn_bp[2]->setBright(false);
		}break;
	case em_ModifyPWCel:
		{
			m_moreInfo->setVisible(true);
			m_ModifyPW->setVisible(false);
			m_backpack->setVisible(false);
			m_record->setVisible(false);

			m_btnInfo->setBright(false);
			m_btnRecord->setBright(true);
			m_btnBackpack->setBright(true);
			m_btnModifyPW->setBright(true);
		}break;
	case em_ModifyPWSend:
		{
			
		}break;
	case em_BackpackDestroy:
		{
			//CMissionPlaza::GetInstance()->PropertyHandle(3,m_bpIme[m_nPropertyIndex]->getTag());
		}break;
	case em_BackpackSend:
		{
			/*int tag = m_bpIme[m_nPropertyIndex]->getTag();
			if(tag == 30)
			{
			m_tf_Nickname->setEnabled(true);
			m_tf_Nickname->setText("");
			OnUserInfoSel(m_btnInfo,TouchEventType::TOUCH_EVENT_ENDED);
			return;
			}
			else if((tag == 18 || tag == 20) && GameRoom::GetInstance()->GetIGameServerItem()->GetServiceStatus()==ServiceStatus_ServiceIng)
			{
			LobbyScene::GetInstance()->m_chatBox = ChatBox::create(tag);
			LobbyScene::GetInstance()->addChild(LobbyScene::GetInstance()->m_chatBox,98);
			LobbyScene::GetInstance()->m_userInfoLayer->removeFromParent();
			LobbyScene::GetInstance()->m_userInfoLayer = nullptr;
			LobbyScene::GetInstance()->ShowRoomView(3);
			}*/
		}break;

	case em_Record_btn_get_record:
		{
			L_record[0]->setVisible(true);
			L_record[1]->setVisible(false);

			m_btn_get_record->setBright(false);
			m_btn_game_record->setBright(true);
		}break;
	case em_Record_btn_game_record:
		{
			L_record[0]->setVisible(false);
			L_record[1]->setVisible(true);

			m_btn_get_record->setBright(true);
			m_btn_game_record->setBright(false);
		}break;

 	default:
 		break;
 	}
}
void UserInfoLayer::OnPropertyCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();

	m_bpIme[m_nPropertyIndex]->loadTexture("property/property_bg_nor.png");

	for (int i=0;i<m_nPropertyCount;i++)
	{
		if(m_bpIme[i]->getTag() == tag)
		{
			m_nPropertyIndex = i;
			m_bpIme[m_nPropertyIndex]->loadTexture("property/property_bg_sel.png");
			m_bpTxtName->setText("");
			m_bpTxtDescribe->setText("");
			return ;
		}
	}
}
void UserInfoLayer::OnPropertyCallBack1(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();

	m_cfIme[m_nPropertyIndex]->loadTexture("property/property_bg_nor.png");

	for (int i=0;i<m_nPropertyCount;i++)
	{
		
		if(m_cfIme[i]->getTag() == tag)
		{
			m_nPropertyIndex = i;
			m_cfIme[m_nPropertyIndex]->loadTexture("property/property_bg_sel.png");
			m_bpTxtName->setText("");
			m_bpTxtDescribe->setText("");
			return ;
		}
	}
}
void UserInfoLayer::OnPropertyCallBack2(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();

	m_dfIme[m_nPropertyIndex]->loadTexture("property/property_bg_nor.png");

	for (int i=0;i<m_nPropertyCount;i++)
	{
		if(m_dfIme[i]->getTag() == tag)
		{
			m_nPropertyIndex = i;
			m_dfIme[m_nPropertyIndex]->loadTexture("property/property_bg_sel.png");
			m_bpTxtName->setText("");
			m_bpTxtDescribe->setText("");
			return ;
		}
	}
}
void UserInfoLayer::updateBackpack()
{

}
void UserInfoLayer::updateBackpack1()
{
	
}
void UserInfoLayer::updateBackpack2()
{
	
}

void UserInfoLayer::updateRecord()
{
    //更新历史记录
}

void UserInfoLayer::updateView()
{
	
}


void UserInfoLayer::ViewPageSwitch(VIEW_PAGE page)
{
	switch (page)
	{
	case PAGE_USERINFO:
		break;
	case PAGE_MODIFY_PASSWORD:
		break;
	case PAGE_MESSAGE_RECORD:
		break;
	default:
		break;
	}

}


CCDrawNode* UserInfoLayer::DrawQREncode(char* str)
{
	bool bRet = m_QREncode.EncodeData(0, 0, 1, -1, str);

	// 添加生成图像代码, 这边我采用的是CCDrawNode这个类直接绘制

	if(bRet)
	{
		int nSize = 5; 
		int originalSize = m_QREncode.m_nSymbleSize + (QR_MARGIN * 2);
		CCDrawNode *pQRNode = CCDrawNode::create();

		Vec2 pt[6];
		ccColor4F color;

		pt[0] = ccp(0, 0);
		pt[1] = ccp((m_QREncode.m_nSymbleSize + QR_MARGIN * 2)*nSize, (m_QREncode.m_nSymbleSize + QR_MARGIN * 2)*nSize);
		pt[2] = ccp((m_QREncode.m_nSymbleSize + QR_MARGIN * 2)*nSize, 0);

		pt[3] = pt[0];
		pt[4] = ccp(0, (m_QREncode.m_nSymbleSize + QR_MARGIN * 2)*nSize);
		pt[5] = pt[1];
		color = ccc4f(1, 1, 1, 1);
		pQRNode->drawPolygon(pt, 6, color, 0, color);

		for (int i = 0; i < m_QREncode.m_nSymbleSize; ++i)
		{
			for (int j = 0; j < m_QREncode.m_nSymbleSize; ++j)
			{
				pt[0] = ccp((i + QR_MARGIN)*nSize, (j + QR_MARGIN)*nSize);
				pt[1] = ccp(((i + QR_MARGIN) + 1)*nSize, ((j + QR_MARGIN) + 1)*nSize);
				pt[2] = ccp(((i + QR_MARGIN) + 1)*nSize, ((j + QR_MARGIN) + 0)*nSize);

				pt[3] = pt[0];
				pt[4] = ccp(((i + QR_MARGIN) + 0)*nSize, ((j + QR_MARGIN) + 1)*nSize);
				pt[5] = pt[1];
				if (m_QREncode.m_byModuleData[i][j] == 1)
				{
					color = ccc4f(0, 0, 0, 1);
				}
				else
				{
					color = ccc4f(1, 1, 1, 1);
				}
				pQRNode->drawPolygon(pt, 6, color, 0, color);
			}
		}
		pQRNode->setScaleY(-1);
		return pQRNode;
	}
	
	return nullptr;

}



void UserInfoLayer::PrintfShareLink()
{
	

}

void UserInfoLayer::Printf2VCode()
{

}

void UserInfoLayer::onBtnFaceTap(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type !=  TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}}

void UserInfoLayer::cleanUpPasswordEditBox()
{
	m_mpPW[0]->setText("");
	m_mpPW[1]->setText("");
	m_mpPW[2]->setText("");
}

