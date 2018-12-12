#ifndef _COMM_H_
#define _COMM_H_

//账户长度
#define		ACCOUNT_LENGTH		21
//密码长度
#define		PASSWORD_LENGTH		21
//IP地址长度
#define		IP_LENGTH			11
//时间长度
#define		TIME_LENGTH			51
//QQ号码长度
#define		QQ_LENGTH			12
//号码最大长度
#define		HAOMAO_LENGTH		60

//开奖信息
#define	KJ_QIHAO_LENGTH			30
#define	KJ_HAOMA_LENGTH			30
#define	KJ_SHIJIAN_LENGTH		30

#define	TOP_KAIJIANG_NUMBERS	5

#define MAX_BEISHU				10000
#define MAX_ZHUIHAO_BEISHU		999


//彩票种类
typedef enum {
	CZChongQingSSC=1,		//重庆时时彩
	CZJiangXiSSC=2,			//江西时时彩
	CZ3D=3,					//福彩3D
	CZXinJiangSSC = 4,		//新疆时时彩
	CZPailie3,				//排列3
	CZGD11Xuan5,			//广东11选5
	CZCQ11Xuan5,			//重庆11选5 
	CZSD11Xuan5,			//山东11选5
	CZJX11Xuan5,			//江西11选5
	CZKUAILE8,				//北京快乐8
	CZ_FENFEN_CAI,			//分分彩：5分钟开奖，24小时不停。每天288期
	CZ_WUFEN_CAI,			//5分彩
	CZ_BJKUAICHE,			//北京PK10
	CZ_LIUHECAI = 14,
	CZ_QiXingCai = 15,		//七星彩--lly
	CZHLJ11Xuan5 = 16,		//黑龙江11选5
	CZXingYun28,
	CZ_KUAI_3,				//江苏快3		1-6
	CZ_GD_HAPPY10MIN,		//广东快乐10分	8 of 20
	CZ_SECOND,				//秒秒彩
	CZ_LUCKY_FARM,			//幸运农场：重庆快乐10分=21
	CZ_ERFEN_CAI,			//2分彩
	CZCount
}CaiZhong;

//时时彩玩法种类
typedef enum 
{
	inValid,
	//QianSanZhiXuan,			//前三直选
	QianSanZhiXuan_FuShi=1,		//前三直选复式，也叫五星前三
	QianSanZhiXuan_DanShi,		//

	//HouSanZhiXuan,			//后三直选
	HouSanZhiXuan_FuShi,		//后三直选复式，也叫五星后三
	HouSanZhiXuan_DanShi,

	//QianSanZuXuan,			//前三组选
	QianSanZuXuan_ZuSanFuShi,
	QianSanZuXuan_ZuSanDanShi,
	QianSanZuXuan_ZuLiuFuShi,
	QianSanZuXuan_ZuLiuDanShi,

	//HouSanZuXuan,				//后三组选
	HouSanZuXuan_ZuSanFuShi,
	HouSanZuXuan_ZuSanDanShi,
	HouSanZuXuan_ZuLiuFuShi,
	HouSanZuXuan_ZuLiuDanShi,

	//WuXingQianEr,				//五星前二
	WuXingQianEr_FuShi,
	WuXingQianEr_DanShi,

	//WuXingHouEr,				//五星后二
	WuXingHouEr_FuShi,
	WuXingHouEr_DanShi,

	//QianErZuXuan,				//前二组选
	QianErZuXuan_FuShi,
	QianErZuXuan_DanShi,

	//HouErZuXuan,				//后二组选
	HouErZuXuan_FuShi,
	HouErZuXuan_DanShi,

	DaXiaoDanShuang,			//大小单双,后二

	//BuDingWei,				//不定位
	BuDingWei_QianSan,
	BuDingWei_HouSan,

	DingWeiDan,					//定位胆=24
		
	RenXuan2,					//任选2,3,4
	RenXuan3,
	RenXuan4,
	
	QianSanHunhe,				//前三混合
	ZhongSanHunhe,				//中三混合
	HouSanHunhe,				//后三混合
	
	//新增直选复式玩法
	WuxingZhixuan,				//五星直选（0209新增）=31
	WuxingQianSi,				//五星前四
	WuxingHousi,				//五星后四
	WuxingZhongsan,				//五星中三（五星前三和五星后三见第一排）

	//新增组选玩法
	ZhongsanZusan,				//中三组三（0209新增）
	ZhongsanZuliu,				//中三组六
	
	//新增组选玩法
	Zuxuan120,					//组选120
	Zuxuan60,					//组选60
	Zuxuan30,					//组选30
	Zuxuan20,					//组选20
	Zuxuan10,					//组选10
	Zuxuan5,					//组选5
								
	//新增和值玩法
	QianSanZhiXuanHeZhi,		//前三直选和值
	HouSanZhiXuanHeZhi,			//后三直选和值
	ZhongSanZhiXuanHeZhi,		//中三直选和值
	
	//新增不定位
	BuDingWei_QianEr,			//不定位：前2
	BuDingWei_HouEr,			//不定位：后2

	//新增趣味玩法
	QW_YiFanFengShun,			//趣味：一帆风顺，一个一样一个数字就中奖
	QW_HaoShiChengShuang,		//趣味：好事成双，2个一样的数字才中奖
	QW_SanXingBaoXi,			//趣味：三星报喜，3个一样的数字才中奖
	QW_SiJiFaCai,				//趣味：四季发财，4个一样的数字才中奖
	NiuNiu_Num = 52,			//牛牛：5个数字之和是10的倍数；牛1--牛10（牛牛） 1:10（无牛通吃）
	NiuNiu_None,				//牛牛：无牛 1:2
	NiuNiu_DxDs,				//牛牛：大小单双 1:2.5
	DaXiaoDanShuang_DW,			//定位胆大小单双;
	Zonghe_DXDSLh = 56,			//总和大小单双：总和值大於等於23為大，小於等於22為小。
	SSC_LongHu,					//龙虎：头大于尾为龙，尾大于头为虎；和 ？
	QianSan_Quwei,				//前三豹子顺子对子半顺杂六
	ZhongSan_Quwei,				//中三豹子顺子对子半顺杂六
	HouSan_Quwei,				//后三豹子顺子对子半顺杂六
	HouErZhixuan_Hezhi = 61,	//后二直选和值
	QianErZhixuan_Hezhi,		//前二直选和值
	RenSanZhixuan_Hezhi,		//任三直选和值	0
	RenErZhixuan_Hezhi,			//任二直选和值	0
	SSC_LongHuYaJun = 65,		//龙虎：亚军
	Ren3Zu3_Fushi,				//任3组3复式	
	Ren3Zu6_Fushi,				//任3组6复式	
	Ren3Hunhe_Zuxuan,			//任3混合 组选	
	Ren2Zuxuan_Fushi,			//任2组选 复式	
	Ren2Zuxuan_Danshi,			//任2组选 单式	

	//新增单式玩法
	WuxingZhixuan_DanShi,	//五星直选单式
	WuxingQianSi_DanShi,	//五星前四单式
	WuxingHousi_DanShi,		//五星后四单式	
	WuxingZhongsan_DanShi, //五星中三单式
	ZhongsanZuliu_DanShi,	//中三组六单式
	ZhongsanZusan_DanShi,	//中三组三单式

	WanFaCount
}SSCWanFaKind;



//11选5的玩法
typedef enum tagEnumIIXuan5
{
		IIinValid,

		IIRenXuan2,						//任选二

		IIRenXuan3,						//任选三
		IIRenXuan4,						//任选四
		IIRenXuan5,						//任选五

		IIRenXuan6,						//任选六

		IIRenXuan7,						//任选七

		IIRenXuan8,						//任选八
		QianYi,							//前一


		QianEr_ZhiXuan,					//前二 直选选号
		QianEr_ZuXuan,					//前二组选选号


		QianSan_ZhiXuan,				//前三直选选号
		QianSan_ZuXuan,					//前三组选选号

		//新增单式玩法
		QianEr_ZhiXuan_DanShi,			//前二直选单式
		
		QianSan_ZhiXuan_DanShi,
		
		IIRenXuan2_DanShi,
		
		IIRenXuan3_DanShi,
	
		IIRenXuan4_DanShi,
		
		IIRenXuan5_DanShi,
	
		IIRenXuan6_DanShi,
		
		IIRenXuan7_DanShi,
		
		IIRenXuan8_DanShi,

		IIWanFaCount
}IIXuan5GameKind;




//	北京赛车PK10
typedef enum
{
	PKinValid,

	WF_GuanJun,		//冠军
	WF_GuanYaJun,	//冠亚军
	WF_YaJun,		//亚军
	WF_QianSan,		//	前三
	WF_DiSan,		//第三
	WF_QianSi,		//	前四
	WF_DiSi,		//第四
	WF_QianWu,		//	前五
	WF_DiWu,		//第五
	WF_QianLiu,		//	前六
	WF_DiLiu,		//第六
	WF_QianQi,		//	前七
	WF_DiQi,		//第七
	WF_QianBa,		//	前八
	WF_DiBa,		//第八
	WF_QianJiu,		//	前九
	WF_DiJiu,		//第九
	WF_QianShi,		//	前十
	WF_DiShi,		//第十
	WF_DXDS,		//大小单双龙虎（PK10没有和）
	WF_DWD,			//定位胆
	WF_GYHZ,		//冠亚和值：下注为大小单双
	//新增玩法
	WF_GuanYaJun_DanShi,//冠亚军单式
	WF_QianSan_DanShi,//前三单式
	WF_QianSi_DanShi,//前四单式
	WF_QianWu_DanShi,//前五单式
	PK10WanFaCount
}PK10GameKind;



//3D和排列三的玩法（包括快3）
typedef	enum	tagEnumTreeNumbers
{
	Invalid,
	enWF_ZhixuanFushi,					//三星直选复式,
	enWF_ZhixuanDanshi,					//三星直选单式,
	enWF_QiansanzusanFushi,				//前三组三复式,
	enWF_QiansanzusanDanshi,			//前三组三单式,
	enWF_QiansanzuliuFushi,				//前三组六复式,
	enWF_QiansanzuliuDanshi,			//前三组六单式,
	enWF_QianyizhixuanFushi,			//前一直选复式,
	enWF_QianerzhixuanFushi,			//前二直选复式,
	enWF_QianerzhixuanDanshi,			//前二直选单式,
	enWF_Budingwei,						//三星不定位,
	enWF_HouyizhixuanFushi,				//后一直选复式,
	enWF_HouerzhixuanFushi,				//后二直选复式,
	enWF_HouerzhixuanDanshi,			//后二直选单式,
	enWF_ZhixuanHezhi,					//三星直选和值,
	enWF_DaxiaoDanshuang,				//大小单双
	enWF_DingWeiDan,					//定位胆
	enWF_Count							//玩法个数
}ThreeNumbersKind;


//追号所用状态
enum ZhuiHaoType
{
	YuanTag = 0,                                   //退出
	JiaoTag,                                         //设置
	FenTag,                                           //帮助
};

//圆角分
enum xIazhuyShiYiXuanWuType
{
	YuanShiYiXuanWuTag = 0,                                   //元
	JiaoShiYiXuanWuTag,                                       //角
	FenShiYiXuanWuTag,                                        //分
};

//important:这里的顺序，代表了选择玩法的时候，点击按钮时对应为什么玩法ID
//该按钮上面显示什么玩法名称，由displaytext.txt里面的文字来确定。
static SSCWanFaKind wanFaMap[100] = {
	inValid, 
	WuxingZhixuan,
	WuxingZhixuan_DanShi,	//五星直选单式
	WuxingQianSi,
	WuxingQianSi_DanShi,	//五星前四单式
	WuxingHousi,	
	WuxingHousi_DanShi,		//五星后四单式
	QianSanZhiXuan_FuShi,
	QianSanZhiXuan_DanShi,
	QianSanZuXuan_ZuLiuFuShi,
	QianSanZuXuan_ZuLiuDanShi,
	QianSanZuXuan_ZuSanFuShi,
	QianSanZuXuan_ZuSanDanShi,
	QianSanHunhe,
	WuxingZhongsan,
	WuxingZhongsan_DanShi, //五星中三单式
	ZhongsanZuliu,
	ZhongsanZuliu_DanShi,	//中三组六单式
	ZhongsanZusan,
	ZhongsanZusan_DanShi,	//中三组三单式
	ZhongSanHunhe,
	HouSanZhiXuan_FuShi,
	HouSanZhiXuan_DanShi,
	HouSanZuXuan_ZuLiuFuShi,
	HouSanZuXuan_ZuLiuDanShi,
	HouSanZuXuan_ZuSanFuShi,
	HouSanZuXuan_ZuSanDanShi,
	HouSanHunhe,
	WuXingQianEr_FuShi,
	WuXingQianEr_DanShi,
	QianErZuXuan_FuShi,
	QianErZuXuan_DanShi,
	WuXingHouEr_FuShi,
	WuXingHouEr_DanShi,
	HouErZuXuan_FuShi,
	HouErZuXuan_DanShi,
	QianSanZhiXuanHeZhi,
	ZhongSanZhiXuanHeZhi,
	HouSanZhiXuanHeZhi,
	//QianErZhixuan_Hezhi, //目前不存在
	//HouErZhixuan_Hezhi,	//目前没有
	BuDingWei_QianSan,
	BuDingWei_HouSan,
	BuDingWei_QianEr,
	BuDingWei_HouEr,
	DaXiaoDanShuang_DW,
	DingWeiDan,
	QW_YiFanFengShun,				//趣味：一帆风顺，一个一样一个数字就中奖
	QW_HaoShiChengShuang,			//趣味：好事成双，2个一样的数字才中奖
	QW_SanXingBaoXi,				//趣味：三星报喜，3个一样的数字才中奖
	QW_SiJiFaCai,					//趣味：四季发财，4个一样的数字才中奖
	SSC_LongHu						//龙虎斗  虎和龙456  
};

static PK10GameKind PK10WanfaMap[100] = {
	PKinValid,
	WF_GuanJun,		//冠军
	WF_GuanYaJun,	//冠亚军
	WF_GuanYaJun_DanShi,//冠亚军单式
	WF_QianSan,		//	前三
	WF_QianSan_DanShi,
	WF_QianSi,		//	前四
	WF_QianSi_DanShi,
	WF_QianWu,		//	前五
	WF_QianWu_DanShi,
	WF_DWD,			//定位胆
	WF_DXDS,		//大小单双龙虎（PK10没有和）
	WF_GYHZ,		//冠亚和值：下注为大小单双
	PK10WanFaCount
};

static ThreeNumbersKind treeNumbersMap[100] = {
	Invalid,
	enWF_QianyizhixuanFushi,			//前一直选复式,
	enWF_QianerzhixuanFushi,			//前二直选复式,
	enWF_QianerzhixuanDanshi,			//前二直选单式
	enWF_ZhixuanFushi,					//三星直选复式,
	enWF_ZhixuanDanshi,					//三星直选单式
	enWF_QiansanzusanFushi,				//前三组三复式,
	enWF_QiansanzusanDanshi,
	enWF_QiansanzuliuFushi,				//前三组六复式,
	enWF_QiansanzuliuDanshi,
	enWF_HouyizhixuanFushi,				//后一直选复式,
	enWF_HouerzhixuanFushi,				//后二直选复式,
	enWF_HouerzhixuanDanshi,
	enWF_Budingwei,						//三星不定位,
	enWF_DingWeiDan,					//定位胆
	enWF_DaxiaoDanshuang,				//大小单双	
	enWF_ZhixuanHezhi,					//三星直选和值,
};

static tagEnumIIXuan5 GuangDongwanFaMap[100] = {	
	IIinValid,
	QianYi,							//前一
	QianEr_ZhiXuan,					//前二 直选选号
	QianEr_ZhiXuan_DanShi,			//前二直选单式
	QianEr_ZuXuan,					//前二组选选号
	QianSan_ZhiXuan,				//前三直选选号
	QianSan_ZhiXuan_DanShi,
	QianSan_ZuXuan,					//前三组选选号	
	IIRenXuan2,						//任选二
	IIRenXuan2_DanShi,
	IIRenXuan3,						//任选三
	IIRenXuan3_DanShi,
	IIRenXuan4,						//任选四
	IIRenXuan4_DanShi,
	IIRenXuan5,						//任选五
	IIRenXuan5_DanShi,
	IIRenXuan6,						//任选六
	IIRenXuan6_DanShi,
	IIRenXuan7,						//任选七
	IIRenXuan7_DanShi,
	IIRenXuan8,						//任选八
	IIRenXuan8_DanShi,

	IIWanFaCount
};

enum xIazhuyShiShiCAIWuType
{ 
	YuanShiShiCAITag = 0,                                   //元
	JiaoShiShiCAITag,                                         //角
	FenShiShiCAITag,                                           //分
};

#endif //_COMM_H_