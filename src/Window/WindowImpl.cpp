/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/WindowImpl.hpp>
#include <Tyrant/Window/Event.hpp>
#include <Tyrant/Window/JoystickManager.hpp>
#include <Tyrant/System/Sleep.hpp>
#include <algorithm>
#include <cmath>

#if defined(OS_WINDOWS)

    #include <Tyrant/Window/Win32/WindowImplWin32.hpp>
    typedef TGE::priv::WindowImplWin32 WindowImplType;

#elif defined(OS_LINUX)

    #include <Tyrant/Window/Unix/WindowImplX11.hpp>
    typedef TGE::priv::WindowImplX11 WindowImplType;

#elif defined(OS_MAC)

    #include <Tyrant/Window/OSX/WindowImplCocoa.hpp>
    typedef TGE::priv::WindowImplCocoa WindowImplType;

#endif


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::create(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
{
    return new WindowImplType(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::create(WindowHandle handle)
{
    return new WindowImplType(handle);
}


////////////////////////////////////////////////////////////
WindowImpl::WindowImpl() :
m_joystickThreshold(0.1f)
{
    // Get the initial joystick states
    JoystickManager::getInstance().update();
    for (unsigned int i = 0; i < Joystick::Count; ++i)
        m_joystickStates[i] = JoystickManager::getInstance().getState(i);
}


////////////////////////////////////////////////////////////
WindowImpl::~WindowImpl()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void WindowImpl::setJoystickThreshold(float threshold)
{
    m_joystickThreshold = threshold;
}


////////////////////////////////////////////////////////////
bool WindowImpl::popEvent(Event& event, bool block)
{
    // If the event queue is empty, let's first check if new events are available from the OS
    if (m_events.empty())
    {
        // Get events from the system
        processJoystickEvents();
        processEvents();

        // In blocking mode, we must process events until one is triggered
        if (block)
        {
            // Here we use a manual wait loop instead of the optimized
            // wait-event provided by the OS, so that we don't skip joystick
            // events (which require polling)
            while (m_events.empty())
            {
                sleep(milliseconds(10));
                processJoystickEvents();
                processEvents();
            }
        }
    }

    // Pop the first event of the queue, if it is not empty
    if (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void WindowImpl::pushEvent(const Event& event)
{
    m_events.push(event);
}


////////////////////////////////////////////////////////////
void WindowImpl::processJoystickEvents()
{
    // First update the global joystick states
    JoystickManager::getInstance().update();

    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState previousState = m_joystickStates[i];
        m_joystickStates[i] = JoystickManager::getInstance().getState(i);
        JoystickCaps caps = JoystickManager::getInstance().getCapabilities(i);

        // Connection state
        bool connected = m_joystickStates[i].connected;
        if (previousState.connected ^ connected)
        {
            Event event;
            event.type = connected ? Event::JoystickConnected : Event::JoystickDisconnected;
            event.joystickButton.joystickId = i;
            pushEvent(event);
        }

        if (connected)
        {
            // Axes
            for (unsigned int j = 0; j < Joystick::AxisCount; ++j)
            {
                if (caps.axes[j])
                {
                    Joystick::Axis axis = static_cast<Joystick::Axis>(j);
                    float prevPos = previousState.axes[axis];
                    float currPos = m_joystickStates[i].axes[axis];
                    if (fabs(currPos - prevPos) >= m_joystickThreshold)
                    {
                        Event event;
                        event.type = Event::JoystickMoved;
                        event.joystickMove.joystickId = i;
                        event.joystickMove.axis = axis;
                        event.joystickMove.position = currPos;
                        pushEvent(event);
                    }
                }
            }

            // Buttons
            for (unsigned int j = 0; j < caps.buttonCount; ++j)
            {
                bool prevPressed = previousState.buttons[j];
                bool currPressed = m_joystickStates[i].buttons[j];

                if (prevPressed ^ currPressed)
                {
                    Event event;
                    event.type = currPressed ? Event::JoystickButtonPressed : Event::JoystickButtonReleased;
                    event.joystickButton.joystickId = i;
                    event.joystickButton.button = j;
                    pushEvent(event);
                }
            }
        }
    }
}

} // namespace priv

} // namespace TGE
