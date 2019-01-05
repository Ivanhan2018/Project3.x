#ifndef __PPC_DIALOG_QUIT_H__
#define __PPC_DIALOG_QUIT_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "ui/CocosGUI.h"
#include "PPCEnum.h"
#include "PPCRes.h"

USING_NS_CC;
using namespace ui;

//	碰碰车——退出弹窗
class PPCDialogQuit: public BaseDialog
{
public:
	PPCDialogQuit();
	~PPCDialogQuit();
	CREATE_FUNC(PPCDialogQuit);
private:
	bool init();
	void initUI();

	//	点击确定按钮
	void onClickBtnOK(Ref* pSender);

	//	点击取消按钮
	void onClickBtnNO(Ref* pSender);

	//	背景
	Sprite* m_pBG;

	//	提示语
	Label* m_pLabTips;

	//	按钮——确定
	Button* m_pBtnOK;

	//	按钮——取消
	Button* m_pBtnNO;

};


#endif  // __PPC_DIALOG_QUIT_H__