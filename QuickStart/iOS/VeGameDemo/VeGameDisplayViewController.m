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
#import <CommonCrypto/CommonDigest.h>
#import "UIView+Draggable.h"
#import "CustomViewController.h"

@implementation VeCloudGameConfigObject

@end

@interface VeGameDisplayViewController () <VeGameManagerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>

@property (nonatomic, assign) BOOL alreadyStart;
@property (nonatomic, assign) NSInteger rotation;
@property (nonatomic, strong) UIView *localVideoView;
@property (nonatomic, strong) UILabel *timelylogLabel;
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, copy) NSString *operationDelayTime;
@property (nonatomic, strong) UILabel *netProbeStatsLabel;
@property (nonatomic, strong, readwrite) UIView *containerView;

@end

@implementation VeGameDisplayViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self configSubView];
    
    self.rotation = self.configObj.rotation;
    [VeGameManager sharedInstance].delegate = self;
    [VeGameManager sharedInstance].containerView = self.containerView;
    // 附加信息
    [[VeGameManager sharedInstance] setExtraParameters: @{
    }];
    if (NO) { // 网络探测
        [SVProgressHUD showWithStatus: @"正在进行网络探测..."];
        VeGameConfigObject *configObj = [VeGameConfigObject new];
        configObj.ak = self.configObj.ak;
        configObj.sk = self.configObj.sk;
        configObj.token = self.configObj.token;
        configObj.userId = self.configObj.userId;
        [[VeGameManager sharedInstance] probeStart: configObj];
    } else {
        [self startGame];
    }
    
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(receiveAppWillTerminateNotification:)
                                                 name: UIApplicationWillTerminateNotification
                                               object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(receiveAppDidEnterBackgroundNotification:)
                                                 name: UIApplicationDidEnterBackgroundNotification
                                               object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(receiveAppWillEnterForegroundNotification:)
                                                 name: UIApplicationWillEnterForegroundNotification
                                               object: nil];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear: animated];

    [self.navigationController setNavigationBarHidden: YES animated: YES];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear: animated];

    [self.navigationController setNavigationBarHidden: NO animated: YES];
}

- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    
    UIButton *menuBtn = [self.view viewWithTag: 999];
    if (menuBtn.draggingType == DraggingTypeDisabled) {
        menuBtn.draggingType = DraggingTypePullOver;
    }
}

- (UIStatusBarStyle)preferredStatusBarStyle
{
    return UIStatusBarStyleLightContent;
}

- (void)configSubView
{
    // 画布
    self.containerView = ({
        UIView *containerView = [[UIView alloc] init];
        containerView.backgroundColor = [UIColor blackColor];
        [self.view addSubview: containerView];
        [containerView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.top.right.bottom.mas_equalTo(0);
        }];
        containerView;
    });
    
    // 本地视频采集视图
    self.localVideoView = ({
        UIView *localVideoView = [[UIView alloc] init];
        localVideoView.hidden = YES;
        localVideoView.backgroundColor = [UIColor blackColor];
        [self.view addSubview: localVideoView];
        [localVideoView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.right.mas_equalTo(self.view).offset(-10.0f);
            make.size.mas_equalTo(CGSizeMake(150.0f, 200.0f));
            make.top.mas_equalTo(self.view.mas_top).offset(44);
        }];
        localVideoView;
    });
    
    // 网络探测数据
    self.netProbeStatsLabel = ({
        UILabel *label = [[UILabel alloc] init];
        label.numberOfLines = 0;
        label.userInteractionEnabled = NO;
        label.textColor = [UIColor yellowColor];
        label.textAlignment = NSTextAlignmentLeft;
        label.font = [UIFont systemFontOfSize: 13];
        [self.view addSubview: label];
        [label mas_makeConstraints:^(MASConstraintMaker *make) {
            make.right.mas_equalTo(self.view).offset(-10.0f);
            make.top.mas_equalTo(self.view).offset(44);
        }];
        label;
    });
    
    // 网络探测按钮
    UIButton *netProbeOkBtn = [self createButton: @""];
    netProbeOkBtn.tag = 888;
    netProbeOkBtn.hidden = YES;
    [self.view addSubview: netProbeOkBtn];
    [netProbeOkBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(CGSizeMake(35, 20));
        make.right.mas_equalTo(self.netProbeStatsLabel);
        make.top.mas_equalTo(self.netProbeStatsLabel.mas_bottom).offset(5.0f);
    }];
    
    // 实时日志
    self.timelylogLabel = ({
        UILabel *label = [[UILabel alloc] init];
        label.userInteractionEnabled = NO;
        label.textColor = [UIColor greenColor];
        label.textAlignment = NSTextAlignmentCenter;
        label.font = [UIFont systemFontOfSize: 11.0f];
        label.backgroundColor = [UIColor grayColor];
        label.layer.masksToBounds = YES;
        label.layer.cornerRadius = 6.0f;
        label.adjustsFontSizeToFitWidth = YES;
        [self.view addSubview: label];
        [label mas_makeConstraints:^(MASConstraintMaker *make) {
            make.centerX.mas_equalTo(self.view);
            make.bottom.mas_equalTo(self.view).offset(-34);
        }];
        label;
    });
    
    // 菜单
    UIButton *menuBtn = [UIButton buttonWithType: UIButtonTypeCustom];
    menuBtn.tag = 999;
    menuBtn.layer.cornerRadius = 14.0f;
    menuBtn.backgroundColor = [UIColor redColor];
    menuBtn.titleLabel.font = [UIFont systemFontOfSize: 9.0f];
    [menuBtn setTitle: @"Menu" forState: UIControlStateNormal];
    [menuBtn setTitleColor: [UIColor yellowColor] forState: UIControlStateNormal];
    [menuBtn addTarget: self action: @selector(tappedButton:) forControlEvents: UIControlEventTouchUpInside];
    [self.view addSubview: menuBtn];
    [menuBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.mas_equalTo(self.view);
        make.size.mas_equalTo(CGSizeMake(28, 28));
        make.bottom.mas_equalTo(self.view).offset(-100);
    }];
    
    self.scrollView = ({
        UIScrollView *scrollView = [[UIScrollView alloc] init];
        scrollView.hidden = YES;
        scrollView.showsVerticalScrollIndicator = NO;
        [self.view addSubview: scrollView];
        [scrollView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.width.mas_equalTo(170);
            make.left.mas_equalTo(self.view).offset(10.0f);
            make.top.mas_equalTo(self.view).offset(44);
            make.bottom.mas_equalTo(self.view).offset(-34);
        }];
        scrollView;
    });
    
    UIView *btnView = [[UIView alloc] init];
    [self.scrollView addSubview: btnView];
    [btnView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.width.mas_equalTo(self.scrollView);
        make.top.left.right.bottom.mas_equalTo(self.scrollView);
    }];
    
    UIButton *button0 = [self createButton: @"退出"];
    button0.tag = 100;
    button0.backgroundColor = [UIColor redColor];
    [button0 setTitleColor: [UIColor yellowColor] forState: UIControlStateNormal];
    [btnView addSubview: button0];
    [button0 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(btnView);
        make.left.mas_equalTo(btnView);
        make.size.mas_equalTo(CGSizeMake(80, 40));
    }];
    
    UIButton *button1 = [self createButton: @"清晰度"];
    button1.tag = 101;
    [btnView addSubview: button1];
    [button1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button0);
        make.top.mas_equalTo(button0);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button2 = [self createButton: @"发送图片"];
    button2.tag = 102;
    [btnView addSubview: button2];
    [button2 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button0);
        make.left.mas_equalTo(button0);
        make.top.mas_equalTo(button0.mas_bottom).offset(10);
    }];
    
    UIButton *button3 = [self createButton: @"设置键盘开关"];
    button3.tag = 103;
    [btnView addSubview: button3];
    [button3 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button2);
        make.top.mas_equalTo(button2);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button4 = [self createButton: @"获取键盘开关"];
    button4.tag = 104;
    [btnView addSubview: button4];
    [button4 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button2);
        make.left.mas_equalTo(button2);
        make.top.mas_equalTo(button2.mas_bottom).offset(10);
    }];
    
    UIButton *button5 = [self createButton: @"重启游戏"];
    button5.tag = 105;
    [btnView addSubview: button5];
    [button5 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button4);
        make.top.mas_equalTo(button4);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button6 = [self createButton: @"切换远端游戏到前台"];
    button6.tag = 106;
    [btnView addSubview: button6];
    [button6 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button4);
        make.left.mas_equalTo(button4);
        make.top.mas_equalTo(button4.mas_bottom).offset(10);
    }];
    
    UIButton *button7 = [self createButton: @"通用消息"];
    button7.tag = 107;
    [btnView addSubview: button7];
    [button7 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button6);
        make.top.mas_equalTo(button6);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button8 = [self createButton: @"剪贴板数据"];
    button8.tag = 108;
    [btnView addSubview: button8];
    [button8 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button6);
        make.left.mas_equalTo(button6);
        make.top.mas_equalTo(button6.mas_bottom).offset(10);
    }];
    
    UIButton *button9 = [self createButton: @"设置配置信息"];
    button9.tag = 109;
    [btnView addSubview: button9];
    [button9 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button8);
        make.top.mas_equalTo(button8);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button10 = [self createButton: @"获取配置信息"];
    button10.tag = 110;
    [btnView addSubview: button10];
    [button10 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button8);
        make.left.mas_equalTo(button8);
        make.top.mas_equalTo(button8.mas_bottom).offset(10);
    }];
    
    UIButton *button11 = [self createButton: @"切换前后台"];
    button11.tag = 111;
    [btnView addSubview: button11];
    [button11 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button10);
        make.top.mas_equalTo(button10);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button12 = [self createButton: @"后台保活时长"];
    button12.tag = 112;
    [btnView addSubview: button12];
    [button12 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button10);
        make.left.mas_equalTo(button10);
        make.top.mas_equalTo(button10.mas_bottom).offset(10);
    }];
    
    UIButton *button13 = [self createButton: @"设置无操作回收时长"];
    button13.tag = 113;
    [btnView addSubview: button13];
    [button13 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button12);
        make.top.mas_equalTo(button12);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button14 = [self createButton: @"获取无操作回收时长"];
    button14.tag = 114;
    [btnView addSubview: button14];
    [button14 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button12);
        make.left.mas_equalTo(button12);
        make.top.mas_equalTo(button12.mas_bottom).offset(10);
    }];
    
    UIButton *button15 = [self createButton: @"发送消息(无回执)"];
    button15.tag = 115;
    [btnView addSubview: button15];
    [button15 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button14);
        make.top.mas_equalTo(button14);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button16 = [self createButton: @"发送消息(超时3秒)"];
    button16.tag = 116;
    [btnView addSubview: button16];
    [button16 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button14);
        make.left.mas_equalTo(button14);
        make.top.mas_equalTo(button14.mas_bottom).offset(10);
    }];
    
    UIButton *button17 = [self createButton: @"角色切换"];
    button17.tag = 117;
    [btnView addSubview: button17];
    [button17 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button16);
        make.top.mas_equalTo(button16);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button18 = [self createButton: @"暂停"];
    button18.tag = 118;
    [btnView addSubview: button18];
    [button18 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button16);
        make.left.mas_equalTo(button16);
        make.top.mas_equalTo(button16.mas_bottom).offset(10);
    }];
    
    UIButton *button19 = [self createButton: @"摄像头类型"];
    button19.tag = 119;
    [btnView addSubview: button19];
    [button19 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button18);
        make.top.mas_equalTo(button18);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button20 = [self createButton: @"挂机模式"];
    button20.tag = 120;
    [btnView addSubview: button20];
    [button20 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button18);
        make.left.mas_equalTo(button18);
        make.top.mas_equalTo(button18.mas_bottom).offset(10);
    }];
    
    UIButton *button21 = [self createButton: @"采集视图隐藏"];
    button21.tag = 121;
    [btnView addSubview: button21];
    [button21 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button20);
        make.top.mas_equalTo(button20);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button22 = [self createButton: @"设置采集音量"];
    button22.tag = 122;
    [btnView addSubview: button22];
    [button22 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button20);
        make.left.mas_equalTo(button20);
        make.top.mas_equalTo(button20.mas_bottom).offset(10);
    }];
    
    UIButton *button23 = [self createButton: @"获取采集音量"];
    button23.tag = 123;
    [btnView addSubview: button23];
    [button23 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button22);
        make.top.mas_equalTo(button22);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button24 = [self createButton: @"设置远端音量"];
    button24.tag = 124;
    [btnView addSubview: button24];
    [button24 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button22);
        make.left.mas_equalTo(button22);
        make.top.mas_equalTo(button22.mas_bottom).offset(10);
    }];
    
    UIButton *button25 = [self createButton: @"获取远端音量"];
    button25.tag = 125;
    [btnView addSubview: button25];
    [button25 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button24);
        make.top.mas_equalTo(button24);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button26 = [self createButton: @"开始音频采集"];
    button26.tag = 126;
    [btnView addSubview: button26];
    [button26 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button24);
        make.left.mas_equalTo(button24);
        make.top.mas_equalTo(button24.mas_bottom).offset(10);
    }];
    
    UIButton *button27 = [self createButton: @"停止音频采集"];
    button27.tag = 127;
    [btnView addSubview: button27];
    [button27 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button26);
        make.top.mas_equalTo(button26);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button28 = [self createButton: @"开始视频采集"];
    button28.tag = 128;
    [btnView addSubview: button28];
    [button28 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button26);
        make.left.mas_equalTo(button26);
        make.top.mas_equalTo(button26.mas_bottom).offset(10);
    }];
    
    UIButton *button29 = [self createButton: @"停止视频采集"];
    button29.tag = 129;
    [btnView addSubview: button29];
    [button29 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button28);
        make.top.mas_equalTo(button28);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button30 = [self createButton: @"镜像开关"];
    button30.tag = 130;
    [btnView addSubview: button30];
    [button30 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button28);
        make.left.mas_equalTo(button28);
        make.top.mas_equalTo(button28.mas_bottom).offset(10);
    }];
    
    UIButton *button31 = [self createButton: @"开启触控事件"];
    button31.tag = 131;
    [btnView addSubview: button31];
    [button31 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button30);
        make.top.mas_equalTo(button30);
        make.right.mas_equalTo(btnView);
        make.bottom.mas_equalTo(btnView);
    }];
}

#pragma mark - Utils

- (UIButton *)createButton:(NSString *)title
{
    UIButton *button = [UIButton buttonWithType: UIButtonTypeCustom];
    button.layer.cornerRadius = 3.0f;
    button.titleLabel.adjustsFontSizeToFitWidth = YES;
    button.backgroundColor = [UIColor systemBlueColor];
    button.titleLabel.font = [UIFont systemFontOfSize: 13.0f];
    [button setTitle: title forState: UIControlStateNormal];
    [button setTitleColor: [UIColor whiteColor] forState: UIControlStateNormal];
    [button addTarget: self action: @selector(tappedButton:) forControlEvents: UIControlEventTouchUpInside];
    return button;
}

- (void)setNetProbeStats:(NSString *)stats
{
    self.netProbeStatsLabel.text = stats;
}

- (void)showNetProbeBtn:(NSString *)title
{
    UIButton *btn = [self.view viewWithTag: 888];
    btn.hidden = NO;
    [btn setTitle: title forState: UIControlStateNormal];
}

- (void)hideNetProbeView
{
    [@[self.netProbeStatsLabel, [self.view viewWithTag: 888]] makeObjectsPerformSelector: @selector(removeFromSuperview)];
}

#pragma mark - button action

- (void)tappedButton:(UIButton *)btn
{
    if (btn.tag == 888) { // 网络探测
        if ([btn.currentTitle isEqualToString: @"中断"]) {
            [self showNetProbeBtn: @"探测"];
            [[VeGameManager sharedInstance] probeInterrupt];
        } else if ([btn.currentTitle isEqualToString: @"探测"]) {
            [self showNetProbeBtn: @"中断"];
            VeGameConfigObject *configObj = [VeGameConfigObject new];
            configObj.ak = self.configObj.ak;
            configObj.sk = self.configObj.sk;
            configObj.token = self.configObj.token;
            configObj.userId = self.configObj.userId;
            [[VeGameManager sharedInstance] probeStart: configObj];
        } else if ([btn.currentTitle isEqualToString: @"完成"]) {
            [self hideNetProbeView];
        }
    } else if (btn.tag == 999) { // Menu
        btn.selected = !btn.selected;
        self.scrollView.hidden = !btn.selected;
    } else if (btn.tag == 100) { // 退出
        self.alreadyStart = NO;
        [SVProgressHUD dismiss];
        [[VeGameManager sharedInstance] stop];
        [self.navigationController popViewControllerAnimated: YES];
    } else if (btn.tag == 101) { // 清晰度
        [self setCustomViewController: @"请输入清晰度ID" hintText: nil tappedSureBlock:^(UITextField *tf) {
            if ([tf.text integerValue] != NSNotFound) {
                [[VeGameManager sharedInstance] switchVideoStreamProfile: tf.text.integerValue];
            }
        }];
    } else if (btn.tag == 102) { // 大文件传输
        UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
        imagePicker.delegate = self;
        imagePicker.allowsEditing = YES;
        imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        imagePicker.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentViewController:imagePicker animated:YES completion:nil];
    } else if (btn.tag == 103) { // 设置键盘开关
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] setKeyboardEnable: btn.selected];
    } else if (btn.tag == 104) { // 获取键盘开关
        [self.view makeToast: [[VeGameManager sharedInstance] getKeyboardEnable] ? @"键盘已开启" : @"键盘已禁用"
                    duration: 2.0f
                    position: CSToastPositionCenter];
    } else if (btn.tag == 105) { // 重启游戏
        [[VeGameManager sharedInstance] restartGame];
    } else if (btn.tag == 106) { // 切换远端游戏到前台
        [[VeGameManager sharedInstance] setRemoteGameForeground];
    } else if (btn.tag == 107) { // 通用消息
        [[VeGameManager sharedInstance] sendGeneralStringMessage: @"ByteDance is The Best Company"];
    } else if (btn.tag == 108) { // 剪贴板数据
        [[VeGameManager sharedInstance] sendClipBoardMessage: @[@"123", @"456", @"789"]];
    } else if (btn.tag == 109) { // 设置配置信息
        [[VeGameManager sharedInstance] setUserProfilePathList: @[@"User/Desktop/Profile"]];
    } else if (btn.tag == 110) { // 获取配置信息
        [[VeGameManager sharedInstance] getUserProfilePathList];
    } else if (btn.tag == 111) { // 切换前后台
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] switchBackground: btn.selected];
        [btn setTitle: btn.selected ? @"切换到后台" : @"切换到前台" forState: UIControlStateNormal];
    } else if (btn.tag == 112) { // 后台保活时长
        __weak typeof(self)weakSelf = self;
        [self setCustomViewController: @"设置后台保活时长" hintText: nil tappedSureBlock:^(UITextField *tf) {
            if ([tf.text integerValue] != NSNotFound) {
                if ([[VeGameManager sharedInstance] setIdleTime: [tf.text integerValue]] == -2) {
                    [weakSelf.view makeToast: @"设置的时间小于等于0，非法！"
                                    duration: 2.0f
                                    position: CSToastPositionCenter];
                }
            }
        }];
    } else if (btn.tag == 113) { // 设置无操作回收时长
        __weak typeof(self)weakSelf = self;
        [self setCustomViewController: @"设置无操作回收时长" hintText: nil tappedSureBlock:^(UITextField *tf) {
            if ([tf.text integerValue] != NSNotFound) {
                if ([[VeGameManager sharedInstance] setAutoRecycleTime: [tf.text integerValue]] == -2) {
                    [weakSelf.view makeToast: @"设置的时间小于等于0，非法！"
                                    duration: 2.0f
                                    position: CSToastPositionCenter];
                }
            }
        }];
    } else if (btn.tag == 114) { // 获取无操作回收时长
        [[VeGameManager sharedInstance] getAutoRecycleTime];
    } else if (btn.tag == 115) { // 发送消息(无回执)
        NSString *payload = @"ByteDance Is The Best Internet Company";
        VeBaseChannelMessage *msg = [[VeGameManager sharedInstance] sendMessage: payload];
        NSLog(@"send no ack msg: %@", [msg description]);
    } else if (btn.tag == 116) { // 发送消息(超时3秒)
        NSString *payload = @"ByteDance‘s CEO is LiangRuBo";
        VeBaseChannelMessage *msg = [[VeGameManager sharedInstance] sendMessage: payload timeout: 3000];
        NSLog(@"send timeout msg: %@", [msg description]);
    } else if (btn.tag == 117) { // 切换角色
        CustomViewController *alert = [CustomViewController alertControllerWithTitle: @"请输入目标Uid&角色" message:nil preferredStyle: UIAlertControllerStyleAlert];
        alert.rotation = self.rotation;
        __block UITextField *tf1 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
            tf1 = textField;
            tf1.placeholder = @"目标用户的Uid";
            tf1.keyboardType = UIKeyboardTypeDefault;
        }];
        __block UITextField *tf2 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
            tf2 = textField;
            tf2.placeholder = @"0：Viewer，1：Player";
            tf2.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        UIAlertAction *action0 = [UIAlertAction actionWithTitle: @"取消" style:UIAlertActionStyleCancel handler: nil];
        [alert addAction: action0];
        UIAlertAction *action1 = [UIAlertAction actionWithTitle: @"确认" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
            if (tf1.text.length > 0 && [tf2.text integerValue] != NSNotFound) {
                [[VeGameManager sharedInstance] changeRole: tf1.text role: tf2.text.integerValue];
            }
        }];
        [alert addAction: action1];
        [self presentViewController: alert animated: NO completion: nil];
    } else if (btn.tag == 118) { // 暂停
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] switchPaused: btn.selected];
        [btn setTitle: btn.selected ? @"恢复" : @"暂停" forState: UIControlStateNormal];
    } else if (btn.tag == 119) { // 摄像头切换
        btn.selected = !btn.selected;
        [btn setTitle: btn.selected ? @"前置摄像头" : @"后置摄像头" forState: UIControlStateNormal];
        [self.view makeToast: btn.currentTitle
                    duration: 2.0f
                    position: CSToastPositionCenter];
        [[VeGameManager sharedInstance] switchCamera: btn.selected ? VeBaseCameraIdFront : VeBaseCameraIdBack];
    } else if (btn.tag == 120) { // 挂机模式
        [self setCustomViewController: @"请输入会话模式" hintText: @"0：普通模式，1：挂机模式" tappedSureBlock:^(UITextField *tf) {
            if ([tf.text integerValue] != NSNotFound) {
                [[VeGameManager sharedInstance] setSessionMode: tf.text.integerValue];
            }
        }];
    } else if (btn.tag == 121) { // 挂机模式
        btn.selected = !btn.selected;
        self.localVideoView.hidden = !btn.selected;
        [btn setTitle: btn.selected ? @"采集视图显示" : @"采集视图隐藏" forState: UIControlStateNormal];
    } else if (btn.tag == 122) { // 设置本地采集音量
        [self setCustomViewController: @"设置本地采集音量" hintText: @"本地采集音量，[0，100]" tappedSureBlock:^(UITextField *tf) {
            if ([tf.text integerValue] != NSNotFound) {
                [[VeGameManager sharedInstance] setLocalAudioCaptureVolume: tf.text.integerValue];
            }
        }];
    } else if (btn.tag == 123) { // 获取本地采集音量
        [self.view makeToast: [NSString stringWithFormat: @"本地采集音量：%ld", [[VeGameManager sharedInstance] getLocalAudioCaptureVolume]]
                    duration: 2.0f
                    position: CSToastPositionCenter];
    } else if (btn.tag == 124) { // 设置远端播放音量
        [self setCustomViewController: @"设置远端播放音量" hintText: @"远端播放音量，[0，100]" tappedSureBlock:^(UITextField *tf) {
            if ([tf.text integerValue] != NSNotFound) {
                [[VeGameManager sharedInstance] setRemoteAudioPlaybackVolume: tf.text.integerValue];
            }
        }];
    } else if (btn.tag == 125) { // 获取远端播放音量
        [self.view makeToast: [NSString stringWithFormat: @"本地采集音量：%ld", [[VeGameManager sharedInstance] getRemoteAudioPlaybackVolume]]
                    duration: 2.0f
                    position: CSToastPositionCenter];
    } else if (btn.tag == 126) { // 开始音频采集
        [[VeGameManager sharedInstance] startAudioStream];
    } else if (btn.tag == 127) { // 停止音频采集
        [[VeGameManager sharedInstance] stopAudioStream];
    } else if (btn.tag == 128) { // 开始视频采集
        [[VeGameManager sharedInstance] startVideoStream: 0];
    } else if (btn.tag == 129) { // 停止视频采集
        [[VeGameManager sharedInstance] stopVideoStream];
    } else if (btn.tag == 130) { // 镜像开关
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] setLocalVideoMirrorType: btn.selected ? VeBaseMirrorTypeRender : VeBaseMirrorTypeNone];
        [btn setTitle: btn.selected ? @"开启镜像" : @"关闭镜像" forState: UIControlStateNormal];
        [self.view makeToast: btn.currentTitle
                    duration: 2.0f
                    position: CSToastPositionCenter];
    } else if (btn.tag == 131) { // 拦截触控事件
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] setInterceptSendTouchEvent: btn.selected];
        [btn setTitle: btn.selected ? @"禁止触控事件" : @"开启触控事件" forState: UIControlStateNormal];
    }
}

- (void)startGame
{
    if (!self.alreadyStart) {
        self.alreadyStart = YES;
        [SVProgressHUD showWithStatus: @"正在启动..."];
        VeGameConfigObject *configObj = [VeGameConfigObject new];
        configObj.ak = self.configObj.ak;
        configObj.sk = self.configObj.sk;
        configObj.token = self.configObj.token;
        configObj.userId = self.configObj.userId;
        configObj.gameId = self.configObj.gameId;
        configObj.roundId = self.configObj.roundId;
        VeGameControlObject *control = [VeGameControlObject new];
        control.role = self.configObj.role;
        control.roomType = self.configObj.roomType;
        configObj.control = control;
        configObj.sessionMode = self.configObj.sessionMode;
        configObj.queuePriority = self.configObj.queuePriority;
        configObj.keyboardEnable = self.configObj.keyboardEnable;
        configObj.autoRecycleTime = self.configObj.autoRecycleTime;
        configObj.videoStreamProfileId = self.configObj.videoStreamProfileId;
        configObj.reservedId = self.configObj.reservedId.length ? self.configObj.reservedId : nil;
        [[VeGameManager sharedInstance] startWithConfig: configObj];
    }
}

- (void)setCustomViewController:(NSString *)title hintText:(NSString *)hint tappedSureBlock:(void(^)(UITextField *tf))block
{
    __block UITextField *tf = [UITextField new];
    CustomViewController *alert = [CustomViewController alertControllerWithTitle: title message:nil preferredStyle: UIAlertControllerStyleAlert];
    alert.rotation = self.rotation;
    [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.keyboardType = UIKeyboardTypeDefault;
        textField.placeholder = hint;
        tf = textField;
    }];
    UIAlertAction *action0 = [UIAlertAction actionWithTitle: @"取消" style:UIAlertActionStyleCancel handler: nil];
    [alert addAction: action0];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle: @"确认" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action) {
        !block ?: block(tf);
    }];
    [alert addAction: action1];
    [self presentViewController: alert animated: NO completion: nil];
}

#pragma mark - setter

- (void)setRotation:(NSInteger)rotation
{
    if (_rotation != rotation) {
        _rotation = rotation;
        if (@available(iOS 16, *)) {
            [self setNeedsUpdateOfSupportedInterfaceOrientations];
        } else {
            [Utils rotateDeviceToOrientation: rotation];
        }
        if (rotation == 0) {
            [self.scrollView mas_updateConstraints:^(MASConstraintMaker *make) {
                make.left.mas_equalTo(self.view).offset(20.0f);
                make.top.mas_equalTo(self.view).offset(44);
            }];
        } else {
            [self.scrollView mas_updateConstraints:^(MASConstraintMaker *make) {
                make.top.mas_equalTo(self.view).offset(20.0f);
                make.left.mas_equalTo(self.view).offset(44);
            }];
        }
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            UIButton *menuBtn = [self.view viewWithTag: 999];
            menuBtn.center = CGPointMake(self.view.bounds.size.width - menuBtn.bounds.size.width / 2, self.view.bounds.size.height / 2 + menuBtn.bounds.size.height);
            [self.scrollView setContentOffset: CGPointMake(0, 0) animated: NO];
        });
    }
}

#pragma mark - VeGameManagerDelegate

- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId planId:(NSString *)planId extra:(NSDictionary *)extra
{
    [SVProgressHUD dismiss];
    [manager setLocalVideoCanvas: self.localVideoView];
}

- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation
{
    self.rotation = rotation;
}

- (void)gameManager:(VeGameManager *)manager operationDelay:(NSInteger)delayTime
{
    self.operationDelayTime = [NSString stringWithFormat: @"%ld", (long)delayTime];
}

- (void)gameManager:(VeGameManager *)manager onLocalStreamStats:(VeBaseLocalStreamStats *)stats
{
    // NSLog(@"local stream stats: %@", [stats description]);
}

- (void)gameManager:(VeGameManager *)manager onRemoteStreamStats:(VeBaseRemoteStreamStats *)stats
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.timelylogLabel.text = [NSString stringWithFormat: @"delay: %@ms rtt: %ldms loss: %.2f%% bit: %ldkbps fps: %ldfps",
                                    self.operationDelayTime ?: @"0", (long)stats.videoRtt, stats.audioLossRate + stats.videoLossRate, stats.receivedVideoKBitrate, (long)stats.rendererOutputFrameRate];
    });
}

- (void)gameManager:(VeGameManager *)manager onNetworkQuality:(VeBaseNetworkQuality)quality
{
    switch (quality) {
        case VeBaseNetworkQualityGood:
        {
             // NSLog(@"--- 网络情况良好 Quality:%li ---", quality);
        }
            break;
        case VeBaseNetworkQualityBad:
        {
             // NSLog(@"--- 网络情况较差 Quality:%li ---", quality);
        }
            break;
        case VeBaseNetworkQualityVeryBad:
        {
             // NSLog(@"--- 网络情况糟糕 Quality:%li ---", quality);
        }
            break;
        case VeBaseNetworkQualityDown:
        {
             // NSLog(@"--- 网络不可用 Quality:%li ---", quality);
        }
            break;
        default:
            break;
    }
}

- (void)gameManager:(VeGameManager *)manager onJoinRoomRoleResult:(VeBaseRoleType)role reason:(NSInteger)reason playerUserId:(NSString *)player_uid
{
    NSLog(@"当前用户角色：%@，result：%@，player_uid：%@", role == VeBaseRoleTypeViewer ? @"VeBaseRoleTypeViewer": @"VeBaseRoleTypePlayer", reason == 0 ? @"成功" : @"失败", player_uid);
}

- (void)gameManager:(VeGameManager *)manager onChangeRoleCallBack:(VeBaseRoleType)role result:(NSInteger)result destUserId:(NSString *)dest_uid
{
    NSLog(@"要修改用户的最终角色：%@，result：%@，dest_uid：%@", role == VeBaseRoleTypeViewer ? @"VeBaseRoleTypeViewer": @"VeBaseRoleTypePlayer", result == 0 ? @"成功" : @"失败", dest_uid);
}

- (void)gameManager:(VeGameManager *)manager onPlayerChanged:(NSString *)player_uid
{
    NSLog(@"当前的操作者Uid：%@", player_uid);
}

- (void)gameManager:(VeGameManager *)manager onSessionMode:(VeBaseSessionMode)mode result:(BOOL)result
{
    NSString *str = [NSString stringWithFormat: @"当前会话模式：%@，切换结果：%@", mode == VeBaseSessionModeNormal ? @"正常模式" : @"挂机模式", result ? @"成功" : @"失败"];
    [self.view makeToast: str duration: 2.0f position: CSToastPositionCenter];
}

- (void)onNetProbeStartedFromEngineManager:(VeGameManager *)manager
{
    NSLog(@"---开始网络探测---");
}

- (void)gameManager:(VeGameManager *)manager onNetProbeProcess:(VeGameNetworkProbeStats *)stats
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self showNetProbeBtn: @"中断"];
        [self setNetProbeStats: [NSString stringWithFormat: @"往返延迟rtt：%dms\n上行网络：%dms\n下行网络：%dms\n上行网络带宽：%d kbit/s\n下行网络带宽：%d kbit/s\n上行网络丢包率：%0.2f%%\n下行网络丢包率：%0.2f%%", stats.rtt, stats.uploadJitter, stats.downloadJitter, stats.uploadBandwidth, stats.downloadBandwidth, stats.uploadLossPercent, stats.downloadLossPercent]];
    });
}

- (void)gameManager:(VeGameManager *)manager onNetProbeCompleted:(VeGameNetworkProbeStats *)stats quality:(VeBaseNetProbeQuality)quality
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self showNetProbeBtn: @"完成"];
        [self setNetProbeStats: [NSString stringWithFormat: @"往返延迟rtt：%dms\n上行网络：%dms\n下行网络：%dms\n上行网络带宽：%d kbit/s\n下行网络带宽：%d kbit/s\n上行网络丢包率：%0.2f%%\n下行网络丢包率：%0.2f%%\n网络探测结果：%@", stats.rtt, stats.uploadJitter, stats.downloadJitter, stats.uploadBandwidth, stats.downloadBandwidth, stats.uploadLossPercent, stats.downloadLossPercent, quality == VeBaseNetProbeQualityExcellent ? @"极致" : (quality == VeBaseNetProbeQualityGood ? @"良好" : @"糟糕")]];
        if (quality != VeBaseNetProbeQualityPoor) {
            [self startGame];
        } else {
            [self.view makeToast: @"当前网络环境不足，无法体验游戏" duration: 2.0f position: CSToastPositionCenter];
        }
    });
}

- (void)gameManager:(VeGameManager *)manager onTouchEvent:(NSArray<VeBaseTouchEventItem *> *)touchArray
{
    NSLog(@"touchArray = %@", touchArray);
}

- (void)gameManager:(VeGameManager *)manager networkTypeChangedToType:(VeBaseNetworkType)networkType
{
    NSString *str = @"";
    switch (networkType) {
        case VeBaseNetworkTypeUnknown:
            str = @"当前网络类型：未知";
            break;
        case VeBaseNetworkTypeDisconnected:
            str = @"当前网络已断开";
            break;
        case VeBaseNetworkTypeLAN:
            str = @"当前网络类型：LAN局域网";
            break;
        case VeBaseNetworkTypeWIFI:
            str = @"当前网络类型：WIFI";
            break;
        case VeBaseNetworkTypeMobile2G:
            str = @"当前网络类型：2G";
            break;
        case VeBaseNetworkTypeMobile3G:
            str = @"当前网络类型：3G";
            break;
        case VeBaseNetworkTypeMobile4G:
            str = @"当前网络类型：4G";
            break;
        case VeBaseNetworkTypeMobile5G:
            str = @"当前网络类型：5G";
            break;
    }
    if (str.length > 0) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.view makeToast: str
                        duration: 2.0f
                        position: CSToastPositionBottom];
        });
    }
}

- (void)gameManager:(VeGameManager *)manager onNetProbeError:(VeBaseNetProbeErrorCode)code
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [SVProgressHUD dismiss];
        NSString *toast = nil;
        switch (code) {
            case VeBaseNetProbeErrorCodeBadNetwork:
            {
                toast = @"网络探测错误";
            }
                break;
            case VeBaseNetProbeErrorCodeCancelByUser:
            {
                [self startGame];
                toast = @"用户中断网络探测";
                [self showNetProbeBtn: @"探测"];
            }
                break;
            case VeBaseNetProbeErrorCodeEmptyStats:
            {
                toast = @"网络探测数据为空";
            }
                break;
        }
        [self.view makeToast: toast
                    duration: 2.0f
                    position: CSToastPositionCenter];
    });
}

- (void)startAudioCaptureRequestFromGameManager:(VeGameManager *)manager
{
    [SVProgressHUD showWithStatus: @"正在启动麦克风..."];
    [manager startAudioStream];
}

- (void)stopAudioCaptureRequestFromGameManager:(VeGameManager *)manager
{
    [manager stopAudioStream];
}

- (void)gameManager:(VeGameManager *)manager onAudioCaptureDeviceState:(VeBaseMediaDeviceState)state deviceError:(VeBaseMediaDeviceError)error
{
    [SVProgressHUD dismiss];
    if (error == VeBaseMediaDeviceErrorDeviceNoPermission) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.view makeToast: @"错误，没有麦克风权限"
                        duration: 2.0f
                        position: CSToastPositionCenter];
        });
    }
}

- (void)gameManager:(VeGameManager *)manager onRemoteAudioPlaybackVolumeChanged:(NSInteger)volume fromUid:(NSString *)uid
{
    // NSLog(@"RemoteAudioPlaybackVolumeChanged: %ld, uid: %@", volume, uid);
}

- (void)gameManager:(VeGameManager *)manager startVideoCaptureRequest:(VeBaseCameraId)cameraId
{
    [manager startVideoStream: cameraId];
}

- (void)stopVideoCaptureRequestFromGameManager:(VeGameManager *)manager
{
    [manager stopVideoStream];
}

- (void)firstLocalVideoFrameCapturedFromGameManager:(VeGameManager *)manager
{
    NSLog(@"本地视频采集：首帧到达");
}

- (void)gameManager:(VeGameManager *)manager onVideoCaptureDeviceState:(VeBaseMediaDeviceState)state deviceError:(VeBaseMediaDeviceError)error
{
    if (error == VeBaseMediaDeviceErrorDeviceNoPermission) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.view makeToast: @"错误，没有摄像头权限"
                        duration: 2.0f
                        position: CSToastPositionCenter];
        });
    }
}

- (void)gameManager:(VeGameManager *)manager switchVideoStreamProfile:(BOOL)result fromIndex:(NSInteger)index1 toIndex:(NSInteger)index2 targetParams:(NSDictionary *)paramsDict
{
    [self.view makeToast: [NSString stringWithFormat: @"清晰度切换：%@，fromIndex = %@, toIndex = %@", result ? @"成功" : @"失败", @(index1), @(index2)]
                duration: 2.0f
                position: CSToastPositionCenter];
}

- (void)gameManager:(VeGameManager *)manager keyboardEnable:(BOOL)enable
{
    UIButton *btn = [self.view viewWithTag: 104];
    btn.selected = enable;
    [btn setTitle: btn.selected ? @"键盘已开启" : @"键盘已禁用" forState: UIControlStateNormal];
}

- (void)gameManager:(VeGameManager *)manager onBackgroundSwitched:(NSInteger)code
{
    NSLog(@"backgroundSwitched：%@", code == 0 ? @"切换为后台" : @"切换为前台");
}

- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedForeground:(VeBaseRemoteAppSwitchedType)switchType
{
    NSLog(@"---切换远端游戏到前台---");
}

- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedBackground:(VeBaseRemoteAppSwitchedType)switchType
{
    NSLog(@"---切换远端游戏到后台---");
}

- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedFailedWithCode:(VeGameWarningCode)warningCode errorMsg:(NSString *)errorMsg
{
    NSLog(@"---切换远端游戏前后台失败---");
}

- (void)gameManager:(VeGameManager *)manager receivedClipBoardMessage:(NSArray *)datArray
{
    NSLog(@"剪贴板数据：%@", datArray);
}

- (void)gameManager:(VeGameManager *)manager receivedGeneralStringMessage:(NSString *)dataString
{
    NSLog(@"通用消息：%@", dataString);
}

- (void)gameManager:(VeGameManager *)manager setUserProfilePathType:(NSInteger)type result:(BOOL)result
{
    NSLog(@"设置配置信息：%@，结果：%@", type == 0 ? @"设置" : @"还原", result ? @"成功" : @"失败");
}

- (void)gameManager:(VeGameManager *)manager getUserProfilePathList:(NSArray<NSString *> *)list
{
    NSLog(@"获取配置信息：%@", list);
}

- (void)gameManager:(VeGameManager *)manager setAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time
{
    NSLog(@"%@", [NSString stringWithFormat: @"设置无操作回收时长：%@，time = %ld", code == 0 ? @"成功" : @"失败", time]);
}

- (void)gameManager:(VeGameManager *)manager getAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time
{
    NSLog(@"%@", [NSString stringWithFormat: @"获取无操作回收时长：%@，time = %ld", code == 0 ? @"成功" : @"失败", time]);
}

- (void)gameManager:(VeGameManager *)manager onRemoteMessageOnline:(NSString *)channel_uid
{
    NSLog(@"Message Channel Online = %@", channel_uid);
}

- (void)gameManager:(VeGameManager *)manager onRemoteMessageOffline:(NSString *)channel_uid
{
    NSLog(@"Message Channel Offline = %@", channel_uid);
}

- (void)gameManager:(VeGameManager *)manager onReceiveMessage:(VeBaseChannelMessage *)message
{
    NSLog(@"Receive Message = %@", [message description]);
}

- (void)gameManager:(VeGameManager *)manager onSendMessageResult:(BOOL)result messageId:(NSString *)mid
{
    NSLog(@"Send Message Result = %@，MessageId = %@", result ? @"成功" : @"失败", mid);
}

#pragma mark - 大文件传输下载回调

- (void)gameManager:(VeGameManager *)manager downloadOnStart:(VeFile *)file options:(NSDictionary *)options
{
    NSLog(@"开始下载\n");
}

- (void)gameManager:(VeGameManager *)manager downloadOnProgress:(VeFile *)file options:(NSDictionary *)options progress:(NSInteger)progress
{
    NSLog(@"下载进度------%ld\n", progress);
}

- (void)gameManager:(VeGameManager *)manager downloadOnComplete:(VeFile *)file options:(NSDictionary *)options
{
    NSLog(@"下载完成\n");
}

- (void)gameManager:(VeGameManager *)manager downloadOnError:(VeFile *)file options:(NSDictionary *)options err:(VeGameErrorCode)err
{
    NSLog(@"下载错误\n");
}

- (void)gameManager:(VeGameManager *)manager downloadOnCancel:(VeFile *)file options:(NSDictionary *)options
{
    NSLog(@"下载取消\n");
}

- (void)gameManager:(VeGameManager *)manager onPodExit:(VeGameErrorCode)errCode
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [SVProgressHUD dismiss];
        NSString *toast = @"";
        if (errCode == ERROR_GAME_ABNORMAL_EXIT) {
            toast = @"40000 云端游戏异常退出";
        } else if (errCode == ERROR_GAME_CRASH) {
            toast = @"40001 云端游戏崩溃";
        } else if (errCode == ERROR_GAME_STOPPED_IDLE) {
            toast = @"40004 长期未操作，云端游戏自动断开";
        } else if (errCode == ERROR_GAME_STOPPED_API) {
            toast = @"40006 服务端主动停止云端游戏";
        } else if (errCode == ERROR_POD_STOPPED_BACKGROUND_TIMEOUT) {
            toast = @"40008 云端后台超时";
        } else if (errCode == ERROR_POD_EXIT_GENERAL) {
            toast = @"40009 云端游戏退出";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_NORMAL) {
            toast = @"40023 实例正常释放";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_NO_USER) {
            toast = @"40024 实例异常释放：客户端超时未加入";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_OS_MISSED) {
            toast = @"40026 实例异常释放：游戏镜像缺失";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_GAME_START_FAILURE) {
            toast = @"40027 实例异常释放：游戏启动失败";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_STREAMING_ERROR) {
            toast = @"40028 实例异常释放：rtc推流成功，但是推流过程中出现异常";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_3RD_APP_MISSED) {
            toast = @"40029 实例异常释放：伴随包镜像缺失";
        } else if (errCode == MESSAGE_3RD_APP_START_FAILURE) {
            toast = @"40031 伴随包启动失败";
        } else if (errCode == MESSAGE_CLOUD_GAME_CRASH_OFTEN) {
            toast = @"40032 游戏频繁崩溃";
        } else if (errCode == MESSAGE_GAME_STEAMING_FAILURE) {
            toast = @"40033 Rtc推流不成功";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_INVALID_PARAMETER) {
            toast = @"40047 Pod收到的参数非法";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_HEART_BEAT_TIMEOUT) {
            toast = @"40048 实例离线";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_INGAME_EXIT) {
            toast = @"40049 游戏侧主动退出";
        } else if (errCode == MESSAGE_RESOURCE_RELEASED_START_ERROR_ARCHIVE_DOWNLOAD_FAILURE) {
            toast = @"40050 存档下载失败，Pod启动失败";
        } else if (errCode == ERROR_GAME_EXIT_INTERNAL_ERROR) {
            toast = @"40051 内部错误，云服务重启或GS重启";
        }
        if (toast.length > 0) {
            [self.view makeToast: toast
                        duration: 2.0f
                        position: CSToastPositionCenter];
        }
    });
}

- (void)gameManager:(VeGameManager *)manager onMessageChannleError:(VeGameErrorCode)errCode
{
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString *toast = @"";
        if (errCode == ERROR_MESSAGE_GENERAL) {
            toast = @"50000 消息通道通用错误";
        } else if (errCode == ERROR_MESSAGE_NOT_CONNECTED) {
            toast = @"50001 消息通道无连接";
        } else if (errCode == ERROR_MESSAGE_FAILED_TO_PARSE_MSG) {
            toast = @"50002 消息通道数据解析失败";
        } else if (errCode == ERROR_MESSAGE_CHANNEL_UID_ILLEGAL) {
            toast = @"50003 消息通道ID非法";
        } else if (errCode == ERROR_MESSAGE_OVER_SIZED) {
            toast = @"50007 消息体超过60kb";
        } else if (errCode == ERROR_MESSAGE_TIMEOUT_ILLEGAL) {
            toast = @"50009 消息发送超时时间非法";
        }
        if (toast.length > 0) {
            [self.view makeToast: toast
                        duration: 2.0f
                        position: CSToastPositionCenter];
        }
    });
}

- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode
{
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString *toast = @"";
        if (warnCode == WARNING_START_NO_STOP_BEFORE) {
            toast = @"10010 启动游戏失败，原因：连续调用了两次Start之间没有调用 Stop";
        } else if (warnCode == WARNING_START_INVALID_AUTO_RECYCLE_TIME) {
            toast = @"10019 设置无操作回收服务时长非法";
        } else if (warnCode == WARNING_START_WITH_FRAMEWORK_NOT_FOUND) {
            toast = @"10023 伴随程序：全部未找到";
        } else if (warnCode == WARNING_START_WITH_FRAMEWORK_PART_MATCH) {
            toast = @"10024 伴随程序：部分找到";
        } else if (warnCode == WARNING_START_WITH_FRAMEWORK_WRONG_INPUT_FORMAT) {
            toast = @"10025 伴随程序：格式错误，解析失败";
        } else if (warnCode == WARNING_QUEUEING_LACK_RESOURCE) {
            toast = @"10030 还需要继续排队";
        } else if (warnCode == WARNING_SDK_LACK_OF_LOCATION_PERMISSION) {
            toast = @"30007 无定位权限";
        } else if (warnCode == WARNING_VIEWER_METHOD_CALLED) {
            toast = @"30011 VeBaseRoleTypeViewer 操作被调用";
        } else if (warnCode == WARNING_LOCAL_ALREADY_SET_BACKGROUND) {
            toast = @"40037 用户重复调用切换后台接口";
        } else if (warnCode == WARNING_LOCAL_ALREADY_SET_FOREGROUND) {
            toast = @"40038 用户重复调用切换前台接口";
        } else if (warnCode == WARNING_GAME_STOPPED_INGAME_EXIT) {
            toast = @"40044 游戏实例退出";
        } else if (warnCode == WARNING_VIDEO_PROFILE_NOT_SUPPORT_CURRENT_PLAN) {
            toast = @"40052 套餐不支持";
        } else if (warnCode == WARNING_START_NET_REQUEST_CANCEL) {
            toast = @"61001 网络请求取消";
        }
        if (toast.length > 0) {
            [self.view makeToast: toast
                        duration: 2.0f
                        position: CSToastPositionCenter];
        }
    });
}

- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errCode
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [SVProgressHUD dismiss];
        NSString *toast = @"";
        if (errCode == ERROR_START_GENERAL) {
            toast = @"10000 通用错误";
        } else if (errCode == ERROR_START_AUTHENTICATION_FAILED) {
            toast = @"10001 火山服务鉴权失败";
        } else if (errCode == ERROR_START_GAME_ID_OR_CUSTOM_GAME_ID_NOT_EXIST) {
            toast = @"10002 当前游戏不存在";
        } else if (errCode == ERROR_START_GAME_ID_NOT_READY) {
            toast = @"10003 当前游戏尚在适配中";
        } else if (errCode == ERROR_START_CONFIGURATION_CODE_NOT_EXIST) {
            toast = @"10004 套餐 ID 不存在";
        } else if (errCode == ERROR_START_CONFIGURATION_CODE_NOT_REDAY) {
            toast = @"10005 套餐 ID 未就绪";
        } else if (errCode == ERROR_START_RESOURCE_NOT_READY) {
            toast = @"10006 当前游戏订购资源未就绪";
        } else if (errCode == ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH) {
            toast = @"10007 当前游戏没有订购资源";
        } else if (errCode == ERROR_START_AUTHENTICATION_KEY_FAILED) {
            toast = @"10009 鉴权 Token 过期";
        } else if (errCode == ERROR_START_CONNECTION_ENDED) {
            toast = @"10011 启动游戏失败，原因：在调用Start接口后，Start成功回调触发前，游戏被停止";
        } else if (errCode == ERROR_START_RESERVED_ID_NOT_FOUND) {
            toast = @"10012 ReservedId 不存在";
        } else if (errCode == ERROR_START_RESERVED_ID_EXPIRED) {
            toast = @"10013 ReservedId 过期";
        } else if (errCode == ERROR_START_RESERVED_ID_ALREADY_USED_RELEASE) {
            toast = @"10014 ReservedId 无效";
        } else if (errCode == ERROR_START_RESERVED_ID_USING) {
            toast = @"10015 ReservedId 已被使用";
        } else if (errCode == ERROR_START_RESERVED_ID_MISMATCH_PREPARE) {
            toast = @"10016 ReservedId 相应的预锁定与 Start 参数不匹配";
        } else if (errCode == ERROR_START_NO_SUFFICIENT_FUND) {
            toast = @"10017 后付费账户欠费，服务不可用";
        } else if (errCode == ERROR_START_USER_CONFLICT) {
            toast = @"10018 触发了游戏多开限制，建议：请联系火山技术之";
        } else if (errCode == ERROR_START_MISMATCH_ACCOUNTID) {
            toast = @"10026 AccountId错误";
        } else if (errCode == ERROR_START_INVALID_LOCAL_TIME) {
            toast = @"10027 本地时间导致token过期";
        } else if (errCode == ERROR_START_INVALID_ROLE) {
            toast = @"10028 观察者启用挂机模式报错";
        } else if (errCode == ERROR_USER_NOT_IN_QUEUE) {
            toast = @"10029 退出排队队列";
        } else if (errCode == ERROR_STREAM_GENERAL) {
            toast = @"20000 游戏串流连接错误";
        } else if (errCode == ERROR_STREAM_CHANGE_CLARITY_ID_NOT_IN_START_STATE) {
            toast = @"20002 切换清晰度失败，原因：在非播放状态下";
        } else if (errCode == ERROR_SDK_GENERAL) {
            toast = @"30000 SDK 通用错误";
        } else if (errCode == ERROR_SDK_INIT_FAILED) {
            toast = @"30001 初始化 SDK 实例化失败";
        } else if (errCode == ERROR_SDK_CONFIG_OR_AUTH_PARAMETER_EMPTY) {
            toast = @"30002 启动参数为空";
        } else if (errCode == ERROR_SDK_INVALID_VIDEO_CONTAINER) {
            toast = @"30008 画布尺寸无效";
        } else if (errCode == ERROR_INIT_ACCOUNT_ID_ILLEGAL) {
            toast = @"30009 火山账户ID非法";
        } else if (errCode == ERROR_GAME_STOPPED_DUPLICATE_START) {
            toast = @"游戏停止，原因：在不同设备上使用相同参数请求Start";
        } else if (errCode == ERROR_NET_REQUEST_ERROR) {
            toast = @"60001 网络请求失败";
        } else if (errCode == ERROR_HTTP_REQUEST_ERROR) {
            toast = @"60002 网络请求失败";
        }
        if (toast.length > 0) {
            [self.view makeToast: toast
                        duration: 2.0f
                        position: CSToastPositionCenter];
        }
    });
}

#pragma mark - UIImagePickerControllerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey, id> *)info
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    UIImage *image = [info objectForKey: UIImagePickerControllerOriginalImage];
    NSData *imageData;
    if (UIImagePNGRepresentation(image)) {
        imageData = UIImagePNGRepresentation(image);
    } else {
        imageData = UIImageJPEGRepresentation(image, 1.0f);
    }
    VeFile *file = [VeFile new];
    file.fileData = imageData;
    file.name = @"1.png";
    file.md5 = [self md5StringOfData: imageData];
    [[VeGameManager sharedInstance] startSendFile:file options:@{@"test_key" : @"test_value"} onStart:^(VeFile *file, NSDictionary *options) {
        NSLog(@"文件上传开始-------%@\n", file);
    } onProgress:^(VeFile *file, NSDictionary *options, NSInteger progress) {
        NSLog(@"文件上传进度-------%ld\n", progress);
    } onComplete:^(VeFile *file, NSDictionary *options) {
        NSLog(@"文件上传完成-------%@-----%@\n", file, file.localFilePath);
    } onCancel:^(VeFile *file, NSDictionary *options) {
        NSLog(@"文件上传取消-------%@\n", file);
    } onError:^(VeFile *file, NSDictionary *options, VeGameErrorCode err) {
        NSLog(@"文件上传出错-------%@\n", file);
    }];
}

- (void)gameManager:(VeGameManager *)manager onQueueUpdate:(NSArray<NSDictionary *> *)queueInfoList
{
    NSLog(@"开始排队：%@", queueInfoList);
}

- (void)gameManager:(VeGameManager *)manager onQueueSuccessAndStart:(NSInteger)remainTime
{
    NSLog(@"排队完毕%ld", remainTime);
}

- (NSString *)md5StringOfData:(NSData *)data
{
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    CC_MD5(data.bytes, (CC_LONG)data.length,  result);
    return [NSString stringWithFormat:
            @"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];

}

#pragma mark - receive notification

- (void)receiveAppWillTerminateNotification:(NSNotification *)notification
{
    [[VeGameManager sharedInstance] stop];
}

- (void)receiveAppDidEnterBackgroundNotification:(NSNotification *)notification
{
    [[VeGameManager sharedInstance] switchPaused: YES];
}

- (void)receiveAppWillEnterForegroundNotification:(NSNotification *)notification
{
    [[VeGameManager sharedInstance] switchPaused: NO];
}

#pragma mark - getter

- (BOOL)shouldAutorotate
{
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    UIInterfaceOrientationMask mask = UIInterfaceOrientationMaskPortrait;
    if (self.rotation == 90 || self.rotation == 270) {
        mask = UIInterfaceOrientationMaskLandscapeRight;
    }
    return mask;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationPortrait;
}

- (void)dealloc
{
    NSLog(@"--- VeCloudGameDisplayViewController Dealloc ---");
}


@end
