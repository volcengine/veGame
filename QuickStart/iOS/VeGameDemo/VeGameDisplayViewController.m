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

@implementation VeCloudGameConfigObject

@end

@interface VeGameDisplayViewController () <VeGameManagerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>

@property (nonatomic, assign) NSInteger rotation;
@property (nonatomic, strong) UIView *containerView;
@property (nonatomic, assign) BOOL alreadyStart;

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
    [VeGameManager sharedInstance].delegate = self;
    [VeGameManager sharedInstance].containerView = self.containerView;
    
    // 附加信息
    [[VeGameManager sharedInstance] setExtraParameters: @{
        
    }];
    
    if (NO) { // 是否进行网络探测
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
    
    // 文件上传
    UIButton *fileButton = [UIButton buttonWithType: UIButtonTypeCustom];
    fileButton.layer.borderWidth = 1.0f;
    fileButton.layer.borderColor = [UIColor whiteColor].CGColor;
    fileButton.layer.cornerRadius = 5.0f;
    fileButton.titleLabel.adjustsFontSizeToFitWidth = YES;
    fileButton.titleLabel.font = [UIFont systemFontOfSize: 14.0f];
    [fileButton setTitle: @"上传文件" forState: UIControlStateNormal];
    [fileButton setTitleColor: [UIColor whiteColor] forState: UIControlStateNormal];
    [fileButton addTarget: self action: @selector(tappedFileButton:) forControlEvents: UIControlEventTouchUpInside];
    [self.view addSubview: fileButton];
    [fileButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(CGSizeMake(80, 30));
        make.left.mas_equalTo(exitBtn.mas_right).offset(5.0f);
        make.top.mas_equalTo(exitBtn);
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

- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode
{
    [self.view makeToast: [NSString stringWithFormat: @"Warning Code: %ld", warnCode]
                duration: 2.0f
                position: CSToastPositionCenter];
}

#pragma mark - button action

- (void)tappedExitButton:(UIButton *)btn
{
    [SVProgressHUD dismiss];
    self.alreadyStart = NO;
    [[VeGameManager sharedInstance] stop];
    [self.navigationController popViewControllerAnimated: YES];
}

- (void)tappedFileButton:(UIButton *)btn
{
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    imagePicker.delegate = self;
    imagePicker.allowsEditing = YES;
    imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    imagePicker.modalPresentationStyle = UIModalPresentationFullScreen;
    [self presentViewController: imagePicker animated: YES completion: nil];
}

- (void)startGame
{
    if (!self.alreadyStart) {
        self.alreadyStart = YES;
        
        // 配置信息
        [SVProgressHUD showWithStatus: @"正在启动..."];
        VeGameConfigObject *configObj = [VeGameConfigObject new];
        configObj.ak = self.configObj.ak;
        configObj.sk = self.configObj.sk;
        configObj.token = self.configObj.token;
        configObj.userId = self.configObj.userId;
        configObj.gameId = self.configObj.gameId;
        configObj.roundId = self.configObj.roundId;
//        VeGameControlObject *control = [VeGameControlObject new];
//        control.role = self.configObj.role;
//        control.roomType = self.configObj.roomType;
//        configObj.control = control;
//        configObj.sessionMode = self.configObj.sessionMode;
//        configObj.queuePriority = self.configObj.queuePriority;
//        configObj.keyboardEnable = self.configObj.keyboardEnable;
//        configObj.autoRecycleTime = self.configObj.autoRecycleTime;
//        configObj.videoStreamProfileId = self.configObj.videoStreamProfileId;
//        configObj.reservedId = self.configObj.reservedId.length ? self.configObj.reservedId : nil;
        [[VeGameManager sharedInstance] startWithConfig: configObj];
    }
}

#pragma mark - UIImagePickerControllerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey, id> *)info
{
    [picker dismissViewControllerAnimated: YES completion: nil];
    UIImage *image = [info objectForKey: UIImagePickerControllerOriginalImage];
    NSData *imageData;
    if (UIImagePNGRepresentation(image)) {
        imageData = UIImagePNGRepresentation(image);
    } else {
        imageData = UIImageJPEGRepresentation(image, 1.0f);
    }
    NSDate *date = [NSDate dateWithTimeIntervalSinceNow: 0];
    NSTimeInterval timeInterval = [date timeIntervalSince1970];
    NSString *fileName = [NSString stringWithFormat: @"%0.f.png", timeInterval];
    VeFile *file = [VeFile new];
    file.fileData = imageData;
    file.name = fileName;
    file.md5 = [self md5StringOfData: imageData];
    [[VeGameManager sharedInstance] startSendFile:file options:@{@"options_key" : @"options_value"} onStart:^(VeFile *file, NSDictionary<NSString *,NSString *> *options) {
        NSLog(@"上传开始--------%@\n", options);
    } onProgress:^(VeFile *file, NSDictionary<NSString *,NSString *> *options, NSInteger progress) {
        NSLog(@"上传进度--------%ld\n", progress);
    } onComplete:^(VeFile *file, NSDictionary<NSString *,NSString *> *options) {
        NSLog(@"上传完成--------%@\n", file.name);
    } onCancel:^(VeFile *file, NSDictionary<NSString *,NSString *> *options) {
        NSLog(@"上传取消--------%@\n", file);
    } onError:^(VeFile *file, NSDictionary<NSString *,NSString *> *options, VeGameErrorCode err) {
        NSLog(@"上传出错--------%@\n", file);
    }];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    NSLog(@"取消选择---------");
}

- (void)gameManager:(VeGameManager *)manager onQueueUpdate:(NSArray<NSDictionary *> *)queueInfoList
{
    NSLog(@"开始排队：%@", queueInfoList);
}

- (void)gameManager:(VeGameManager *)manager onQueueSuccessAndStart:(NSInteger)remainTime
{
    NSLog(@"排队完毕%ld", remainTime);
}

#pragma mark - 大文件下载

- (void)gameManager:(VeGameManager *)manager downloadOnStart:(VeFile *)file
{
    NSLog(@"开始下载\n");
}

- (void)gameManager:(VeGameManager *)manager downloadOnProgress:(VeFile *)file progress:(NSInteger)progress
{
    NSLog(@"下载进度：%ld\n", progress);
}

- (void)gameManager:(VeGameManager *)manager downloadOnComplete:(VeFile *)file
{
    NSLog(@"下载完成，文件已经保存至XX\n");
}

- (void)gameManager:(VeGameManager *)manager downloadOnCancel:(VeFile *)file
{
    NSLog(@"取消下载\n");
}

- (void)gameManager:(VeGameManager *)manager downloadOnError:(VeFile *)file err:(VeGameErrorCode)err
{
    NSLog(@"下载出错，错误码：%ld\n", err);
}

#pragma mark - utils

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
