//
//  SPiDClient.h
//  SPiDSDK
//
//  Copyright (c) 2012 Schibsted Payment. All rights reserved.
//

#define SPID_IOS_SDK_VERSION_STRING @"2.2.2"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SPiDUtils.h"

@class SPiDResponse;
@class SPiDAccessToken;
@class SPiDRequest;

static NSString *const defaultAPIVersionSPiD = @"2";
static NSString *const AccessTokenKeychainIdentification = @"AccessToken";
#define SPiDDebugLog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
// debug print used by SPiDSDK
//#ifdef DEBUG
//
//#else
//#   define SPiDDebugLog(...)
//#endif

/**
 The main SDK class, all interaction with SPiD goes through this class

 `SPiDClient` contains a singleton instance and all calls to SPiD should go through this instance.
 */

@interface SPiDClient : NSObject

///---------------------------------------------------------------------------------------
/// @name Properties
///---------------------------------------------------------------------------------------

/** Client ID provided by SPiD  */
@property(strong, nonatomic) NSString *clientID;

/** Client ID to be used when generating a one time code. Defaults to `clientID`. */
@property(strong, nonatomic) NSString *serverClientID;

/** Client secret provided by SPiD  */
@property(strong, nonatomic) NSString *clientSecret;

/** Signing secret provided by SPiD  */
@property(strong, nonatomic) NSString *signSecret;

/** App URL schema

 This is used for generating the redirect URI needed for switching back from safari to app
 */
@property(strong, nonatomic) NSString *appURLScheme;

/** Redirect URI

 This is normally generated by the SDK using the `appURLSchema`://SPiD
 */
@property(strong, nonatomic) NSURL *redirectURI; // Note: Defaults to appURLScheme://spid/{login|logout|failure}

/** Redirect URI for the server client

 Defaults to the redirect uri for app client
 */
@property(strong, nonatomic) NSURL *serverRedirectUri;

/** URL to the SPiD server */
@property(strong, nonatomic) NSURL *serverURL;

/** URL to use for web authorization with SPiD

 This URL is normally generated using the `serverURL`/flow/login
 */
@property(strong, nonatomic) NSURL *authorizationURL;

/** URL to use for web signup with SPiD

 This URL is normally generated using the `serverURL`/flow/signup
 */
@property(strong, nonatomic) NSURL *signupURL;

/** URL to use for account summary
 *
 * This URL is normally generated using the 'serverURL/account/summary'
 * */
@property (nonatomic, strong) NSURL *accountSummaryURL;

/** URL to use for web forgot password with SPiD

 This URL is normally generated using the `serverURL`/flow/password?client_id=123&redirect_uri=urlscheme://spid
 */
@property(strong, nonatomic) NSURL *forgotPasswordURL;

/** URL to use for logout with SPiD

 This URL is normally generated using the `serverURL`/logout
 */
@property(nonatomic, strong) NSURL *logoutURL;

/** URL to use for requesting access token from SPiD

 This URL is normally generated using the `serverURL`/oauth/token
 */
@property(strong, nonatomic) NSURL *tokenURL;

/** Sets if access token should be saved in keychain, default value is YES */
@property(nonatomic) BOOL saveToKeychain;

/** The API version that should be used, defaults to 2 */
@property(strong, nonatomic) NSString *apiVersionSPiD;

/** Use mobile web version for SPiD, default YES */
@property(nonatomic) BOOL useMobileWeb;

/** HTML string that will be show when WebView is loading */
@property(strong, nonatomic) NSString *webViewInitialHTML;

/** The SPiD access token */
@property(strong, nonatomic) SPiDAccessToken *accessToken;

/** Queue for waiting requests */
@property(nonatomic, strong, readonly) NSMutableArray *waitingRequests;

/** NSURLSession for the SPiDClient */
@property (nonatomic, strong, readonly) NSURLSession *URLSession;

///---------------------------------------------------------------------------------------
/// @name Public Methods
///---------------------------------------------------------------------------------------

/** Returns the singleton instance of SPiDClient

 The SPiDClient needs to be configured first with setClientID:clientSecret:appURLScheme:serverURL:

 @return Returns singleton instance
 */
+ (SPiDClient * _Nonnull)sharedInstance;

/** Configures the `SPiDClient` and creates a singleton instance

 @param clientID The client ID provided by SPiD
 @param clientSecret The client secret provided by SPiD
 @param appURLSchema The url schema for the app (eg spidtest://)
 @param serverURL The url to SPiD
 */
+ (void)setClientID:(NSString *)clientID
       clientSecret:(NSString *)clientSecret
       appURLScheme:(NSString *)appURLSchema
          serverURL:(NSURL *)serverURL;

/** Redirects to safari for authorization

 @param completionHandler Called on login completion or error
*/
- (void)browserRedirectAuthorizationWithCompletionHandler:(void (^)(NSError *))completionHandler __WATCHOS_PROHIBITED __TVOS_PROHIBITED;

/** Redirects to safari for signup

 @param completionHandler Called on signup completion or error
*/
- (void)browserRedirectSignupWithCompletionHandler:(void (^)(NSError *))completionHandler __WATCHOS_PROHIBITED __TVOS_PROHIBITED;

- (void)browserRedirectForgotPasswordWithCompletionHandler:(void (^)(NSError *response))completionHandler __WATCHOS_PROHIBITED __TVOS_PROHIBITED;

/** Redirects to safari for forgot password */
- (void)browserRedirectForgotPassword __WATCHOS_PROHIBITED __TVOS_PROHIBITED;

/** Redirects to safari for forgot password */
- (void)browserRedirectAccountSummary __WATCHOS_PROHIBITED __TVOS_PROHIBITED;

/** Redirects to safari for logout

 @param completionHandler Called on logout completion or error
*/
- (void)browserRedirectLogoutWithCompletionHandler:(void (^)(NSError *))completionHandler __WATCHOS_PROHIBITED __TVOS_PROHIBITED; // TODO: Should not care about errors...

/** Handles URL redirects to the app with completion handler
 
 @param url Input URL
 @param completionHandler Called on successful login/logout or error
 @return Returns YES if URL was handled by `SPiDClient`
 */
- (BOOL)handleOpenURL:(NSURL *)url completionHandler:(void (^)(NSError * _Nullable response))completionHandler;

/** Handles URL redirects to the app

@param url Input URL
@return Returns YES if URL was handled by `SPiDClient` and there is a registered completionHandler for the SPiDClient
*/
- (BOOL)handleOpenURL:(NSURL *)url;

/** Logout from SPiD

 This requires that the app has obtained a access token.
 Redirects to safari to logout from SPiD and remove cookie.
 Also removes access token from keychain

 @warning `SPiDClient` has to be logged in before this call. The receiver must also check if a error was returned to the completionHandler.
 @param completionHandler Called on logout completion or error
 @see isAuthorized
 */
- (SPiDRequest *)logoutRequestWithCompletionHandler:(void (^)(NSError * _Nullable response))completionHandler;

/** Tries to refresh access token and rerun waiting requests

 @param request The request to retry after a new access token has been acquired
 */
- (void)refreshAccessTokenAndRerunRequest:(SPiDRequest *)request;

/** Clears current authorization request and waiting requests */
- (void)clearAuthorizationRequest;

/** Called when authorization is complete */
- (void)authorizationComplete;

/** Generates the authorization url with query parameters

 @return The url with query parameters
 */
- (NSURL *)authorizationURLWithQuery;

/** Generates the signup url with query parameters

 @return The url with query parameters
 */
- (NSURL *)signupURLWithQuery;

/** Generates the forgot password url with query parameters

 @return The url with query parameters
 */
- (NSURL *)forgotPasswordURLWithQuery;

/** Generates the logout url with query parameters

 @return The url with query parameters
 */
- (NSURL *)logoutURLWithQuery;

/** Checks if the access token has expired

 @return Returns YES if access token has expired
 */
- (BOOL)hasTokenExpired;

/** Returns the time when access token expires

 @return Returns the date when the access token expires
 */
- (NSDate * _Nullable)tokenExpiresAt;

/** Returns the user ID for the current user

 @return Returns user ID
 */
- (NSString * _Nullable)currentUserID;

/** Returns YES if `SPiDClient` has a access token

@return Returns YES if `SPiDClient` is authorized
*/
- (BOOL)isAuthorized;

/** Returns YES if `SPiDClient` has a client access token

 @return Returns YES if `SPiDClient` has a client access token
*/
- (BOOL)isClientToken;

///---------------------------------------------------------------------------------------
/// @name Request wrappers
///---------------------------------------------------------------------------------------

/** Requests a one time code to be used server side.
*
 @note The code is generated using the server client id and not the applications client id.
 @warning Requires that the user is authorized with SPiD
 @param completionHandler Called on request completion or error
 */

- (void)oneTimeCodeRequestWithCompletionHandler:(void (^)(SPiDResponse *))completionHandler;

/** Requests a session code to be used in a WebView.
*
 @note The code is generated using the server client id/redirect uri and not the applications client id/redirect uri.
 @warning Requires that the user is authorized with SPiD
 @param completionHandler Called on request completion or error
 */
- (void)sessionCodeRequestWithCompletionHandler:(void (^)(SPiDResponse *response))completionHandler;

/** Requests the currently logged in user’s object. Note that the user session does not last as long as the access token, therefor the me request should only be used right after the app has received a access token. The user id should then be saved and used with the `getUserRequestWithID:andCompletionHandler`

 For information about the return object see: <http://www.schibstedpayment.no/docs/doku.php?id=wiki:user_api>

 @warning Requires that the user is authorized with SPiD
 @param completionHandler Called on request completion or error
 @see isAuthorized
 */
- (void)meRequestWithCompletionHandler:(void (^)(SPiDResponse *response))completionHandler;

/** Requests the userinformation for the specified userID

 For information about the return object see: <http://www.schibstedpayment.no/docs/doku.php?id=wiki:user_api>

 @warning Requires that the user is authorized with SPiD
 @param userID ID for the selected user
 @param completionHandler Called on request completion or error
 @see isAuthorized
 */
- (void)userRequestWithID:(NSString *)userID completionHandler:(void (^)(SPiDResponse *response))completionHandler;

/** Requests the userinformation for the current user

 For information about the return object see: <http://www.schibstedpayment.no/docs/doku.php?id=wiki:user_api>

 @warning Requires that the user is authorized with SPiD
 @param completionHandler Called on request completion or error
 @see isAuthorized
 */
- (void)currentUserRequestWithCompletionHandler:(void (^)(SPiDResponse *))completionHandler;

/** Request all login attempts for a specific client

 For information about the return object see: <http://www.schibstedpayment.no/docs/doku.php?id=wiki:login_api>

 @warning Requires that the user is authorized with SPiD
 @param userID The userID that logins should be fetched for
 @param completionHandler Called on request completion or error
 @see isAuthorized
 */
- (void)userLoginsRequestWithUserID:(NSString *)userID completionHandler:(void (^)(SPiDResponse *response))completionHandler;

/** Checks of status of email
 
 @warning Requires a client token with SPiD
 @param email The email that should be checked
 @param completionHandler Called on request completion or error
 */
- (void)emailStatusWithEmail:(NSString *)email completionHandler:(void (^)(SPiDResponse *responce)) completionHandler;

@end
