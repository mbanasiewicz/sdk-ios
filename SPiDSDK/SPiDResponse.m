#import "SPiDResponse.h"
#import "SPiDClient.h"
#import "NSError+SPiD.h"

@implementation SPiDResponse

- (instancetype _Nonnull)initWithJSONData:(NSData *)data {
  if (self = [super init]) {
    NSError *jsonError = nil;
    if (data.length > 0) {
      NSString *JSONString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
      self.rawJSON = JSONString;
      self.message = [NSJSONSerialization JSONObjectWithData:data
                                                     options:NSJSONReadingMutableContainers
                                                       error:&jsonError];
      if (jsonError) {
        self.error = jsonError;
        SPiDDebugLog(@"JSON parse error: %@", self.rawJSON);
      } else {
        NSDictionary *errorDictionary = [self.message objectForKey:@"error"];
        
        if (errorDictionary && ![errorDictionary isEqual:NSNull.null]) {
          self.error = [NSError sp_errorFromJSONData:self.message];
        }
      }
    } else {
      NSDictionary * descriptions = @{
                                      @"error" : @"Recevied empty response"
                                      };
      NSError *error = [NSError sp_apiErrorWithCode:SPiDUserAbortedLogin
                                             reason:@"ApiException"
                                       descriptions:descriptions];
      [self setError:error];
    }
  }
  return self;
}

- (instancetype _Nonnull)initWithError:(NSError *)error {
  if (self = [super init]) {
    self.error = error;
  }
  return self;
}

@end
