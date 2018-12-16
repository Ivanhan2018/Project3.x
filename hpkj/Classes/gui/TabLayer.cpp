#include "TabLayer.h"
#include "RoomLayer.h"
#include "GroupSprite.h"
#include "TaskInfo.h"
#include "PromptBox.h"
#include "MovingLabelLayer.h"
#include "SceneControl.h"
#include "CocosUnits.h"


#define minInWarning(a,b) (((a) < (b)) ? (a) : (b))

using namespace cocos2d;
TabLayer::TabLayer()
{
	currentItem = 0;
	itemGap = 10;
	tabMenuWidth = 0;
	normalName = NULL;
	selectedName = NULL;
	//tabMenuItem = NULL;
}

TabLayer::~TabLayer()
{
	if(normalName!=NULL)
	{
		normalName->release();
		normalName = NULL;
	}
	if(selectedName!=NULL)
	{
		selectedName->release();
		selectedName = NULL;
	}
}

TabLayer *TabLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	TabLayer *tabLayer = new TabLayer();
	if (tabLayer && tabLayer->initWithTabItems(itemNormalName, itemSelectedName))
	{
		tabLayer->autorelease();
		return tabLayer;
	}
	else
	{
		CC_SAFE_DELETE(tabLayer);
		return NULL;
	}
}

bool TabLayer::initWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	bool bRet = false;
	do 
	{
		if(itemNormalName==NULL||itemSelectedName==NULL)
		{
			return false;
		}
		if(itemNormalName->count()!=itemSelectedName->count())
		{
			return false;
		}
		normalName = Array::createWithArray(itemNormalName);
		selectedName = Array::createWithArray(itemSelectedName);
		normalName->retain();
		selectedName->retain();

		cocos2d::Size winSize = WINSIZE;
		/**���ư�͸���ɰ�**/
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);

		/**���Ʊ���ͼƬ**/
		pBJ  = Sprite::create("bg2.png");
		pBJ->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBJ,0);

		maskSprite = Sprite::createWithSpriteFrame(spriteFrame("bg2_2.png"));
		maskSprite->setPosition(Vec2(winSize.width/2,winSize.height-maskSprite->getContentSize().height/2));
		//this->addChild(maskSprite,2);

		//tab�˵� ÿ������ �������� ѫ��
		tabNum = itemNormalName->count();
		tabMenuWidth = (tabNum+1)*itemGap;

		for(int i = 0 ; i < tabNum; i++)
		{
			if(i == currentItem)
			{
				String *imageName = (String *)selectedName->objectAtIndex(i);
				string str = imageName->getCString();
				Sprite *sprite = Sprite::createWithSpriteFrame(spriteFrame(imageName->getCString()));
				tabMenuWidth+=sprite->getContentSize().width;
				MenuItemSprite *menuItem = MenuItemSprite::create(sprite,sprite,CC_CALLBACK_1(TabLayer::pressTabButton,this));
				menuItem->setTag(i+TabLayerBaseTag);
				tabMenuItem.pushBack(menuItem);
			}else{
				String *imageName = (String *)normalName->objectAtIndex(i);
				Sprite *sprite = Sprite::createWithSpriteFrame(spriteFrame(imageName->getCString()));
				tabMenuWidth+=sprite->getContentSize().width;
				MenuItemSprite *menuItem = MenuItemSprite::create(sprite,sprite,CC_CALLBACK_1(TabLayer::pressTabButton,this));
				menuItem->setTag(i+TabLayerBaseTag);
				tabMenuItem.pushBack(menuItem);
			}
		}
		tabMenu = Menu::createWithArray(tabMenuItem);
		tabMenu->setAnchorPoint(Vec2(0.5,0.5));
		tabMenu->setPosition(Vec2(tabMenuWidth/2,winSize.height*0.9f-2));
		tabMenu->alignItemsHorizontallyWithPadding(itemGap);//�������з�ʽ��������20
		this->addChild(tabMenu, 2);

		/**�رհ�ť**/
		Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pButtonClose = MenuItemSprite::create
			(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(TabLayer::dismiss,this));
		pbuttonSelectClose->setScale(0.9f);
		pbuttonSelectClose->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pButton0 = Menu::create(pButtonClose,NULL);
		pButton0->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93)); 
		this->addChild(pButton0,2);

		bRet = true;
		this->setKeypadEnabled(true);
	} while (0);

	return bRet;

}

void TabLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		//RoomLayer* layer = (RoomLayer*)this->getParent();
		//layer->permitButtonClick();
		//this->removeFromParentAndCleanup(true);
		playButtonSound();
		Scene *pScene = Scene::create();
		RoomLayer *pRoomLayer = RoomLayer::create();
		pRoomLayer->automaticLogin(false);
		pRoomLayer->closeLoginAward();
		pScene->addChild(pRoomLayer);
		Director::getInstance()->replaceScene(pScene);
	}
}

void TabLayer::dismiss(Object *obj){
	//playButtonSound();
	//RoomLayer* layer = (RoomLayer*)this->getParent();
	//layer->permitButtonClick();
	//this->removeFromParentAndCleanup(true);
	doSomethingBeforeClosed();
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void TabLayer::pressTabButton( Object* obj)
{
	int itemTag = ((Node *)obj)->getTag();
	if(currentItem == itemTag)
	{
		return;
	}
	currentItem = itemTag;
	for(int i = 0 ; i < tabNum; i++)
	{
		MenuItemSprite *menuItem = (MenuItemSprite *)tabMenu->getChildByTag(TabLayerBaseTag+i);
		if(TabLayerBaseTag+i == currentItem)
		{
			String *imageName = (String *)selectedName->objectAtIndex(i);
			menuItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame(imageName->getCString())));
			menuItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame(imageName->getCString())));
		}else{
			String *imageName = (String *)normalName->objectAtIndex(i);
			menuItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame(imageName->getCString())));
			menuItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame(imageName->getCString())));
		}
	}
	doSomething();
}

void TabLayer::doSomething()
{
	CCLOG("TabLayer::doSomething");
}
