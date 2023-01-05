//
//  VeGameDisplayViewController.h
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

#import <UIKit/UIKit.h>

@interface VeCloudGameConfigObject : NSObject

@property (nonatomic, copy) NSString *ak;
@property (nonatomic, copy) NSString *sk;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *gameId;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *roundId;
@property (nonatomic, assign) NSInteger rotation;
@property (nonatomic, assign) NSInteger videoStreamProfileId;

@end

@interface VeGameDisplayViewController : UIViewController

@property (nonatomic, strong) VeCloudGameConfigObject *configObj;

@end
