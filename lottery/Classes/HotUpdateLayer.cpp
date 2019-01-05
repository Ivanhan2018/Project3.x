#include "HotUpdateLayer.h"
#include "GBEventIDs.h"
#include "define.h"
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "MyConfig.h"//by hxh
#include "MyNSString.h"//by hxh
#include "VersionControl.h"

USING_NS_CC;
USING_NS_CC_EXT;

HotUpdateLayer* HotUpdateLayer::create(int idx,int serverId)
{
	HotUpdateLayer *pRet = new HotUpdateLayer();
	if (pRet && pRet->init(idx,serverId))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

HotUpdateLayer::HotUpdateLayer(void):m_pProgressLabel(NULL),m_idx(-1),m_serverId(-1),faild_down_count(0)
{

}

HotUpdateLayer::~HotUpdateLayer(void)
{
	if(_assets_manager_ex)
	_assets_manager_ex->release();
}

bool HotUpdateLayer::init(int idx,int serverId)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	faild_down_count = 0;
	m_idx=idx;
	m_serverId=serverId;

    CreatePopWndDown();
	//this->setVisible(false);

	MyConfig& myConfig=MyConfig::Instance();
	int platform=(myConfig.m_platform>=0 && myConfig.m_platform<MAX_PLATFORM_NUM?myConfig.m_platform:0);
	std::string strManiFestFile =  FileUtils::getInstance()->getWritablePath()+myConfig.m_storagePath[platform][idx];
	_assets_manager_ex = AssetsManagerEx::create(myConfig.m_manifestUrl[platform][idx], strManiFestFile);

	//_assets_manager_ex = AssetsManagerEx::create("fishproject.manifest", FileUtils::getInstance()->getWritablePath() + "fishDownLoad");
    _assets_manager_ex->retain();
    
    if (!_assets_manager_ex->getLocalManifest()->isLoaded())
    {
        onLoadSuccess();
    }
    else
    {
        _assets_manager_listener = cocos2d::extension::EventListenerAssetsManagerEx::create(_assets_manager_ex, [this](EventAssetsManagerEx * event){
            std::string EventMessage[] = { "ERROR_NO_LOCAL_MANIFEST","ERROR_DOWNLOAD_MANIFEST","ERROR_PARSE_MANIFEST","NEW_VERSION_FOUND","ALREADY_UP_TO_DATE","UPDATE_PROGRESSION","ASSET_UPDATED","ERROR_UPDATING","UPDATE_FINISHED","UPDATE_FAILED","ERROR_DECOMPRESS" };
            CCLOG("EventListenerAssetsManagerEx %s", EventMessage[(int)event->getEventCode()].c_str());
            
            switch (event->getEventCode()) {
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
                case cocos2d::extension::EventAssetsManagerEx::EventCode::UPDATE_FAILED:
                {
                    this->onLoadError((int)event->getEventCode());
                    
                    CCLOG("%s", event->getMessage().c_str());
                    
                    break;
                }
                    
                    
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_UPDATING:
                {
                    ++faild_down_count;
                    
                    tryDownloadFaildAssets();
                    
                    break;
                }
                    
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ASSET_UPDATED:
                {
                    tryDownloadFaildAssets();
                    
                    break;
                }
                    
                case cocos2d::extension::EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                {
                    //CCLOG("已经是最新版本，直接进入主界面");
                    this->onAllFileIsNew();
                    break;
                }
                    
                case cocos2d::extension::EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
                {
                    CCLOG("更新完成重新加载");
                    this->onLoadSuccess();
                    break;
                }
                    
                case cocos2d::extension::EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
                {
                    //this->onLoadPercent(event->getPercent());
                    this->onLoadPercent(event->getPercentByFile());
                    break;
                }
                    
                case cocos2d::extension::EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND:
                {
                    //CCLOG("发现新本版开始升级");
                    break;
                }
                    
                default:
                    break;
            }
        });
        
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_assets_manager_listener, 1);
        _assets_manager_ex->update();
    }
    
    
    return true;
}

void HotUpdateLayer::CreatePopWndDown()
{
	MyConfig& myConfig=MyConfig::Instance();
	
	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	LayerColor* colorLayer = LayerColor::create(ccc4(166, 166, 166, 100), winSize.width, winSize.height);
	this->addChild(colorLayer);
    //LayerColor * white_bg = LayerColor::create(Color4B::WHITE);
    //this->addChild(white_bg);

	//禁掉点击事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){return true;};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, colorLayer);

	//禁掉返回键
	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){
		event->stopPropagation();
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, colorLayer);
    
    _loading_spr = cocos2d::Sprite::create("loading.png");
	_loading_spr->setPosition(Vec2(myConfig.m_loadingPos[0], myConfig.m_loadingPos[1]));
    _loading_spr->runAction(RepeatForever::create(RotateBy::create(0.1f, 30)));
    addChild(_loading_spr);
   
    _loading_progress_bg = cocos2d::Sprite::create("98.png");
    _loading_progress_bg->setPosition(Vec2(myConfig.m_loadingProgressBgPos[0], myConfig.m_loadingProgressBgPos[1]));
    this->addChild(_loading_progress_bg);
    
    _loading_progress = ProgressTimer::create(Sprite::create("97.png"));
    _loading_progress->setPosition(Vec2(myConfig.m_loadingProgressPos[0], myConfig.m_loadingProgressPos[1]));
    _loading_progress->setType(ProgressTimer::Type::BAR);
    _loading_progress->setMidpoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_loading_progress);
    
    _loading_progress->setPercentage(0);

	m_pProgressLabel=LabelTTF::create("", "", 30);
	m_pProgressLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pProgressLabel->setFontFillColor(ccc3(100,100,100));
	m_pProgressLabel->setPosition(Vec2(myConfig.m_ProgressLabelPos[0], myConfig.m_ProgressLabelPos[1]));
	m_pProgressLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	this->addChild(m_pProgressLabel);
    
    _sprite_loading_success = cocos2d::Sprite::create("success.png");
    _sprite_loading_success->setPosition(Vec2(myConfig.m_loadingSuccessPos[0], myConfig.m_loadingSuccessPos[1]));
	_sprite_loading_success->setScale(myConfig.m_loadingSuccessScale);
    _sprite_loading_success->setVisible(false);
    this->addChild(_sprite_loading_success);
    
    _sprite_loading_error = cocos2d::Sprite::create("error.png");
    _sprite_loading_error->setPosition(Vec2(myConfig.m_loadingErrorPos[0], myConfig.m_loadingErrorPos[1]));
	_sprite_loading_error->setScale(myConfig.m_loadingErrorScale);
    _sprite_loading_error->setVisible(false);
    this->addChild(_sprite_loading_error);

	//背景图
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame("beting46.png"));
	bk1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bk1->setPosition(Vec2(myConfig.m_bkPos[0], myConfig.m_bkPos[1]));
	//bk1->setScaleY(1.5f);
	colorLayer->addChild(bk1,1);

	//系统提示
	LabelTTF* ltfTitle = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t1100"), "", 42);
	ltfTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ltfTitle->setPosition(Vec2(myConfig.m_ltfTitlePos[0], myConfig.m_ltfTitlePos[1]));
	ltfTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ltfTitle->setFontFillColor(ccc3(255,255,255));
	bk1->addChild(ltfTitle);

	//前图
	Sprite *sptTips = Sprite::createWithSpriteFrame(spriteFrame("setting_07_2.png"));
	sptTips->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptTips->setPosition(Vec2(myConfig.m_sptTipsPos[0], myConfig.m_sptTipsPos[1]));
	bk1->addChild(sptTips);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("my_cancel.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("my_cancel_2.png"));
	//取消
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create(pCloseNormalButton,pCloseSelectButton,[=](Ref* ){
		//停止监听
		Director::getInstance()->getEventDispatcher()->removeEventListener(_assets_manager_listener);
		//停止下载
		//关闭窗口
		removeFromParent();
	});
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->alignItemsHorizontallyWithPadding(100);
	pCloseButton->setPosition(Vec2(myConfig.m_CloseDownPos[0], myConfig.m_CloseDownPos[1]));
	this->addChild(pCloseButton,1);
}

void HotUpdateLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HotUpdateLayer::onLoadPercent(float percent)
{
	if(_loading_spr == nullptr || _loading_progress == nullptr || _loading_progress_bg == nullptr || _sprite_loading_success == nullptr || _sprite_loading_error == nullptr) return;  //直接退出就行了  有可能进度条没结束，界面已取消

	if(percent>=0.0f)
		this->setVisible(true);
	Node * show_nodes[] = {_loading_spr, _loading_progress, _loading_progress_bg};
    int show_count = sizeof(show_nodes) / sizeof(Node *);
    for (int i = 0; i < show_count; ++i) {
		if(show_nodes[i] != nullptr)
        show_nodes[i]->setVisible(true);
    }
    
    Node * hide_nodes[] = {_sprite_loading_success, _sprite_loading_error};
    int hide_count = sizeof(hide_nodes) / sizeof(Node *);
    for (int i = 0; i < hide_count; ++i) {
		if(show_nodes[i] != nullptr)
        hide_nodes[i]->setVisible(false);
    }
    
    this->_loading_progress->setPercentage(percent);

	char progress[80];
	memset( progress, '\0', sizeof(progress) );
	snprintf(progress, sizeof(progress), "资源文件更新中 %d%%", (int)percent);	
	CCLOG("percent=%f %s",percent,progress);
	string str=progress;
	MyNSString::GBKToUTF8(str);
	m_pProgressLabel->setString(str);
}


void HotUpdateLayer::onAllFileIsNew()
{
    CCLOG("HotUpdateLayer::onAllFileIsNew");
    onLoadSuccess();
}

void HotUpdateLayer::onLoadSuccess()
{
    CCLOG("HotUpdateLayer::onLoadSuccess");
	if(m_pProgressLabel == 0)
		return;
  	m_pProgressLabel->setString("download success.");

    Node * nodes[] = {_loading_spr, _loading_progress, _loading_progress_bg, _sprite_loading_success, _sprite_loading_error};
    int count = sizeof(nodes) / sizeof(Node *);
    for (int i = 0; i < count; ++i) {
        nodes[i]->setVisible(false);
    }
    
    _sprite_loading_success->setVisible(true);

	//this->schedule([=](float delta){//iOS下会崩??
		//this->unschedule("onLoadSuccess");
		OnStartConnectGameServer();
	//},1.0f,"onLoadSuccess");
}

void HotUpdateLayer::onLoadError(int eventCode)
{
    CCLOG("HotUpdateLayer::onLoadError");

	// 临时添加
	//if(m_idx==4 && eventCode==(int)EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST)
	//{
	//	OnStartConnectGameServer();    
	//	return;
	//}
	this->setVisible(true);
    
    Node * nodes[] = {_loading_spr, _loading_progress, _loading_progress_bg, _sprite_loading_success, _sprite_loading_error};
    int count = sizeof(nodes) / sizeof(Node *);
    for (int i = 0; i < count; ++i) {
		if(nodes[i] != nullptr)
        nodes[i]->setVisible(false);
    }
    
    _sprite_loading_error->setVisible(true);

	//this->schedule([=](float delta){
		//this->unschedule("onLoadError");
		this->setVisible(false);
		removeFromParent();
	//},1.0f,"onLoadError");
}

void HotUpdateLayer::tryDownloadFaildAssets()
{
    //if (_assets_manager_ex->getFailedAssets().size() > 0) {
    //    CCLOG("%d", (int)_assets_manager_ex->getState());
    //}
    _assets_manager_ex->downloadFailedAssets();
}

void HotUpdateLayer::OnStartConnectGameServer()
{
	// 下载完毕不可见
	this->setVisible(false);
	// 更新完毕切换场景
#ifdef USE_ROOM_LIST
	__Array *arr=__Array::create();
	arr->addObject(Integer::create(m_idx));
	arr->addObject(Integer::create(m_serverId));
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_TOCONNECTGAMEROOMSERVER,arr);
#else
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_TOCONNECTGAMEROOMSERVER,Integer::create(m_idx));
#endif
	removeFromParent();
}
