#ifndef __DZPK_CHIP_H__
#define __DZPK_CHIP_H__

#include "cocos2d.h"
#include "Define.h"//by hxh

USING_NS_CC;
using namespace std;

//	德州扑克——筹码
class DZPKChip: public Node
{
public:
	DZPKChip();
	~DZPKChip();
	CREATE_FUNC(DZPKChip);
	virtual bool init();
	void initUI();

	/*
	**	@brief	更新筹码
	**	@lnValue	筹码数值
	*/
	void changeChip(LONGLONG llValue);

private:

	/*
	**	@brief	根据筹码数值更变筹码图片
	**	@lnValue	筹码数值
	*/
	void changeChipImg(LONGLONG llValue);

	/*
	**	@brief	设置筹码数值
	**	@lnValue	筹码数值
	*/
	void changeChipNum(LONGLONG llValue);

	//	根据数值获取筹码图像显示个数
	int getChipGroupCount(LONGLONG llValue);
	
	//	根据数值获取筹码图像的组合类型
	vector<int> getChipGroup(LONGLONG llValue);

protected:

	//	图像呈现——筹码
	Node* m_pChipImg;
	
	//	图像呈现——筹码数字
	Label* m_pChipNum;
};



#endif	//	__DZPK_CHIP_H__