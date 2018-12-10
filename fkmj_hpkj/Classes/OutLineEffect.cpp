#include "OutLineEffect.h"
#include "ScriptData.h"

USING_NS_CC;

OutLineEffect*	MaskSprite::s_pProgramState = NULL;


bool OutLineEffect::initGLProgramState(const std::string &fragmentFilename)
{
	auto fileUtiles = FileUtils::getInstance();
	auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
	_glprogramstate = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	_fragSource = fragSource;
#endif

	m_kWidthLocation = glGetUniformLocation(getGLProgram()->getProgram(),"MinValue");
	m_kMaskLocation = glGetUniformLocation(getGLProgram()->getProgram(),"u_texture1");
	return _glprogramstate != nullptr;
}
GLint OutLineEffect::getWidthLocation()
{
	return m_kWidthLocation;
}
GLint OutLineEffect::getMaskLocation()
{
	return m_kMaskLocation;
}

OutLineEffect::OutLineEffect()
	: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	_backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
		[this](EventCustom*)
	{
		auto glProgram = _glprogramstate;
		glProgram->reset();
		glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
		glProgram->link();
		glProgram->updateUniforms();

		m_kWidthLocation = glGetUniformLocation(getGLProgram()->getProgram(),"MinValue");
		m_kMaskLocation = glGetUniformLocation(getGLProgram()->getProgram(),"u_texture1");
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

OutLineEffect::~OutLineEffect()
{
	CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}


MaskSprite::MaskSprite()
	:m_pMaskTexture(NULL)
	,m_kWidthLocation(0)
	,m_kMaskLocation(0)
{

}
MaskSprite::~MaskSprite()
{
	if (s_pProgramState)
	{
		s_pProgramState->release();
	}
}
MaskSprite* MaskSprite::create(std::string kFileName)
{
	auto ret = new (std::nothrow) MaskSprite;
	if(ret && ret->init() && ret->initWithFile(kFileName)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}
bool MaskSprite::initProgram()
{
	if (!s_pProgramState)
	{
		s_pProgramState = OutLineEffect::create();
		s_pProgramState->retain();
	}
	auto ret = cocos2d::GLProgramState::create(s_pProgramState->getGLProgram());
	if (!ret)
	{
		return false;
	}
	s_pProgramState->retain();
	setGLProgramState(ret);
	setPixWidth(2);
	return true;
}
void MaskSprite::setPixWidth(float fWidth)
{
	if (!getTexture())
	{
		return;
	}
	cocos2d::Vec2 kSize;
	kSize.x = 1.f/(float)getTexture()->getPixelsWide()*fWidth;
	kSize.y = 1.f/(float)getTexture()->getPixelsHigh()*fWidth;
	setPixWidth(kSize);
}
void MaskSprite::setPixWidth(cocos2d::Vec2 fValue)
{
	if(!s_pProgramState ||
		getGLProgramState()->getGLProgram() != s_pProgramState->getGLProgram())
	{
		initProgram();
	}
	m_kWidthValue = fValue;
	getGLProgramState()->setUniformVec2(s_pProgramState->getWidthLocation(), fValue);
}
void MaskSprite::useMaskTexture(std::string kName)
{
	if(!s_pProgramState ||
		getGLProgramState()->getGLProgram() != s_pProgramState->getGLProgram())
	{
		initProgram();
	}
	m_pMaskTexture = cocos2d::TextureCache::sharedTextureCache()->addImage(kName);
	getGLProgramState()->setUniformTexture(s_pProgramState->getMaskLocation(), m_pMaskTexture);
}