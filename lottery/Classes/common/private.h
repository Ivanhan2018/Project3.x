/*
 
 私有
 给已有类添加私有方法
 
 */

#import "define.h"
#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonDigest.h>

#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>

#pragma mark -
#pragma mark NSString(NSStringMD5)
@interface NSString(NSStringMD5)
//旧
//创建md5string
+(NSString*)CreateMD5WithString:(NSString*)str;
//tchar转换string
+(NSString*)CreateStringWithTHCAR:(TCHART*)tchar size:(int)size;
//string转换tchar
+(void)CreateTCHARWithString:(NSString*)string size:(int)size TCHART:(TCHART*)tchar;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//新
//doc完整路径
+(NSString*)listBasePath:(NSString*)filenme;
//资源完整路径
+(NSString*)txtBasePath:(NSString*)filename ofType:(NSString*)txt;
+(NSString*)fullResourcePath:(NSString*)file ofType:(NSString*)ext;

//tchar    -> NSString
+(NSString*)getStringWithTCHAR:(UniChar*)tchar  size:(int)size;
//NSString -> MD5 NSString
+(NSString*)createMD5StringWithString:(NSString*)string;
//NSString -> TCHART
+(void)createTCHARWithString:(NSString*)string dest:(UniChar*)tchar;
//NSString -> MD5 TCHART
+(void)createMD5TCHARWithString:(NSString*)string dest:(UniChar*)tchar;
@end

@implementation NSString(NSStringMD5)
//旧
//创建md5string
+(NSString*)CreateMD5WithString:(NSString*)str 
{
	const char* cStr = [str UTF8String];
	u_char result[16];
	CC_MD5(cStr,strlen(cStr),result);
	
	NSString* digest=[NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
					  result[0], result[1], result[2], result[3],
					  result[4], result[5], result[6], result[7],
					  result[8], result[9], result[10],result[11],
					  result[12],result[13],result[14],result[15]];
	return digest;
}
//tchar转换string
+(NSString*)CreateStringWithTHCAR:(TCHART*)tchar size:(int)size
{
	NSMutableString *string = [NSMutableString stringWithCapacity:1];
	
	for (int i=0; i<size/(int)sizeof(TCHART); ++i)
	{
		if (!tchar[i]) break;
		[string appendFormat:@"%C",tchar[i]];
	}
	return string;
}
//string转换tchar
+(void)CreateTCHARWithString:(NSString*)string size:(int)size TCHART:(TCHART*)tchar 
{
	for(int i=0; i<size; i++)
		tchar[i]=[string characterAtIndex:i];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//返回list完成路径
+(NSString*)listBasePath:(NSString*)filenme;
{
	NSString *pathname = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	return [pathname stringByAppendingPathComponent:filenme];
}

// 返回引用txt完整路
+(NSString*)txtBasePath:(NSString*)filename ofType:(NSString*)txt
{
	return [[NSBundle mainBundle] pathForResource:filename ofType:txt];
}

//资源完整路径
+(NSString*)fullResourcePath:(NSString*)file ofType:(NSString*)ext{
    return [[NSBundle mainBundle] pathForResource:file ofType:ext];
}

//tchar -> NSString
+(NSString*)getStringWithTCHAR:(UniChar*)tchar size:(int)size
{
	NSMutableString *string = [NSMutableString stringWithCapacity:0];
	for (int i=0; i<size/(int)sizeof(UniChar); ++i)
	{
		if (!tchar[i]) break;
		[string appendFormat:@"%C",tchar[i]];
	}
	return string;
}

//NSString -> MD5 NSString
+(NSString*)createMD5StringWithString:(NSString*)string
{
    const char* cStr = [string UTF8String];
	u_char result[16];
	CC_MD5(cStr,strlen(cStr),result);
	
	NSString* digest=[NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
					  result[0], result[1], result[2], result[3],
					  result[4], result[5], result[6], result[7],
					  result[8], result[9], result[10],result[11],
					  result[12],result[13],result[14],result[15]];
	return digest;
}

//NSString -> TCHART
+(void)createTCHARWithString:(NSString*)string dest:(UniChar*)tchar
{
    for(int i=0; i<[string length]; i++)
    {
		tchar[i]=[string characterAtIndex:i];
    }
}

//NSString -> MD5 TCHART
+(void)createMD5TCHARWithString:(NSString*)string dest:(UniChar*)tchar
{
    NSString* md5Str=[NSString createMD5StringWithString:string];
    [NSString createTCHARWithString:md5Str dest:tchar];
}

@end


#pragma mark -
#pragma mark 字典方法
@interface NSMutableDictionary(CGPoint)
//CGPoint点
-(void)setObjectPoint:(CGPoint)anPoint forKey:(NSString*)aKey pointIndex:(int)index;
- (CGPoint)objectPointForKey:(NSString*)aKey pointIndex:(int)index;

//CGRect
-(void)setObjectRect:(CGRect)rect forKey:(NSString*)key;
-(CGRect)objectRectForKey:(NSString*)key;

@end

@implementation NSMutableDictionary(CGPoint)
//CGPoint点
-(void)setObjectPoint:(CGPoint)anPoint forKey:(NSString*)aKey pointIndex:(int)index
{
	[self setObject:[NSValue valueWithCGPoint:anPoint] forKey:[NSString stringWithFormat:@"%@_%d",aKey,index]];
}
- (CGPoint)objectPointForKey:(NSString*)aKey pointIndex:(int)index
{
	CGPoint point;[[self objectForKey:[NSString stringWithFormat:@"%@_%d",aKey,index]] getValue:&point];
	return	point;
}

//CGRect
-(void)setObjectRect:(CGRect)rect forKey:(NSString*)key
{
	[self setObject:[NSValue valueWithCGRect:rect] forKey:key];
}
-(CGRect)objectRectForKey:(NSString*)key
{
	CGRect rect;[[self objectForKey:key] getValue:&rect];
	return rect;
}
@end

#pragma mark -
#pragma mark UIDevice(UDID)
@interface UIDevice(UDID)
-(NSString *) macaddress;
/*
 * @method uniqueDeviceIdentifier
 * @description use this method when you need a unique identifier in one app.
 * It generates a hash from the MAC-address in combination with the bundle identifier
 * of your app.
 */
+(NSString *) uniqueDeviceIdentifier;

/*
 * @method uniqueGlobalDeviceIdentifier
 * @description use this method when you need a unique global identifier to track a device
 * with multiple apps. as example a advertising network will use this method to track the device
 * from different apps.
 * It generates a hash from the MAC-address only.
 */
+(NSString *) uniqueGlobalDeviceIdentifier;
@end

@implementation UIDevice (UDID)

////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Private Methods

// Return the local MAC addy
// Courtesy of FreeBSD hackers email list
// Accidentally munged during previous update. Fixed thanks to erica sadun & mlamb.
- (NSString *) macaddress{
    
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        NSAssert(0,@"Error: if_nametoindex error\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        NSAssert(0,@"Error: sysctl, take 1\n");
        return NULL;
    }
    
    if ((buf=(char*)malloc(len)) == NULL) {
		NSAssert(0,@"Could not allocate memory. error!\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        NSAssert(0,@"Error: sysctl, take 2");
        free(buf);
        return NULL;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
	
    NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    free(buf);
    
    return outstring;
}

////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Public Methods

+(NSString *) uniqueDeviceIdentifier
{
    NSString *macaddress = [[UIDevice currentDevice] macaddress];
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    
    NSString *stringToHash = [NSString stringWithFormat:@"%@%@", macaddress, bundleIdentifier];
    
    return [NSString createMD5StringWithString:stringToHash];
}

+(NSString *) uniqueGlobalDeviceIdentifier
{
    NSString *macaddress = [[UIDevice currentDevice] macaddress];
    
    return [NSString createMD5StringWithString:macaddress];
}

@end



