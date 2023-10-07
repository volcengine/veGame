//
//  AppDelegate.m
//  SampleCode
//
//  Created by zhaozuotong on 2023/9/21.
//

#import "AppDelegate.h"
#import "VeRootViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    self.window = [[UIWindow alloc] initWithFrame: [UIScreen mainScreen].bounds];
    VeRootViewController *rootVC = [[VeRootViewController alloc] init];
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController: rootVC];
    self.window.rootViewController = nav;
    [self.window makeKeyAndVisible];
    return YES;
}

@end
