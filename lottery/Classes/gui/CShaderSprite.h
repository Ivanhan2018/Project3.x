#ifndef		__WUCAN_SHADER_SPRITE__
#define		__WUCAN_SHADER_SPRITE__
#include "cocos2d.h"
USING_NS_CC;


#define sUniformMapHeight		320
#define sUniformMapWidth			854

class CCShaderSprite:public Node
{
public:
	CCShaderSprite(void);
	~CCShaderSprite(void);
	static CCShaderSprite* create(const char *pszFileName);

	virtual bool initWithFile(const char *pszFilename);
	virtual void update(float delta);
	//virtual void draw(void);

	//--…Ë÷√≈ˆ◊≤µ„
	void setTouchPos(float x,float y);
	void upHaiou(float ft);
	void haiouCall(Object* _obj);
private:
	GLuint					m_uniformPos,m_uniformTime,m_uniformMapHeight,m_uniformMapWidth,m_uniformMonsterTex;
	ccVertex2F			m_Pos;
	float						m_RippleTime;
	float						m_RippleSpeed;
	Texture2D *		m_MonsterTexture;
	Sprite*             m_haiouSpt;
};

#endif