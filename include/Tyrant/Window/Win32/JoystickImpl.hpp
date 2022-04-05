/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_JOYSTICKIMPLWIN32_HPP
#define TGE_JOYSTICKIMPLWIN32_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#ifdef _WIN32_WINDOWS
    #undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
    #undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <Tyrant/Window/Joystick.hpp>
#include <Tyrant/Window/JoystickImpl.hpp>
#include <Tyrant/System/String.hpp>
#include <windows.h>
#include <mmsystem.h>
#include <cmath>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Windows implementation of joysticks
///
////////////////////////////////////////////////////////////
class JoystickImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global initialization of the joystick module
    ///
    ////////////////////////////////////////////////////////////
    static void initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global cleanup of the joystick module
    ///
    ////////////////////////////////////////////////////////////
    static void cleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick is currently connected
    ///
    /// \param index Index of the joystick to check
    ///
    /// \return True if the joystick is connected, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isConnected(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Open the joystick
    ///
    /// \param index Index assigned to the joystick
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool open(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Close the joystick
    ///
    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick capabilities
    ///
    /// \return Joystick capabilities
    ///
    ////////////////////////////////////////////////////////////
    JoystickCaps getCapabilities() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick identification
    ///
    /// \return Joystick identification
    ///
    ////////////////////////////////////////////////////////////
    Joystick::Identification getIdentification() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    JoystickState update();

private :

    ////////////////////////////////////////////////////////////
    /// Get the joystick's name
    ///
    /// \param index Index of the joystick
    /// \param caps JOYCAPS
    ///
    /// \return Joystick name
    ///
    ////////////////////////////////////////////////////////////
    TGE::String getDeviceName(unsigned int index, JOYCAPS caps);

    ////////////////////////////////////////////////////////////
    /// Get a system error string from an error code
    ///
    /// \param errorCode Error code
    ///
    /// \return Error message string
    ///
    ////////////////////////////////////////////////////////////
    TGE::String getErrorString(DWORD errorCode);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int m_index;                      ///< Index of the joystick
    JOYCAPS      m_caps;                       ///< Joystick capabilities
    Joystick::Identification m_identification; ///< Joystick identification
};

} // namespace priv

} // namespace TGE


#endif // TGE_JOYSTICKIMPLWIN32_HPP
