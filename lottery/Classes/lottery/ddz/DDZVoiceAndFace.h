//  [4/4/2014 wucan]
#ifndef	DDZ___VOICE_FACE__
#define	DDZ___VOICE_FACE__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <limits>
USING_NS_CC;
USING_NS_CC_EXT;

#define TEXBACK_WIDTH			541
#define TEXBACK_HEIGHT		303

#define Voice_Width					1
#define Voice_Height				40

#define Face_W_Count				3
#define Face_H_Count				3


enum Vioce_Type
{
	Vioce_Type_1=0,
};

class DDZCC_scrollView:public ScrollViewDelegate ,public Node
{
public:
	CREATE_FUNC(DDZCC_scrollView);
	virtual bool init();
	void addViewNode(Node* _Node,cocos2d::Size _size);								//--加入容器
	//--CCScrollViewDelegate
	virtual void scrollViewDidScroll(ScrollView* view);			//--滑动
	virtual void scrollViewDidZoom(ScrollView* view){};		//--缩放
public:
	ScrollView*   m_scrollView;
};

class DDZCC_listView: public TableViewDataSource, public TableViewDelegate,public Node
{
public:
	CREATE_FUNC(DDZCC_listView);
	virtual bool init();
	//--CCTableViewDataSource
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	//--CCTableViewDelegate
	virtual void tableCellTouched(TableView* table, TableViewCell* cell) ;
	virtual void scrollViewDidScroll(ScrollView* view){};
	virtual void scrollViewDidZoom(ScrollView* view){};

public:
	TableView*  m_tableView;

};

class DDZVoiceAndFace:public Layer
{
public:
	CREATE_FUNC(DDZVoiceAndFace);
	virtual bool init();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	virtual void onEnter();
	virtual void onExit();
	//--控件
	void setFaceElement(int _type,const char * _name);
	void runOutOrIn(bool isVisible=true);
private:
	DDZCC_listView*				m_VoiceNode;				//--语音控件
	DDZCC_scrollView*				m_scrollView;
	Node*					m_FaceNode;				//--表情控件
	Sprite*					m_VoiceSlide;
	Sprite*					m_BackSpt;
	int			  m_faceCount;
	int			  m_voiceCount;
	CC_SYNTHESIZE(bool,m_isOut,IsOut);
	Vec2     m_TouchBeganPos;
};

#endif