#pragma once
#include "cocos2d.h"

class NotCheckSprite : public cocos2d::Sprite
{
public:
	static NotCheckSprite* create(const std::string& filename)
	{
		NotCheckSprite *sprite = new (std::nothrow) NotCheckSprite();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override
	{
		//_quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
		//renderer->addCommand(&_quadCommand);
	}
};

class OutLineEffect : public cocos2d::Ref
{
public:
	CREATE_FUNC(OutLineEffect);

	bool init()
	{
		initGLProgramState("shader/maskSprite.fsh");

		return true;
	}
public:
	cocos2d::GLProgram* getGLProgram() const { return _glprogramstate; }

public:
	bool initGLProgramState(const std::string &fragmentFilename);
	GLint getWidthLocation();
	GLint getMaskLocation();
public:
	OutLineEffect();
	virtual ~OutLineEffect();
	cocos2d::GLProgram* _glprogramstate;
	GLint m_kWidthLocation;
	GLint m_kMaskLocation;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	std::string _fragSource;
	cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

class MaskSprite:
	public cocos2d::Sprite
{
public:
	MaskSprite();
	~MaskSprite();
public:
	static MaskSprite* create(std::string kFileName);
public:
	bool initProgram();
	void useMaskTexture(std::string kName);
	void setPixWidth(float fWidth);
	void setPixWidth(cocos2d::Vec2 fValue);
public:
	cocos2d::Texture2D* m_pMaskTexture;
	cocos2d::Vec2 m_kWidthValue;
	GLint m_kWidthLocation;
	GLint m_kMaskLocation;

	static OutLineEffect*	s_pProgramState;
};
