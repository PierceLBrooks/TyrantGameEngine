/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Log.hpp>
#include <Tyrant/Window/OSX/WindowImplCocoa.hpp>

#import <Tyrant/Window/OSX/TGEApplication.h>
#import <Tyrant/Window/OSX/TGEOpenGLView.h>
#import <Tyrant/Window/OSX/TGEViewController.h>

@implementation TGEViewController


////////////////////////////////////////////////////////
-(id)initWithView:(NSView *)view
{
    if ((self = [super init]))
    {
        m_requester = 0;

        // Retain the view for our own use.
        m_view = view;

        if (m_view == nil)
        {
            TGE::Log() << "No view was given to initWithWindow:." << std::endl;
            return self;
        }

        // Create the view.
        NSRect frame = [m_view frame];
        frame.origin.x = 0;
        frame.origin.y = 0;
        m_oglView = [[TGEOpenGLView alloc] initWithFrame:frame];

        if (m_oglView == nil)
        {
            TGE::Log() << "Could not create an instance of NSOpenGLView "
                     << "in (TGEViewController -initWithView:)."
                     << std::endl;

            return self;
        }

        // Set the (OGL) view to the view as its "content" view.
        [m_view addSubview:m_oglView];

        [m_oglView setAutoresizingMask:[m_view autoresizingMask]];
    }

    return self;
}


////////////////////////////////////////////////////////
-(void)dealloc
{
    [self closeWindow];

    m_view = nil;
    m_oglView = nil;
}


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
    return (__bridge TGE::WindowHandle)m_view;
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
    // Origin is bottom-left corner of the window
    return [m_view convertPoint:NSMakePoint(0, 0) toView:nil]; // nil means window
}


////////////////////////////////////////////////////////.
-(void)setWindowPositionToX:(int)x Y:(int)y
{
    (void)x;
    (void)y;
    TGE::Log() << "Cannot move TGE area when TGE is integrated in a NSView. Use the view handler directly instead." << std::endl;
}


////////////////////////////////////////////////////////////
-(NSSize)size
{
    return [m_oglView frame].size;
}


////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height
{
    NSRect frame = NSMakeRect([m_view frame].origin.x,
                              [m_view frame].origin.y,
                              width,
                              height);

    [m_view setFrame:frame];
    [m_oglView setFrame:frame];
}


////////////////////////////////////////////////////////
-(void)changeTitle:(NSString*)title
{
    (void)title;
    TGE::Log() << "Cannot change the title of the TGE area when TGE is integrated in a NSView." << std::endl;
}


////////////////////////////////////////////////////////
-(void)hideWindow
{
    [m_view setHidden:YES];
}


////////////////////////////////////////////////////////
-(void)showWindow
{
    [m_view setHidden:NO];
}


////////////////////////////////////////////////////////
-(void)closeWindow
{
    TGE::Log() << "Cannot close TGE area when TGE is integrated in a NSView." << std::endl;
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
    (void)width;
    (void)height;
    (void)pixels;
    TGE::Log() << "Cannot set an icon when TGE is integrated in a NSView." << std::endl;
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


@end
