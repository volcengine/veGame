//
//  VeRootViewController.m
//  Demo4S
//
//  Created by zhaozuotong on 2023/8/22.
//

#import "VeRootViewController.h"
#import "VeChatRoomViewController.h"
#import "Masonry/Masonry.h"
#import "VeCommonDefine.h"

@interface VeRootViewController () <UITextFieldDelegate>

@property (nonatomic, strong) UITextField *roomInputTextField;

@property (nonatomic, strong) UITextField *userInputTextField;

@property (nonatomic, strong) UITextField *reservedIdTextField;

@property (nonatomic, strong) UIButton *enterRoomButton;

@end

@implementation VeRootViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self.navigationController setNavigationBarHidden:YES];
    [self buildUI];
}

- (void)buildUI {
    [self.view addSubview:self.roomInputTextField];
    [self.roomInputTextField mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(@16);
        make.top.mas_equalTo(@150);
        make.right.mas_equalTo(@-16);
        make.height.mas_equalTo(@44);
    }];
    
    [self.view addSubview:self.userInputTextField];
    [self.userInputTextField mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(@16);
        make.top.equalTo(self.roomInputTextField.mas_bottom).offset(20.);
        make.right.equalTo(self.roomInputTextField);
        make.height.equalTo(self.roomInputTextField);
    }];
    
    [self.view addSubview:self.reservedIdTextField];
    [self.reservedIdTextField mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(@16);
        make.top.equalTo(self.userInputTextField.mas_bottom).offset(20.);
        make.right.equalTo(self.roomInputTextField);
        make.height.equalTo(self.roomInputTextField);
    }];
    
    [self.view addSubview:self.enterRoomButton];
    [self.enterRoomButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.view).offset(-50);
        make.centerX.equalTo(self.view);
        make.width.mas_equalTo(@200);
        make.height.mas_equalTo(@44);
    }];
}

- (void)enterRoomButtonClicked:(UIButton *)sender {
    VeChatRoomViewController *chatRoomVC = [[VeChatRoomViewController alloc] init];
    chatRoomVC.roomID = self.roomInputTextField.text;
    chatRoomVC.userID = self.userInputTextField.text;
    chatRoomVC.reservedID = self.reservedIdTextField.text;
    [self.navigationController pushViewController:chatRoomVC animated:YES];
}

- (UITextField *)roomInputTextField {
    if(!_roomInputTextField){
        _roomInputTextField = [[UITextField alloc] init];
        _roomInputTextField.borderStyle = UITextBorderStyleRoundedRect;
        _roomInputTextField.placeholder = @"请输入语聊房间ID";
        _roomInputTextField.text = @"";
        _roomInputTextField.delegate = self;
    }
    return _roomInputTextField;
}

- (UITextField *)userInputTextField {
    if(!_userInputTextField){
        _userInputTextField = [[UITextField alloc] init];
        _userInputTextField.borderStyle = UITextBorderStyleRoundedRect;
        _userInputTextField.placeholder = @"请输入用户ID";
        _userInputTextField.text = @"";
        _userInputTextField.delegate = self;
    }
    return _userInputTextField;
}

- (UITextField *)reservedIdTextField {
    if(!_reservedIdTextField){
        _reservedIdTextField = [[UITextField alloc] init];
        _reservedIdTextField.borderStyle = UITextBorderStyleRoundedRect;
        _reservedIdTextField.placeholder = @"请输入Reserved ID";
        _reservedIdTextField.text = @"";
        _reservedIdTextField.delegate = self;
    }
    return _reservedIdTextField;
}

- (UIButton *)enterRoomButton {
    if (!_enterRoomButton) {
        _enterRoomButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_enterRoomButton setTitle:@"进入房间" forState:UIControlStateNormal];
        _enterRoomButton.backgroundColor = [UIColor systemBlueColor];
        [_enterRoomButton addTarget:self action:@selector(enterRoomButtonClicked:) forControlEvents:UIControlEventTouchUpInside];
        _enterRoomButton.layer.masksToBounds = YES;
        _enterRoomButton.layer.cornerRadius = 4.;
    }
    return _enterRoomButton;
}


@end
