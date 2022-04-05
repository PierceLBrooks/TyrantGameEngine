/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_JOYSTICKIMPL_HPP
#define TGE_JOYSTICKIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Window/Joystick.hpp>
#include <Tyrant/System/String.hpp>
#include <algorithm>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Structure holding a joystick's information
///
////////////////////////////////////////////////////////////
struct JoystickCaps
{
    JoystickCaps()
    {
        buttonCount = 0;
        std::fill(axes, axes + Joystick::AxisCount, false);
    }

    unsigned int buttonCount;               ///< Number of buttons supported by the joystick
    bool         axes[Joystick::AxisCount]; ///< Support for each axis
};


////////////////////////////////////////////////////////////
/// \brief Structure holding a joystick's state
///
////////////////////////////////////////////////////////////
struct JoystickState
{
    JoystickState()
    {
        connected = false;
        std::fill(axes, axes + Joystick::AxisCount, 0.f);
        std::fill(buttons, buttons + Joystick::ButtonCount, false);
    }

    bool  connected;                      ///< Is the joystick currently connected?
    float axes[Joystick::AxisCount];      ///< Position of each axis, in range [-100, 100]
    bool  buttons[Joystick::ButtonCount]; ///< Status of each button (true = pressed)
};

} // namespace priv

} // namespace TGE


#if defined(OS_WINDOWS)

    #include <Tyrant/Window/Win32/JoystickImpl.hpp>

#elif defined(OS_LINUX)

    #include <Tyrant/Window/Unix/JoystickImpl.hpp>

#elif defined(OS_MAC)

    #include <Tyrant/Window/OSX/JoystickImpl.hpp>

#endif


#endif // TGE_JOYSTICKIMPL_HPP
