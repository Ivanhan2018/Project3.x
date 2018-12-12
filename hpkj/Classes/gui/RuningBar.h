//
//  MatchError.h
//  Game
//
//  Created by ÂÞÃô on 13-7-10.
//
//

#ifndef __Game__RuningBar__
#define __Game__RuningBar__

#include "cocos2d.h"
#include "CocosUnits.h"

class RuningBar : public Layer
{
public:
    CREATE_FUNC(RuningBar);
    virtual bool init();
    RuningBar();
    ~RuningBar();
	//virtual void visit();
	void reSertRunHolePosition(Node* node);
private:
	LabelTTF* labelRunHole;				//ÅÜÂíÌõ
};

#endif /* defined(__Game__MatchError__) */
