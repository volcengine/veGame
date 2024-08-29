//
//  AppDelegate.m
//  VeGamePublicDemo
//
// Copyright (c) 2022 Volcengine
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#import "AppDelegate.h"
#import "VeGame/VeGameManager.h"
#import "VeGameViewController.h"
#import "VePCViewController.h"
#import "VeNavigationController.h"
#import <SVProgressHUD/SVProgressHUD.h>
#import "CustomTabBarController.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame: [UIScreen mainScreen].bounds];
    CustomTabBarController *tabBarController = [[CustomTabBarController alloc] init];
    // tabBar 选中的 item 的图标的颜色
    tabBarController.tabBar.tintColor = UICOLOR_RGB(0x455078);
    // 分割线
    UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, -0.5, SCREEN_WIDTH, 0.5)];
    view.backgroundColor = UICOLOR_RGB(0xe0e0e0);
    [tabBarController.tabBar insertSubview:view atIndex:0];

    tabBarController.selectedIndex = 0;
    self.window.rootViewController = tabBarController;
    [self.window makeKeyAndVisible];

    [self configNavigationController];
    
    [self configProgressHud];
    
    [self configVeGame];
    
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

    return YES;
}

- (void)configNavigationController
{
    if (@available(iOS 15.0, *)) {
        UINavigationBarAppearance *appearance = [[UINavigationBarAppearance alloc] init];
        [appearance configureWithOpaqueBackground];
        appearance.backgroundColor = UIColor.clearColor;
        [appearance setTitleTextAttributes: @{NSForegroundColorAttributeName: [UIColor blackColor], NSFontAttributeName: [UIFont boldSystemFontOfSize: 18.0f]}];
        [[UINavigationBar appearance] setScrollEdgeAppearance: appearance];
        [[UINavigationBar appearance] setStandardAppearance: appearance];
    } else {
        [[UINavigationBar appearance] setBarStyle: UIBarStyleDefault];
        [[UINavigationBar appearance] setShadowImage: [UIImage new]];
        [[UINavigationBar appearance] setBarTintColor: [UIColor whiteColor]];
        [[UINavigationBar appearance] setBackgroundImage: [Utils imageFromColor: [UIColor whiteColor]] forBarMetrics: UIBarMetricsDefault];
        [[UINavigationBar appearance] setTitleTextAttributes: @{NSForegroundColorAttributeName: [UIColor blackColor], NSFontAttributeName: [UIFont boldSystemFontOfSize: 18.0f]}];
    }
}

- (void)configProgressHud
{
    [SVProgressHUD setDefaultStyle: SVProgressHUDStyleDark];
    [SVProgressHUD setDefaultMaskType: SVProgressHUDMaskTypeNone];
    [SVProgressHUD setDefaultAnimationType: SVProgressHUDAnimationTypeNative];
}

- (void)configVeGame
{
    [[VeGameManager sharedInstance] initWithAccountId:@"-----AccountID-----"];
}

@end
