//
//  VeGameViewController.m
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
#import <Toast/Toast.h>
#import <VeGame/VeGame.h>
#import "VeGameViewController.h"
#import "VeGameDisplayViewController.h"
#import <SVProgressHUD/SVProgressHUD.h>

@interface VeGameViewController () <VeGameManagerDelegate, VeGameDisplayViewControllerDelegate>

@property (weak, nonatomic) IBOutlet UILabel *versionLabel;
@property (weak, nonatomic) IBOutlet UITextField *akTextField;
@property (weak, nonatomic) IBOutlet UITextField *skTextField;
@property (weak, nonatomic) IBOutlet UITextField *tokenTextField;
@property (weak, nonatomic) IBOutlet UITextField *userIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *gameIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *roundIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *rotationTextField;
@property (nonatomic, strong) VeGameDisplayViewController *gameDisplayVc;

@end

@implementation VeGameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.navigationItem.title = @"云游戏演示";
    
    self.akTextField.text = @"";
    self.skTextField.text = @"";
    self.tokenTextField.text = @"";
    
    self.gameIdTextField.text = @"";
    self.userIdTextField.text = @"888765";
    self.rotationTextField.text = @"270"; // 竖屏：0 横屏：270
    self.roundIdTextField.text = [NSString stringWithFormat: @"round_id_%@", self.userIdTextField.text];
    
    // 版本号
    self.versionLabel.text = [NSString stringWithFormat: @"VeGameSDK 版本: V%@", [VeGameManager currentVersion]];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear: animated];

    [Utils rotateDeviceToOrientation: UIDeviceOrientationPortrait];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing: YES];
}

- (IBAction)tappedStartGameButton:(UIButton *)sender
{
    if (self.akTextField.text.length == 0 || self.skTextField.text.length == 0 || self.tokenTextField.text.length == 0 || self.userIdTextField.text.length == 0 || self.gameIdTextField.text.length == 0 || self.roundIdTextField.text.length == 0) {
        NSString *str = @"";
        if (self.akTextField.text.length == 0) {
            str = @"Ak不能为空";
        } else if (self.skTextField.text.length == 0) {
            str = @"Sk不能为空";
        } else if (self.tokenTextField.text.length == 0) {
            str = @"Token不能为空";
        } else if (self.userIdTextField.text.length == 0) {
            str = @"UserId不能为空";
        } else if (self.gameIdTextField.text.length == 0) {
            str = @"GameId不能为空";
        } else if (self.roundIdTextField.text.length == 0) {
            str = @"RoundId不能为空";
        }
        [self.view makeToast: str duration: 2.0f position: CSToastPositionCenter];
        return;
    }
    
    [SVProgressHUD showWithStatus: @"正在启动云游戏..."];
    // 显示控制器
    [self.navigationController pushViewController: self.gameDisplayVc animated: YES];
    [self.gameDisplayVc setRotation: self.rotationTextField.text.integerValue];
    // 初始化云游戏实例
    [VeGameManager sharedManagerWithContainerView: self.gameDisplayVc.containerView delegate: self];
    // 配置信息
    VeGameConfigObject *configObj = [VeGameConfigObject new];
    configObj.ak = self.akTextField.text;
    configObj.sk = self.skTextField.text;
    configObj.token = self.tokenTextField.text;
    configObj.userId = self.userIdTextField.text;
    configObj.gameId = self.gameIdTextField.text;
    configObj.roundId = self.roundIdTextField.text;
    // 启动
    [[VeGameManager sharedInstance] startWithConfig: configObj];
}

#pragma mark - VeGameDisplayViewControllerDelegate

- (void)gameDisplayViewDidStopGame
{
    [SVProgressHUD dismiss];
    
    [[VeGameManager sharedInstance] stop];
    [self.navigationController popViewControllerAnimated: YES];
    self.gameDisplayVc = nil;
}

#pragma mark - VeGameManagerDelegate

- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId extra:(NSDictionary *)extra
{
    [SVProgressHUD dismiss];
}

- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation
{
    [self.gameDisplayVc setRotation: rotation];
}

- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errorCode
{
    [SVProgressHUD dismiss];
    
    [self.gameDisplayVc.view makeToast: [NSString stringWithFormat: @"Error Code: %ld", errorCode]
                              duration: 2.0f
                              position: CSToastPositionCenter];
}

#pragma mark - getter

- (VeGameDisplayViewController *)gameDisplayVc
{
    if (_gameDisplayVc == nil) {
        _gameDisplayVc= [[VeGameDisplayViewController alloc] initWithDelegate: self];
    }
    return _gameDisplayVc;
}

@end
