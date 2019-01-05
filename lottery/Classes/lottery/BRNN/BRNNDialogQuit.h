#ifndef __BRNN_DIALOG_QUIT_H__
#define __BRNN_DIALOG_QUIT_H__

#include "cocos2d.h"
#include "BaseDialog.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

/*
**	@brief	百人牛牛——退出弹窗
*/
class BRNNDialogQuit: public BaseDialog
{
public:
	BRNNDialogQuit();
	~BRNNDialogQuit();
	CREATE_FUNC(BRNNDialogQuit);
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


#endif  // __BRNN_DIALOG_QUIT_H__