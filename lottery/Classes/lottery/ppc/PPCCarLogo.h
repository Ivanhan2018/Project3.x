#ifndef __PPC_CAR_LOGO_H__
#define __PPC_CAR_LOGO_H__

#include "cocos2d.h"
#include "PPCEnum.h"
#include "PPCRes.h"

USING_NS_CC;

//	碰碰车——汽车logo
class PPCCarLogo: public Node
{
public:
	PPCCarLogo();
	~PPCCarLogo();
	CREATE_FUNC(PPCCarLogo);

	/*
	**	@brief	初始化汽车logo
	**	@eType	类型
	**	@eSize	尺寸
	*/
	void initCarLogoWithType(EPPCCarLogoType eType);

	//	重置
	void resetPPCCarLogo();

	//	设置汽车logo是否高亮
	void setHighLight(bool bIsHighLight);

	//	设置汽车是否可见
	void setCarVisible(bool bIsCarVisible);

	//	获得汽车logo类型
	EPPCCarLogoType getCarLogoType();

	//	汽车logo的ID
	CC_SYNTHESIZE(int, m_nLogoID, LogoID);

	//	汽车logo
	Sprite* m_pImgCarLogo;

private:
	bool init();
	void initData();

	//	自动设置位置
	void autoPosition();

	//	汽车logo背景
	Sprite* m_pImgBG;


	//	汽车logo半透明遮罩
	Sprite* m_pImgMask;

	//	汽车logo高亮光环
	Sprite* m_pHighLight;

	//	汽车
	Sprite* m_pImgCar;

	//	汽车logo类型
	EPPCCarLogoType m_eCarLogoType;

};

#endif  // __PPC_CAR_LOGO_H__