/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/OSX/WindowImplCocoa.hpp>
#include <Tyrant/System/Log.hpp>
#include <Tyrant/System/String.hpp>

#import <Tyrant/Window/OSX/cpp_objc_conversion.h>
#import <Tyrant/Window/OSX/TGEApplication.h>
#import <Tyrant/Window/OSX/TGEApplicationDelegate.h>
#import <Tyrant/Window/OSX/TGEKeyboardModifiersHelper.h>
#import <Tyrant/Window/OSX/TGEViewController.h>
#import <Tyrant/Window/OSX/TGEWindowController.h>

namespace TGE
{
namespace priv
{

////////////////////////////////////////////////////////////
/// \brief Scale TGE coordinates to backing coordinates
///
/// Use -[NSScreen backingScaleFactor] to find out if the user
/// has a retina display or not.
///
/// \param in TGE coordinates to be converted
///
////////////////////////////////////////////////////////////
template <class T>
void scaleIn(T& in)
{
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    in /= scale;
}

template <class T>
void scaleInWidthHeight(T& in)
{
    scaleIn(in.width);
    scaleIn(in.height);
}

template <class T>
void scaleInXY(T& in)
{
    scaleIn(in.x);
    scaleIn(in.y);
}

////////////////////////////////////////////////////////////
/// \brief Scale backing coordinates to TGE coordinates
///
/// Use -[NSScreen backingScaleFactor] to find out if the user
/// has a retina display or not.
///
/// \param out backing coordinates to be converted
///
////////////////////////////////////////////////////////////
template <class T>
void scaleOut(T& out)
{
    CGFloat scale = [[NSScreen mainScreen] backingScaleFactor];
    out *= scale;
}

template <class T>
void scaleOutWidthHeight(T& out)
{
    scaleOut(out.width);
    scaleOut(out.height);
}

template <class T>
void scaleOutXY(T& out)
{
    scaleOut(out.x);
    scaleOut(out.y);
}

#pragma mark
#pragma mark WindowImplCocoa's ctor/dtor

////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(WindowHandle handle) :
m_showCursor(true)
{
@autoreleasepool
{
    // Treat the handle as it real type
    id nsHandle = (__bridge id)handle;
    if ([nsHandle isKindOfClass:[NSWindow class]])
    {
        // We have a window.
        m_delegate = [[TGEWindowController alloc] initWithWindow:nsHandle];
    }
    else if ([nsHandle isKindOfClass:[NSView class]])
    {
        // We have a view.
        m_delegate = [[TGEViewController alloc] initWithView:nsHandle];
    }
    else
    {

        TGE::Log() << "Cannot import this Window Handle because it is neither "
                  << "a <NSWindow*> nor <NSView*> object "
                  << "(or any of their subclasses). You gave a <"
                  << [[nsHandle className] UTF8String]
                  << "> object."
                  << std::endl;
        return;

    }

    [m_delegate setRequesterTo:this];

    // Finally, set up keyboard helper
    initialiseKeyboardHelper();
} // pool
}


////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(VideoMode mode,
                                 const String& title,
                                 unsigned long style,
                                 const ContextSettings& /*settings*/) :
m_showCursor(true)
{
@autoreleasepool
{
    // Transform the app process.
    setUpProcess();

    // Use backing size
    scaleInWidthHeight(mode);

    m_delegate = [[TGEWindowController alloc] initWithMode:mode andStyle:style];
    [m_delegate changeTitle:tgeStringToNSString(title)];
    [m_delegate setRequesterTo:this];

    // Finally, set up keyboard helper
    initialiseKeyboardHelper();
} // pool
}


////////////////////////////////////////////////////////////
WindowImplCocoa::~WindowImplCocoa()
{
@autoreleasepool
{
    [m_delegate closeWindow];
    m_delegate = nil;

    // Put the next window in front, if any.
    NSArray* windows = [NSApp orderedWindows];
    if ([windows count] > 0)
        [[windows objectAtIndex:0] makeKeyAndOrderFront:nil];
} // pool
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::applyContext(NSOpenGLContextRef context) const
{
    [m_delegate applyContext:context];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setUpProcess(void)
{
    static bool isTheProcessSetAsApplication = false;

    if (!isTheProcessSetAsApplication)
    {
        // Do it only once !
        isTheProcessSetAsApplication = true;

        // Set the process as a normal application so it can get focus.
        ProcessSerialNumber psn;
        if (!GetCurrentProcess(&psn))
        {
            TransformProcessType(&psn, kProcessTransformToForegroundApplication);
            SetFrontProcess(&psn);
        }

        // Register an application delegate if there is none
        if (![[TGEApplication sharedApplication] delegate])
            [NSApp setDelegate:[TGEApplicationDelegate instance]];

        // Create menus for the application (before finishing launching!)
        [TGEApplication setUpMenuBar];

        // Tell the application to stop bouncing in the Dock.
        [[TGEApplication sharedApplication] finishLaunching];
        // NOTE : This last call won't harm anything even if TGE window was
        // created with an external handle.
    }
}


#pragma mark
#pragma mark WindowImplCocoa's window-event methods


////////////////////////////////////////////////////////////
void WindowImplCocoa::windowClosed(void)
{
    Event event;
    event.type = Event::Closed;

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::windowResized(unsigned int width, unsigned int height)
{
    Event event;
    event.type = Event::Resized;
    event.size.width  = width;
    event.size.height = height;
    scaleOutWidthHeight(event.size);

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::windowLostFocus(void)
{
    if (!m_showCursor)
        [m_delegate showMouseCursor]; // Make sure the cursor is visible

    Event event;
    event.type = Event::LostFocus;

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::windowGainedFocus(void)
{
    if (!m_showCursor)
        [m_delegate hideMouseCursor]; // Restore user's setting

    Event event;
    event.type = Event::GainedFocus;

    pushEvent(event);
}

#pragma mark
#pragma mark WindowImplCocoa's mouse-event methods


////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseDownAt(Mouse::Button button, int x, int y)
{
    Event event;
    event.type = Event::MouseButtonPressed;
    event.mouseButton.button = button;
    event.mouseButton.x = x;
    event.mouseButton.y = y;
    scaleOutXY(event.mouseButton);

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseUpAt(Mouse::Button button, int x, int y)
{
    Event event;
    event.type = Event::MouseButtonReleased;
    event.mouseButton.button = button;
    event.mouseButton.x = x;
    event.mouseButton.y = y;
    scaleOutXY(event.mouseButton);

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseMovedAt(int x, int y)
{
    Event event;
    event.type = Event::MouseMoved;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    scaleOutXY(event.mouseMove);

    pushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseWheelScrolledAt(float delta, int x, int y)
{
    Event event;
    event.type = Event::MouseWheelMoved;
    event.mouseWheel.delta = delta;
    event.mouseWheel.x = x;
    event.mouseWheel.y = y;
    scaleOutXY(event.mouseWheel);

    pushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseMovedIn(void)
{
    if (!m_showCursor)
        [m_delegate hideMouseCursor]; // Restore user's setting

    Event event;
    event.type = Event::MouseEntered;

    pushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseMovedOut(void)
{
    if (!m_showCursor)
        [m_delegate showMouseCursor]; // Make sure the cursor is visible

    Event event;
    event.type = Event::MouseLeft;

    pushEvent(event);
}


#pragma mark
#pragma mark WindowImplCocoa's key-event methods


////////////////////////////////////////////////////////////
void WindowImplCocoa::keyDown(Event::KeyEvent key)
{
    Event event;
    event.type = Event::KeyPressed;
    event.key = key;

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::keyUp(Event::KeyEvent key)
{
    Event event;
    event.type = Event::KeyReleased;
    event.key = key;

    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::textEntered(unichar charcode)
{
    Event event;
    event.type = Event::TextEntered;
    event.text.unicode = charcode;

    pushEvent(event);
}


#pragma mark
#pragma mark WindowImplCocoa's event-related methods

////////////////////////////////////////////////////////////
void WindowImplCocoa::processEvents()
{
    [m_delegate processEvent];
}

#pragma mark
#pragma mark WindowImplCocoa's private methods

////////////////////////////////////////////////////////////
WindowHandle WindowImplCocoa::getSystemHandle() const
{
    return [m_delegate getSystemHandle];
}


////////////////////////////////////////////////////////////
Vector2i WindowImplCocoa::getPosition() const
{
    NSPoint pos = [m_delegate position];
    TGE::Vector2i ret(pos.x, pos.y);
    scaleOutXY(ret);
    return ret;
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setPosition(const Vector2i& position)
{
    TGE::Vector2i backingPosition = position;
    scaleInXY(backingPosition);
    [m_delegate setWindowPositionToX:backingPosition.x Y:backingPosition.y];
}


////////////////////////////////////////////////////////////
Vector2u WindowImplCocoa::getSize() const
{
    NSSize size = [m_delegate size];
    Vector2u ret(size.width, size.height);
    scaleOutXY(ret);
    return ret;
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setSize(const Vector2u& size)
{
    TGE::Vector2u backingSize = size;
    scaleInXY(backingSize);
    [m_delegate resizeTo:backingSize.x by:backingSize.y];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setTitle(const String& title)
{
    [m_delegate changeTitle:tgeStringToNSString(title)];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    [m_delegate setIconTo:width by:height with:pixels];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setVisible(bool visible)
{
    if (visible)
        [m_delegate showWindow];
    else
        [m_delegate hideWindow];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setMouseCursorVisible(bool visible)
{
    m_showCursor = visible;

    if (m_showCursor)
        [m_delegate showMouseCursor];
    else
        [m_delegate hideMouseCursor];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setKeyRepeatEnabled(bool enabled)
{
    if (enabled)
        [m_delegate enableKeyRepeat];
    else
        [m_delegate disableKeyRepeat];
}


} // namespace priv

} // namespace TGE
