#ifndef _LockLineLayer_H_
#define _LockLineLayer_H_
#include "cocos2d.h"

USING_NS_CC;
using namespace ui;

class LockLineLayer : public Layer
{
private:
	int line_space;//点与点之间的距离
	Vector<Sprite*> m_vectorLine;//线上的点

	Vec2 m_start;
	Vec2 m_end;
public:
	LockLineLayer(void);
	virtual ~LockLineLayer(void);

	virtual bool init();
	CREATE_FUNC(LockLineLayer);

	void initLineView(int num);

	void initLineNodePositon();

	void setPointStartAndEnd(Vec2 m_start, Vec2 m_end);
};

#endif // !_CharityLeaveLayer_H_
