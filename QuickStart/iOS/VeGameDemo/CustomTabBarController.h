//
//  CustomTabBarController.h
//  VEPlayerDemo
//
//  Created by Jihua Huang on 2020/9/27.
//  Copyright © 2020 ByteDance Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const kTabBarPcCustomVCFlag;
extern NSString *const kTabBarGameCustomVCFlag;

typedef NS_ENUM(NSUInteger, VETabBarControllerIndex) {
    /** 云游戏 */
    VETabBarControllerIndexGame = 0,
    /** 云端游 */
    VETabBarControllerIndexPc = 2,
};

@interface CustomTabBarController : UITabBarController

- (void)resetTabBarControllerWithViewController:(UIViewController *)viewController withIndex:(VETabBarControllerIndex)index;

@end
