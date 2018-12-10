#ifndef GLOBAL_UNITS_HEAD_FILE
#define GLOBAL_UNITS_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
#include "types.h"
//全局单元
class CGlobalUnits
{
	//界面配置
public:
	bool                            m_bMuteStatuts;                     //静音状态
	bool							m_bAllowSound;						//允许声音
	bool							m_bAllowBackGroundSound;			//允许背景声音
	bool							m_bNotifyUserInOut;					//进出消息
	bool							m_bDebarDetestChat;					//屏蔽消息

	//旁观配置
public:
	bool							m_bAllowLookon;						//允许旁观
	bool							m_bAllowFriendLookon;				//允许旁观

	//模式变量
public:
	byte							m_cbMessageMode;					//消息模式

	//函数定义
public:
	//构造函数
	CGlobalUnits();
	//析构函数
	virtual ~CGlobalUnits();

	//组件配置
public:
	//创建组件
	bool InitGlobalUnits();
	//注销组件
	bool UnInitGlobalUnits();

	//配置函数
public:
	//加载参数
	void LoadParameter();
	//保存参数
	void SaveParameter();
	//默认参数
	void DefaultParameter();

public:
	//允许游戏音效
	bool IsAllowGameSound(){return m_bAllowSound;}
	//允许背景音效
	bool IsAllowBackGroundSound(){return m_bAllowBackGroundSound;}
};

//////////////////////////////////////////////////////////////////////////////////
//对象说明
extern CGlobalUnits					gGlobalUnits;						//全局单元

//////////////////////////////////////////////////////////////////////////

#endif