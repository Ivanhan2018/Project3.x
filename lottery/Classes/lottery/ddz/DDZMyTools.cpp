#include "DDZMyTools.h"

DDZCCShake::DDZCCShake():m_strength_x(0),m_strength_y(0)
{

}

DDZCCShake * DDZCCShake::create(float d,float strength)
{
	return createWithStrength(d,strength,strength);
}

DDZCCShake * DDZCCShake::createWithStrength(float d,float strength_x,float strength_y)
{
	DDZCCShake * pRet = new DDZCCShake();
	if (pRet&&pRet->initWithDuration(d,strength_x,strength_y))
	{
		pRet->autorelease();
	} 
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool DDZCCShake::initWithDuration(float d,float stength_x,float strength_y)
{
	if (CCActionInterval::initWithDuration(d))
	{
		m_strength_x = stength_x;
		m_strength_y = strength_y;
		return true;
	}
	return false;
}

static float fgRangeRand(float min,float max)
{
	float rnd = ((float)rand()/(float)RAND_MAX);
	return rnd *(max-min)+min;
}

void DDZCCShake::update(float dt)
{
	float randx = fgRangeRand(-m_strength_x,m_strength_x);
	float randy = fgRangeRand(-m_strength_y,m_strength_y);
#if 0
	m_pTarget->setPosition(ccpAdd(m_startPosition,ccp(randx,randy)));
#endif
}

void DDZCCShake::startWithTarget(cocos2d::Node * pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPosition();
	//m_startPosition = Vec2::ZERO;
}

void DDZCCShake::stop()
{
	this->getTarget()->setPosition(m_startPosition);
	CCActionInterval::stop();
}

DDZCCStrokeLabel* DDZCCStrokeLabel::create(cocos2d::LabelTTF* labelTTF,cocos2d::Color3B fullColor,cocos2d::Color3B StrokeColor,float strokeSize)
{
	DDZCCStrokeLabel* tmp = new DDZCCStrokeLabel;
	tmp->autorelease();
	tmp->m_label = labelTTF;
	tmp->m_fullColor = fullColor;
	tmp->m_StrokeColor = StrokeColor;
	tmp->m_strokeSize = strokeSize;
	tmp->init();
	return tmp;
}

bool DDZCCStrokeLabel::init()
{
	float strokeSize = getStrokeSize();
	cocos2d::Size textureSize = m_label->getContentSize();
	textureSize.width += 2 * strokeSize;
	textureSize.height += 2 * strokeSize;
	//call to clear error
	glGetError();
	CCRenderTexture *rt = CCRenderTexture::create(
		textureSize.width, textureSize.height);
	if(!rt)
	{
		//ERROR_MSG("create render texture failed !!!!");
		addChild(m_label);
		return true;
	}
	m_label->setColor(m_fullColor);
	ccBlendFunc originalBlend = m_label->getBlendFunc();
	ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};
	m_label->setBlendFunc(func);
	m_label->setAnchorPoint(ccp(0.5, 0.5));
	rt->begin();
	for(int i = 0; i < 360; i += 15)
	{
		float r = CC_DEGREES_TO_RADIANS(i);
		m_label->setPosition(ccp(
			textureSize.width * 0.5f + sin(r) * strokeSize,
			textureSize.height * 0.5f + cos(r) * strokeSize));
		//m_label->visit();
	}
	m_label->setColor(m_StrokeColor);
	m_label->setBlendFunc(originalBlend);
	m_label->setPosition(ccp(textureSize.width * 0.5f, textureSize.height * 0.5f));
	//m_label->visit();
	rt->end();
	CCTexture2D *texture = rt->getSprite()->getTexture();
	texture->setAliasTexParameters();
	m_sprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
	setContentSize(m_sprite->getContentSize());
	m_sprite->setAnchorPoint(ccp(0, 0));
	m_sprite->setPosition(ccp(0, 0));
	((Sprite *)m_sprite)->setFlipY(true);
	addChild(m_sprite);
	return true;
}
float DDZCCStrokeLabel::getStrokeSize()
{
	return m_strokeSize;
}
void DDZCCStrokeLabel::setStrokeSize(float strokeSize)
{
	m_strokeSize = strokeSize;
}