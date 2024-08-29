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
#import "VePCDisplayViewController.h"
#import <CommonCrypto/CommonDigest.h>
#import "UIView+Draggable.h"
#import "CustomViewController.h"
#import <CoreMotion/CoreMotion.h>
#import <VeGamePad/VeGamePad.h>
#import <VeVirtualKeyboard/VeVirtualKeyboard.h>
#import <VeGameVirtualInputSuite/VeGameVirtualInputSuite.h>

@implementation VeCloudPCConfigObject

@end

@interface VePCDisplayViewController () <VeGameManagerDelegate, VeGamePadViewDelegate, VeVirtualKeyboardViewDelegate, VeGameVirtualInputSuiteDelegate>

@property (nonatomic, assign) NSInteger rotation;
@property (nonatomic, strong) UIView *containerView;
@property (nonatomic, strong) UILabel *timelylogLabel;
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, copy) NSString *operationDelayTime;
@property (nonatomic, assign) int32_t last_mouse_move_x;
@property (nonatomic, assign) int32_t last_mouse_move_y;
@property (nonatomic, strong) VeGamePadView *gamePadView;
@property (nonatomic, strong) VeVirtualKeyboardView *virtualKeyboardView;
@property (nonatomic, strong) VeGameVirtualInputSuiteManager *suite;

@end

@implementation VePCDisplayViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    [self configSubView];

    self.rotation = 270;
    [SVProgressHUD showWithStatus:@"正在启动..."];
    [VeGameManager sharedInstance].delegate = self;
    [VeGameManager sharedInstance].containerView = self.containerView;
    // 设置游戏手柄
    VeGamePadAuth *auth = [VeGamePadAuth new];
    auth.ak = self.configObj.ak;
    auth.sk = self.configObj.sk;
    auth.token = self.configObj.token;
    self.gamePadView = [VeGamePadView createGamePadViewWithGameId:self.configObj.gameId auth:auth delegate:self];
    /// 加载手柄Mana配置
    [self.gamePadView loadKeyMappingConfigs];
    [[VeGameManager sharedInstance] setGamePadView:self.gamePadView];
    // 附加信息
    [[VeGameManager sharedInstance] setExtraParameters:@{
        @"protocol_type" : @"bytertc"
    }];
    // 配置信息
    VeGameConfigObject *configObj = [VeGameConfigObject new];
    configObj.ak = self.configObj.ak;
    configObj.sk = self.configObj.sk;
    configObj.token = self.configObj.token;
    configObj.userId = self.configObj.userId;
    configObj.gameId = self.configObj.gameId;
    configObj.roundId = self.configObj.roundId;
    [[VeGameManager sharedInstance] startWithConfig:configObj];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveAppWillTerminateNotification:)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveAppDidEnterBackgroundNotification:)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveAppWillEnterForegroundNotification:)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    [self.navigationController setNavigationBarHidden:YES animated:YES];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];

    [self.navigationController setNavigationBarHidden:NO animated:YES];
}

- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];

    UIButton *menuBtn = [self.view viewWithTag:999];
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
    // 容器视图
    self.containerView = ({
        UIView *containerView = [[UIView alloc] init];
        containerView.backgroundColor = [UIColor blackColor];
        [self.view addSubview:containerView];
        [containerView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.top.right.bottom.mas_equalTo(0);
        }];
        containerView;
    });

    // 实时日志
    self.timelylogLabel = ({
        UILabel *label = [[UILabel alloc] init];
        label.userInteractionEnabled = NO;
        label.textColor = [UIColor greenColor];
        label.textAlignment = NSTextAlignmentCenter;
        label.font = [UIFont systemFontOfSize:11.0f];
        label.backgroundColor = [UIColor grayColor];
        label.layer.masksToBounds = YES;
        label.layer.cornerRadius = 6.0f;
        label.adjustsFontSizeToFitWidth = YES;
        [self.view addSubview:label];
        [label mas_makeConstraints:^(MASConstraintMaker *make) {
            make.centerX.mas_equalTo(self.view);
            make.bottom.mas_equalTo(self.view).offset(-34);
        }];
        label;
    });

    // 菜单
    UIButton *menuBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    menuBtn.tag = 999;
    menuBtn.layer.cornerRadius = 14.0f;
    menuBtn.backgroundColor = [UIColor redColor];
    menuBtn.titleLabel.font = [UIFont systemFontOfSize:9.0f];
    [menuBtn setTitle:@"Menu" forState:UIControlStateNormal];
    [menuBtn setTitleColor:[UIColor yellowColor] forState:UIControlStateNormal];
    [menuBtn addTarget:self action:@selector(tappedButton:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:menuBtn];
    [menuBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.mas_equalTo(self.view);
        make.size.mas_equalTo(CGSizeMake(28, 28));
        make.bottom.mas_equalTo(self.view).offset(-100);
    }];

    self.scrollView = ({
        UIScrollView *scrollView = [[UIScrollView alloc] init];
        scrollView.hidden = YES;
        scrollView.showsVerticalScrollIndicator = NO;
        [self.view addSubview:scrollView];
        [scrollView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.width.mas_equalTo(170);
            make.left.mas_equalTo(self.view).offset(10.0f);
            make.top.mas_equalTo(self.view).offset(44);
            make.bottom.mas_equalTo(self.view).offset(-34);
        }];
        scrollView;
    });

    UIView *btnView = [[UIView alloc] init];
    [self.scrollView addSubview:btnView];
    [btnView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.right.bottom.mas_equalTo(self.scrollView);
        make.width.mas_equalTo(self.scrollView);
    }];

    UIButton *button0 = [self createButton:@"退出"];
    button0.tag = 100;
    button0.backgroundColor = [UIColor redColor];
    [button0 setTitleColor:[UIColor yellowColor] forState:UIControlStateNormal];
    [btnView addSubview:button0];
    [button0 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(btnView);
        make.left.mas_equalTo(btnView);
        make.size.mas_equalTo(CGSizeMake(80, 40));
    }];

    UIButton *button1 = [self createButton:@"显示虚拟键盘"];
    button1.tag = 101;
    [btnView addSubview:button1];
    [button1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button0);
        make.top.mas_equalTo(button0);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button2 = [self createButton:@"获取虚拟键盘状态"];
    button2.tag = 102;
    [btnView addSubview:button2];
    [button2 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button0);
        make.left.mas_equalTo(button0);
        make.top.mas_equalTo(button0.mas_bottom).offset(10);
    }];

    UIButton *button3 = [self createButton:@"设置无操作时长"];
    button3.tag = 103;
    [btnView addSubview:button3];
    [button3 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button2);
        make.top.mas_equalTo(button2);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button4 = [self createButton:@"获取无操作时长"];
    button4.tag = 104;
    [btnView addSubview:button4];
    [button4 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button2);
        make.left.mas_equalTo(button2);
        make.top.mas_equalTo(button2.mas_bottom).offset(10);
    }];

    UIButton *button5 = [self createButton:@"切换前后台"];
    button5.tag = 105;
    [btnView addSubview:button5];
    [button5 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button4);
        make.top.mas_equalTo(button4);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button6 = [self createButton:@"重启游戏"];
    button6.tag = 106;
    [btnView addSubview:button6];
    [button6 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button4);
        make.left.mas_equalTo(button4);
        make.top.mas_equalTo(button4.mas_bottom).offset(10);
    }];

    UIButton *button7 = [self createButton:@"隐藏手柄"];
    button7.tag = 107;
    [btnView addSubview:button7];
    [button7 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button6);
        make.top.mas_equalTo(button6);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button8 = [self createButton:@"键盘事件"];
    button8.tag = 108;
    [btnView addSubview:button8];
    [button8 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(button6);
        make.left.mas_equalTo(button6);
        make.top.mas_equalTo(button6.mas_bottom).offset(10);
    }];

    UIButton *button9 = [self createButton:@"鼠标移动"];
    button9.tag = 109;
    [btnView addSubview:button9];
    [button9 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button8);
        make.size.mas_equalTo(button8);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button10 = [self createButton:@"鼠标按键"];
    button10.tag = 110;
    [btnView addSubview:button10];
    [button10 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button8);
        make.size.mas_equalTo(button8);
        make.top.mas_equalTo(button8.mas_bottom).offset(10);
    }];

    UIButton *button11 = [self createButton:@"鼠标滚轮"];
    button11.tag = 111;
    [btnView addSubview:button11];
    [button11 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button10);
        make.size.mas_equalTo(button10);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button12 = [self createButton:@"切换手柄"];
    button12.tag = 112;
    [btnView addSubview:button12];
    [button12 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button10);
        make.size.mas_equalTo(button10);
        make.top.mas_equalTo(button10.mas_bottom).offset(10);
    }];

    UIButton *button13 = [self createButton:@"发送字符串消息"];
    button13.tag = 113;
    [btnView addSubview:button13];
    [button13 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button12);
        make.size.mas_equalTo(button12);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button14 = [self createButton:@"发送二进制消息"];
    button14.tag = 114;
    [btnView addSubview:button14];
    [button14 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button12);
        make.size.mas_equalTo(button12);
        make.top.mas_equalTo(button12.mas_bottom).offset(10);
    }];

    UIButton *button15 = [self createButton:@"清晰度"];
    button15.tag = 115;
    [btnView addSubview:button15];
    [button15 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button14);
        make.size.mas_equalTo(button14);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button16 = [self createButton:@"禁止触控事件"];
    button16.tag = 116;
    [btnView addSubview:button16];
    [button16 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button14);
        make.size.mas_equalTo(button14);
        make.top.mas_equalTo(button14.mas_bottom).offset(10);
    }];

    UIButton *button17 = [self createButton:@"发送二进制消息(无回执)"];
    button17.tag = 117;
    [btnView addSubview:button17];
    [button17 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button16);
        make.size.mas_equalTo(button16);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button18 = [self createButton:@"发送二进制消息(超时3秒)"];
    button18.tag = 118;
    [btnView addSubview:button18];
    [button18 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button16);
        make.size.mas_equalTo(button16);
        make.top.mas_equalTo(button16.mas_bottom).offset(10);
    }];

    UIButton *button19 = [self createButton:@"发送消息(无回执)"];
    button19.tag = 119;
    [btnView addSubview:button19];
    [button19 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button18);
        make.size.mas_equalTo(button18);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button20 = [self createButton:@"发送消息(超时3秒)"];
    button20.tag = 120;
    [btnView addSubview:button20];
    [button20 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button18);
        make.size.mas_equalTo(button18);
        make.top.mas_equalTo(button18.mas_bottom).offset(10);
    }];

    UIButton *button21 = [self createButton:@"切换角色"];
    button21.tag = 121;
    [btnView addSubview:button21];
    [button21 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button20);
        make.size.mas_equalTo(button20);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button22 = [self createButton:@"设置保活时长"];
    button22.tag = 122;
    [btnView addSubview:button22];
    [button22 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button20);
        make.size.mas_equalTo(button20);
        make.top.mas_equalTo(button20.mas_bottom).offset(10);
    }];

    UIButton *button23 = [self createButton:@"编辑手柄"];
    button23.tag = 123;
    [btnView addSubview:button23];
    [button23 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button22);
        make.size.mas_equalTo(button22);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button24 = [self createButton:@"刷新手柄"];
    button24.tag = 124;
    [btnView addSubview:button24];
    [button24 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button22);
        make.size.mas_equalTo(button22);
        make.top.mas_equalTo(button22.mas_bottom).offset(10);
    }];

    UIButton *button25 = [self createButton:@"关闭手柄震动"];
    button25.tag = 125;
    [btnView addSubview:button25];
    [button25 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button24);
        make.size.mas_equalTo(button24);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button26 = [self createButton:@"设置无操作倒计时"];
    button26.tag = 126;
    [btnView addSubview:button26];
    [button26 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button24);
        make.size.mas_equalTo(button24);
        make.top.mas_equalTo(button24.mas_bottom).offset(10);
    }];

    UIButton *button27 = [self createButton:@"获取无操作倒计时"];
    button27.tag = 127;
    [btnView addSubview:button27];
    [button27 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button26);
        make.size.mas_equalTo(button26);
        make.right.mas_equalTo(btnView);
    }];

    UIButton *button28 = [self createButton:@"重置无操作倒计时"];
    button28.tag = 128;
    [btnView addSubview:button28];
    [button28 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button26);
        make.size.mas_equalTo(button26);
        make.top.mas_equalTo(button26.mas_bottom).offset(10);
    }];

    UIButton *button29 = [self createButton:@"替换远端输入法内容"];
    button29.tag = 129;
    [btnView addSubview:button29];
    [button29 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button28);
        make.size.mas_equalTo(button28);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button30 = [self createButton:@"发送带key的二进制"];
    button30.tag = 130;
    [btnView addSubview:button30];
    [button30 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button28);
        make.size.mas_equalTo(button28);
        make.top.mas_equalTo(button28.mas_bottom).offset(10);
    }];
    
    UIButton *button31 = [self createButton:@"键鼠SDK展示"];
    button31.tag = 131;
    [btnView addSubview:button31];
    [button31 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button30);
        make.size.mas_equalTo(button30);
        make.right.mas_equalTo(btnView);
    }];
    
    UIButton *button32 = [self createButton:@"键鼠SDK编辑模式"];
    button32.tag = 132;
    [btnView addSubview:button32];
    [button32 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(button30);
        make.size.mas_equalTo(button30);
        make.top.mas_equalTo(button30.mas_bottom).offset(10);
    }];
    
    UIButton *button33 = [self createButton:@"键鼠touch开关"];
    button33.tag = 133;
    [btnView addSubview:button33];
    [button33 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(button32);
        make.size.mas_equalTo(button32);
        make.right.mas_equalTo(btnView);
        make.bottom.mas_equalTo(btnView);
    }];
}

#pragma mark - Utils

- (UIButton *)createButton:(NSString *)title
{
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.layer.cornerRadius = 3.0f;
    button.titleLabel.adjustsFontSizeToFitWidth = YES;
    button.backgroundColor = [UIColor systemBlueColor];
    button.titleLabel.font = [UIFont systemFontOfSize:13.0f];
    [button setTitle:title forState:UIControlStateNormal];
    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [button addTarget:self action:@selector(tappedButton:) forControlEvents:UIControlEventTouchUpInside];
    return button;
}

#pragma mark - button action

- (void)tappedButton:(UIButton *)btn
{
    if (btn.tag == 999) { // Menu
        btn.selected = !btn.selected;
        self.scrollView.hidden = !btn.selected;
    } else if (btn.tag == 100) {
        [SVProgressHUD dismiss];
        [[VeGameManager sharedInstance] stop];
        [self.navigationController popViewControllerAnimated:YES];
    } else if (btn.tag == 101) {
        if (!btn.selected) {
            [self showVirtualKeyboardView];
        } else {
            [self hideVirtualKeyboardView];
        }
        [btn setTitle:!btn.selected ? @"隐藏虚拟键盘" : @"显示虚拟键盘" forState:UIControlStateNormal];
        btn.selected = !btn.selected;
    } else if (btn.tag == 102) {
        NSString *state = self.virtualKeyboardView.hidden ? @"隐藏" : @"显示";
        [self.view makeToast:[NSString stringWithFormat:@"虚拟键盘%@", state] duration:2.0f position:CSToastPositionCenter];
    } else if (btn.tag == 103) {
        __weak typeof(self) weakSelf = self;
        [self setCustomViewController:@"设置无操作回收时长"
                             hintText:nil
                      tappedSureBlock:^(UITextField *tf) {
                          if ([tf.text integerValue] != NSNotFound) {
                              if ([[VeGameManager sharedInstance] setAutoRecycleTime:[tf.text integerValue]] == -2) {
                                  [weakSelf.view makeToast:@"设置的时间小于等于0，非法！"
                                                  duration:2.0f
                                                  position:CSToastPositionCenter];
                              }
                          }
                      }];
    } else if (btn.tag == 104) {
        [[VeGameManager sharedInstance] getAutoRecycleTime];
    } else if (btn.tag == 105) {
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] switchBackground:btn.selected];
    } else if (btn.tag == 106) {
        [[VeGameManager sharedInstance] restartGame];
    } else if (btn.tag == 107) {
        btn.selected = !btn.selected;
        [VeGameManager sharedInstance].gamePadView.hidden = btn.selected;
        [btn setTitle:btn.selected ? @"显示手柄" : @"隐藏手柄" forState:UIControlStateNormal];
    } else if (btn.tag == 108) { // 键盘事件
        CustomViewController *alert = [CustomViewController alertControllerWithTitle:@"请输入键盘事件" message:nil preferredStyle:UIAlertControllerStyleAlert];
        alert.rotation = self.rotation;
        __block UITextField *tf1 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf1 = textField;
            tf1.placeholder = @"键盘keycode";
            tf1.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf2 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf2 = textField;
            tf2.placeholder = @"0：按下，1：抬起";
            tf2.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        UIAlertAction *action0 = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
        [alert addAction:action0];
        UIAlertAction *action1 = [UIAlertAction actionWithTitle:@"确认"
                                                          style:UIAlertActionStyleDestructive
                                                        handler:^(UIAlertAction *_Nonnull action) {
                                                            if ([tf1.text integerValue] != NSNotFound && [tf2.text integerValue] != NSNotFound) {
                                                                [[VeGameManager sharedInstance] sendKeyboardData:tf1.text.integerValue state:tf2.text.integerValue == 0];
                                                            }
                                                        }];
        [alert addAction:action1];
        [self presentViewController:alert animated:NO completion:nil];
    } else if (btn.tag == 109) { // 鼠标移动
        CustomViewController *alert = [CustomViewController alertControllerWithTitle:@"请输入鼠标增量" message:nil preferredStyle:UIAlertControllerStyleAlert];
        alert.rotation = self.rotation;
        __block UITextField *tf0 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf0 = textField;
            tf0.placeholder = @"x坐标比例值，[0, 1]";
            tf0.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf1 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf1 = textField;
            tf1.placeholder = @"y坐标比例值，[0, 1]";
            tf1.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf2 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf2 = textField;
            tf2.placeholder = @"x坐标值";
            tf2.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf3 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf3 = textField;
            tf3.placeholder = @"y坐标值";
            tf3.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        UIAlertAction *action0 = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
        [alert addAction:action0];
        UIAlertAction *action1 = [UIAlertAction actionWithTitle:@"确认"
                                                          style:UIAlertActionStyleDestructive
                                                        handler:^(UIAlertAction *_Nonnull action) {
                                                            if ([tf0.text floatValue] != NSNotFound && [tf1.text floatValue] != NSNotFound && [tf2.text floatValue] != NSNotFound && [tf3.text floatValue] != NSNotFound) {
                                                                [[VeGameManager sharedInstance] sendMoveEventWithAbsX:tf0.text.floatValue
                                                                                                                 absY:tf1.text.floatValue
                                                                                                               deltaX:tf2.text.floatValue - self.last_mouse_move_x
                                                                                                               deltaY:tf3.text.floatValue - self.last_mouse_move_y];
                                                                self.last_mouse_move_x = tf2.text.floatValue;
                                                                self.last_mouse_move_y = tf3.text.floatValue;
                                                            }
                                                        }];
        [alert addAction:action1];
        [self presentViewController:alert animated:NO completion:nil];
    } else if (btn.tag == 110) { // 鼠标按键
        CustomViewController *alert = [CustomViewController alertControllerWithTitle:@"请输入鼠标按键" message:nil preferredStyle:UIAlertControllerStyleAlert];
        alert.rotation = self.rotation;
        __block UITextField *tf0 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf0 = textField;
            tf0.placeholder = @"1：左键，2：右键，4：中间，5：XButton1，6：XButton2";
            tf0.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf1 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf1 = textField;
            tf1.placeholder = @"0：按下，1：抬起";
            tf1.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf2 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf2 = textField;
            tf2.placeholder = @"x坐标比例值，[0, 1]";
            tf2.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf3 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf3 = textField;
            tf3.placeholder = @"y坐标比例值，[0, 1]";
            tf3.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        UIAlertAction *action0 = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
        [alert addAction:action0];
        UIAlertAction *action1 = [UIAlertAction actionWithTitle:@"确认"
                                                          style:UIAlertActionStyleDestructive
                                                        handler:^(UIAlertAction *_Nonnull action) {
                                                            if ([tf0.text intValue] != NSNotFound && [tf1.text intValue] != NSNotFound && [tf2.text intValue] != NSNotFound && [tf3.text intValue] != NSNotFound) {
                                                                [[VeGameManager sharedInstance] sendKeyEventWithKeyType:tf0.text.intValue action:tf1.text.intValue absX:tf2.text.floatValue absY:tf3.text.floatValue];
                                                            }
                                                        }];
        [alert addAction:action1];
        [self presentViewController:alert animated:NO completion:nil];
    } else if (btn.tag == 111) { // 鼠标滚轮
        [self setCustomViewController:@"请输入鼠标滚轮"
                             hintText:@"滚轮值"
                      tappedSureBlock:^(UITextField *tf) {
                          if ([tf.text intValue] != NSNotFound) {
                              [[VeGameManager sharedInstance] sendWheelEventWithHorizontalScroll:0 verticalScroll:tf.text.intValue];
                          }
                      }];
    } else if (btn.tag == 112) {
        [self setCustomViewController:@"请输入方案ID（keyMappingId）"
                             hintText:@"keyMappingId"
                      tappedSureBlock:^(UITextField *tf) {
                          [(VeGamePadView *)[VeGameManager sharedInstance].gamePadView switchGamePadViewWithKeyMappingId:tf.text];
                      }];
    } else if (btn.tag == 113) {
        NSString *message = @"this is a test";
        [[VeGameManager sharedInstance] sendGeneralStringMessage:message];
    } else if (btn.tag == 114) {
        NSString *message = @"this is a binary message";
        NSData *data = [message dataUsingEncoding:NSUTF8StringEncoding];
        [[VeGameManager sharedInstance] sendGeneralBinaryMessage:data];
    } else if (btn.tag == 115) { // 清晰度
        [self setCustomViewController:@"请输入清晰度ID"
                             hintText:nil
                      tappedSureBlock:^(UITextField *tf) {
                          if ([tf.text integerValue] != NSNotFound) {
                              [[VeGameManager sharedInstance] switchVideoStreamProfile:tf.text.integerValue];
                          }
                      }];
    } else if (btn.tag == 116) { // 拦截触控事件
        btn.selected = !btn.selected;
        [[VeGameManager sharedInstance] setInterceptSendTouchEvent:btn.selected];
        [btn setTitle:btn.selected ? @"开启触控事件" : @"禁止触控事件" forState:UIControlStateNormal];
    } else if (btn.tag == 117) { // 发送二进制消息(无回执)
        NSString *payload = @"Hello World（binary no ack）";
        VeBaseChannelMessage *msg = [[VeGameManager sharedInstance] sendBinaryMessage:[payload dataUsingEncoding:NSUTF8StringEncoding]];
        NSLog(@"send no ack msg: %@", [msg description]);
    } else if (btn.tag == 118) { // 发送二进制消息(超时3秒)
        NSString *payload = @"Hello World（binary need ack）";
        VeBaseChannelMessage *msg = [[VeGameManager sharedInstance] sendBinaryMessage:[payload dataUsingEncoding:NSUTF8StringEncoding] timeout:3000];
        NSLog(@"send timeout msg: %@", [msg description]);
    } else if (btn.tag == 119) { // 发送消息(无回执)
        NSString *payload = @"Hello World（string no ack）";
        VeBaseChannelMessage *msg = [[VeGameManager sharedInstance] sendMessage:payload];
        NSLog(@"send no ack msg: %@", [msg description]);
    } else if (btn.tag == 120) { // 发送消息(超时3秒)
        NSString *payload = @"Hello World（string need ack）";
        VeBaseChannelMessage *msg = [[VeGameManager sharedInstance] sendMessage:payload timeout:3000];
        NSLog(@"send timeout msg: %@", [msg description]);
    } else if (btn.tag == 121) { // 切换角色
        CustomViewController *alert = [CustomViewController alertControllerWithTitle:@"请输入目标Uid&角色" message:nil preferredStyle:UIAlertControllerStyleAlert];
        alert.rotation = self.rotation;
        __block UITextField *tf1 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf1 = textField;
            tf1.placeholder = @"目标用户的Uid";
            tf1.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        __block UITextField *tf2 = [UITextField new];
        [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
            tf2 = textField;
            tf2.placeholder = @"0：Viewer，1：Player";
            tf2.keyboardType = UIKeyboardTypeDecimalPad;
        }];
        UIAlertAction *action0 = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
        [alert addAction:action0];
        UIAlertAction *action1 = [UIAlertAction actionWithTitle:@"确认"
                                                          style:UIAlertActionStyleDestructive
                                                        handler:^(UIAlertAction *_Nonnull action) {
                                                            if ([tf1.text integerValue] != NSNotFound) {
                                                                [[VeGameManager sharedInstance] changeRole:tf1.text role:tf2.text.integerValue];
                                                            }
                                                        }];
        [alert addAction:action1];
        [self presentViewController:alert animated:NO completion:nil];
    } else if (btn.tag == 122) { // 设置保活时长
        __weak typeof(self) weakSelf = self;
        [self setCustomViewController:@"设置后台保活时长"
                             hintText:nil
                      tappedSureBlock:^(UITextField *tf) {
                          if ([tf.text integerValue] != NSNotFound) {
                              if ([[VeGameManager sharedInstance] setIdleTime:[tf.text integerValue]] == -2) {
                                  [weakSelf.view makeToast:@"设置的时间小于等于0，非法！"
                                                  duration:2.0f
                                                  position:CSToastPositionCenter];
                              }
                          }
                      }];
    } else if (btn.tag == 123) {
        [self.gamePadView modifyGamePadLayout];
    } else if (btn.tag == 124) {
        NSDictionary *testGamePadConfig = [[NSUserDefaults standardUserDefaults] objectForKey:@"VeGamePadTestConfig"];
        if (testGamePadConfig && [testGamePadConfig isKindOfClass:[NSDictionary class]]) {
            [self.gamePadView refreshGamePadLayoutWithConfig:testGamePadConfig];
        }
    } else if (btn.tag == 125) { // 手柄震动开关
        [self.gamePadView enableVibration:btn.selected];
        btn.selected = !btn.selected;
        [btn setTitle:btn.selected ? @"开启手柄震动" : @"关闭手柄震动" forState:UIControlStateNormal];
    } else if (btn.tag == 126) {
        __weak typeof(self) weakSelf = self;
        [self setCustomViewController:@"设置无操作回收倒计时"
                             hintText:nil
                      tappedSureBlock:^(UITextField *tf) {
                          if ([tf.text integerValue] != NSNotFound) {
                              if ([[VeGameManager sharedInstance] setIdleReminderDuration:[tf.text intValue]] == -2) {
                                  [weakSelf.view makeToast:@"设置的时间小于等于0，非法！"
                                                  duration:2.0f
                                                  position:CSToastPositionCenter];
                              }
                          }
                      }];
    } else if (btn.tag == 127) {
        [[VeGameManager sharedInstance] getIdleReminderDuration];
    } else if (btn.tag == 128) {
        [[VeGameManager sharedInstance] resetIdleReminderDuration];
    } else if (btn.tag == 129) {
        [self setCustomViewController:@"输入法内容"
                             hintText:nil
                      tappedSureBlock:^(UITextField *tf) {
                          [[VeGameManager sharedInstance] replaceIMEContent:tf.text];
                      }];
    } else if (btn.tag == 130) {
        [self setCustomViewController:@"发送带key的二进制"
                             hintText:@"输入key"
                      tappedSureBlock:^(UITextField *tf) {
                          [[VeGameManager sharedInstance] sendBinaryMessage:[@"测试二进制" dataUsingEncoding:NSUTF8StringEncoding] key:tf.text];
                      }];
    } else if (btn.tag == 131) {
        self.suite = [[VeGameVirtualInputSuiteManager alloc] initWithParentView:self.containerView];
        self.suite.delegate = self;
        if ([[NSUserDefaults standardUserDefaults] objectForKey:@"VeGameVirtualInputSuiteConfig"]) {
            [self.suite showWithConfig:[[NSUserDefaults standardUserDefaults] objectForKey:@"VeGameVirtualInputSuiteConfig"]];
        }
    } else if (btn.tag == 132) {
        [self.suite modifyVirtualInputLayout];
    } else if (btn.tag == 133) {
        btn.selected = !btn.selected;
        [self.suite enableTouch:btn.selected];
    }
}

- (void)setCustomViewController:(NSString *)title hintText:(NSString *)hint tappedSureBlock:(void (^)(UITextField *tf))block
{
    __block UITextField *tf = [UITextField new];
    CustomViewController *alert = [CustomViewController alertControllerWithTitle:title message:nil preferredStyle:UIAlertControllerStyleAlert];
    alert.rotation = self.rotation;
    [alert addTextFieldWithConfigurationHandler:^(UITextField *_Nonnull textField) {
        textField.keyboardType = UIKeyboardTypeDefault;
        textField.placeholder = hint;
        tf = textField;
    }];
    UIAlertAction *action0 = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
    [alert addAction:action0];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle:@"确认"
                                                      style:UIAlertActionStyleDestructive
                                                    handler:^(UIAlertAction *_Nonnull action) {
                                                        !block ?: block(tf);
                                                    }];
    [alert addAction:action1];
    [self presentViewController:alert animated:NO completion:nil];
}

- (void)showVirtualKeyboardView
{
    [self.virtualKeyboardView setHidden:NO];
}

- (void)hideVirtualKeyboardView
{
    [self.virtualKeyboardView setHidden:YES];
}

#pragma mark - setter

- (void)setRotation:(NSInteger)rotation
{
    if (_rotation != rotation) {
        _rotation = rotation;
        if (@available(iOS 16, *)) {
            [self setNeedsUpdateOfSupportedInterfaceOrientations];
        } else {
            [Utils rotateDeviceToOrientation:rotation];
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
            UIButton *menuBtn = [self.view viewWithTag:999];
            menuBtn.center = CGPointMake(self.view.bounds.size.width - menuBtn.bounds.size.width / 2, self.view.bounds.size.height / 2 + menuBtn.bounds.size.height);
            [self.scrollView setContentOffset:CGPointMake(0, 0) animated:NO];
        });
    }
}

#pragma mark - VeVirtualKeyboardViewDelegate

- (void)virtualKeyOnClicked:(NSInteger)keyCode state:(BOOL)state
{
    NSLog(@"%ld-----%d", keyCode, state);
    [[VeGameManager sharedInstance] sendKeyboardData:(VeGameKeyboardKeyType)keyCode state:state];
}

#pragma mark - VeGameManagerDelegate

- (void)firstRemoteAudioFrameArrivedFromGameManager:(VeGameManager *)manager
{
    NSLog(@"--- PC云游戏首帧“音频”到达 ---");
}

- (void)firstRemoteVideoFrameArrivedFromGameManager:(VeGameManager *)manager
{
    NSLog(@"--- PC云游戏首帧“视频”到达 ---");
}

- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId planId:(NSString *)planId extra:(NSDictionary *)extra
{
    [SVProgressHUD dismiss];
}

- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation
{
    self.rotation = rotation;
}

- (void)gameManager:(VeGameManager *)manager operationDelay:(NSInteger)delayTime
{
    self.operationDelayTime = [NSString stringWithFormat:@"%ld", (long)delayTime];
}

- (void)gameManager:(VeGameManager *)manager onRemoteStreamStats:(VeBaseRemoteStreamStats *)stats
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.timelylogLabel.text = [NSString stringWithFormat:@"delay: %@ms rtt: %ldms loss: %.2f%% bit: %ldkbps fps: %ldfps  e2eDelay：%ld - %ld",
                                                              self.operationDelayTime ?: @"0", (long)stats.videoRtt, stats.audioLossRate + stats.videoLossRate, stats.receivedVideoKBitrate, (long)stats.renderOutputFrameRate, stats.audioE2eDelay, stats.videoE2eDelay];
    });
}

- (void)gameManager:(VeGameManager *)manager switchVideoStreamProfile:(BOOL)result fromIndex:(NSInteger)index1 toIndex:(NSInteger)index2 targetParams:(NSDictionary *)paramsDict
{
    [self.view makeToast:[NSString stringWithFormat:@"清晰度切换：%@，fromIndex = %@, toIndex = %@", result ? @"成功" : @"失败", @(index1), @(index2)]
                duration:2.0f
                position:CSToastPositionCenter];
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
            [self.view makeToast:str
                        duration:2.0f
                        position:CSToastPositionBottom];
        });
    }
}

- (void)gameManager:(VeGameManager *)manager onNetworkQuality:(VeBaseNetworkQuality)quality
{
    switch (quality) {
        case VeBaseNetworkQualityGood: {
            // NSLog(@"--- 网络情况良好 Quality:%li ---", quality);
        } break;
        case VeBaseNetworkQualityBad: {
            // NSLog(@"--- 网络情况较差 Quality:%li ---", quality);
        } break;
        case VeBaseNetworkQualityVeryBad: {
            // NSLog(@"--- 网络情况糟糕 Quality:%li ---", quality);
        } break;
        case VeBaseNetworkQualityDown: {
            // NSLog(@"--- 网络不可用 Quality:%li ---", quality);
        } break;
        default:
            break;
    }
}

- (void)gameManager:(VeGameManager *)manager onJoinRoomRoleResult:(VeBaseRoleType)role reason:(NSInteger)reason playerUserId:(NSString *)player_uid
{
    NSLog(@"当前用户角色：%@，result：%@，player_uid：%@", role == VeBaseRoleTypeViewer ? @"VeBaseRoleTypeViewer" : @"VeBaseRoleTypePlayer", reason == 0 ? @"成功" : @"失败", player_uid);
}

- (void)gameManager:(VeGameManager *)manager onChangeRoleCallBack:(VeBaseRoleType)role result:(NSInteger)result destUserId:(NSString *)dest_uid
{
    NSLog(@"要修改用户的最终角色：%@，result：%@，dest_uid：%@", role == VeBaseRoleTypeViewer ? @"VeBaseRoleTypeViewer" : @"VeBaseRoleTypePlayer", result == 0 ? @"成功" : @"失败", dest_uid);
}

- (void)gameManager:(VeGameManager *)manager onPlayerChanged:(NSString *)player_uid
{
    NSLog(@"当前的操作者Uid：%@", player_uid);
}

- (void)gameManager:(VeGameManager *)manager setAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time
{
    NSLog(@"设置无操作回收时长，code = %ld, time = %ld", code, time);
}

- (void)gameManager:(VeGameManager *)manager getAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time
{
    NSLog(@"获取无操作回收时长，code = %ld, time = %ld", code, time);
}

- (void)gameManager:(VeGameManager *)manager setIdleReminderDuration:(NSInteger)code countdown:(int)countdown
{
    NSLog(@"设置无操作回收倒计时，code = %ld, countdown = %d", code, countdown);
}

- (void)gameManager:(VeGameManager *)manager getIdleReminderDuration:(NSInteger)code countdown:(int)countdown
{
    NSLog(@"获取无操作回收倒计时，code = %ld, countdown = %d", code, countdown);
    [self.view makeToast:[NSString stringWithFormat:@"无操作倒计时%d", countdown]
                duration:2.0f
                position:CSToastPositionBottom];
}

- (void)gameManager:(VeGameManager *)manager onIdleReminder:(int)countdown
{
    [self setCustomViewController:[NSString stringWithFormat:@"无操作倒计时%d", countdown]
                         hintText:nil
                  tappedSureBlock:^(UITextField *tf){

                  }];
}

- (void)gameManager:(VeGameManager *)manager onBackgroundSwitched:(NSInteger)code
{
    NSLog(@"设置切换前后台，结果：%@", code == 0 ? @"后台" : @"前台");
}

- (void)gameManager:(VeGameManager *)manager onTouchEvent:(NSArray<VeBaseTouchEventItem *> *)touchArray currentCanvasSize:(CGSize)canvasSize
{
    VeBaseTouchEventItem *item = [touchArray lastObject];
    if (item.action == VeBaseTouchActionTypeStart) {
        self.last_mouse_move_x = item.x;
        self.last_mouse_move_y = item.y;
    } else if (item.action == VeBaseTouchActionTypeMoved) {
        [manager sendMoveEventWithAbsX:item.x
                                  absY:item.y
                                deltaX:(item.x - self.last_mouse_move_x) * canvasSize.width * 3
                                deltaY:(item.y - self.last_mouse_move_y) * canvasSize.height * 3];
        self.last_mouse_move_x = item.x;
        self.last_mouse_move_y = item.y;
    } else if (item.action == VeBaseTouchActionTypeEnded) {
        // 按下
        [manager sendKeyEventWithKeyType:VeGameMouseKeyTypeLeft
                                  action:VeGameMouseActionTypeDown
                                    absX:item.x
                                    absY:item.y];
        // 抬起
        [manager sendKeyEventWithKeyType:VeGameMouseKeyTypeLeft
                                  action:VeGameMouseActionTypeUp
                                    absX:item.x
                                    absY:item.y];
    }
}

- (void)gameManager:(VeGameManager *)manager onReceiveMessage:(VeBaseChannelMessage *)message
{
    if (message.type == 0) { // 文本
        NSLog(@"收到远端 Pod 通过 MCC 发送给客户端的文本消息：%@", message.text);
    } else if (message.type == 1) { // 二进制
        NSLog(@"message.data = %@", @(message.data.length));
        // NSLog(@"收到远端 Pod 通过 MCC 发送给客户端的二进制消息：%@", [[NSString alloc] initWithData: message.data encoding: NSUTF8StringEncoding]);
    }
}

- (void)gameManager:(VeGameManager *)manager onReceiveMessage:(VeBaseChannelMessage *)message key:(NSString *)key
{
    NSString *toast = [NSString stringWithFormat:@"key = %@   message.data = %@", key, @(message.data.length)];
    [self.view makeToast:toast
                duration:2.0f
                position:CSToastPositionCenter];
    NSLog(@"%@", toast);
}

- (void)gameManager:(VeGameManager *)manager onSendMessageResult:(BOOL)result messageId:(NSString *)mid
{
    NSLog(@"Send Message Result = %@，MessageId = %@", result ? @"成功" : @"失败", mid);
}

- (void)gameManager:(VeGameManager *)manager remoteMouseVisibility:(BOOL)visible
{
    NSLog(@"PC 游戏鼠标可见性：%@", visible ? @"true" : @"false");
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
        }
        if (toast.length > 0) {
            [self.view makeToast:toast
                        duration:2.0f
                        position:CSToastPositionCenter];
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
        } else if (errCode == ERROR_MESSAGE_VERSION_NOT_SUPPORT) {
            toast = @"50010 消息通道版本不支持";
        }
        if (toast.length > 0) {
            [self.view makeToast:toast
                        duration:2.0f
                        position:CSToastPositionCenter];
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
        } else if (warnCode == WARNING_SDK_LACK_OF_LOCATION_PERMISSION) {
            toast = @"30007 无定位权限";
        } else if (warnCode == WARNING_LOCAL_ALREADY_SET_BACKGROUND) {
            toast = @"40037 用户重复调用切换后台接口";
        } else if (warnCode == WARNING_LOCAL_ALREADY_SET_FOREGROUND) {
            toast = @"40038 用户重复调用切换前台接口";
        }
        if (toast.length > 0) {
            [self.view makeToast:toast
                        duration:2.0f
                        position:CSToastPositionCenter];
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
        } else if (errCode == ERROR_NET_PROBE_FAILED) {
            toast = @"32000 网络探测失败";
        } else if (errCode == ERROR_FIRST_FRAME_TIME_OUT) {
            toast = @"33001 首帧超时";
        } else if (errCode == ERROR_GAME_STOPPED_DUPLICATE_START) {
            toast = @"40007 游戏停止，原因：在不同设备上使用相同参数请求Start";
        } else if (errCode == ERROR_NET_REQUEST_ERROR) {
            toast = @"60001 网络请求失败";
        } else if (errCode == ERROR_HTTP_REQUEST_ERROR) {
            toast = @"60002 网络请求失败";
        }
        if (toast.length > 0) {
            [self.view makeToast:toast
                        duration:2.0f
                        position:CSToastPositionCenter];
        }
    });
}

- (void)gameManager:(VeGameManager *)manager receivedGeneralStringMessage:(NSString *)dataString
{
    NSLog(@"%@", dataString);
}

- (void)gameManager:(VeGameManager *)manager receivedGeneralBinaryMessage:(NSData *)data
{
    NSString *dataStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"%@", dataStr);
}

- (void)onCommandHideFromeGameManager:(VeGameManager *)manager
{
    [self.view makeToast:@"输入法隐藏" duration:2.f position:CSToastPositionCenter];
}

- (void)gameManager:(VeGameManager *)manager onCommandShow:(NSString *)content
{
    [self.view makeToast:@"输入法显示" duration:2.f position:CSToastPositionCenter];
}

#pragma mark - VeGamePadViewDelegate

- (void)sendGamePadDataWithButtons:(int32_t)btn_values lt:(BOOL)lt_status rt:(BOOL)rt_status lx:(CGFloat)left_joystick_x ly:(CGFloat)left_joystick_y rx:(CGFloat)right_joystick_x ry:(CGFloat)right_joystick_y
{
    VeGameGamePadMessage *data = [VeGameGamePadMessage new];
    data.lt_status = lt_status;
    data.rt_status = rt_status;
    data.btn_values = btn_values;
    data.left_joystick_x = left_joystick_x;
    data.left_joystick_y = left_joystick_y;
    data.right_joystick_x = right_joystick_x;
    data.right_joystick_y = right_joystick_y;
    [[VeGameManager sharedInstance] sendGamePadData:data];
}

- (void)keyMappingListLoadCompleted:(NSArray<NSDictionary *> *)keyMappingList
{
    NSLog(@"gamepad key map list = %@", keyMappingList);
}

- (void)gamePadModificationCompleted:(NSDictionary *)config
{
    NSLog(@"gamepad key config = %@", config);
    if (!config || ![config isKindOfClass:[NSDictionary class]]) {
        return;
    }
    [[NSUserDefaults standardUserDefaults] setObject:config forKey:@"VeGamePadTestConfig"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)onCancel
{
    [self.view makeToast:@"手柄取消编辑" duration:2.f position:CSToastPositionCenter];
}

- (void)restoreDefaultConfig
{
    [self.view makeToast:@"手柄恢复默认配置" duration:2.f position:CSToastPositionCenter];
}

#pragma mark - receive notification

- (void)receiveAppWillTerminateNotification:(NSNotification *)notification
{
    [[VeGameManager sharedInstance] stop];
}

- (void)receiveAppDidEnterBackgroundNotification:(NSNotification *)notification
{
    [[VeGameManager sharedInstance] switchPaused:YES];
}

- (void)receiveAppWillEnterForegroundNotification:(NSNotification *)notification
{
    [[VeGameManager sharedInstance] switchPaused:NO];
}

- (void)gameManager:(VeGameManager *)manager onResetIdleReminderDuration:(NSInteger)status
{
    NSString *result = (status == 1) ? @"成功" : @"失败";
    [self.view makeToast:[NSString stringWithFormat:@"重置无操作倒计时%@", result] duration:2.f position:CSToastPositionCenter];
}

#pragma mark - VeGameVirtualInputSuiteDelegate

- (void)virtualInputSuiteKeyboardKeyClicked:(VeGameVirtualInputSuiteKeyboardKeyType)type state:(BOOL)state
{
    [[VeGameManager sharedInstance] sendKeyboardData:(VeGameKeyboardKeyType)type state:state];
}

- (void)virtualInputSuiteMouseKeyClicked:(VeGameVirtualInputSuiteMouseKeyType)type action:(VeGameVirtualInputSuiteMouseActionType)action horizontalValue:(int32_t)horizontalValue
{
    VeGameMouseMessage *mouse = [VeGameMouseMessage new];
    if (type != VeGameVirtualInputSuiteMouseKeyTypeWheelUp && type != VeGameVirtualInputSuiteMouseKeyTypeWheelDown) {
        mouse.key = (VeGameMouseKeyType)type;
        mouse.action = (VeGameMouseActionType)action;
    } else {
        mouse.action = (VeGameMouseActionType)action;
        mouse.wheel = horizontalValue;
    }
    [[VeGameManager sharedInstance] sendMouseData:mouse];
}

- (void)virtualInputSuiteModificationCompleted:(NSDictionary *)config
{
    [[NSUserDefaults standardUserDefaults] setObject:config forKey:@"VeGameVirtualInputSuiteConfig"];
}

- (void)virtualInputSuiteExitModify
{
    [self.view makeToast:@"退出编辑模式" duration:2.f position:CSToastPositionCenter];
}

- (void)virtualInputSuiteDidRestoreDefaultConfiguration
{
    [self.view makeToast:@"恢复默认设置" duration:2.f position:CSToastPositionCenter];
}

#pragma mark - getter

- (VeVirtualKeyboardView *)virtualKeyboardView
{
    if (!_virtualKeyboardView) {
        _virtualKeyboardView = [[VeVirtualKeyboardView alloc] initWithFrame:CGRectMake(44, SCREEN_HEIGHT - 236, SCREEN_WIDTH - 78, 236)];
        _virtualKeyboardView.delegate = self;
        _virtualKeyboardView.uid = self.configObj.userId;
        _virtualKeyboardView.did = [VeGameManager serviceDeviceId];
        _virtualKeyboardView.hidden = YES;
        [self.view addSubview:_virtualKeyboardView];
    }
    return _virtualKeyboardView;
}

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
    return UIInterfaceOrientationLandscapeRight;
}

- (void)dealloc
{
    NSLog(@"--- VeCloudPcDisplayViewController Dealloc ---");
}

@end
