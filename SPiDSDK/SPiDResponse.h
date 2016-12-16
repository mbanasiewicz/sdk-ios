  //
  //  SPiDResponse.h
  //  SPiDSDK
  //
  //  Copyright (c) 2012 Schibsted Payment. All rights reserved.
  //

#import <Foundation/Foundation.h>

/**
 `SPiDResponse` is created for each response from SPiD made by a `SPiDRequest`
 
 It contains the message as both a object and as raw JSON.
 
 @warning Received should always check the error property upon receiving a response.
 */

@interface SPiDResponse : NSObject

  ///---------------------------------------------------------------------------------------
  /// @name Properties
  ///---------------------------------------------------------------------------------------

/** Contains error if there was any, otherwise nil */
@property(strong, nonatomic, nullable) NSError *error;

/** Received JSON message converted to a dictionary */
@property(strong, nonatomic, nullable) NSDictionary *message;

/** Received JSON message as a raw string */
@property(strong, nonatomic, nullable) NSString *rawJSON;

  ///---------------------------------------------------------------------------------------
  /// @name Public methods
  ///---------------------------------------------------------------------------------------

/** Initializes SPiD response with the received message
 
 @param data Data received from SPiD
 @return SPiDAccessToken
 */
- (instancetype _Nonnull)initWithJSONData:(NSData * _Nonnull)data;

/** Initializes SPiD response with a error
 
 @param error The received error
 @return SPiDAccessToken
 */
- (instancetype _Nonnull)initWithError:(NSError * _Nonnull)error;

@end
