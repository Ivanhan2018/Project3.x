#include "SGLYPopupQuit.h"
#include "SGLYGameScene.h"
#include "SGLYRes.h"
#include "SGLYDefine.h"
#include "MyNSString.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "VersionControl.h"

SGLYPopupQuit::SGLYPopupQuit()
{
}

SGLYPopupQuit::~SGLYPopupQuit()
{
}

bool SGLYPopupQuit::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseDialog::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void SGLYPopupQuit::initUI()
{
	m_pBG = Sprite::create(SGLYTextureName::sSGLY_PopupQuitBG);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(m_sVSize / 2));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pLabTips = Label::create();
	m_pBG->addChild(m_pLabTips);
	m_pLabTips->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pLabTips->setSystemFontSize(30);
	m_pLabTips->setColor(Color3B::WHITE);
	m_pLabTips->setLineBreakWithoutSpace(true);
	m_pLabTips->setAlignment(TextHAlignment::CENTER);
	m_pLabTips->setWidth(sBGSize.width * 0.82);

	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(SGLY_XML_FILE_NAME);
	string sText = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_QUIT))->getCString();

	m_pLabTips->setString(sText);

	m_pBtnOK = Button::create(
		SGLYTextureName::sSGLY_PopupQuitBtnOKNormal,
		SGLYTextureName::sSGLY_PopupQuitBtnOKPressed,
		"",
		Widget::TextureResType::PLIST);
	m_pBG->addChild(m_pBtnOK);
	m_pBtnOK->addClickEventListener(CC_CALLBACK_1(SGLYPopupQuit::onClickBtnOK, this));
	m_pBtnOK->setPosition(Vec2(sBGSize.width * 0.3, sBGSize.height * 0.3));

	m_pBtnNO = Button::create(
		SGLYTextureName::sSGLY_PopupQuitBtnNONormal,
		SGLYTextureName::sSGLY_PopupQuitBtnNOPressed,
		"",
		Widget::TextureResType::PLIST);
	m_pBG->addChild(m_pBtnNO);
	m_pBtnNO->addClickEventListener(CC_CALLBACK_1(SGLYPopupQuit::onClickBtnNO, this));
	m_pBtnNO->setPosition(Vec2(sBGSize.width * 0.7, sBGSize.height * 0.3));

#ifdef VER_9YI
	m_pLabTips->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.75));
#endif // VER_9YI

#ifdef VER_QIANBO
	m_pLabTips->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.65));
#endif // VER_QIANBO

#ifdef VER_369
	m_pLabTips->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.65));
#endif // VER_QIANBO

}

//	点击确定按钮
void SGLYPopupQuit::onClickBtnOK(Ref* pSender)
{
	MyNSString::toShowInfo();

	SGLYGameScene::getGameScene()->clear();
	EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
	Scene* pScene = Scene::create();
	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	pScene->addChild(layer);

	EntityMgr::instance()->getDispatch()->closeGameSocket();
	EntityMgr::instance()->roomFrame()->reset();
	ConfigMgr::instance()->m_Config.m_isGameRun=false;
	Director::getInstance()->replaceScene(pScene);
}

//	点击取消按钮
void SGLYPopupQuit::onClickBtnNO(Ref* pSender)
{
	closed();
}
