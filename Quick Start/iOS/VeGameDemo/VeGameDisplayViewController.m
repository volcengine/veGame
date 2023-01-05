//
//  VeGameDisplayViewController.m
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

#import "Masonry.h"
#import <Toast/Toast.h>
#import <VeGame/VeGame.h>
#import <SVProgressHUD/SVProgressHUD.h>
#import "VeGameDisplayViewController.h"

@implementation VeCloudGameConfigObject

@end

@interface VeGameDisplayViewController () <VeGameManagerDelegate>

@property (nonatomic, assign) NSInteger rotation;
@property (nonatomic, strong) UIView *containerView;

@end

@implementation VeGameDisplayViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.hidesBottomBarWhenPushed = YES;
    self.view.backgroundColor = [UIColor blackColor];
    
    self.rotation = self.configObj.rotation;
    
    [self configUI];
    [SVProgressHUD showWithStatus: @"正在启动..."];
    // 初始化云游戏实例
    [VeGameManager sharedManagerWithContainerView: self.containerView delegate: self];
    // 配置信息
    VeGameConfigObject *configObj = [VeGameConfigObject new];
    configObj.ak = self.configObj.ak;
    configObj.sk = self.configObj.sk;
    configObj.token = self.configObj.token;
    configObj.userId = self.configObj.userId;
    configObj.gameId = self.configObj.gameId;
    configObj.roundId = self.configObj.roundId;
    // 启动
    [[VeGameManager sharedInstance] startWithConfig: configObj];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear: animated];

    [self.navigationController setNavigationBarHidden: YES animated: YES];
    self.navigationController.interactivePopGestureRecognizer.enabled = NO;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear: animated];

    [self.navigationController setNavigationBarHidden: NO animated: YES];
    self.navigationController.interactivePopGestureRecognizer.enabled = YES;
}

- (UIStatusBarStyle)preferredStatusBarStyle
{
    return UIStatusBarStyleLightContent;
}

- (void)configUI
{
    // 容器视图
    self.containerView = ({
        UIView *containerView = [[UIView alloc] init];
        [self.view addSubview: containerView];
        [containerView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.top.right.bottom.mas_equalTo(0);
        }];
        containerView;
    });
    
    // 退出
    UIButton *exitBtn = [UIButton buttonWithType: UIButtonTypeCustom];
    exitBtn.layer.borderWidth = 1.0f;
    exitBtn.layer.borderColor = [UIColor whiteColor].CGColor;
    exitBtn.layer.cornerRadius = 5.0f;
    exitBtn.titleLabel.font = [UIFont systemFontOfSize: 14.0f];
    [exitBtn setTitle: @"关闭" forState: UIControlStateNormal];
    [exitBtn setTitleColor: [UIColor whiteColor] forState: UIControlStateNormal];
    [exitBtn addTarget: self action: @selector(tappedExitButton:) forControlEvents: UIControlEventTouchUpInside];
    [self.view addSubview: exitBtn];
    [exitBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(self.view).offset(50.0f);
        make.left.mas_equalTo(self.view).offset(30.0f);
        make.size.mas_equalTo(CGSizeMake(50.0f, 30.0f));
    }];
}

#pragma mark - VeGameDisplayViewControllerDelegate

- (void)gameDisplayViewDidStopGame
{
    [SVProgressHUD dismiss];
    [[VeGameManager sharedInstance] stop];
    [self.navigationController popViewControllerAnimated: YES];
}

#pragma mark - VeGameManagerDelegate

- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId extra:(NSDictionary *)extra
{
    [SVProgressHUD dismiss];
}

- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation
{
    [self setRotation: rotation];
}

- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errorCode
{
    [SVProgressHUD dismiss];
    
    [self.view makeToast: [NSString stringWithFormat: @"Error Code: %ld", errorCode]
                duration: 2.0f
                position: CSToastPositionCenter];
}

#pragma mark - button action

- (void)tappedExitButton:(UIButton *)btn
{
    [SVProgressHUD dismiss];
    [[VeGameManager sharedInstance] stop];
    [self.navigationController popViewControllerAnimated: YES];
}

#pragma mark - setter

- (void)setRotation:(NSInteger)rotation
{
    if (_rotation != rotation) {
        
        _rotation = rotation;
        
        [Utils rotateDeviceToOrientation: rotation];
    }
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    UIInterfaceOrientationMask mask = UIInterfaceOrientationMaskPortrait;
    if (self.rotation == 270) {
        mask = UIInterfaceOrientationMaskLandscapeRight;
    }
    return mask;
}

- (void)dealloc
{
    NSLog(@"--- VeGameDisplayViewController Dealloc ---");
}


@end
