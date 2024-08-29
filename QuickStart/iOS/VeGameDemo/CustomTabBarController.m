//
//  CustomTabBarController.m
//  VEPlayerDemo
//
//  Created by Jihua Huang on 2020/9/27.
//  Copyright © 2020 ByteDance Ltd. All rights reserved.
//

#import "CustomTabBarController.h"

#import "VeNavigationController.h"
#import "VePCViewController.h"
#import "VeGameViewController.h"

NSString *const kTabBarPcCustomVCFlag = @"tab_bar_pc_custom_vc";
NSString *const kTabBarGameCustomVCFlag = @"tab_bar_game_custom_vc";

@implementation CustomTabBarController

- (void)viewDidLoad
{
    [super viewDidLoad];

    // 云手游
    UIViewController *gameViewController = nil;
    gameViewController = VIEW_CONTROLLER_FROM_XIB(VeGameViewController);
    VeNavigationController *nav1 = [self navigationControllerWithRootViewController:gameViewController andTabBarControllerIndex:VETabBarControllerIndexGame];

    // PC游戏
    UIViewController *pcViewController = nil;
    pcViewController = VIEW_CONTROLLER_FROM_XIB(VePCViewController);
    VeNavigationController *nav2 = [self navigationControllerWithRootViewController:pcViewController andTabBarControllerIndex:VETabBarControllerIndexPc];

    self.viewControllers = @[ nav1, nav2 ];

    // 设置当前页面支持摇动事件
    [UIApplication sharedApplication].applicationSupportsShakeToEdit = YES;
    [self becomeFirstResponder];
}

- (VeNavigationController *)navigationControllerWithRootViewController:(UIViewController *)rootVc andTabBarControllerIndex:(VETabBarControllerIndex)index
{
    VeNavigationController *nav = [[VeNavigationController alloc] initWithRootViewController:rootVc];
    NSString *title = nil;
    NSString *imageName = nil;
    NSString *selectedImageName = nil;
    switch (index) {
        case VETabBarControllerIndexGame: {
            title = @"云手游";
            imageName = @"tab_bar_mobile_game_normal";
            selectedImageName = @"tab_bar_mobile_game_selected";
        } break;
        case VETabBarControllerIndexPc: {
            title = @"云端游";
            imageName = @"tab_bar_pc_game_normal";
            selectedImageName = @"tab_bar_pc_game_selected";
        } break;
    }
    nav.tabBarItem.title = title;
    nav.tabBarItem.image = [UIImage imageNamed:imageName];
    nav.tabBarItem.selectedImage = [UIImage imageNamed:selectedImageName];

    return nav;
}

- (void)resetTabBarControllerWithViewController:(UIViewController *)viewController withIndex:(VETabBarControllerIndex)index
{
    NSMutableArray *vcArray = [NSMutableArray arrayWithArray:self.viewControllers];
    if (index < vcArray.count) {
        vcArray[index] = [self navigationControllerWithRootViewController:viewController andTabBarControllerIndex:index];
        self.viewControllers = vcArray;
    }
}

// 当前 viewcontroller 是否支持转屏
- (BOOL)shouldAutorotate
{
    return [self.selectedViewController shouldAutorotate];
}

// 当前 viewcontroller 支持哪些转屏方向
- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return [self.selectedViewController supportedInterfaceOrientations];
}

// 当前 viewcontroller 默认的屏幕方向
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return [self.selectedViewController preferredInterfaceOrientationForPresentation];
}

@end
