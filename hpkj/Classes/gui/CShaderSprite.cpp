#include "CShaderSprite.h"


CCShaderSprite::CCShaderSprite(void)
{
	m_uniformPos=0;
	m_uniformTime=0;
	m_RippleTime=0;
	m_RippleSpeed=10;
}


CCShaderSprite::~CCShaderSprite(void)
{
}

CCShaderSprite* CCShaderSprite::create( const char *pszFileName )
{
	CCShaderSprite *pobSprite = new CCShaderSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

bool CCShaderSprite::initWithFile( const char *pszFilename )
{
	bool ret=false;
	do 
	{
			cocos2d::Size winSize=Director::getInstance()->getWinSize();
			m_haiouSpt=Sprite::create(pszFilename);
			m_haiouSpt->setAnchorPoint(Vec2(0,0.5f));
			m_haiouSpt->setPosition(Vec2(0, winSize.height * 0.5f));
			m_haiouSpt->getTexture()->setAntiAliasTexParameters();
			this->addChild(m_haiouSpt);

			Sprite* spt=Sprite::create(pszFilename);
			spt->setAnchorPoint(Vec2(0,0));
			spt->setPosition(Vec2(m_haiouSpt->getContentSize().width-1,0));
			spt->getTexture()->setAntiAliasTexParameters();
			m_haiouSpt->addChild(spt);
			//CCGLProgram* shader = new CCGLProgram();
			//shader->initWithVertexShaderFilename("example_Water.vsh","example_Water.fsh");
			//shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			//shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			//shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			//shader->link();
			//shader->updateUniforms();
			
			//m_uniformPos					= glGetUniformLocation(shader->getProgram(), "f_point");
			//m_uniformTime				= glGetUniformLocation(shader->getProgram(), "f_time");
			//m_uniformMapHeight		= glGetUniformLocation(shader->getProgram(), "f_height");
			//m_uniformMapWidth		= glGetUniformLocation(shader->getProgram(), "f_width");
			//m_uniformMonsterTex		= glGetUniformLocation(shader->getProgram(), "CC_Texture1");

			//this->setShaderProgram(shader);
			//shader->release();
			//CHECK_GL_ERROR_DEBUG();

			scheduleUpdate();
			schedule(schedule_selector(CCShaderSprite::upHaiou),4.0f);

			ret=true;
			
	} while (0);
	return ret;
}

void CCShaderSprite::update( float delta )
{
	m_RippleTime+=delta*0.01f;
	if (m_RippleTime>=1.0)
	{
		m_RippleTime=0;
	}
}

//void CCShaderSprite::draw( void )
//{
//	float y=m_haiouSpt->getPositionY();
//	m_haiouSpt->setPosition(Vec2(-m_RippleTime*m_haiouSpt->getContentSize().width,y));
//	//--you can set Uniform in here
//	//--you can set Uniform in here
//	//CC_NODE_DRAW_SETUP();
//
//	//getShaderProgram()->setUniformLocationWith1i(m_uniformMonsterTex,1);
//	//getShaderProgram()->setUniformLocationWith1f(m_uniformMapHeight,sUniformMapHeight);
//	//getShaderProgram()->setUniformLocationWith1f(m_uniformMapWidth,sUniformMapWidth);
//	//getShaderProgram()->setUniformLocationWith2f(m_uniformPos,m_Pos.x,m_Pos.y);
//	//getShaderProgram()->setUniformLocationWith1f(m_uniformTime,m_RippleTime);
//	//ccGLBindTexture2DN(1,m_MonsterTexture->getName());
//
//	Node::draw();
//}

void CCShaderSprite::setTouchPos( float x,float y )
{
	cocos2d::Size size= Director::getInstance()->getWinSize();

	m_Pos.x=x;
	m_Pos.y=y;
}

void CCShaderSprite::upHaiou(float ft)
{
	cocos2d::Size winSize=Director::getInstance()->getWinSize();
	SpriteFrame * frame=NULL;
	Animation* animation = Animation::create();
	String* buf=String::createWithFormat("haiou%d.png",rand()%2);
	for(int i=0;i<3;i++)
	{
		Sprite* sprite = Sprite::create(buf->getCString());
		frame = SpriteFrame::createWithTexture(sprite->getTexture(),CCRectMake(43*i,0,43,32));
		//frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
		animation->addSpriteFrame(frame);
	}
	Vec2 ptStar=Vec2(-50,rand()%sUniformMapHeight+winSize.height * 0.2f);
	Vec2 ptEnd=Vec2(sUniformMapWidth+50,rand()%sUniformMapHeight+winSize.height * 0.2f);
	float    time=rand()%10+10;
	float   scal=time/20;
	animation->setDelayPerUnit(0.2f);

	Sprite * HAIOUsPT=Sprite::createWithSpriteFrame(frame);

	if (rand()%4==0)
	{
		ptStar=Vec2(sUniformMapWidth+50,rand()%sUniformMapHeight);
		ptEnd=Vec2(-50,rand()%sUniformMapHeight);
		HAIOUsPT->setRotationY(180.0f);
	}
	HAIOUsPT->setPosition(ptStar);
	HAIOUsPT->runAction(RepeatForever::create(Animate::create(animation)));
	HAIOUsPT->runAction(Sequence::create(Spawn::create(MoveTo::create(time,ptEnd),ScaleTo::create(time,scal),NULL),
		CCCallFuncO::create(this,callfuncO_selector(CCShaderSprite::haiouCall),HAIOUsPT),NULL));

	this->addChild(HAIOUsPT);
}

void CCShaderSprite::haiouCall( Object* _obj )
{
	Node*node=(Node*)_obj;
	if(node)node->removeFromParent();
}
