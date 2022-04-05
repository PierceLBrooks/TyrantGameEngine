/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_WINDOWIMPLCOCOA_HPP
#define TGE_WINDOWIMPLCOCOA_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Event.hpp>
#include <Tyrant/Window/WindowImpl.hpp>
#include <Tyrant/System/String.hpp>

////////////////////////////////////////////////////////////
/// Predefine OBJ-C classes
////////////////////////////////////////////////////////////
#ifdef __OBJC__

#import <Tyrant/Window/OSX/WindowImplDelegateProtocol.h>
typedef id<WindowImplDelegateProtocol,NSObject> WindowImplDelegateRef;

@class NSOpenGLContext;
typedef NSOpenGLContext* NSOpenGLContextRef;

#else // If C++

typedef unsigned short unichar; // See NSString.h

typedef void* WindowImplDelegateRef;
typedef void* NSOpenGLContextRef;

#endif

namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Mac OS X (Cocoa) implementation of WindowImpl
///
////////////////////////////////////////////////////////////
class WindowImplCocoa : public WindowImpl
{
public :
    ////////////////////////////////////////////////////////////
    /// \brief Construct the window implementation from an existing control
    ///
    /// \param handle Platform-specific handle of the control
    ///
    ////////////////////////////////////////////////////////////
    WindowImplCocoa(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Create the window implementation
    ///
    /// \param mode  Video mode to use
    /// \param title Title of the window
    /// \param style Window style (resizeable, fixed, or fullscren)
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    WindowImplCocoa(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WindowImplCocoa();

    ////////////////////////////////////////////////////////////
    /// \brief Window Closed Event – called by the cocoa window object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void windowClosed(void);

    ////////////////////////////////////////////////////////////
    /// \brief Window Resized Event – called by the cocoa window object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param width new width
    /// \param height new height
    ///
    ////////////////////////////////////////////////////////////
    void windowResized(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Window Lost Focus Event – called by the cocoa window object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void windowLostFocus(void);

    ////////////////////////////////////////////////////////////
    /// \brief Window Get Focus Event – called by the cocoa window object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void windowGainedFocus(void);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Down Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param button active button
    /// \param x mouse x position
    /// \param y mouse y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseDownAt(Mouse::Button button, int x, int y);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Up Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param button active button
    /// \param x mouse x position
    /// \param y mouse y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseUpAt(Mouse::Button button, int x, int y);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Moved Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param x mouse x position
    /// \param y mouse y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseMovedAt(int x, int y);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Wheel Scrolled Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param delta scrolling delta
    /// \param x mouse x position
    /// \param y mouse y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseWheelScrolledAt(float delta, int x, int y);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse In Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void mouseMovedIn(void);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Out Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void mouseMovedOut(void);

    ////////////////////////////////////////////////////////////
    /// \brief Key Down Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param key active key
    ///
    ////////////////////////////////////////////////////////////
    void keyDown(Event::KeyEvent key);

    ////////////////////////////////////////////////////////////
    /// \brief Key Up Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param key active key
    ///
    ////////////////////////////////////////////////////////////
    void keyUp(Event::KeyEvent key);

    ////////////////////////////////////////////////////////////
    /// \brief Text Entred Event – called by the cocoa view object
    ///
    /// Send the event to TGE WindowImpl class.
    ///
    /// \param charcode Unicode input
    ///
    ////////////////////////////////////////////////////////////
    void textEntered(unichar charcode);

    ////////////////////////////////////////////////////////////
    /// \brief Apply the context to the view
    ///
    /// Called by the TGE context object to finalize its creation.
    ///
    /// \param context The context to bind to the window
    ///
    ////////////////////////////////////////////////////////////
    void applyContext(NSOpenGLContextRef context) const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the type of the current process
    ///
    /// The type of the process is changed to become a full GUI app.
    /// Also ensure NSApp is constructed.
    ///
    ////////////////////////////////////////////////////////////
    static void setUpProcess(void);

public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the OS-specific handle of the window
    ///
    /// \return Handle of the window
    ///
    ////////////////////////////////////////////////////////////
    virtual WindowHandle getSystemHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the window
    ///
    /// \return Position of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2i getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the window on screen
    ///
    /// \param position New position of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setPosition(const Vector2i& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the client size of the window
    ///
    /// \return Size of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the rendering region of the window
    ///
    /// \param size New size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSize(const Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Change the title of the window
    ///
    /// \param title New title
    ///
    ////////////////////////////////////////////////////////////
    virtual void setTitle(const String& title);

    ////////////////////////////////////////////////////////////
    /// \brief Change the window's icon
    ///
    /// \param width  Icon's width, in pixels
    /// \param height Icon's height, in pixels
    /// \param pixels Pointer to the pixels in memory, format must be RGBA 32 bits
    ///
    ////////////////////////////////////////////////////////////
    virtual void setIcon(unsigned int width, unsigned int height, const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the window
    ///
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void setVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void setMouseCursorVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    virtual void setKeyRepeatEnabled(bool enabled);

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Process incoming events from the operating system
    ///
    ////////////////////////////////////////////////////////////
    virtual void processEvents();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    WindowImplDelegateRef m_delegate;   ///< Implementation in Obj-C.
    bool                  m_showCursor; ///< Is the cursor displayed or hidden?
};

} // namespace priv

} // namespace TGE


#endif // TGE_WINDOWIMPLCOCOA_HPP
