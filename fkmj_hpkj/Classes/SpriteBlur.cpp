#include "SpriteBlur.h"
#include "ScriptData.h"

USING_NS_CC;

namespace cocos2d
{
	SpriteBlur::SpriteBlur()
		:m_pNode(0)
	{

	}
	SpriteBlur::~SpriteBlur()
	{
	}

	SpriteBlur* SpriteBlur::create()
	{
		SpriteBlur* pRet = new SpriteBlur();
		if (pRet && pRet->init())
		{
			pRet->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(pRet);
		}

		return pRet;
	}

	void SpriteBlur::setTexture(Texture2D *texture)
	{
		Sprite::setTexture(texture);
		if (!texture)
		{
			return;
		}
		m_pNode = cocos2d::Node::create();
		m_pNode->setCascadeOpacityEnabled(true);
		addChild(m_pNode);
		float fWidth = texture->getPixelsWide();
		float fHeight = texture->getPixelsHigh();
		static ScriptData<int> s_iTest1("SpriteOpacity");
		static ScriptData<int> s_iTest2("SpriteCout");
		static ScriptData<int> s_iTest3("SpriteWidth1");
		static ScriptData<int> s_iTest4("SpriteWidth2");
		for (int i = 0;i<s_iTest2.Value() && texture;i++)
		{
			Sprite* pSprite = Sprite::createWithTexture(texture);
			cocos2d::Vec2 kTemPos = Point(float(rand()%100-50)/50.f*(float)s_iTest3.Value()
				,float(rand()%100-50)/50.f*float(s_iTest3.Value()));

			pSprite->setPosition(fWidth/2+kTemPos.x
				,fHeight/2+kTemPos.y);
			pSprite->setOpacity(s_iTest1.Value()+(1.0f - kTemPos.getLength()/100.0f)*s_iTest4.Value());
			pSprite->setFlippedY(true);
			m_pNode->addChild(pSprite);
		}
		Sprite* pSprite = Sprite::createWithTexture(texture);
		pSprite->setOpacity(s_iTest1.Value()+s_iTest4.Value());
		pSprite->setPosition(fWidth/2
			,fHeight/2);
		pSprite->setFlippedY(true);
		m_pNode->addChild(pSprite);
		
	}
	float SpriteBlur::getBlurRadius()
	{
		return (float)m_pNode->getOpacity()/255.0f;
	}
	void SpriteBlur::setBlurRadius(float radius)
	{
		int iOpacity = radius*255;
		m_pNode->setOpacity(iOpacity);
		m_pNode->setVisible(iOpacity > 2);
	}
	void SpriteBlur::SortPos()
	{
		float fWidth = getTexture()->getPixelsWide();
		float fHeight = getTexture()->getPixelsHigh();
		static ScriptData<int> s_iTest1("SpriteOpacity");
		static ScriptData<int> s_iTest2("SpriteCout");
		static ScriptData<int> s_iTest3("SpriteWidth1");
		static ScriptData<int> s_iTest4("SpriteWidth2");
		for (int i = 0;i<m_pNode->getChildrenCount();i++)
		{
			cocos2d::Vec2 kTemPos = Point(float(rand()%100-50)/50.f*(float)s_iTest3.Value()
				,float(rand()%100-50)/50.f*float(s_iTest3.Value()));
			Vector<Node*>& kChild = m_pNode->getChildren();
			kChild.at(i)->setPosition(fWidth/2+kTemPos.x
				,fHeight/2+kTemPos.y);
			kChild.at(i)->setOpacity(s_iTest1.Value()+(1.0f - kTemPos.getLength()/100.0f)*s_iTest4.Value());
		}
		
	}
}
