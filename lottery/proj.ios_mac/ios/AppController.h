#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;

}

+(void)changeRootViewControllerH;//静态方法(修改屏幕为横屏)
+(void)changeRootViewControllerV;//静态方法(修改屏幕为竖屏)

@property(nonatomic, readonly) RootViewController* viewController;

@end

