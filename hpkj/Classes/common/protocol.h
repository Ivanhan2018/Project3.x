#ifndef _protocol_h
#define _protocol_h

#ifdef DEBUG
#define DebugLog(format, ...)                   //NSLog(format,## __VA_ARGS__)
#else
#define DebugLog(format, ...)
#endif

// 条件输出
#ifdef DEBUG
#define DebugAssert(condition,format, ...)		if(!(condition)) NSLog(format,## __VA_ARGS__)
#else
#define DebugAssert(condition,format, ...)
#endif

// false输出
#define DebugReleaseAssert(condition,format, ...) if(!(condition)) NSLog(format,## __VA_ARGS__)

// 文件跟踪输出
#ifdef DEBUG
#define DebugFileLine(format)			//NSLog(@"%s %d				%@",__FILE__,__LINE__,format)
#else
#define DebugFileLine(format)
#endif

// FrameDate跟踪输出
#ifdef DEBUG
#define DebugFrameDate(format)			//NSLog(@"====================FrameDate:	%d	%@", __LINE__, format)
#else
#define DebugFrameDate(format)
#endif

// FramePlaza跟踪输出
#ifdef DEBUG
#define DebugFramePlaza(format)			//NSLog(@"====================FramePlaza:	%d					%@",__LINE__,format)
#else
#define DebugFramePlaza(format)
#endif

// 游戏跟踪输出
#ifdef DEBUG
#define DebugGameLog(format)			//NSLog(@"CGameClient：%d	 %@",__LINE__,format)
#else
#define DebugGameLog(format)
#endif

// 点
#define Vec2(__X__,__Y__)				CGPointMake(__X__,__Y__)
#define ccpZero							CGPointZero
#define ccpMax							Vec2(1.0f, 1.0f)
#define ccpHalf(size)					Vec2(size.width/2,size.height/2)
#define ccpAdd(p1,p2)                   Vec2(p1.x+p2.x,p1.y+p2.y)
#define ccpSub(p1,p2)                   Vec2(p1.x-p2.x,p1.y-p2.y)

#define ccpDot(p1,p2)                   p1.x*p2.x+p1.y*p2.y
#define ccpLength(p)                    sqrt(ccpDot(p,p))
#define ccpDistance(p1,p2)              ccpLength(ccpSub(p1,p2))

// 矩形
#define ccr(__X__,__Y__,__W__,__H__)	CGRectMake(__X__,__Y__,	__W__,__H__)
#define ccrZero							CGRectZero
#define ccr2(p, s)						ccr(p.x, p.y, s.width, s.height)
#define ccsiPhoneV                      ccs(320, 480)   // 竖屏 Vertical screen
#define ccsiPhoneH                      ccs(480, 320)   // 横屏 Horizontal screen
#define ccsiPhoneH5                     ccs(568, 320)
#define ccsiPhoneV5                     ccs(320, 568)
#define ccsiPadV                        ccs(768, 1024)
#define ccsiPadH                        ccs(1024, 768)

// size
#define ccs(__W__, __H__)				CGSizeMake(__W__, __H__)
#define ccsZero							CGSizeZero
#define ccsScale(size, scale)            ccs(size.width*scale, size.height*scale)
#define ccsAdd(s1, s2)                   ccs(s1.width+s2.width, s1.height+s2.height)
#define ccsSub(s1, s2)                   ccs(s1.width-s2.width, s1.height-s2.height)

// range
#define between(num, min, max)          ((num<max)&&(num>min))
#define betweenCMin(num,min,max)        ((num<max)&&(num>=min))
#define betweenCMax(num,min,max)        ((num<=max)&&(num>min))
#define betweenCMinMax(num,min,max)     ((num<=max)&&(num>=min))

// 设备宽高  竖屏模式下
#define P_SCREEN                        [[UIScreen mainScreen] bounds]

// 点size转换
#define ccpWithSize(size)               CGPointMake(size.width,size.height)
#define ccsWithPoint(point)             CGSizeMake(point.x,point.y)

// 兼容PC函数
#define ASSERT(e)                       assert(e)
#define CopyMemory(des,src,size_t)      memcpy(des,src,size_t)

// 角度转换
#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f)	// PI * 180

// 键盘值
#define	KEY_ABC                     @"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define KEY_NUMBER                  @"0123456789"
#define KEY_ABC_NUMBER              @"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define KEY_NUMBER_SPERIOD          @"0123456789."


// 显示标识 DP = display
typedef int DP_TYPE;

#define     DP_DELETE               0       // 删除
#define     DP_SHOW                 1       // 显示
#define     DP_SPECIAL_SHOW         2       // 指定显示
#define     DP_SPECIAL_DELETE       3       // 指定删除
#define     DP_SKIP                 4       // 跳过

// 设备标识缩放
#define IPHONE_NORMAL               1.0f
#define IPHONE_RETINA               0.5f
#define IPAD_NORMAL                 1.0f
#define IPAD_RETINA                 0.5f

// 字体库
#define FONT_ARIAL                  @""
#define FONT_ARIAL_HOLD             @"Arial-BoldMT"
#define FONT_HELVETICANEUE          @"Helvetica Neue"
#define FONT_HELVETIBOLD            @"Helvetica-Bold"
#define FONT_OPTIMA_EXTRABLACE      @"Optima-ExtraBlack"


#endif
