/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoMode.hpp>
#include <Tyrant/Window/WindowHandle.hpp>
#include <Tyrant/Window/WindowStyle.hpp>
#include <Tyrant/Window/OSX/WindowImplCocoa.hpp>
#include <Tyrant/System/Log.hpp>
#include <ApplicationServices/ApplicationServices.h>

#import <Tyrant/Window/OSX/TGEApplication.h>
#import <Tyrant/Window/OSX/TGEOpenGLView.h>
#import <Tyrant/Window/OSX/TGEWindow.h>
#import <Tyrant/Window/OSX/TGEWindowController.h>
#import <OpenGL/OpenGL.h>

////////////////////////////////////////////////////////////
/// TGEBlackView is a simple view filled with black, nothing more
///
////////////////////////////////////////////////////////////
@interface TGEBlackView : NSView
@end

@implementation TGEBlackView

////////////////////////////////////////////////////////////
-(void)drawRect:(NSRect)dirtyRect
{
    [[NSColor blackColor] setFill];
    NSRectFill(dirtyRect);
}

@end

////////////////////////////////////////////////////////////
/// TGEWindowController class: private interface
///
////////////////////////////////////////////////////////////
@interface TGEWindowController ()

////////////////////////////////////////////////////////////
/// \brief Retrieves the screen height
///
/// \return screen height
///
////////////////////////////////////////////////////////////
-(float)screenHeight;

////////////////////////////////////////////////////////////
/// \brief Retrieves the title bar height
///
/// \return title bar height
///
////////////////////////////////////////////////////////////
-(float)titlebarHeight;

@end

@implementation TGEWindowController

#pragma mark
#pragma mark TGEWindowController's methods

////////////////////////////////////////////////////////
-(id)initWithWindow:(NSWindow*)window
{
    if ((self = [super init]))
    {
        m_window = nil;
        m_oglView = nil;
        m_requester = 0;
        m_fullscreen = NO;

        // Retain the window for our own use.
        m_window = window;

        if (m_window == nil)
        {
            TGE::Log() << "No window was given to -[TGEWindowController initWithWindow:]." << std::endl;
            return self;
        }

        // Create the view.
        m_oglView = [[TGEOpenGLView alloc] initWithFrame:[[m_window contentView] frame]];

        if (m_oglView == nil)
        {
            TGE::Log() << "Could not create an instance of NSOpenGLView "
                      << "in -[TGEWindowController initWithWindow:]."
                      << std::endl;
            return self;
        }

        // Set the view to the window as its content view.
        [m_window setContentView:m_oglView];
    }

    return self;
}


////////////////////////////////////////////////////////
-(id)initWithMode:(const TGE::VideoMode&)mode andStyle:(unsigned long)style
{
    // If we are not on the main thread we stop here and advice the user.
    if ([NSThread currentThread] != [NSThread mainThread])
    {
        /*
         * See http://lists.apple.com/archives/cocoa-dev/2011/Feb/msg00460.html
         * for more information.
         */
        TGE::Log() << "Cannot create a window from a worker thread. (OS X limitation)" << std::endl;

        return nil;
    }

    if ((self = [super init]))
    {
        m_window = nil;
        m_oglView = nil;
        m_requester = 0;
        m_fullscreen = style & TGE::Style::Fullscreen;

        if (m_fullscreen)
            [self setupFullscreenViewWithMode:mode];
        else
            [self setupWindowWithMode:mode andStyle:style];
    }
    return self;
}


////////////////////////////////////////////////////////
-(void)setupFullscreenViewWithMode:(const TGE::VideoMode&)mode
{
    // Create a screen-sized window on the main display
    TGE::VideoMode desktop = TGE::VideoMode::getDesktopMode();
    NSRect windowRect = NSMakeRect(0, 0, desktop.width, desktop.height);
    m_window = [[TGEWindow alloc] initWithContentRect:windowRect
                                           styleMask:NSBorderlessWindowMask
                                             backing:NSBackingStoreBuffered
                                               defer:NO];

    if (m_window == nil)
    {
        TGE::Log() << "Could not create an instance of NSWindow "
                  << "in -[TGEWindowController setupFullscreenViewWithMode:]."
                  << std::endl;
        return;
    }

    // Set the window level to be above the menu bar
    [m_window setLevel:NSMainMenuWindowLevel+1];

    // More window configuration...
    [m_window setOpaque:YES];
    [m_window setHidesOnDeactivate:YES];
    [m_window setAutodisplay:YES];
    [m_window setReleasedWhenClosed:YES];

    // Register for event
    [m_window setDelegate:self];
    [m_window setAcceptsMouseMovedEvents:YES];
    [m_window setIgnoresMouseEvents:NO];

    // Create a master view containing our OpenGL view
    NSView* masterView = [[TGEBlackView alloc] initWithFrame:windowRect];

    if (masterView == nil)
    {
        TGE::Log() << "Could not create an instance of TGEBlackView "
                  << "in -[TGEWindowController setupFullscreenViewWithMode:]."
                  << std::endl;
        return;
    }

    // Create our OpenGL view size and the view
    CGFloat x = (desktop.width - mode.width) / 2.0;
    CGFloat y = (desktop.height - mode.height) / 2.0;
    NSRect oglRect = NSMakeRect(x, y, mode.width, mode.height);

    m_oglView = [[TGEOpenGLView alloc] initWithFrame:oglRect];

    if (m_oglView == nil)
    {
        TGE::Log() << "Could not create an instance of NSOpenGLView "
                  << "in -[TGEWindowController setupFullscreenViewWithMode:]."
                  << std::endl;
        return;
    }

    // Populate the window and views
    [masterView addSubview:m_oglView];
    [m_window setContentView:masterView];

    // Finalize setup
    [m_oglView enterFullscreen];
}


////////////////////////////////////////////////////////
-(void)setupWindowWithMode:(const TGE::VideoMode&)mode andStyle:(unsigned long)style
{
    // We know that style & TGE::Style::Fullscreen is false.

    // Create our window size.
    NSRect rect = NSMakeRect(0, 0, mode.width, mode.height);

    // Convert the TGE window style to Cocoa window style.
    unsigned int nsStyle = NSBorderlessWindowMask;
    if (style & TGE::Style::Titlebar)
        nsStyle |= NSTitledWindowMask | NSMiniaturizableWindowMask;
    if (style & TGE::Style::Resize)
        nsStyle |= NSResizableWindowMask;
    if (style & TGE::Style::Close)
        nsStyle |= NSClosableWindowMask;

    // Create the window.
    m_window = [[TGEWindow alloc] initWithContentRect:rect
                                           styleMask:nsStyle
                                             backing:NSBackingStoreBuffered
                                               defer:NO]; // Don't defer it!
    /*
     "YES" produces some "invalid drawable".
     See http://www.cocoabuilder.com/archive/cocoa/152482-nsviews-and-nsopenglcontext-invalid-drawable-error.html

     [...]
     As best as I can figure, this is happening because the NSWindow (and
     hence my view) are not visible on screen yet, and the system doesn't like that.
     [...]
     */

    if (m_window == nil)
    {
        TGE::Log() << "Could not create an instance of NSWindow "
                  << "in -[TGEWindowController setupWindowWithMode:andStyle:]."
                  << std::endl;

        return;
    }

    // Create the view.
    m_oglView = [[TGEOpenGLView alloc] initWithFrame:[[m_window contentView] frame]];

    if (m_oglView == nil)
    {
        TGE::Log() << "Could not create an instance of NSOpenGLView "
                  << "in -[TGEWindowController setupWindowWithMode:andStyle:]."
                  << std::endl;

        return;
    }

    // Set the view to the window as its content view.
    [m_window setContentView:m_oglView];

    // Register for event.
    [m_window setDelegate:self];
    [m_window setAcceptsMouseMovedEvents:YES];
    [m_window setIgnoresMouseEvents:NO];

    // And some other things...
    [m_window center];
    [m_window setAutodisplay:YES];
    [m_window setReleasedWhenClosed:YES];
}


////////////////////////////////////////////////////////
-(void)dealloc
{
    [self closeWindow];
    [NSMenu setMenuBarVisible:YES];

    m_window = nil;
    m_oglView = nil;
}


#pragma mark
#pragma mark WindowImplDelegateProtocol's methods


////////////////////////////////////////////////////////
-(void)setRequesterTo:(TGE::priv::WindowImplCocoa*)requester
{
    // Forward to the view.
    [m_oglView setRequesterTo:requester];
    m_requester = requester;
}


////////////////////////////////////////////////////////
-(TGE::WindowHandle)getSystemHandle
{
    return (__bridge TGE::WindowHandle)m_window;
}


////////////////////////////////////////////////////////
-(void)hideMouseCursor
{
    [NSCursor hide];
}


////////////////////////////////////////////////////////
-(void)showMouseCursor
{
    [NSCursor unhide];
}


////////////////////////////////////////////////////////////
-(NSPoint)position
{
    // First, get the top left corner of the view in its own base system
    const NSPoint origin = [m_oglView frame].origin;
    const NSSize  size = [m_oglView frame].size;
    const NSPoint topLeftCornerOfView = NSMakePoint(origin.x, origin.y + size.height);
    const NSPoint positionInView = [m_oglView convertPointToBacking:topLeftCornerOfView];

    // Then, convert it to window base system
    const NSPoint positionInWindow = [m_oglView convertPoint:positionInView toView:nil];
    // here nil denotes the window containing the view

    // Next, convert it to the screen base system
    const NSPoint positionInScreen = [[m_oglView window] convertBaseToScreen:positionInWindow];

    // Finally, flip for TGE window coordinate system
    // Don't forget to discard the title bar !
    const NSPoint positionInTGE = NSMakePoint(positionInScreen.x,
                                               ([self screenHeight] - [self titlebarHeight]) - positionInScreen.y);

    return positionInTGE;
}


////////////////////////////////////////////////////////.
-(void)setWindowPositionToX:(int)x Y:(int)y
{
    NSPoint point = NSMakePoint(x, y);

    // Flip for TGE window coordinate system.
    point.y = [self screenHeight] - point.y;

    // Place the window.
    [m_window setFrameTopLeftPoint:point];
}


////////////////////////////////////////////////////////
-(NSSize)size
{
    return [m_oglView frame].size;
}


////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height
{
    // Before resizing, remove resizeable mask to be able to resize
    // beyond the desktop boundaries.
    NSUInteger styleMask = [m_window styleMask];

    [m_window setStyleMask:styleMask ^ NSResizableWindowMask];

    // Add titlebar height.
    height += [self titlebarHeight];

    // Corner case: don't set the window height bigger than the screen height
    // or the view will be resized _later_ without generating a resize event.
    NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
    CGFloat maxVisibleHeight = screenFrame.size.height;
    if (height > maxVisibleHeight)
    {
        height = maxVisibleHeight;

        // The size is not the requested one, we fire an event
        if (m_requester != 0)
            m_requester->windowResized(width, height - [self titlebarHeight]);
    }

    NSRect frame = NSMakeRect([m_window frame].origin.x,
                              [m_window frame].origin.y,
                              width,
                              height);

    [m_window setFrame:frame display:YES];

    // And restore the mask
    [m_window setStyleMask:styleMask];
}


////////////////////////////////////////////////////////
-(void)changeTitle:(NSString*)title
{
    [m_window setTitle:title];
}


////////////////////////////////////////////////////////
-(void)hideWindow
{
    [m_window orderOut:nil];
}


////////////////////////////////////////////////////////
-(void)showWindow
{
    [m_window makeKeyAndOrderFront:nil];
}


////////////////////////////////////////////////////////
-(void)closeWindow
{
    [m_window close];
    [m_window setDelegate:nil];
    [self setRequesterTo:0];
}


////////////////////////////////////////////////////////
-(void)enableKeyRepeat
{
    [m_oglView enableKeyRepeat];
}


////////////////////////////////////////////////////////
-(void)disableKeyRepeat
{
    [m_oglView disableKeyRepeat];
}


////////////////////////////////////////////////////////
-(void)setIconTo:(unsigned int)width
              by:(unsigned int)height
            with:(const TGE::Uint8*)pixels
{
    // Create an empty image representation.
    NSBitmapImageRep* bitmap =
    [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:0 // if 0 : only allocate memory
                                            pixelsWide:width
                                            pixelsHigh:height
                                         bitsPerSample:8 // The number of bits used to specify
                                                         // one pixel in a single component of the data.
                                       samplesPerPixel:4 // 3 if no alpha, 4 with it
                                              hasAlpha:YES
                                              isPlanar:NO   // I don't know what it is but it works
                                        colorSpaceName:NSCalibratedRGBColorSpace
                                           bytesPerRow:0    // 0 == determine automatically
                                          bitsPerPixel:0];  // 0 == determine automatically

    // Load data pixels.
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x, pixels+=4)
        {
            NSUInteger pixel[4] = { pixels[0], pixels[1], pixels[2], pixels[3] };
            [bitmap setPixel:pixel atX:x y:y];
        }
    }

    // Create an image from the representation.
    NSImage* icon = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [icon addRepresentation:bitmap];

    // Set app icon.
    [[TGEApplication sharedApplication] setApplicationIconImage:icon];
}


////////////////////////////////////////////////////////
-(void)processEvent
{
    // If we are not on the main thread we stop here and advice the user.
    if ([NSThread currentThread] != [NSThread mainThread])
    {
        /*
         * See http://lists.apple.com/archives/cocoa-dev/2011/Feb/msg00460.html
         * for more information.
         */
        TGE::Log() << "Cannot fetch event from a worker thread. (OS X restriction)" << std::endl;

        return;
    }

    // If we don't have a requester we don't fetch event.
    if (m_requester != 0)
        [TGEApplication processEvent];
}


////////////////////////////////////////////////////////
-(void)applyContext:(NSOpenGLContext*)context
{
    [m_oglView setOpenGLContext:context];
    [context setView:m_oglView];
}


#pragma mark
#pragma mark NSWindowDelegate's methods


////////////////////////////////////////////////////////
-(BOOL)windowShouldClose:(id)sender
{
    (void)sender;

    if (m_requester == 0)
        return YES;

    m_requester->windowClosed();
    return NO;
}


////////////////////////////////////////////////////////
-(void)windowDidBecomeKey:(NSNotification*)notification
{
    (void)notification;

    // Send event.
    if (m_requester == 0)
        return;

    m_requester->windowGainedFocus();

    if (m_fullscreen)
        [m_oglView enterFullscreen];
}


////////////////////////////////////////////////////////
-(void)windowDidResignKey:(NSNotification*)notification
{
    (void)notification;

    // Send event.
    if (m_requester == 0)
        return;

    m_requester->windowLostFocus();

    if (m_fullscreen)
        [m_oglView exitFullscreen];
}


#pragma mark
#pragma mark Other methods

////////////////////////////////////////////////////////
-(float)screenHeight
{
    NSDictionary* deviceDescription = [[m_window screen] deviceDescription];
    NSNumber* screenNumber = [deviceDescription valueForKey:@"NSScreenNumber"];
    CGDirectDisplayID screenID = (CGDirectDisplayID)[screenNumber intValue];
    CGFloat height = CGDisplayPixelsHigh(screenID);
    return height;
}


////////////////////////////////////////////////////////
-(float)titlebarHeight
{
    return NSHeight([m_window frame]) - NSHeight([[m_window contentView] frame]);
}

@end
