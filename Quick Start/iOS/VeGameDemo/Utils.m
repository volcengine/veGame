//
//  Utils.m
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

#import "Utils.h"

@implementation Utils

+ (BOOL)iPhoneXSeries
{
    BOOL result = NO;
    if (UIDevice.currentDevice.userInterfaceIdiom != UIUserInterfaceIdiomPhone) {
        return result;
    }
    if (@available(iOS 11.0, *)) {
        UIWindow *mainWindow = [[[UIApplication sharedApplication] delegate] window];
        if (mainWindow.safeAreaInsets.bottom > 0.0f) {
            result = YES;
        }
    }
    return result;
}

+ (void)rotateDeviceToOrientation:(NSInteger)rotation
{
    UIDeviceOrientation orientation = (rotation == 90 || rotation == 270) ? UIDeviceOrientationLandscapeRight : UIDeviceOrientationPortrait;
    [[UIDevice currentDevice] setValue: @(UIDeviceOrientationUnknown) forKey: @"orientation"];
    [[UIDevice currentDevice] setValue: @(orientation) forKey: @"orientation"];
}

+ (UIImage *)imageFromColor:(UIColor *)color
{
    return [Utils imageFromColor:color withSize: CGSizeMake(10, 10)];
}

+ (UIImage *)imageFromColor:(UIColor *)color withSize:(CGSize)size
{
    CGSize imageSize = size;
    UIGraphicsBeginImageContextWithOptions(imageSize, 0, [UIScreen mainScreen].scale);
    [color set];
    UIRectFill(CGRectMake(0, 0, imageSize.width, imageSize.height));
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}

+ (UIViewController *)getCurrentViewController
{
    UIViewController *rootViewController = ([UIApplication sharedApplication].delegate).window.rootViewController;
    if ([rootViewController isKindOfClass: [UITabBarController class]]) {
        UITabBarController *tabBarController = (UITabBarController *)rootViewController;
        UINavigationController *navigationController = tabBarController.selectedViewController;
        if ([navigationController isKindOfClass: [UINavigationController class]]) {
            return navigationController.topViewController;
        }
    }
    return nil;
}

@end
