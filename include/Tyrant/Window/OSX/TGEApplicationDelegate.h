/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>


////////////////////////////////////////////////////////////
/// \brief Process some application specific events
///
////////////////////////////////////////////////////////////
@interface TGEApplicationDelegate : NSObject <NSApplicationDelegate>

////////////////////////////////////////////////////////////
/// \brief React to a termination notification
///
/// Send a close message to all windows and cancel the termination.
///
////////////////////////////////////////////////////////////
-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender;

////////////////////////////////////////////////////////////
/// \brief Exit the app when all windows are closed
///
////////////////////////////////////////////////////////////
-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication;

////////////////////////////////////////////////////////////
/// \brief Return a singleton instance of this class
///
////////////////////////////////////////////////////////////
+(id)instance;

@end
