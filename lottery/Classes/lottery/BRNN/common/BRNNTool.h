#ifndef __BRNN_TOOL_H__
#define __BRNN_TOOL_H__

#include "cocos2d.h"
#include "Define.h"
#include "BRNNDefine.h"

USING_NS_CC;
using namespace std;

/*
**	@brief	百人牛牛——工具
*/
class BRNNTool
{
public:
	BRNNTool();
	~BRNNTool();
	/*
	**	@brief	将筹码类型转换为对应的筹码值
	**	@eType	筹码类型
	**	@return	返回对应筹码面值
	*/
	static int convertToChipValue(E_BRNNChipType eType);

	/*
	**	@brief	将筹码值转换为对应的筹码类型
	**	@sValue	筹码面值
	**	@return	返回对应筹码类型
	*/
	static E_BRNNChipType convertToChipType(SCORE sValue);

	/*
	**	@brief	将手机端定义的区域类型转换为服务器定义的区域数值
	**	@eType	手机端定义的区域类型
	**	@return	返回服务器定义的区域数值
	*/
	static int convertToServerArea(E_BRNNBetAreaType eType);

	/*
	**	@brief			将服务器定义的区域数值转换为手机端定义的区域类型
	**	@nServerArea	服务器定义的区域类型
	**	@return			返回手机端定义的区域类型
	*/
	static E_BRNNBetAreaType convertToClientArea(int nServerArea);

private:

};

#endif  // __BRNN_TOOL_H__