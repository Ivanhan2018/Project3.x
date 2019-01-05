#ifndef _WATIDISTRIBUTE_H_
#define _WATIDISTRIBUTE_H_
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"


class WaitDistribute : public cocos2d::LayerColor
{
private:
	cocos2d::ui::Widget*							UI;
public:
	WaitDistribute(void);
	~WaitDistribute(void);
	virtual bool init();
	CREATE_FUNC(WaitDistribute);
	void OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);
	void deleteView();
};

#endif // !_VIP_LAYER_H_
