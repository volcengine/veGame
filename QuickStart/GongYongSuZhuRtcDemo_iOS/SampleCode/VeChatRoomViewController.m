//
//  VeChatRoomViewController.m
//  Demo4S
//
//  Created by zhaozuotong on 2023/8/22.
//

#import "VeChatRoomViewController.h"
#import "SVProgressHUD/SVProgressHUD.h"
#import "Masonry/Masonry.h"
#import "VeCommonDefine.h"
#import <VeGame/VeGameManager.h>
#import <VolcEngineRTC/VolcEngineRTC.h>

@interface VeChatRoomViewController () <UITextFieldDelegate, ByteRTCRoomDelegate, VeGameManagerDelegate, ByteRTCVideoDelegate>

@property (nonatomic, strong) UIButton *startGameButton;

@property (nonatomic, strong) UIButton *stopGameButton;

@property (nonatomic, strong) UIButton *closeButton;

@property (nonatomic, strong) ByteRTCVideo* rtcEngine;

@property (nonatomic, strong) ByteRTCRoom* chatRoom;

@property (nonatomic, strong) UIView *gameContainer;

@property (nonatomic, strong) UIView *renderView;

@property (nonatomic, strong) UILabel *roomLabel;

@property (nonatomic, strong) UILabel *user1Label;

@property (nonatomic, strong) UILabel *user2Label;

@end

@implementation VeChatRoomViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    [self buildUI];
    [self configVeGame];
    [self configRTC];
}

- (void)buildUI {
    [self.view addSubview:self.gameContainer];
    [self.gameContainer mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.view);
    }];
    [self.gameContainer addSubview:self.renderView];
    [self.renderView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.gameContainer);
    }];
    [self.gameContainer addSubview:self.stopGameButton];
    [self.stopGameButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.gameContainer).offset(-50);
        make.centerX.equalTo(self.gameContainer);
        make.width.mas_equalTo(@200);
        make.height.mas_equalTo(@44);
    }];
    
    [self.view addSubview:self.startGameButton];
    [self.startGameButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.view).offset(-50);
        make.centerX.equalTo(self.view);
        make.width.mas_equalTo(@200);
        make.height.mas_equalTo(@44);
    }];
    
    [self.view addSubview:self.closeButton];
    [self.closeButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.equalTo(self.view).offset(-16);
        make.top.mas_equalTo(@50);
        make.width.height.mas_equalTo(@30);
    }];
    
    [self.view addSubview:self.roomLabel];
    [self.roomLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.view);
        make.centerY.equalTo(self.closeButton);
    }];
    
    [self.view addSubview:self.user1Label];
    [self.user1Label mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.startGameButton.mas_top).offset(-100);
        make.left.equalTo(self.view).offset(20);
        make.right.equalTo(self.view).offset(-20);
    }];
    
    [self.view addSubview:self.user2Label];
    [self.user2Label mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.user1Label.mas_bottom).offset(20);
        make.left.right.equalTo(self.user1Label);
    }];
    self.gameContainer.hidden = YES;
}

- (void)configVeGame {
    [[VeGameManager sharedInstance] initWithAccountId: Account_ID];
    [VeGameManager sharedInstance].delegate = self;
    [VeGameManager sharedInstance].containerView = self.renderView;
}

- (void)configRTC {
    // 创建语聊场景
    self.rtcEngine = [ByteRTCVideo createRTCVideo:App_ID delegate:self parameters:@{}];
    ByteRTCUserInfo* userInfo = [[ByteRTCUserInfo alloc] init];
    userInfo.userId = self.userID;
    ByteRTCRoomConfig* roomConfig = [[ByteRTCRoomConfig alloc] init];
    roomConfig.profile = ByteRTCRoomProfileLiveBroadcasting;
    [self.rtcEngine setAudioScenario:ByteRTCAudioScenarioMedia];
    [self.rtcEngine startAudioCapture];
    self.chatRoom = [self.rtcEngine createRTCRoom:self.roomID];
    [self.chatRoom setRTCRoomDelegate:self];
    [self.chatRoom joinRoom:Chat_Room_Token userInfo:userInfo roomConfig:roomConfig];
}

- (void)startGame:(UIButton *)sender {
    [SVProgressHUD showWithStatus: @"游戏启动中..."];
    [self startGameWithReservedID:self.reservedID];
    
}

- (void)stopGame:(UIButton *)sender {
    [self showAlertWindow:@"你要退出游戏吗" handler:^(UIAlertAction *action) {
        self.gameContainer.hidden = YES;
        self.startGameButton.hidden = NO;
        [[VeGameManager sharedInstance] stop];
    }];
}

- (void)startGameWithReservedID:(NSString *)reservedID
{
    VeGameConfigObject *configObj = [VeGameConfigObject new];
    configObj.ak = Game_AK;
    configObj.sk = Game_SK;
    configObj.token = Game_Token;
    configObj.userId = Game_User_ID;
    configObj.gameId = Game_ID;
    configObj.roundId = Game_Round_ID;
    configObj.rtcVideo = self.rtcEngine;
    // 适用于语聊场景，具体可视具体业务场景而定
//    configObj.rtcRoomProfile = ByteRTCRoomProfileChatRoom;
    // 弹幕游戏模式
//    configObj.sessionMode = VeBaseSessionModeBarrage;
    // 是否需要本地渲染画面
//    configObj.needSubscribeStream = NO;
    configObj.reservedId = reservedID.length ? reservedID : nil;
    [[VeGameManager sharedInstance] startWithConfig: configObj];
}

- (void)closeButtonClicked:(UIButton *)sender {
    [self showAlertWindow:@"你要退出语聊吗" handler:^(UIAlertAction *action) {
        [self.chatRoom leaveRoom];
        [self.chatRoom destroy];
        [[VeGameManager sharedInstance] stop];
        [self.navigationController popViewControllerAnimated:YES];
    }];
}

- (void)showAlertWindow:(NSString *)title handler:(void (^ __nullable)(UIAlertAction *action))handler {
    UIAlertController *alert = [UIAlertController
                                alertControllerWithTitle:title
                                message:@""
                                preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *yesAction = [UIAlertAction actionWithTitle:@"是的" style:UIAlertActionStyleDestructive handler:handler];
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction *_Nonnull action) {
    }];
    [alert addAction:yesAction];
    [alert addAction:cancelAction];
    [self presentViewController:alert animated:YES completion:nil];
}

#pragma mark VeGameManagerDelegate
- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId planId:(NSString *)planId extra:(NSDictionary *)extra {
    NSLog(@"游戏启动成功");
}

- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errCode {
    NSLog(@"%lu", (unsigned long)errCode);
}

- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode {
    NSLog(@"%lu", (unsigned long)warnCode);
}

#pragma mark ByteRTCVideoDelegate
/**
   宿主侧将RTCVideo的回调传给VeGame SDK，根据业务场景，一些回调可能需要根据userid等信息做区分处理
 **/
- (void)rtcEngine:(ByteRTCVideo *)engine onFirstRemoteVideoFrameDecoded:(ByteRTCRemoteStreamKey *)streamKey withFrameInfo:(ByteRTCVideoFrameInfo *)frameInfo {
    NSLog(@"首帧解析完成");
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onFirstRemoteVideoFrameDecoded:streamKey withFrameInfo:frameInfo];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onFirstRemoteVideoFrameRendered:(ByteRTCRemoteStreamKey *)streamKey withFrameInfo:(ByteRTCVideoFrameInfo *)frameInfo {
    [SVProgressHUD dismiss];
    NSLog(@"首帧渲染完成");
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onFirstRemoteVideoFrameRendered:streamKey withFrameInfo:frameInfo];
    dispatch_async(dispatch_get_main_queue(), ^{
        self.gameContainer.hidden = NO;
        self.startGameButton.hidden = YES;
    });
}
- (void)rtcEngine:(ByteRTCVideo *)engine onRemoteVideoSizeChanged:(ByteRTCRemoteStreamKey *)streamKey withFrameInfo:(ByteRTCVideoFrameInfo *)frameInfo {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onRemoteVideoSizeChanged:streamKey withFrameInfo:frameInfo];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onConnectionStateChanged:(ByteRTCConnectionState)state {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onConnectionStateChanged:state];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onNetworkTypeChanged:(ByteRTCNetworkType)type {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onNetworkTypeChanged:type];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onSEIMessageReceived:(ByteRTCRemoteStreamKey *)remoteStreamKey andMessage:(NSData *)message {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onSEIMessageReceived:remoteStreamKey andMessage:message];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onAudioRouteChanged:(ByteRTCAudioRoute)device {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onAudioRouteChanged:device];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onAudioDeviceStateChanged:(NSString *)device_id device_type:(ByteRTCAudioDeviceType)device_type device_state:(ByteRTCMediaDeviceState)device_state device_error:(ByteRTCMediaDeviceError)device_error {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onAudioDeviceStateChanged:device_id device_type:device_type device_state:device_state device_error:device_error];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onRemoteAudioPropertiesReport:(NSArray<ByteRTCRemoteAudioPropertiesInfo *> *)audioPropertiesInfos totalRemoteVolume:(NSInteger)totalRemoteVolume {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onRemoteAudioPropertiesReport:audioPropertiesInfos totalRemoteVolume:totalRemoteVolume];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onFirstLocalVideoFrameCaptured:(ByteRTCStreamIndex)streamIndex withFrameInfo:(ByteRTCVideoFrameInfo *)frameInfo {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onFirstLocalVideoFrameCaptured:streamIndex withFrameInfo:frameInfo];
}
- (void)rtcEngine:(ByteRTCVideo *)engine onVideoDeviceStateChanged:(NSString *)device_id device_type:(ByteRTCVideoDeviceType)device_type device_state:(ByteRTCMediaDeviceState)device_state device_error:(ByteRTCMediaDeviceError)device_error {
    [[[VeGameManager sharedInstance] getRtcVideoHandler] rtcEngine:engine onVideoDeviceStateChanged:device_id device_type:device_type device_state:device_state device_error:device_error];
}

- (void)rtcEngine:(ByteRTCVideo *)engine onWarning:(ByteRTCWarningCode)Code {
    NSLog(@"warningCode = %ld", (long)Code);
}

- (void)rtcEngine:(ByteRTCVideo *)engine onError:(ByteRTCErrorCode)errorCode {
    NSLog(@"errorCode = %ld",(long)errorCode);
}

#pragma mark - ByteRTCRoomDelegate

- (void)rtcRoom:(ByteRTCRoom *_Nonnull)rtcRoom onRoomStateChanged:(NSString *_Nonnull)roomId withUid:(nonnull NSString *)uid
          state:(NSInteger)state
      extraInfo:(NSString *_Nonnull)extraInfo {
    NSLog(@"rtcRoom onRoomStateChanged:%ld", state);
    if (state == 0) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if ([self.userID isEqualToString:uid]) {
                self.user1Label.text = self.userID;
                self.user1Label.hidden = NO;
            } else {
                self.user2Label.text = uid;
                self.user2Label.hidden = NO;
            }
        });
    }
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserJoined:(ByteRTCUserInfo *)userInfo elapsed:(NSInteger)elapsed {
    NSLog(@"rtcRoom onUserJoined:%@", userInfo.userId);
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.userID isEqualToString:userInfo.userId]) {
            self.user1Label.text = self.userID;
            self.user1Label.hidden = NO;
        } else {
            self.user2Label.text = userInfo.userId;
            self.user2Label.hidden = NO;
        }
    });
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserLeave:(NSString *)uid reason:(ByteRTCUserOfflineReason)reason {
    NSLog(@"rtcRoom onUserLeave:%@", uid);
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.userID isEqualToString:uid]) {
            self.user1Label.text = self.userID;
            self.user1Label.hidden = YES;
        } else {
            self.user2Label.text = uid;
            self.user2Label.hidden = YES;
        }
    });
}


- (UIView *)gameContainer {
    if (!_gameContainer) {
        _gameContainer = [[UIView alloc] init];
    }
    return _gameContainer;
}

- (UIView *)renderView {
    if (!_renderView) {
        _renderView = [[UIView alloc] init];
    }
    return _renderView;
}

- (UILabel *)roomLabel {
    if (!_roomLabel) {
        _roomLabel = [[UILabel alloc] init];
        _roomLabel.textColor = [UIColor blackColor];
        _roomLabel.text = self.roomID;
        [_roomLabel sizeToFit];
    }
    return _roomLabel;
}

- (UIButton *)startGameButton {
    if (!_startGameButton) {
        _startGameButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_startGameButton setTitle:@"开始游戏" forState:UIControlStateNormal];
        _startGameButton.backgroundColor = [UIColor systemBlueColor];
        [_startGameButton addTarget:self action:@selector(startGame:) forControlEvents:UIControlEventTouchUpInside];
        _startGameButton.layer.masksToBounds = YES;
        _startGameButton.layer.cornerRadius = 4.;
    }
    return _startGameButton;
}

- (UIButton *)stopGameButton {
    if (!_stopGameButton) {
        _stopGameButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_stopGameButton setTitle:@"退出游戏" forState:UIControlStateNormal];
        _stopGameButton.backgroundColor = [UIColor systemBlueColor];
        [_stopGameButton addTarget:self action:@selector(stopGame:) forControlEvents:UIControlEventTouchUpInside];
        _stopGameButton.layer.masksToBounds = YES;
        _stopGameButton.layer.cornerRadius = 4.;
    }
    return _stopGameButton;
}

- (UIButton *)closeButton {
    if (!_closeButton) {
        _closeButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_closeButton setImage:[UIImage imageNamed:@"close"] forState:UIControlStateNormal];
        _closeButton.backgroundColor = [UIColor grayColor];
        [_closeButton addTarget:self action:@selector(closeButtonClicked:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _closeButton;
}

- (UILabel *)createLabel:(NSString*)text withColor:(UIColor*)color {
    UILabel* label = [[UILabel alloc] init];
    label.text = text;
    [label sizeToFit];
    label.textColor = color;
    label.hidden = YES;
    return label;
}

- (UILabel *)user1Label {
    if (!_user1Label) {
        _user1Label = [[UILabel alloc] init];
        _user1Label.textColor = [UIColor redColor];
        _user1Label.hidden = YES;
    }
    return _user1Label;
}

- (UILabel *)user2Label {
    if (!_user2Label) {
        _user2Label = [[UILabel alloc] init];
        _user2Label.textColor = [UIColor redColor];
        _user2Label.hidden = YES;
    }
    return _user2Label;
}

- (void)dealloc {
    [ByteRTCVideo destroyRTCVideo];
    self.rtcEngine = nil;
}

@end
