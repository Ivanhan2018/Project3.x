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

//通用投注内容转义解释如下：
#define		NSmallNumber		1				//小
#define		NSingleNumber		3				//单
#define		NDoubleNumber		2				//双
#define		NTigerNumber		4				//虎
#define		NPingNumber			5				//和=平
#define		NDragonNumber		6				//龙
#define		NHalfSerialNumber	7				//半顺 如540 790 706
#define		NSimpleNumber		8				//杂六：排除豹子，顺子，对子，半顺 即为中奖
#define		NBigNumber			9				//大
#define		NSmallSingleNumber	10				//小单
#define		NSmallDoubleNumber	11				//小双
#define		NBigSingleNumber	12				//大单
#define		NBigDoubleNumber	13				//大双
#define		NCoupleNumber		14				//对子
#define		NSerialNumber		15				//顺子（一条龙）
#define		NAllSame			16				//豹子（全相同）
#define		NLimitSmall			17				//极值小,0-5
#define		NLimitBig			18				//极值大：22-27
//快乐10分
#define		NSumDigitBig		19				//总尾大（总和各位数大）
#define		NSumDigitSmall		20				//总尾小（总和各位数小）
#define		NHeshuDa			21				//合数大（所有的个位和十位数字相加之和）
#define		NHeshuXiao			22				//合数小（所有的个位和十位数字相加之和）
#define		NZhongNumber		23				//中：開出之號碼為01、02、03、04、05、06、07
#define		NFacaiNumber		24				//發：開出之號碼為08、09、10、11、12、13、14
#define		NBaiNumber			25				//白：開出之號碼為15、16、17、18、19、20
#define		NEastNumber			26				//東：開出的號碼為01、05、09、13、17
#define		NSouthNumber		27				//南：開出的號碼為02、06、10、14、18
#define		NWestNumber			28				//西：開出的號碼為03、07、11、15、19
#define		NNorthNumber		29				//北：開出的號碼為04、08、12、16、20
//快乐8
#define		NFrontMore			30				//前多：前盤號碼（01-40）比後盤號碼是（41~80）個數多時
#define		NBackMore			31				//后多：後盤號碼（41-80）比前盤號碼是（01~40）個數多
#define		NEqualNumber		32				//前后和：前盘号码跟后盘号码个数相同
#define		NSingleMore			33				//单多：開出的20個號碼中，单數號碼比双數號碼個數多
#define		NDoubleMore			34				//双多：開出的20個號碼中，双數號碼比单數號碼個數多
#define		NEqualDigits		35				//单双数相同
//開出的20個號碼相加的总和值分在5個段，以金、木、水、火、土命名：
//金（210～695）、木（696～763）、水（764～855）、火（856～923）和土（924～1410）
#define		NSumGold			36				//金
#define		NSumWood			37				//木
#define		NSumWater			38				//水
#define		NSumFire			39				//火
#define		NSumEarth			40				//土
//其它
#define		NSZero				0				//0 上 奇
#define		NSOne				1				//1	中 和
#define		NSTwo				2				//2	下 偶

//彩票种类
typedef enum {
	CZChongQingSSC=1,		//重庆时时彩	1
	CZJiangXiSSC=2,			//江西时时彩	2
	CZ3D=3,					//福彩3D		3
	CZXinJiangSSC=4,		//新疆时时彩	4
	CZPailie3,				//排列3			5
	CZGD11Xuan5,			//广东11选5		6
	CZCQ11Xuan5,			//重庆11选5		7
	CZSD11Xuan5,			//山东11选5		8
	CZJX11Xuan5,			//江西11选5		9
	CZKUAILE8,				//北京快乐8		10
	CZ_FENFEN_CAI,			//分分彩：5分钟开奖，24小时不停。每天288期	11
	CZ_WUFEN_CAI,			//5分彩			12
	CZ_BJPK10,				//北京赛车		13
	CZ_HK6HeCai,			//香港六和采	14
	CZ_SEVENSTAR,			//七星彩；		15	
	CZ_HLJ11Xuan5,			//黑龙江11选5	16
	CZ_LUCKY_28,			//幸运28		17
	CZ_KUAI_3,				//江苏快3		18
	CZ_GD_HAPPY10MIN,		//广东快乐10分	19
	CZ_KOREA_FEN_CAI,		//韩国1.5分彩	20
	CZ_LUCKY_FARM,			//幸运农场：重庆快乐10分=21
	CZ_XJP_ERFEN_CAI,		//新加坡2分彩	22
	CZ_TIANJIN_SSC,			//天津时时彩	23
	CZ_BEIJING_5FEN_CAI,	//北京5分彩，	24
	CZ_CANADA_FEN_CAI,		//加拿大3.5分	25
	CZ_JILI_2FEN_CAI,		//吉利两分彩	26
	CZ_STOCK_FINANCE,		//金融证券
	CZ_TENCENT_FEN_CAI=28,	//腾讯分分彩
	CZ_QQ_FEN_CAI=29,		//QQ分分彩
	CZ_TENCENT_2FC,			//腾讯2分彩
	CZ_QQ_2FEN_CAI,				//QQ2分彩
	CZ_SOCCER_TEAM,			//足球
	CZ_XYFT,				//幸运飞艇		33
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
	ZhongsanZusan_DanShi,				//中三组三单式
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

	QSZuXuan24=72,	 //前四组选24
	QSZuXuan12=73,	 //前四组选12
	QSZuXuan6=74,	 //前四组选6
	QSZuXuan4=75,	 //前四组选4
	HSZuXuan24=76,	 //后四组选24
	HSZuXuan12=77,	 //后四组选12
	HSZuXuan6=78,	 //后四组选6
	HSZuXuan4=79,	 //后四组选4

	//新增单式玩法
	WuxingZhixuan_DanShi,	//五星直选单式
	WuxingQianSi_DanShi,	//五星前四单式
	WuxingHousi_DanShi,		//五星后四单式	
	WuxingZhongsan_DanShi, //五星中三单式
	ZhongsanZuliu_DanShi,	//中三组六单式
	ZhongsanZusan_DanShi_Bet,	//中三组三单式

	RenXuan4_DanShi,//任选四单式
	RenXuan3_DanShi,//任选3单式
	RenXuan2_DanShi,//任选2单式
	Ren3Zu3_DanShi, //选三组三单式
	Ren3Zu6_DanShi, //任三组六单式	

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
		//胆拖玩法
		IIRenXuan2_DanTuo,
		IIRenXuan3_DanTuo,
		IIRenXuan4_DanTuo,
		IIRenXuan5_DanTuo,
		IIRenXuan6_DanTuo,
		IIRenXuan7_DanTuo,
		IIRenXuan8_DanTuo,
		IIRenXuan1,		//
		//新增单式玩法
		QianEr_ZhiXuan_DanShi,			//前二直选单式		
		QianSan_ZhiXuan_DanShi,		
		IIRenXuan1_DanShi,
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

typedef enum {	
	QiXingValidate,
	QiXingCai_LiangDing=1,
	QiXingCai_SanDing,
	QiXingCai_SiDing,
	QiXingCai_YiDing,
	QiXingCai_LiangZiXian,
	QiXingCai_SanZiXian,
}QiXingCaiKind;


//幸运28玩法
typedef enum {
	xy28_validate,
	xy28_tm0,
	xy28_tm1,
	xy28_tm2,
	xy28_tm3,
	xy28_tm4,
	xy28_tm5,
	xy28_tm6,
	xy28_tm7,
	xy28_tm8,
	xy28_tm9,
	xy28_tm10,
	xy28_tm11,
	xy28_tm12,
	xy28_tm13,
	xy28_tm14,
	xy28_tm15,
	xy28_tm16,
	xy28_tm17,
	xy28_tm18,
	xy28_tm19,
	xy28_tm20,
	xy28_tm21,
	xy28_tm22,
	xy28_tm23,
	xy28_tm24,
	xy28_tm25,
	xy28_tm26,
	xy28_tm27,

	enXY28_DXDS,	//和值大小单双
	enXY28_Funny,		//趣味 组合
	enXY28_JiZhi,		//极值
}XingYun28Kind;

//北京快8
typedef enum {
	enBJK8_validate,
	enBJK8_RX_1,
	enBJK8_RX_2,
	enBJK8_RX_3,
	enBJK8_RX_4,
	enBJK8_RX_5,
	enBJK8_RX_6,
	enBJK8_RX_7,
	enBJK8_RX_8,
	enBJK8_RX_9,
	enBJK8_RX_10,
	enBJK8_ShangXia=15,
	enBJK8_JiOu=16,
	enBJK8_HeZhiDXDS=17,
}BeiJingKuai8Kind;

//香港六合彩
typedef enum {
	LiuHeCai_validate,
	LiuHeCai_Tmdm=1,
	LiuHeCai_Tmds,
	LiuHeCai_Tmdx,
	LiuHeCai_Tmsx,
	LiuHeCai_Tmbs,
	LiuHeCai_Pmdm,
	LiuHeCai_Zm1t,
	LiuHeCai_Zm2t,
	LiuHeCai_Zm3t,
	LiuHeCai_Zm4t,
	LiuHeCai_Zm5t,
	LiuHeCai_Zm6t
}LiuHeCaiKind;

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
	Zuxuan120,					//组选120
	Zuxuan60,					//组选60
	Zuxuan30,					//组选30
	Zuxuan20,					//组选20
	Zuxuan10,					//组选10
	Zuxuan5,					//组选5
	WuxingQianSi,
	WuxingQianSi_DanShi,	//五星前四单式
	QSZuXuan24,	 //前四组选24
	QSZuXuan12,	 //前四组选12
	QSZuXuan6,	 //前四组选6
	QSZuXuan4,	 //前四组选4	
	WuxingHousi,	
	WuxingHousi_DanShi,		//五星后四单式
	HSZuXuan24,	 //后四组选24
	HSZuXuan12,	 //后四组选12
	HSZuXuan6,	 //后四组选6
	HSZuXuan4,	 //后四组选4
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
	BuDingWei_QianSan,
	BuDingWei_HouSan,
	BuDingWei_QianEr,
	BuDingWei_HouEr,
	DingWeiDan,
	DaXiaoDanShuang_DW,
	DaXiaoDanShuang,//后二大小单双
	Zonghe_DXDSLh,//和值大小单双
	RenXuan4,					//任选四	//任选2,3,4
	RenXuan4_DanShi,			//任选四单式					
	RenXuan3,					//任选三
	RenXuan3_DanShi,			//任选三单式	
	Ren3Zu6_Fushi,				//任3组6复式	
	Ren3Zu6_DanShi,				//任3组6单式
	Ren3Zu3_Fushi,				//任3组3复式	
	Ren3Zu3_DanShi,				//任三组3单式
	Ren3Hunhe_Zuxuan,			//任3混合 组选	
	RenXuan2,					//任二
	RenXuan2_DanShi,			//任二单式
	Ren2Zuxuan_Fushi,			//任2组选 复式	
	Ren2Zuxuan_Danshi,			//任2组选 单式
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
	IIRenXuan1,						//任选一
	IIRenXuan1_DanShi,
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
	IIRenXuan2_DanTuo,				//胆拖
	IIRenXuan3_DanTuo,
	IIRenXuan4_DanTuo,
	IIRenXuan5_DanTuo,
	IIRenXuan6_DanTuo,
	IIRenXuan7_DanTuo,
	IIRenXuan8_DanTuo,
	IIWanFaCount
};

static QiXingCaiKind tagQiXingCai[50]=
{
	QiXingValidate,
	QiXingCai_LiangDing,
	QiXingCai_SanDing,
	QiXingCai_SiDing,
	QiXingCai_YiDing,
	QiXingCai_LiangZiXian,
	QiXingCai_SanZiXian
};

static int tagXingYun28[50]=
{
	xy28_validate,
	xy28_tm0,
	xy28_tm1,
	xy28_tm2,
	xy28_tm3,
	xy28_tm4,
	xy28_tm5,
	xy28_tm6,
	xy28_tm7,
	xy28_tm8,
	xy28_tm9,
	xy28_tm10,
	xy28_tm11,
	xy28_tm12,
	xy28_tm13,
	xy28_tm14,
	xy28_tm15,
	xy28_tm16,
	xy28_tm17,
	xy28_tm18,
	xy28_tm19,
	xy28_tm20,
	xy28_tm21,
	xy28_tm22,
	xy28_tm23,
	xy28_tm24,
	xy28_tm25,
	xy28_tm26,
	NBigNumber,
	NSmallNumber,	
	NBigSingleNumber,		
	NSmallSingleNumber,
	NBigDoubleNumber,
	NSmallDoubleNumber,
	NSingleNumber,
	NDoubleNumber,
	NLimitBig,
	NLimitSmall
};

static BeiJingKuai8Kind tagKuai8[50]=
{
	enBJK8_validate,
	enBJK8_RX_1,
	enBJK8_RX_2,
	enBJK8_RX_3,
	enBJK8_RX_4,
	enBJK8_RX_5,
	enBJK8_RX_6,
	enBJK8_RX_7,
	enBJK8_ShangXia,
	enBJK8_JiOu,
	enBJK8_HeZhiDXDS
};

static LiuHeCaiKind tagLiuHe[50] =
{
	LiuHeCai_validate,
	LiuHeCai_Tmdm,	
	LiuHeCai_Tmds,
	LiuHeCai_Tmsx,
	LiuHeCai_Tmbs,
	LiuHeCai_Pmdm,
	LiuHeCai_Zm1t,
	LiuHeCai_Zm2t,
	LiuHeCai_Zm3t,
	LiuHeCai_Zm4t,
	LiuHeCai_Zm5t,
	LiuHeCai_Zm6t
};

enum xIazhuyShiShiCAIWuType
{ 
	YuanShiShiCAITag = 0,                                   //元
	JiaoShiShiCAITag,                                         //角
	FenShiShiCAITag,                                           //分
	LiShiShiTag
};

#endif //_COMM_H_