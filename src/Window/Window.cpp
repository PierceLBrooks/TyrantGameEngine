/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Window.hpp>
#include <Tyrant/Window/GlContext.hpp>
#include <Tyrant/Window/WindowImpl.hpp>
#include <Tyrant/System/Sleep.hpp>
#include <Tyrant/System/Log.hpp>


namespace
{
    const TGE::Window* fullscreenWindow = NULL;
}


namespace TGE
{
////////////////////////////////////////////////////////////
Window::Window() :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero),
m_size          (0, 0)
{

}


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings) :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero),
m_size          (0, 0)
{
    create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings) :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero),
m_size          (0, 0)
{
    create(handle, settings);
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    close();
}


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    close();

    // Fullscreen style requires some tests
    if (style & Style::Fullscreen)
    {
        // Make sure there's not already a fullscreen window (only one is allowed)
        if (fullscreenWindow)
        {
            Log() << "Creating two fullscreen windows is not allowed, switching to windowed mode" << std::endl;
            style &= ~Style::Fullscreen;
        }
        else
        {
            // Make sure that the chosen video mode is compatible
            if (!mode.isValid())
            {
                Log() << "The requested video mode is not available, switching to a valid mode" << std::endl;
                mode = VideoMode::getFullscreenModes()[0];
            }

            // Update the fullscreen window
            fullscreenWindow = this;
        }
    }

    // Check validity of style
    if ((style & Style::Close) || (style & Style::Resize))
        style |= Style::Titlebar;

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(mode, title, style, settings);

    // Recreate the context
    m_context = priv::GlContext::create(settings, m_impl, mode.bitsPerPixel);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void Window::create(WindowHandle handle, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    close();

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(handle);

    // Recreate the context
    m_context = priv::GlContext::create(settings, m_impl, VideoMode::getDesktopMode().bitsPerPixel);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void Window::close()
{
    // Delete the context
    delete m_context;
    m_context = nullptr;

    // Delete the window implementation
    delete m_impl;
    m_impl = nullptr;

    // Update the fullscreen window
    if (this == fullscreenWindow)
        fullscreenWindow = nullptr;
}


////////////////////////////////////////////////////////////
bool Window::isOpen() const
{
    return m_impl != nullptr;
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::getSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return m_context ? m_context->getSettings() : empty;
}


////////////////////////////////////////////////////////////
bool Window::pollEvent(Event& event)
{
    if (m_impl && m_impl->popEvent(event, false))
    {
        return filterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
bool Window::waitEvent(Event& event)
{
    if (m_impl && m_impl->popEvent(event, true))
    {
        return filterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
Vector2i Window::getPosition() const
{
    return m_impl ? m_impl->getPosition() : Vector2i();
}


////////////////////////////////////////////////////////////
void Window::setPosition(const Vector2i& position)
{
    if (m_impl)
        m_impl->setPosition(position);
}


////////////////////////////////////////////////////////////
Vector2u Window::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void Window::setSize(const Vector2u& size)
{
    if (m_impl)
    {
        m_impl->setSize(size);

        // Cache the new size
        m_size.x = size.x;
        m_size.y = size.y;

        // Notify the derived class
        onResize();
    }
}


////////////////////////////////////////////////////////////
void Window::setTitle(const String& title)
{
    if (m_impl)
        m_impl->setTitle(title);
}


////////////////////////////////////////////////////////////
void Window::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (m_impl)
        m_impl->setIcon(width, height, pixels);
}


////////////////////////////////////////////////////////////
void Window::setVisible(bool visible)
{
    if (m_impl)
        m_impl->setVisible(visible);
}


////////////////////////////////////////////////////////////
void Window::setVerticalSyncEnabled(bool enabled)
{
    if (setActive())
        m_context->setVerticalSyncEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::setMouseCursorVisible(bool visible)
{
    if (m_impl)
        m_impl->setMouseCursorVisible(visible);
}


////////////////////////////////////////////////////////////
void Window::setKeyRepeatEnabled(bool enabled)
{
    if (m_impl)
        m_impl->setKeyRepeatEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::setFramerateLimit(unsigned int limit)
{
    if (limit > 0)
        m_frameTimeLimit = seconds(1.f / limit);
    else
        m_frameTimeLimit = Time::Zero;
}


////////////////////////////////////////////////////////////
void Window::setJoystickThreshold(float threshold)
{
    if (m_impl)
        m_impl->setJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
bool Window::setActive(bool active) const
{
    if (m_context)
    {
        if (m_context->setActive(active))
        {
            return true;
        }
        else
        {
            Log() << "Failed to activate the window's context" << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void Window::display()
{
    // Display the backbuffer on screen
    if (setActive())
        m_context->display();

    // Limit the framerate if needed
    if (m_frameTimeLimit != Time::Zero)
    {
        sleep(m_frameTimeLimit - m_clock.getElapsedTime());
        m_clock.restart();
    }
}


////////////////////////////////////////////////////////////
WindowHandle Window::getSystemHandle() const
{
    return m_impl ? m_impl->getSystemHandle() : 0;
}


////////////////////////////////////////////////////////////
void Window::onCreate()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
void Window::onResize()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
bool Window::filterEvent(const Event& event)
{
    // Notify resize events to the derived class
    if (event.type == Event::Resized)
    {
        // Cache the new size
        m_size.x = event.size.width;
        m_size.y = event.size.height;

        // Notify the derived class
        onResize();
    }

    return true;
}


////////////////////////////////////////////////////////////
void Window::initialize()
{
    // Setup default behaviours (to get a consistent behaviour across different implementations)
    setVisible(true);
    setMouseCursorVisible(true);
    setVerticalSyncEnabled(false);
    setKeyRepeatEnabled(true);
    setFramerateLimit(0);

    // Get and cache the initial size of the window
    m_size = m_impl->getSize();

    // Reset frame time
    m_clock.restart();

    // Activate the window
    setActive();

    // Notify the derived class
    onCreate();
}

} // namespace TGE
