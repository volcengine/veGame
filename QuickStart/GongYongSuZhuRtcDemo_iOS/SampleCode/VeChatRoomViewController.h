//
//  VeChatRoomViewController.h
//  Demo4S
//
//  Created by zhaozuotong on 2023/8/22.
//

#import <UIKit/UIKit.h>
#import "VeCommonDefine.h"

NS_ASSUME_NONNULL_BEGIN

@interface VeChatRoomViewController : UIViewController

@property (nonatomic, strong) NSString *roomID;

@property (nonatomic, strong) NSString *userID;

@property (nonatomic, strong) NSString *reservedID;

@end

NS_ASSUME_NONNULL_END
