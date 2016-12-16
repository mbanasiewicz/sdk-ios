  //
  //  SPiDRequest.h
  //  SPiDSDK
  //
  //  Copyright (c) 2012 Schibsted Payment. All rights reserved.
  //

#import <Foundation/Foundation.h>
#import "SPiDClient.h"

/** `SPiDRequest` handles a request against SPiD. */

  //TODO: static NSInteger const MaxRetryAttempts = 2;

@class SPiDAccessToken;
@class SPiDResponse;

@interface SPiDRequest : NSObject

@property (nonatomic, strong, readonly, nullable) NSURL *URL;
@property (nonatomic, strong, readonly, nullable) NSString *HTTPMethod;
@property (nonatomic, strong, readonly, nullable) NSString *HTTPBody;
@property (nonatomic) NSInteger retryCount;

  ///---------------------------------------------------------------------------------------
  /// @name Public methods
  ///---------------------------------------------------------------------------------------

/** Creates a GET `SPiDRequest`
 
 @param requestPath API path for GET request e.g. /user
 @param completionHandler Completion handler run after request is finished, will be called on the main thread.
 @return `SPiDRequest`
 */
+ (instancetype)apiGetRequestWithPath:(NSString * _Nonnull)requestPath
                    completionHandler:(void (^ _Nullable)(SPiDResponse * _Nullable response))completionHandler;

/** Creates a POST `SPiDRequest`
 
 @param requestPath API path for POST request e.g. /user
 @param body The HTTP body
 @param completionHandler Completion handler run after request is finished, will be called on the main thread.
 @return `SPiDRequest`
 */
+ (instancetype)apiPostRequestWithPath:(NSString * _Nonnull)requestPath
                                  body:(NSDictionary * _Nullable)body
                     completionHandler:(void (^ _Nullable)(SPiDResponse * _Nullable response))completionHandler;

/** Creates a `SPiDRequest`
 
 @param requestPath API path for request
 @param method HTTP method for the request
 @param body HTTP body, used if method is POST
 @param completionHandler Completion handler run after request is finished, will be called on the main thread.
 @return `SPiDRequest`
 */
+ (instancetype)requestWithPath:(NSString * _Nonnull)requestPath
                         method:(NSString * _Nonnull)method
                           body:(NSDictionary * _Nullable)body
              completionHandler:(void (^ _Nullable)(SPiDResponse * _Nullable response))completionHandler;

/** Runs the request with the current access token */
- (void)startRequestWithAccessToken; //TODO rename

/** Runs the request without access token */
- (void)start;

/** Runs a SPiDRequest for a given NSURLRequest */
- (void)startWithRequest:(NSURLRequest *)request;

@end
