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

@interface VeGameViewController ()

@property (weak, nonatomic) IBOutlet UILabel *versionLabel;
@property (weak, nonatomic) IBOutlet UITextField *akTextField;
@property (weak, nonatomic) IBOutlet UITextField *skTextField;
@property (weak, nonatomic) IBOutlet UITextField *tokenTextField;
@property (weak, nonatomic) IBOutlet UITextField *userIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *gameIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *roundIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *rotationTextField;

@end

@implementation VeGameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationItem.title = @"云游戏演示";
    
    self.rotationTextField.text = @"270";
    self.roundIdTextField.text = [NSString stringWithFormat: @"round_id_%@", self.userIdTextField.text];
    
    // 版本号
    self.versionLabel.text = [NSString stringWithFormat: @"VeGameSDK版本: V%@\nDeviceId: %@", [VeGameManager currentVersion], [VeGameManager currentDeviceId]];
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
    // 显示控制器
    VeGameDisplayViewController *gameDisplayVc = [[VeGameDisplayViewController alloc] init];
    VeCloudGameConfigObject *obj = [[VeCloudGameConfigObject alloc] init];
    obj.ak = self.akTextField.text;
    obj.sk = self.skTextField.text;
    obj.token = self.tokenTextField.text;
    obj.userId = self.userIdTextField.text;
    obj.gameId = self.gameIdTextField.text;
    obj.roundId = self.roundIdTextField.text;
    obj.rotation = self.rotationTextField.text.integerValue;
    gameDisplayVc.configObj = obj;
    [self.navigationController pushViewController: gameDisplayVc animated: YES];
}

@end
