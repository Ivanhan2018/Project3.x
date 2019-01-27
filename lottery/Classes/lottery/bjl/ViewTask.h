//  [4/1/2014 wucan]

#ifndef		__VIEW_TASK__
#define		__VIEW_TASK__
#include "BJLRes.h"
#include "cocos2d.h"
USING_NS_CC;

class ViewTask:public Node
{
public:
	CREATE_FUNC(ViewTask);
	virtual bool init();
	void setBackTaskView(BYTE* _data,WORD _num);									//--底牌倍率设置
	void setLastTaskView(WORD _key);														//--最后出牌任务
	Node * m_dpNode;		//底牌加倍
	Node * m_jsNode;		//结束牌加倍
private:
	LabelTTF * addTaskString(char * key,Vec2 point,Color3B color,int fontSize);
};
#endif