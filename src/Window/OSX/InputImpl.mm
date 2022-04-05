/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoMode.hpp>
#include <Tyrant/Window/Window.hpp>
#include <Tyrant/Window/OSX/InputImpl.hpp>
#include <Tyrant/Window/OSX/HIDInputManager.hpp>
#include <Tyrant/System/Log.hpp>

#import <Tyrant/Window/OSX/TGEOpenGLView.h>
#import <AppKit/AppKit.h>

////////////////////////////////////////////////////////////
/// In order to keep track of the keyboard's state and mouse buttons' state
/// we use the HID manager. Mouse position is handled differently.
///
/// NB: we probably could use
/// NSEvent +addGlobalMonitorForEventsMatchingMask:handler: for mouse only.
///
////////////////////////////////////////////////////////////

namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Extract the dedicated TGEOpenGLView from the TGE window
///
/// \param window a TGE window
/// \return nil if something went wrong or a TGEOpenGLView*.
///
////////////////////////////////////////////////////////////
TGEOpenGLView* getTGEOpenGLViewFromTGEWindow(const Window& window)
{
    id nsHandle = (__bridge id)window.getSystemHandle();

    // Get our TGEOpenGLView from ...
    TGEOpenGLView* view = nil;

    if ([nsHandle isKindOfClass:[NSWindow class]])
    {
        // If system handle is a window then from its content view.
        view = [nsHandle contentView];

        // Subview doesn't match ?
        if (![view isKindOfClass:[TGEOpenGLView class]])
        {
            TGE::Log() << "The content view is not a valid TGEOpenGLView"
                      << std::endl;
            view = nil;
        }

    }
    else if ([nsHandle isKindOfClass:[NSView class]])
    {
        // If system handle is a view then from a subview of kind TGEOpenGLView.
        NSArray* subviews = [nsHandle subviews];
        for (NSView* subview in subviews)
        {
            if ([subview isKindOfClass:[TGEOpenGLView class]])
            {
                view = (TGEOpenGLView*)subview;
                break;
            }
        }

        // No matching subview ?
        if (view == nil)
            TGE::Log() << "Cannot find a valid TGEOpenGLView subview." << std::endl;
    }
    else
    {
        if (nsHandle != 0)
            TGE::Log() << "The system handle is neither a <NSWindow*> nor <NSView*>"
                      << "object. This shouldn't happen."
                      << std::endl;
        // Else: this probably means the TGE window was previously closed.
    }

    return view;
}

////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    return HIDInputManager::getInstance().isKeyPressed(key);
}


////////////////////////////////////////////////////////////
void InputImpl::setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool InputImpl::isMouseButtonPressed(Mouse::Button button)
{
    return HIDInputManager::getInstance().isMouseButtonPressed(button);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition()
{
    // Reverse Y axis to match TGE coord.
    NSPoint pos = [NSEvent mouseLocation];
    pos.y = TGE::VideoMode::getDesktopMode().height - pos.y;

    return Vector2i(pos.x, pos.y);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition(const Window& relativeTo)
{
    TGEOpenGLView* view = getTGEOpenGLViewFromTGEWindow(relativeTo);

    // No view ?
    if (view == nil)
        return Vector2i();

    // Use -cursorPositionFromEvent: with nil.
    NSPoint pos = [view cursorPositionFromEvent:nil];

    return Vector2i(pos.x, pos.y);
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position)
{
    // Here we don't need to reverse the coordinates.
    CGPoint pos = CGPointMake(position.x, position.y);

    // Place the cursor.
    CGEventRef event = CGEventCreateMouseEvent(NULL,
                                               kCGEventMouseMoved,
                                               pos,
                                               /*we don't care about this : */0);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
    // This is a workaround to deprecated CGSetLocalEventsSuppressionInterval.
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position, const Window& relativeTo)
{
    TGEOpenGLView* view = getTGEOpenGLViewFromTGEWindow(relativeTo);

    // No view ?
    if (view == nil)
        return;

    // Let TGEOpenGLView compute the position in global coordinate
    NSPoint p = NSMakePoint(position.x, position.y);
    p = [view computeGlobalPositionOfRelativePoint:p];
    setMousePosition(TGE::Vector2i(p.x, p.y));
}


////////////////////////////////////////////////////////////
bool InputImpl::isTouchDown(unsigned int /*finger*/)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/)
{
    // Not applicable
    return Vector2i();
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/, const Window& /*relativeTo*/)
{
    // Not applicable
    return Vector2i();
}

} // namespace priv

} // namespace TGE
