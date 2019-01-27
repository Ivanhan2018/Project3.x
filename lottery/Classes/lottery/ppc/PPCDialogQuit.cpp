#include "PPCDialogQuit.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "RoomLayer.h"
#include "PPCSound.h"

PPCDialogQuit::PPCDialogQuit()
{
}

PPCDialogQuit::~PPCDialogQuit()
{
}

bool PPCDialogQuit::init()
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

void PPCDialogQuit::initUI()
{
	m_pBG = Sprite::create(PPCTextureName::s_ppc_popup_quit_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(m_sVSize / 2));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pLabTips = Label::create();
	m_pBG->addChild(m_pLabTips);
	m_pLabTips->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pLabTips->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.58));
	m_pLabTips->setSystemFontSize(25);
	m_pLabTips->setColor(Color3B::WHITE);
	m_pLabTips->setLineBreakWithoutSpace(true);
	m_pLabTips->setAlignment(TextHAlignment::CENTER);
	m_pLabTips->setWidth(sBGSize.width * 0.85);
	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(PPC_XML_FILE_NAME);
	string sText = ((CCString*)pDictionary->objectForKey(PPC_XML_KEY_QUIT))->getCString();
	m_pLabTips->setString(sText);

	m_pBtnOK = Button::create(
		PPCTextureName::s_ppc_btn_ok_normal,
		PPCTextureName::s_ppc_btn_ok_selected,
		"",
		Widget::TextureResType::PLIST);
	m_pBG->addChild(m_pBtnOK);
	m_pBtnOK->addClickEventListener(CC_CALLBACK_1(PPCDialogQuit::onClickBtnOK, this));
	m_pBtnOK->setPosition(Vec2(sBGSize.width * 0.3, sBGSize.height * 0.3));

	m_pBtnNO = Button::create(
		PPCTextureName::s_ppc_btn_no_normal,
		PPCTextureName::s_ppc_btn_no_selected,
		"",
		Widget::TextureResType::PLIST);
	m_pBG->addChild(m_pBtnNO);
	m_pBtnNO->addClickEventListener(CC_CALLBACK_1(PPCDialogQuit::onClickBtnNO, this));
	m_pBtnNO->setPosition(Vec2(sBGSize.width * 0.7, sBGSize.height * 0.3));

	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){	

		if(keycode == EventKeyboard::KeyCode::KEY_BACK)
		{
			event->stopPropagation();
			closed();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, this);
}

//	点击确定按钮
void PPCDialogQuit::onClickBtnOK(Ref* pSender)
{
	PPCSound::getInstance()->StopMusic();
	PPCSound::getInstance()->StopEffect();
	EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
// 	this->scheduleOnce([](float dt){
		MyNSString::toPortrait();
		Scene* pScene = Scene::create();
		RoomLayer* layer = RoomLayer::create();
		layer->automaticLogin(false);
		pScene->addChild(layer);

		EntityMgr::instance()->getDispatch()->closeGameSocket();
		EntityMgr::instance()->roomFrame()->reset();
		ConfigMgr::instance()->m_Config.m_isGameRun=false;
		Director::getInstance()->replaceScene(pScene);
// 	}, 0.1f, "PPCLeaveGame");
}

//	点击取消按钮
void PPCDialogQuit::onClickBtnNO(Ref* pSender)
{
	closed();
}
