#ifndef __PPC_ENUM_H__
#define __PPC_ENUM_H__

/***********************碰碰车自定义宏	begin*************************************************/


#define PPC_CAR_LOGO_SCALE_BIG 0.7f											//	碰碰车——中奖项——汽车logo——尺寸缩放值——大
#define PPC_CAR_LOGO_SCALE_SMALL 0.5f										//	碰碰车——中奖项——汽车logo——尺寸缩放值——小
#define PPC_AREA_ICON_SCALE_SMALL 0.8f										//	碰碰车——区域图标——汽车logo——尺寸缩放值——小
#define PPC_AREA_CHIP_SCALE_SMALL 0.3f										//	碰碰车——区域图标——桌面筹码——尺寸缩放值——小
#define PPC_AREA_SIZE Size(240, 124)										//	碰碰车——桌面区域尺寸
#define PPC_TRAVEL_LIST_VIEW_SIZE Size(62 * 10, 50)							//	碰碰车——路单列表视图尺寸
#define PPC_APPLY_LIST_VIEW_SIZE Size(675.0f, 81.0f * 3.5f)					//	碰碰车——上庄列表视图尺寸
#define PPC_CARLGOG_COUNT 32												//	碰碰车——汽车logo总数
#define PPC_SHOW_ENDING_DURATION 3											//	碰碰车——显示结算框持续时间

//历史记录
#define PPC_MAX_SCORE_HISTORY			65									//历史个数


//	XML相关

#define PPC_XML_FILE_NAME "ppc_strings.xml"									//	碰碰车——XML文件
#define PPC_XML_KEY_QUIT "tips_quit"										//	碰碰车——key——退出提示语
#define PPC_XML_KEY_SYSTEM_BANKER "system_banker"							//	碰碰车——key——系统坐庄时显示的庄家名
#define PPC_XML_KEY_BET_FAILED "bet_failed"									//	碰碰车——key——下注失败

/***********************碰碰车自定义宏	end*************************************************/



/***********************碰碰车枚举	begin*************************************************/


//	碰碰车——汽车logo枚举
enum EPPCCarLogoType
{
	EPPCCarLogo_Porsche_Big = 0,	//	汽车logo——保时捷——大
	EPPCCarLogo_Porsche_Small,		//	汽车logo——保时捷——小
	EPPCCarLogo_BMW_Big,			//	汽车logo——宝马——大
	EPPCCarLogo_BMW_Small,			//	汽车logo——宝马——小
	EPPCCarLogo_Benz_Big,			//	汽车logo——奔驰——大
	EPPCCarLogo_Benz_Small,			//	汽车logo——奔驰——小
	EPPCCarLogo_VW_Big,				//	汽车logo——大众——大
	EPPCCarLogo_VW_Small,			//	汽车logo——大众——小
	EPPCCarLogo_Null				//	汽车logo——无效
};

//	碰碰车——筹码类型枚举
enum EPPCChipType
{
	EPPCChip_1 = 0,	//	筹码——额度为1
	EPPCChip_5,		//	筹码——额度为5
	EPPCChip_10,	//	筹码——额度为10
	EPPCChip_100,	//	筹码——额度为100
	EPPCChip_500,	//	筹码——额度为500
	EPPCChip_1k,	//	筹码——额度为1k
	EPPCChip_Null,	//	无效
};

//	碰碰车——申请上庄状态枚举
enum EPPCApplyState
{
	EPPC_EPPC_Apply_State_Apply = 0,	//	申请上庄
	EPPC_EPPC_Apply_State_Banker,		//	已经坐庄
	EPPC_Apply_State_Null,				//	无
};

//	碰碰车——计时器类型
enum EPPCProcessType
{
	EPPC_Idle = 0,	//	空闲时间
	EPPC_Bet,		//	下注时间	
	EPPC_Lottery,	//	开奖时间
	EPPC_Null		//	无
};

//	碰碰车——对话框类型枚举
enum EPPCDialog
{
	EPPCDialog_PPCEnding = 0	//	游戏结算界面
};

/***********************碰碰车枚举	end*************************************************/

#endif	//	__PPC_ENUM_H__