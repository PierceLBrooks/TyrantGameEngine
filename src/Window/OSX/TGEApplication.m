/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <Tyrant/Window/OSX/TGEApplication.h>


////////////////////////////////////////////////////////////
@implementation TGEApplication


////////////////////////////////////////////////////////////
+(void)processEvent
{
@autoreleasepool
{
    [TGEApplication sharedApplication]; // Make sure NSApp exists
    NSEvent* event = nil;

    while ((event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                       untilDate:[NSDate distantPast]
                                          inMode:NSDefaultRunLoopMode
                                         dequeue:YES])) // Remove the event from the queue
    {
        [NSApp sendEvent:event];
    }
} // pool
}


////////////////////////////////////////////////////////
+(void)setUpMenuBar
{
    [TGEApplication sharedApplication]; // Make sure NSApp exists

    // Set the main menu bar
    NSMenu* mainMenu = [NSApp mainMenu];
    if (mainMenu != nil)
        return;
    mainMenu = [[NSMenu alloc] initWithTitle:@""];
    [NSApp setMainMenu:mainMenu];

    // Application Menu (aka Apple Menu)
    NSMenuItem* appleItem = [mainMenu addItemWithTitle:@"" action:nil keyEquivalent:@""];
    NSMenu* appleMenu = [TGEApplication createAppleMenu];
    [appleItem setSubmenu:appleMenu];

    // File Menu
    NSMenuItem* fileItem = [mainMenu addItemWithTitle:@"" action:nil keyEquivalent:@""];
    NSMenu* fileMenu = [TGEApplication createFileMenu];
    [fileItem setSubmenu:fileMenu];

    // Window menu
    NSMenuItem* windowItem = [mainMenu addItemWithTitle:@"" action:nil keyEquivalent:@""];
    NSMenu* windowMenu = [TGEApplication createWindowMenu];
    [windowItem setSubmenu:windowMenu];
    [NSApp setWindowsMenu:windowMenu];
}


////////////////////////////////////////////////////////
+(NSMenu*)createAppleMenu
{
    // Apple menu is as follow:
    //
    // AppName >
    //    About AppName
    //    --------------------
    //    Preferences...        [greyed]
    //    --------------------
    //    Services >
    //        / default empty menu /
    //    --------------------
    //    Hide AppName      ⌘H
    //    Hide Others      ⌥⌘H
    //    Show All
    //    --------------------
    //    Quit AppName      ⌘Q

    NSString* appName = [TGEApplication applicationName];

    // APPLE MENU
    NSMenu* appleMenu = [[NSMenu alloc] initWithTitle:@""];

    // ABOUT
    [appleMenu addItemWithTitle:[@"About " stringByAppendingString:appName]
                         action:@selector(orderFrontStandardAboutPanel:)
                  keyEquivalent:@""];

    // SEPARATOR
    [appleMenu addItem:[NSMenuItem separatorItem]];

    // PREFERENCES
    [appleMenu addItemWithTitle:@"Preferences…"
                         action:nil
                  keyEquivalent:@""];

    // SEPARATOR
    [appleMenu addItem:[NSMenuItem separatorItem]];

    // SERVICES
    NSMenu* serviceMenu = [[NSMenu alloc] initWithTitle:@""];
    NSMenuItem* serviceItem = [appleMenu addItemWithTitle:@"Services"
                                                  action:nil
                                           keyEquivalent:@""];
    [serviceItem setSubmenu:serviceMenu];
    [NSApp setServicesMenu:serviceMenu];

    // SEPARATOR
    [appleMenu addItem:[NSMenuItem separatorItem]];

    // HIDE
    [appleMenu addItemWithTitle:[@"Hide " stringByAppendingString:appName]
                         action:@selector(hide:)
                  keyEquivalent:@"h"];

    // HIDE OTHER
    NSMenuItem* hideOtherItem = [appleMenu addItemWithTitle:@"Hide Others"
                                                     action:@selector(hideOtherApplications:)
                                              keyEquivalent:@"h"];
    [hideOtherItem setKeyEquivalentModifierMask:(NSAlternateKeyMask | NSCommandKeyMask)];

    // SHOW ALL
    [appleMenu addItemWithTitle:@"Show All"
                         action:@selector(unhideAllApplications:)
                  keyEquivalent:@""];

    // SEPARATOR
    [appleMenu addItem:[NSMenuItem separatorItem]];

    // QUIT
    [appleMenu addItemWithTitle:[@"Quit " stringByAppendingString:appName]
                         action:@selector(terminate:)
                  keyEquivalent:@"q"];

    return appleMenu;
}


////////////////////////////////////////////////////////
+(NSMenu*)createFileMenu
{
    // The File menu is as follow:
    //
    // File >
    //    Close             ⌘W

    // FILE MENU
    NSMenu* fileMenu = [[NSMenu alloc] initWithTitle:@"File"];

    // CLOSE WINDOW
    NSMenuItem* closeItem = [[NSMenuItem alloc] initWithTitle:@"Close Window"
                                                       action:@selector(performClose:)
                                                keyEquivalent:@"w"];
    [fileMenu addItem:closeItem];

    return fileMenu;
}


////////////////////////////////////////////////////////
+(NSMenu*)createWindowMenu
{
    // The Window menu is as follow:
    //
    // Window >
    //    Minimize          ⌘M
    //    Zoom
    //    --------------------
    //    Bring All to Front

    // WINDOW MENU
    NSMenu* windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];

    // MINIMIZE
    NSMenuItem* minimizeItem = [[NSMenuItem alloc] initWithTitle:@"Minimize"
                                                          action:@selector(performMiniaturize:)
                                                   keyEquivalent:@"m"];
    [windowMenu addItem:minimizeItem];

    // ZOOM
    [windowMenu addItemWithTitle:@"Zoom"
                          action:@selector(performZoom:)
                   keyEquivalent:@""];

    // SEPARATOR
    [windowMenu addItem:[NSMenuItem separatorItem]];

    // BRING ALL TO FRONT
    [windowMenu addItemWithTitle:@"Bring All to Front"
                          action:@selector(bringAllToFront:)
                   keyEquivalent:@""];

    return windowMenu;
}


////////////////////////////////////////////////////////
+(NSString*)applicationName
{
    // First, try localized name
    NSString* appName = [[[NSBundle mainBundle] localizedInfoDictionary] objectForKey:@"CFBundleDisplayName"];

    // Then, try non-localized name
    if ((appName == nil) || ([appName length] == 0))
        appName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];

    // Finally, fallback to the process info
    if ((appName == nil) || ([appName length] == 0))
        appName = [[NSProcessInfo processInfo] processName];

    return appName;
}


////////////////////////////////////////////////////////
-(void)bringAllToFront:(id)sender
{
    (void)sender;
    [[NSApp windows] makeObjectsPerformSelector:@selector(orderFrontRegardless)];
}


////////////////////////////////////////////////////////
-(void)sendEvent:(NSEvent *)anEvent
{
    // Fullscreen windows have a strange behaviour with key up. To make
    // sure the user gets an event we call (if possible) tgeKeyUp on our
    // custom OpenGL view. See -[TGEOpenGLView tgeKeyUp:] for more details.

    id firstResponder = [[anEvent window] firstResponder];
    if (([anEvent type] != NSKeyUp) || (![firstResponder tryToPerform:@selector(tgeKeyUp:) with:anEvent]))
    {
        // It's either not a key up event or no responder has a tgeKeyUp
        // message implemented.
        [super sendEvent:anEvent];
    }
}


@end


