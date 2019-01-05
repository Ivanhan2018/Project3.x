#ifndef __TOOL_KIT_H__
#define __TOOL_KIT_H__


#include "cocos2d.h"

USING_NS_CC;
using namespace std;

/*
**	工具箱
*/
class ToolKit
{
public:
	ToolKit();
	~ToolKit();

	//	转换成竖屏
	static void toPortrait();

	//	转换成横屏
	static void toLandscape();

	//	随机生成一个昵称
	static string generateNickname();

	static void showTips(string sTips, float fDuration = 1.0f);

	/*
	**	@brief		拖动调整子节点相对父节点的位置
	**				打印出相对父节点位置的节点坐标、百分比和世界坐标
	**	@pParent	父节点
	**	@pChild		子节点
	*/	
	static void studioUISingle(Node* pParent, Node* pChild);

	/*
	**	@brief		拖动调整子节点相对父节点的位置
	**				打印出相对父节点位置的节点坐标、百分比和世界坐标
	**	@pParent	父节点
	**	@vChildren	子节点集合
	*/
	static void studioUIMulti(Node* pParent, std::vector<Node*> vChildren);

	/*
	**	@brief		将一串字符串后4位用“****”代替
	**	@sString	UTF8编码的字符串
	*/	
	static string dealStringOfPrivacy(const string &sString);

	//隐藏状态栏
	static void toDontShowInfo();

	//显示状态栏
	static void toShowInfo();

private:
};

#endif  // __TOOL_KIT_H__