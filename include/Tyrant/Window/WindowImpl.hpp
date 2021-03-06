/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_WINDOWIMPL_HPP
#define TGE_WINDOWIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/System/NonCopyable.hpp>
#include <Tyrant/System/String.hpp>
#include <Tyrant/Window/Event.hpp>
#include <Tyrant/Window/Joystick.hpp>
#include <Tyrant/Window/JoystickImpl.hpp>
#include <Tyrant/Window/VideoMode.hpp>
#include <Tyrant/Window/WindowHandle.hpp>
#include <Tyrant/Window/ContextSettings.hpp>
#include <queue>
#include <set>

namespace TGE
{
class WindowListener;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for OS-specific window implementation
///
////////////////////////////////////////////////////////////
class WindowImpl : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Create a new window depending on the current OS
    ///
    /// \param mode  Video mode to use
    /// \param title Title of the window
    /// \param style Window style
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    /// \return Pointer to the created window (don't forget to delete it)
    ///
    ////////////////////////////////////////////////////////////
    static WindowImpl* create(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new window depending on to the current OS
    ///
    /// \param handle Platform-specific handle of the control
    ///
    /// \return Pointer to the created window (don't forget to delete it)
    ///
    ////////////////////////////////////////////////////////////
    static WindowImpl* create(WindowHandle handle);

public :

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~WindowImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Change the joystick threshold, ie. the value below which
    ///        no move event will be generated
    ///
    /// \param threshold : New threshold, in range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    void setJoystickThreshold(float threshold);

    ////////////////////////////////////////////////////////////
    /// \brief Return the next window event available
    ///
    /// If there's no event available, this function calls the
    /// window's internal event processing function.
    /// The \a block parameter controls the behaviour of the function
    /// if no event is available: if it is true then the function
    /// doesn't return until a new event is triggered; otherwise it
    /// returns false to indicate that no event is available.
    ///
    /// \param event Event to be returned
    /// \param block Use true to block the thread until an event arrives
    ///
    ////////////////////////////////////////////////////////////
    bool popEvent(Event& event, bool block);

    ////////////////////////////////////////////////////////////
    /// \brief Get the OS-specific handle of the window
    ///
    /// \return Handle of the window
    ///
    ////////////////////////////////////////////////////////////
    virtual WindowHandle getSystemHandle() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the window
    ///
    /// \return Position of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2i getPosition() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the position of the window on screen
    ///
    /// \param position New position of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setPosition(const Vector2i& position) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the client size of the window
    ///
    /// \return Size of the window, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the rendering region of the window
    ///
    /// \param size New size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSize(const Vector2u& size) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the title of the window
    ///
    /// \param title New title
    ///
    ////////////////////////////////////////////////////////////
    virtual void setTitle(const String& title) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the window's icon
    ///
    /// \param width  Icon's width, in pixels
    /// \param height Icon's height, in pixels
    /// \param pixels Pointer to the pixels in memory, format must be RGBA 32 bits
    ///
    ////////////////////////////////////////////////////////////
    virtual void setIcon(unsigned int width, unsigned int height, const Uint8* pixels) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the window
    ///
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void setVisible(bool visible) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    virtual void setMouseCursorVisible(bool visible) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    virtual void setKeyRepeatEnabled(bool enabled) = 0;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    WindowImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Push a new event into the event queue
    ///
    /// This function is to be used by derived classes, to
    /// notify the TGE window that a new event was triggered
    /// by the system.
    ///
    /// \param event Event to push
    ///
    ////////////////////////////////////////////////////////////
    void pushEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Process incoming events from the operating system
    ///
    ////////////////////////////////////////////////////////////
    virtual void processEvents() = 0;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Read the joysticks state and generate the appropriate events
    ///
    ////////////////////////////////////////////////////////////
    void processJoystickEvents();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::queue<Event> m_events;                          ///< Queue of available events
    JoystickState     m_joystickStates[Joystick::Count]; ///< Previous state of the joysticks
    float             m_joystickThreshold;               ///< Joystick threshold (minimum motion for "move" event to be generated)
};

} // namespace priv

} // namespace TGE


#endif // TGE_WINDOWIMPL_HPP
