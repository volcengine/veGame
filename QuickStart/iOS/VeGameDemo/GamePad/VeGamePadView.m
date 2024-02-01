//
//  VeGamePadView.m
//  VePlayer
//
//  Created by yinhaichao on 2024/2/1.
//  Copyright © 2020 ByteDance Ltd. All rights reserved.
//

#import "VeGamePadView.h"
#import <VeGame/VeGame.h>


@class VeGamePadItemView;

@interface VeGamePadItemView ()

@property (nonatomic, assign) BOOL isEditing;
@property (nonatomic, assign) BOOL selectedForEditing;

@property (nonatomic, strong) UIImageView *imageView;

@property (nonatomic, assign, readonly) BOOL highlighted;
@property (nonatomic, strong, readonly) UIPanGestureRecognizer *panGestureRecognizer;
@property (nonatomic, copy) NSArray <NSString *>*combinationNameS;

@end

@implementation VeGamePadItemView

- (instancetype)init
{
    if (self = [super initWithFrame: CGRectZero]) {

        self.imageView = [[UIImageView alloc] initWithFrame: self.bounds];
        self.imageView.contentMode = UIViewContentModeScaleAspectFit;
        self.imageView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [self updateImageView];
        [self addSubview: self.imageView];

        _panGestureRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget: self action: @selector(handlePanGestureRecognizer:)];
        _panGestureRecognizer.enabled = NO;
        [self addGestureRecognizer: _panGestureRecognizer];

    }
    return self;
}

- (void)setHighlighted:(BOOL)highlighted
{
    if (_highlighted != highlighted) {
        _highlighted = highlighted;
        [self updateImageView];
    }
}

- (void)setSelectedForEditing:(BOOL)selectedForEditing
{
    _selectedForEditing = selectedForEditing;
    [self updateImageView];
}

- (void)updateImageView
{
    self.imageView.transform = self.highlighted || self.selectedForEditing ? CGAffineTransformMakeScale(0.9, 0.9) : CGAffineTransformIdentity;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if (self.isEditing) {
        self.selectedForEditing = YES;
    } else {
        self.highlighted = YES;
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    if (self.isEditing) {
        self.selectedForEditing = YES;
    } else {
        self.highlighted = YES;
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    self.highlighted = NO;
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    self.highlighted = NO;
}

- (void)handlePanGestureRecognizer:(UIPanGestureRecognizer *)panGestureRecognizer
{
    CGPoint translation = [panGestureRecognizer translationInView: self.superview];
    [panGestureRecognizer setTranslation: CGPointZero inView: self.superview];

    self.center = CGPointMake(self.center.x + translation.x, self.center.y + translation.y);
    [self adjustFrameAvoidCrossingBoundary];
}

- (void)adjustFrameAvoidCrossingBoundary
{
    CGRect frame = self.frame;
    if (frame.origin.x < 0) {
        frame.origin.x = 0;
    }
    if (frame.origin.y < 20) {
        frame.origin.y = 20;
    }
    if (frame.origin.x + frame.size.width > self.superview.frame.size.width) {
        frame.origin.x = self.superview.frame.size.width - frame.size.width;
    }
    if (frame.origin.y + frame.size.height > self.superview.frame.size.height) {
        frame.origin.y = self.superview.frame.size.height - frame.size.height;
    }
    self.frame = frame;
}

- (void)setIsEditing:(BOOL)isEditing
{
    _isEditing = isEditing;
    self.panGestureRecognizer.enabled = isEditing;
    self.selectedForEditing = NO;
    if (isEditing) {
        self.hidden = NO;
    }
}

- (void)setCombinationNameS:(NSArray<NSString *> *)combinationNameS {
    _combinationNameS = combinationNameS;
    [self updateImageView];
}

@end

@interface VeGamePadJoystickView ()

@property (nonatomic, strong) UITouch *currentTouch;
@property (nonatomic, strong) UIImageView *fingertipImageView;
@property (nonatomic, strong) UIImageView *fingertipImageView_bg;

@end

@implementation VeGamePadJoystickView
{
    BOOL _W;
    BOOL _A;
    BOOL _S;
    BOOL _D;
}

#define fingertipImageViewWidth (self.frame.size.width * 0.4)

- (instancetype)init
{
    if (self = [super init]) {
        _fingertipImageView = [[UIImageView alloc] init];
        
        _fingertipImageView.autoresizingMask = UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleLeftMargin;
        
        _fingertipImageView.image = [UIImage imageNamed: @"rocker_center_gamepad_normal"];
        _fingertipImageView_bg = [[UIImageView alloc] init];
        _fingertipImageView_bg.image = [UIImage imageNamed: @"rocker_bg_gamepad_normal"];
        [self.imageView addSubview:_fingertipImageView_bg];
        self.fingertipImageView_bg.hidden = YES;
        [self addSubview: _fingertipImageView];
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    _fingertipImageView.frame = CGRectMake(self.frame.size.width / 2 - fingertipImageViewWidth / 2, self.frame.size.height / 2 - fingertipImageViewWidth / 2, fingertipImageViewWidth, fingertipImageViewWidth);
    _fingertipImageView_bg.frame = CGRectMake(-7, -7, self.frame.size.width + 14, self.frame.size.height + 14);
}

- (void)setIsEditing:(BOOL)isEditing
{
    [super setIsEditing: isEditing];

    self.hidden = !isEditing && NO;
}

- (void)updateImageView
{
    [super updateImageView];

    NSString *imageName = nil;
    if (self.selectedForEditing || self.highlighted) {
        imageName = @"rocker_center_gamepad_pressed";
    } else {
        imageName = @"rocker_center_gamepad_normal";
    }
    self.fingertipImageView.image = [UIImage imageNamed: imageName];
}

- (void)moveFingertipWithTouch:(UITouch *)touch
{
    self.highlighted = YES;

    CGPoint point = [touch locationInView: self];
    CGFloat distance = sqrt(pow(point.x - self.frame.size.width / 2, 2) + pow(point.y - self.frame.size.height / 2, 2));
    if (distance + self.fingertipImageView.frame.size.width / 2 > self.frame.size.width / 2) {
        CGFloat radian = atan((point.y - self.frame.size.height / 2) / (point.x - self.frame.size.width / 2));
        CGFloat bg_radian = radian + M_PI_2;
        if (point.x > self.frame.size.width / 2) {
            point.x = self.frame.size.width / 2 + (self.frame.size.width - self.fingertipImageView.frame.size.width) / 2 * cos(radian);
            point.y = self.frame.size.height / 2 + (self.frame.size.height - self.fingertipImageView.frame.size.height) / 2 * sin(radian);
            self.fingertipImageView_bg.transform = CGAffineTransformMakeRotation(bg_radian);
            if (bg_radian >= 0 && bg_radian < 0.52) {
//                NSLog(@"W");
                if (_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:NO];
                    _S = NO;
                }
                if (_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:NO];
                    _A = NO;
                }
                if (!_W) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:YES];
                    _W = YES;
                }
            } else if (bg_radian >= 0.52 && bg_radian < 1.05) {
//                NSLog(@"W+D");
                if (!_W) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:YES];
                    _W = YES;
                }
                if (!_D) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:YES];
                    _D = YES;
                }
            } else if (bg_radian >= 1.05 && bg_radian < 2.09) {
//                NSLog(@"D");
                if (_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:NO];
                    _S = NO;
                }
                if (_W) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:NO];
                    _W = NO;
                }
                if (!_D) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:YES];
                    _D = YES;
                }
            } else if (bg_radian >= 2.09 && bg_radian < 2.67) {
//                NSLog(@"D+S");
                if (!_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:YES];
                    _S = YES;
                }
                if (!_D) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:YES];
                    _D = YES;
                }
            } else if (bg_radian >= 2.67 && bg_radian < 3.14) {
//                NSLog(@"S");
                if (_D) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:NO];
                    _D = NO;
                }
                if (_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:NO];
                    _A = NO;
                }
                if (!_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:YES];
                    _S = YES;
                }
            }
        } else {
            point.x = self.frame.size.width / 2 - (self.frame.size.width - self.fingertipImageView.frame.size.width) / 2 * cos(radian);
            point.y = self.frame.size.height / 2 - (self.frame.size.height - self.fingertipImageView.frame.size.height) / 2 * sin(radian);
            self.fingertipImageView_bg.transform = CGAffineTransformMakeRotation(M_PI + bg_radian);
            if (bg_radian >= 0 && bg_radian < 0.52) {
//                NSLog(@"S");
                if (_D) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:NO];
                    _D = NO;
                }
                if (_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:NO];
                    _A = NO;
                }
                if (!_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:YES];
                    _S = YES;
                }
            } else if (bg_radian >= 0.52 && bg_radian < 1.05) {
//                NSLog(@"S+A");
                if (!_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:YES];
                    _S = YES;
                }
                if (!_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:YES];
                    _A = YES;
                }
            } else if (bg_radian >= 1.05 && bg_radian < 2.09) {
//                NSLog(@"A");
                if (_S) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:NO];
                    _S = NO;
                }
                if (_W) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:NO];
                    _W = NO;
                }
                if (!_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:YES];
                    _A = YES;
                }
            } else if (bg_radian >= 2.09 && bg_radian < 2.67) {
//                NSLog(@"A+W");
                if (!_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:YES];
                    _A = YES;
                }
                if (!_W) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:YES];
                    _W = YES;
                }
            } else if (bg_radian >= 2.67 && bg_radian < 3.14) {
//                NSLog(@"W");
                if (_D) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:NO];
                    _D = NO;
                }
                if (_A) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:NO];
                    _A = NO;
                }
                if (!_W) {
                    [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:YES];
                    _W = YES;
                }
            }
        }
    } else {
        [self resetJoystickKey];
    }
    self.fingertipImageView_bg.hidden = NO;
    self.fingertipImageView.center = point;
}

- (void)setHidden:(BOOL)hidden
{
    [super setHidden: hidden];

    if (!hidden) {
        self.fingertipImageView.center = CGPointMake(self.frame.size.width / 2, self.frame.size.height / 2);
    } else {
        self.highlighted = NO;
    }
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan: touches withEvent: event];

    if (!self.isEditing && touches.count == 1 && self.currentTouch == nil) {
        self.currentTouch = touches.allObjects.firstObject;
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved: touches withEvent: event];

    if (!self.isEditing && [touches containsObject: self.currentTouch]) {
        [self moveFingertipWithTouch: self.currentTouch];
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded: touches withEvent: event];

    if (!self.isEditing && [touches containsObject: self.currentTouch]) {
        self.highlighted = NO;
        self.currentTouch = nil;
        [self stop];
    }
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled: touches withEvent: event];

    if (!self.isEditing && [touches containsObject: self.currentTouch]) {
        self.highlighted = NO;
        self.currentTouch = nil;
        [self stop];
    }
}

- (void)stop
{
    self.fingertipImageView.center = CGPointMake(self.frame.size.width / 2, self.frame.size.height / 2);
    self.fingertipImageView_bg.transform = CGAffineTransformIdentity;
    self.fingertipImageView_bg.hidden = YES;
    [self resetJoystickKey];
}

- (void)resetJoystickKey {
    if (_W) {
        [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyW state:NO];
        _W = NO;
    }
    if (_A) {
        [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyA state:NO];
        _A = NO;
    }
    if (_S) {
        [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyS state:NO];
        _S = NO;
    }
    if (_D) {
        [[VeGameManager sharedInstance] sendKeyboardData:VeGameKeyboardKeyD state:NO];
        _D = NO;
    }
}

@end

