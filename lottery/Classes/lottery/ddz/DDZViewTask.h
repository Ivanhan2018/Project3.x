//  [4/1/2014 wucan]

#ifndef	DDZ___VIEW_TASK__
#define	DDZ___VIEW_TASK__
#include "DDZDDZLogic.h"
#include "DDZDDZRes.h"
#include "cocos2d.h"
USING_NS_CC;

class DDZViewTask:public Node
{
public:
	CREATE_FUNC(DDZViewTask);
	virtual bool init();
	void setBackTaskView(BYTE* _data,WORD _num);									//--底牌倍率设置
	void setLastTaskView(WORD _key);														//--最后出牌任务
	Node * m_dpNode;		//底牌加倍
	Node * m_jsNode;		//结束牌加倍
private:
	LabelTTF * addTaskString(char * key,Vec2 point,Color3B color,int fontSize);
};
#endif