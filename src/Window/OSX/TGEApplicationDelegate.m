/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <Tyrant/Window/OSX/TGEApplicationDelegate.h>

////////////////////////////////////////////////////////////
@implementation TGEApplicationDelegate


////////////////////////////////////////////////////////////
-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    (void)sender;
    // Generate close event for each TGE window
    [NSApp makeWindowsPerform:@selector(tgeClose) inOrder:NO];
    return NSTerminateCancel;
}


////////////////////////////////////////////////////////////
-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    (void)theApplication;
    return YES;
}

////////////////////////////////////////////////////////////
+(id)instance
{
    static TGEApplicationDelegate* singleton = nil;
    if (singleton == nil)
        singleton = [[TGEApplicationDelegate alloc] init];
    return singleton;
}

@end

