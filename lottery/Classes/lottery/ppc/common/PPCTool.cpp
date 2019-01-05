#include "PPCTool.h"

PPCTool::PPCTool()
{
}

PPCTool::~PPCTool()
{
}

//	将筹码类型转换为对应的筹码值
int PPCTool::convertToChipValue(EPPCChipType eType)
{
	int nValue = -1;
	switch (eType)
	{
	case EPPCChip_1:
		nValue = 1;
		break;
	case EPPCChip_5:
		nValue = 5;
		break;
	case EPPCChip_10:
		nValue = 10;
		break;
	case EPPCChip_100:
		nValue = 100;
		break;
	case EPPCChip_500:
		nValue = 500;
		break;
	case EPPCChip_1k:
		nValue = 1000;
		break;
	case EPPCChip_Null:
		break;
	default:
		break;
	}
	return nValue;
}

EPPCChipType PPCTool::convertToChipType(SCORE sValue)
{
	EPPCChipType eType = EPPCChip_Null;
	switch ((int)sValue)
	{
	case 1:
		eType = EPPCChip_1;
		break;
	case 5:
		eType = EPPCChip_5;
		break;
	case 10:
		eType = EPPCChip_10;
		break;
	case 100:
		eType = EPPCChip_100;
		break;
	case 500:
		eType = EPPCChip_500;
		break;
	case 1000:
		eType = EPPCChip_1k;
		break;
	default:
		break;
	}

	return eType;
}

//	将区域类型转换为服务器设定的区域数值
int PPCTool::convertToServerArea(EPPCCarLogoType eType)
{
	//	服务器区域定义
	int nArea[EPPCCarLogo_Null] =
	{
		EPPCCarLogo_Porsche_Big,
		EPPCCarLogo_BMW_Big,
		EPPCCarLogo_Benz_Big,
		EPPCCarLogo_VW_Big,
		EPPCCarLogo_Porsche_Small,
		EPPCCarLogo_BMW_Small,
		EPPCCarLogo_Benz_Small,
		EPPCCarLogo_VW_Small
	};
	for (int i = 0; i < EPPCCarLogo_Null; i++)
	{
		if (nArea[i] == eType)
		{
			return i += 1;
		}
	}
	return -1;
}

EPPCCarLogoType PPCTool::convertToClientArea(int nServerArea)
{
	EPPCCarLogoType eAreaType = EPPCCarLogo_Null;
	switch (nServerArea)
	{
	case 1:
		eAreaType = EPPCCarLogo_Porsche_Big;
		break;
	case 2:
		eAreaType = EPPCCarLogo_BMW_Big;
		break;
	case 3:
		eAreaType = EPPCCarLogo_Benz_Big;
		break;
	case 4:
		eAreaType = EPPCCarLogo_VW_Big;
		break;
	case 5:
		eAreaType = EPPCCarLogo_Porsche_Small;
		break;
	case 6:
		eAreaType = EPPCCarLogo_BMW_Small;
		break;
	case 7:
		eAreaType = EPPCCarLogo_Benz_Small;
		break;
	case 8:
		eAreaType = EPPCCarLogo_VW_Small;
		break;
	default:
		break;
	}
	return eAreaType;
}
