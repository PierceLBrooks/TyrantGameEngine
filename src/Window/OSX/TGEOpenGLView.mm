/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/OSX/WindowImplCocoa.hpp>
#include <Tyrant/Window/OSX/HIDInputManager.hpp> // For localizedKeys and nonLocalizedKeys
#include <Tyrant/System/Log.hpp>

#import <Tyrant/Window/OSX/TGEKeyboardModifiersHelper.h>
#import <Tyrant/Window/OSX/TGEOpenGLView.h>
#import <Tyrant/Window/OSX/TGESilentResponder.h>


////////////////////////////////////////////////////////////
/// \brief Check if the event represent some Unicode text
///
/// The event is assumed to be a key down event.
/// False is returned if the event is either escape or a non text Unicode.
///
/// \param event a key down event
///
/// \return true if event represents a Unicode character, false otherwise
///
////////////////////////////////////////////////////////////
BOOL isValidTextUnicode(NSEvent* event);


////////////////////////////////////////////////////////////
/// TGEOpenGLView class: Privates Methods Declaration
///
////////////////////////////////////////////////////////////
@interface TGEOpenGLView ()

////////////////////////////////////////////////////////////
/// \brief Handle view resized event
///
////////////////////////////////////////////////////////////
-(void)viewDidEndLiveResize;

////////////////////////////////////////////////////////////
/// \brief Determine where the mouse is
///
/// \return true when the mouse is inside the OpenGL view, false otherwise
///
////////////////////////////////////////////////////////////
-(BOOL)isMouseInside;

////////////////////////////////////////////////////////////
/// \brief Update the mouse state (in or out)
///
/// Fire an event if its state has changed.
///
////////////////////////////////////////////////////////////
-(void)updateMouseState;

////////////////////////////////////////////////////////////
/// \brief Convert the NSEvent mouse button type to TGE type
///
/// \param event a mouse button event
///
/// \return Left, Right, ..., or ButtonCount if the button is unknown
///
////////////////////////////////////////////////////////////
-(TGE::Mouse::Button)mouseButtonFromEvent:(NSEvent*)event;

////////////////////////////////////////////////////////////
/// \brief Convert a key down/up NSEvent into an TGE key event
///
/// The conversion is based on localizedKeys and nonLocalizedKeys functions.
///
/// \param event a key event
///
/// \return TGE::Keyboard::Unknown as Code if the key is unknown
///
////////////////////////////////////////////////////////////
+(TGE::Event::KeyEvent)convertNSKeyEventToTGEEvent:(NSEvent*)event;

@end

@implementation TGEOpenGLView

#pragma mark
#pragma mark TGEOpenGLView's methods

////////////////////////////////////////////////////////
-(id)initWithFrame:(NSRect)frameRect
{
    if ((self = [super initWithFrame:frameRect]))
    {
        [self setRequesterTo:0];
        [self enableKeyRepeat];

        // Register for mouse move event
        m_mouseIsIn = [self isMouseInside];
        NSUInteger opts = (NSTrackingActiveAlways | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingEnabledDuringMouseDrag);
        m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                      options:opts
                                                        owner:self
                                                     userInfo:nil];
        [self addTrackingArea:m_trackingArea];

        // Create a hidden text view for parsing key down event properly
        m_silentResponder = [[TGESilentResponder alloc] init];
        m_hiddenTextView = [[NSTextView alloc] initWithFrame:NSZeroRect];
        [m_hiddenTextView setNextResponder:m_silentResponder];

        // Request high resolution on high DPI displays
        [self setWantsBestResolutionOpenGLSurface:YES];
    }

    return self;
}


////////////////////////////////////////////////////////
-(void)enterFullscreen
{
    // Remove the tracking area first,
    // just to be sure we don't add it twice!
    [self removeTrackingArea:m_trackingArea];
    [self addTrackingArea:m_trackingArea];

    // Fire an mouse entered event if needed
    if (!m_mouseIsIn && (m_requester != 0))
        m_requester->mouseMovedIn();

    // Update status
    m_mouseIsIn = YES;
}


////////////////////////////////////////////////////////
-(void)exitFullscreen
{
    [self removeTrackingArea:m_trackingArea];

    // Fire an mouse left event if needed
    if (m_mouseIsIn && (m_requester != 0))
        m_requester->mouseMovedOut();

    // Update status
    m_mouseIsIn = NO;
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(TGE::priv::WindowImplCocoa*)requester
{
    m_requester = requester;
}


////////////////////////////////////////////////////////
-(NSPoint)convertPointToScreen:(NSPoint)point
{
    NSRect rect = NSZeroRect;
    rect.origin = point;
    rect = [[self window] convertRectToScreen:rect];
    return rect.origin;
}


////////////////////////////////////////////////////////
-(NSPoint)computeGlobalPositionOfRelativePoint:(NSPoint)point
{
    // Flip TGE coordinates to match window coordinates
    point.y = [self frame].size.height - point.y;

    // Get the position of (x, y) in the coordinate system of the window.
    point = [self convertPoint:point toView:self];
    point = [self convertPoint:point toView:nil]; // nil means window

    // Convert it to screen coordinates
    point = [self convertPointToScreen:point];

    // Flip screen coordinates to match CGDisplayMoveCursorToPoint referential.
    const float screenHeight = [[[self window] screen] frame].size.height;
    point.y = screenHeight - point.y;

    return point;
}


////////////////////////////////////////////////////////
-(void)enableKeyRepeat
{
    m_useKeyRepeat = YES;
}


////////////////////////////////////////////////////////
-(void)disableKeyRepeat
{
    m_useKeyRepeat = NO;
}


////////////////////////////////////////////////////////
-(void)viewDidEndLiveResize
{
    // We use viewDidEndLiveResize to notify the user ONCE
    // only, when the resizing is finished.
    // In a perfect world we would like to notify the user
    // in live that the window is being resized. However,
    // it seems impossible to forward to the user
    // NSViewFrameDidChangeNotification before the resizing
    // is done. Several notifications are emitted but they
    // are all delivered after when the work is done.

    [super viewDidEndLiveResize];

    // Update mouse internal state.
    [self updateMouseState];

    // Update the OGL view to fit the new size.
    [self update];

    // Send an event
    if (m_requester == 0)
        return;

    // The new size
    NSSize newSize = [self frame].size;
    m_requester->windowResized(newSize.width, newSize.height);
}


////////////////////////////////////////////////////////
-(BOOL)isMouseInside
{
    NSPoint relativeToWindow = [[self window] mouseLocationOutsideOfEventStream];
    NSPoint relativeToView = [self convertPoint:relativeToWindow fromView:nil];

    return NSPointInRect(relativeToView, [self bounds]);
}


////////////////////////////////////////////////////////
-(void)updateMouseState
{
    BOOL mouseWasIn = m_mouseIsIn;
    m_mouseIsIn = [self isMouseInside];

    if (m_requester == 0)
        return;

    // Send event if needed.
    if (mouseWasIn && !m_mouseIsIn)
        m_requester->mouseMovedOut();
    else if (!mouseWasIn && m_mouseIsIn)
        m_requester->mouseMovedIn();
}


#pragma mark
#pragma mark Subclassing methods


////////////////////////////////////////////////////////
-(void)dealloc
{
    // Unregister
    [self removeTrackingArea:m_trackingArea];

    [self setRequesterTo:0];

    m_hiddenTextView = nil;
    m_silentResponder = nil;
    m_trackingArea = nil;
}


////////////////////////////////////////////////////////
-(BOOL)acceptsFirstResponder
{
    // Accepts key event.
    return YES;
}


////////////////////////////////////////////////////////
-(BOOL)canBecomeKeyView
{
    // Accepts key event.
    return YES;
}


#pragma mark
#pragma mark Mouse-event methods


////////////////////////////////////////////////////////
-(void)mouseDown:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDown:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] mouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseUp:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseUp:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] mouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseMoved:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] mouseMoved:theEvent];
}


////////////////////////////////////////////////////////
-(void)scrollWheel:(NSEvent*)theEvent
{
    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];
        m_requester->mouseWheelScrolledAt([theEvent deltaY], loc.x, loc.y);
    }

    // Transmit to non-TGE responder
    [[self nextResponder] scrollWheel:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseEntered:(NSEvent*)theEvent
{
    (void)theEvent;
    [self updateMouseState];
}


////////////////////////////////////////////////////////
-(void)mouseExited:(NSEvent*)theEvent
{
    (void)theEvent;
    [self updateMouseState];
}


////////////////////////////////////////////////////////
-(void)rightMouseDown:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDown:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] rightMouseDown:theEvent];
}


////////////////////////////////////////////////////////
-(void)rightMouseUp:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseUp:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] rightMouseUp:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDown:(NSEvent*)theEvent
{
    TGE::Mouse::Button button = [self mouseButtonFromEvent:theEvent];

    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        if (button != TGE::Mouse::ButtonCount)
            m_requester->mouseDownAt(button, loc.x, loc.y);
    }

    // If the event is not forwarded by mouseDown or rightMouseDown...
    if ((button != TGE::Mouse::Left) && (button != TGE::Mouse::Right))
    {
        // ... transmit to non-TGE responder
        [[self nextResponder] otherMouseDown:theEvent];
    }
}


////////////////////////////////////////////////////////
-(void)otherMouseUp:(NSEvent*)theEvent
{
    TGE::Mouse::Button button = [self mouseButtonFromEvent:theEvent];

    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        if (button != TGE::Mouse::ButtonCount)
            m_requester->mouseUpAt(button, loc.x, loc.y);
    }

    // If the event is not forwarded by mouseUp or rightMouseUp...
    if ((button != TGE::Mouse::Left) && (button != TGE::Mouse::Right))
    {
        // ... transmit to non-TGE responder
        [[self nextResponder] otherMouseUp:theEvent];
    }
}


////////////////////////////////////////////////////////
-(void)rightMouseDragged:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] rightMouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)mouseDragged:(NSEvent*)theEvent
{
    // Forward to...
    [self otherMouseDragged:theEvent];

    // Transmit to non-TGE responder
    [[self nextResponder] mouseDragged:theEvent];
}


////////////////////////////////////////////////////////
-(void)otherMouseDragged:(NSEvent*)theEvent
{
    if (m_requester != 0)
    {
        NSPoint loc = [self cursorPositionFromEvent:theEvent];

        // Make sure the point is inside the view.
        // (mouseEntered: and mouseExited: are not immediately called
        //  when the mouse is dragged. That would be too easy!)
        [self updateMouseState];
        if (m_mouseIsIn)
            m_requester->mouseMovedAt(loc.x, loc.y);
    }

    // If the event is not forwarded by mouseDragged or rightMouseDragged...
    TGE::Mouse::Button button = [self mouseButtonFromEvent:theEvent];
    if ((button != TGE::Mouse::Left) && (button != TGE::Mouse::Right))
    {
        // ... transmit to non-TGE responder
        [[self nextResponder] otherMouseUp:theEvent];
    }
}


////////////////////////////////////////////////////////
-(NSPoint)cursorPositionFromEvent:(NSEvent*)eventOrNil
{
    NSPoint loc;
    // If no event given then get current mouse pos.
    if (eventOrNil == nil)
    {
        NSPoint rawPos = [[self window] mouseLocationOutsideOfEventStream];
        loc = [self convertPoint:rawPos fromView:nil];
    }
    else
    {
        loc = [self convertPoint:[eventOrNil locationInWindow] fromView:nil];
    }

    // Don't forget to change to TGE coord system.
    float h = [self frame].size.height;
    loc.y = h - loc.y;

    return loc;
}


////////////////////////////////////////////////////////
-(TGE::Mouse::Button)mouseButtonFromEvent:(NSEvent*)event
{
    switch ([event buttonNumber])
    {
        case 0:     return TGE::Mouse::Left;
        case 1:     return TGE::Mouse::Right;
        case 2:     return TGE::Mouse::Middle;
        case 3:     return TGE::Mouse::XButton1;
        case 4:     return TGE::Mouse::XButton2;
        default:    return TGE::Mouse::ButtonCount; // Never happens! (hopefully)
    }
}


#pragma mark
#pragma mark Key-event methods


////////////////////////////////////////////////////////
-(void)keyDown:(NSEvent*)theEvent
{
    // Transmit to non-TGE responder
    [[self nextResponder] keyDown:theEvent];

    if (m_requester == 0)
        return;

    // Handle key down event
    if (m_useKeyRepeat || ![theEvent isARepeat])
    {
        TGE::Event::KeyEvent key = [TGEOpenGLView convertNSKeyEventToTGEEvent:theEvent];

        if (key.code != TGE::Keyboard::Unknown) // The key is recognized.
            m_requester->keyDown(key);
    }


    // Handle text entred event:
    // Ignore event if we don't want repeated keystrokes
    if (m_useKeyRepeat || ![theEvent isARepeat])
    {
        // Ignore escape key and other non text keycode (See NSEvent.h)
        // because they produce a sound alert.
        if (isValidTextUnicode(theEvent))
        {
            // Send the event to the hidden text view for processing
            [m_hiddenTextView interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
        }

        // Carefully handle backspace and delete..
        // Note: the event is intentionally sent to the hidden view
        //       even if we do something more specific below. This way
        //       key combination are correctly interpreted.

        unsigned short keycode = [theEvent keyCode];

        // Backspace
        if (keycode == 0x33)
        {
            // Send the correct unicode value (i.e. 8) instead of 127 (which is 'delete')
            m_requester->textEntered(8);
        }

        // Delete
        else if ((keycode == 0x75) || (keycode == NSDeleteFunctionKey))
        {
            // Instead of the value 63272 we send 127.
            m_requester->textEntered(127);
        }

        // Otherwise, let's see what our hidden field has computed
        else
        {
            NSString* string = [m_hiddenTextView string];

            // Send each character to TGE event requester
            for (NSUInteger index = 0; index < [string length]; ++index)
                m_requester->textEntered([string characterAtIndex:index]);

            // Empty our hidden cache
            [m_hiddenTextView setString:@""];
        }
    }
}


////////////////////////////////////////////////////////
-(void)tgeKeyUp:(NSEvent*)theEvent
{
    // For some mystic reasons, key released events don't work the same way
    // as key pressed events... We somewhat hijack the event chain of response
    // in -[TGEApplication sendEvent:] and resume this chain with the next
    // responder.
    // This is workaround to make sure key released events are fired in
    // fullscreen window too.

    // Transmit to non-TGE responder
    [[self nextResponder] keyUp:theEvent];

    if (m_requester == 0)
        return;

    TGE::Event::KeyEvent key = [TGEOpenGLView convertNSKeyEventToTGEEvent:theEvent];

    if (key.code != TGE::Keyboard::Unknown) // The key is recognized.
        m_requester->keyUp(key);
}


////////////////////////////////////////////////////////
-(void)flagsChanged:(NSEvent*)theEvent
{
    // Transmit to non-TGE responder
    [[self nextResponder] flagsChanged:theEvent];

    if (m_requester == 0)
        return;

    NSUInteger modifiers = [theEvent modifierFlags];
    handleModifiersChanged(modifiers, *m_requester);
}


////////////////////////////////////////////////////////
+(TGE::Event::KeyEvent)convertNSKeyEventToTGEEvent:(NSEvent*)event
{
    // Key code
    TGE::Keyboard::Key key = TGE::Keyboard::Unknown;

    // First we look if the key down is from a list of characters
    // that depend on keyboard localization.
    NSString* string = [event charactersIgnoringModifiers];
    if ([string length] > 0)
        key = TGE::priv::HIDInputManager::localizedKeys([string characterAtIndex:0]);

    // If the key is not a localized one, we try to find a corresponding code
    // through virtual key code.
    if (key == TGE::Keyboard::Unknown)
        key = TGE::priv::HIDInputManager::nonLocalizedKeys([event keyCode]);

//#ifdef TGE_DEBUG // Don't bother the final customers with annoying messages.
//    if (key.code == TGE::Keyboard::Unknown) { // The key is unknown.
//        TGE::Log() << "This is an unknow key. Virtual key code is 0x"
//                  << std::hex
//                  << [event keyCode]
//                  << "."
//                  << std::endl;
//    }
//#endif

    return keyEventWithModifiers([event modifierFlags], key);
}

@end


#pragma mark - C-like functions

BOOL isValidTextUnicode(NSEvent* event)
{
    if ([event keyCode] == 0x35) // Escape
    {
        return false;
    }
    else if ([[event characters] length] > 0)
    {
        unichar code = [[event characters] characterAtIndex:0];
        return ((code < 0xF700) || (code > 0xF8FF));
    }
    else
    {
        return true;
    }
}

