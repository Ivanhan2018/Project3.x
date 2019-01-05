#ifndef _USERINFO_LAYER_H_
#define _USERINFO_LAYER_H_

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
//
#include "QR_Encode.h"

#define MAX_FACEID				20

enum UserInfoBTN
{
	em_UserInfo=0,
	em_PerfectInfo,
	em_EmailEdit,
	em_NickNameEdit,

	em_ModidySucceed,
	em_Share,
	em_2V_Code,
	em_CopyLink,


	em_Record=20,

	em_Record_btn_get_record,
	em_Record_btn_game_record,

	em_Backpack=40,
	em_BackpackDestroy,
	em_BackpackSend,
	em_Backpack_btn_bp0,
	em_Backpack_btn_bp1,
	em_Backpack_btn_bp2,



	em_ModifyPW=60,
	em_ModifyPWClear0,
	em_ModifyPWClear1,
	em_ModifyPWClear2,
	em_ModifyPWCel,
	em_ModifyPWSend
};
enum UserInfoCinTag
{
	emUserInfo_ModifyNickName,
	emUserInfo_phone,
	emUserInfo_Emai,
	emUserInfo_underWrite,
	em_NickNameTF,


	emModifPW_PW0,
	emModifPW_PW1,
	emModifPW_PW2,
};

enum VIEW_PAGE
{
	PAGE_USERINFO,
	PAGE_MODIFY_PASSWORD,
	PAGE_MESSAGE_RECORD,
	PAGE_SAFE_CENTER_BIND,
	PAGE_SAFE_CENTER_RELEASE,

};
class UserInfoLayer :public cocos2d::Layer
{
private:
	cocos2d::ui::Widget*			uiWidget;
	cocos2d::ui::Layout*			m_Touchbg;
	cocos2d::ui::Layout*			m_moreInfo;
	cocos2d::ui::Layout*			m_ModifyPW;
	cocos2d::ui::Layout*			m_backpack;
	cocos2d::ui::Layout*			m_record;
	cocos2d::ui::Layout*            L_bp[3];
	cocos2d::ui::Layout*            L_record[2];



	cocos2d::ui::Button*			m_btnInfo;
	cocos2d::ui::Button*			m_btnRecord;
	cocos2d::ui::Button*			m_btnBackpack;
	cocos2d::ui::Button*			m_btnModifyPW;
	cocos2d::ui::Button*            m_btnClose;

	cocos2d::ui::Text*				m_txt_NickName;
	cocos2d::ui::TextField*			m_tf_Nickname;				//昵称
	cocos2d::ui::Text*				m_txtGameID;				//游戏ID
	cocos2d::ui::Text*				m_txtAccount;				//游戏帐号

	cocos2d::ui::Text*		        m_txtScore;					//金币
	cocos2d::ui::Text*			m_moreinfoPhone;			//输入
	cocos2d::ui::TextField*			m_moreinfoEmail;			//输入
	cocos2d::ui::TextField*			m_UnderWrite;				//输入
	cocos2d::ui::Button*			m_btnUnderWrite;		//修改信息
	cocos2d::ui::Button*			m_btnEmail;		//修改信息
	cocos2d::ui::Button*			m_btnNickName;		//修改信息
	cocos2d::ui::Button*			m_btnShare;		//分享
	cocos2d::ui::Button*			m_btn2VCode;		//2围马
	cocos2d::ui::Button*			m_btnCopyLink;		//拷贝
	cocos2d::ui::Text*				m_txtShareLink;		//分享链接
	cocos2d::ui::Button*			m_btnModifyOK;			//修改成功

	cocos2d::ui::TextField*			m_mpPW[3];
	cocos2d::ui::Button*			m_mpBtnClear[3];
	cocos2d::ui::Button*			m_mpBtnCel;
	cocos2d::ui::Button*			m_mpBtnSend;
	

	cocos2d::ui::Text*				m_bpTxtName;
	cocos2d::ui::Text*				m_bpTxtDescribe;
	cocos2d::ui::Button*			m_bpDestroy;
	cocos2d::ui::Button*			m_bpExercise;
	cocos2d::ui::ImageView*			m_bpIme[2*5];
	cocos2d::ui::ImageView*			m_cfIme[2*5];
	cocos2d::ui::ImageView*			m_dfIme[2*5];
	cocos2d::ui::Button*            m_btn_bp[3];

	cocos2d::ui::Button*            m_btn_get_record;
	cocos2d::ui::Button*            m_btn_game_record;
	cocos2d::ui::ListView*          ListView0;
	cocos2d::ui::ListView*          ListView1;

	cocos2d::ui::ImageView*			m_imgMoreInfoBack;//个性签名
	cocos2d::ui::ImageView*			m_imgPhoneNumBack;//邮箱
	cocos2d::ui::ImageView*			m_imgNickBack;//昵称背景


	std::int16_t					m_nPropertyCount;
	std::int16_t					m_nPropertyIndex;
	bool							m_isEmailEditing;
	bool							m_isNickNameEditing;
	bool							m_isUnderWriteEditing;

	CC_SYNTHESIZE_READONLY(	cocos2d::ui::ImageView*, m_faceID, FaceID)
public:
	//0123,
	//手机，邮箱，签名，昵称
	const char*						m_szCinText[4];
	const char*						m_szVerifyCode;
	bool							m_isUserInfoModify[4];
	std::int16_t					m_wFaceID;
// 
// 	const char*						m_srcPW;
// 	const char*						m_newPW;
// 	const char*						m_confrimPW;

	int							m_VerifyCodeLastTime;

private:
	CQR_Encode m_QREncode;

public:
	UserInfoLayer(void);
	~UserInfoLayer(void);
	virtual bool init();
	CREATE_FUNC(UserInfoLayer);
	void OnTextFieldCallBack(cocos2d::Ref* sender, cocos2d::ui::TextFiledEventType type);
	void SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector);
	void OnUserInfoSel(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	//头像修改回调
	void OnPropertyCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void OnPropertyCallBack1(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void OnPropertyCallBack2(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void updateBackpack();
	void updateBackpack1();
	void updateBackpack2();

	void updateRecord();
	void updateView();
	void cleanUpPasswordEditBox();
private:
	void GetCocosUI();//获得ui

	void PrintfShareLink();//显示分享链接
	void Printf2VCode();//生成二维码
	void ViewPageSwitch(VIEW_PAGE page);//切换视图
	cocos2d::DrawNode* DrawQREncode(char* str);
	void onBtnFaceTap(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);//点击头像
};

#endif // _USERINFO_LAYER_H_
