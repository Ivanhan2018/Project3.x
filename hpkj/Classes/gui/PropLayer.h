#ifndef __Game__PropLayer__
#define __Game__PropLayer__

#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class PropLayer : public Layer
{


public:
	PropLayer();
	~PropLayer();
	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(PropLayer);

public:
	void dismiss(Object *obj);
	void makeSure(Object *obj);
	
private:
	EditBox *m_pUserInput;
	EditBox *m_pUserVerifyInput;

};
#endif  //__Game__PropLayer__