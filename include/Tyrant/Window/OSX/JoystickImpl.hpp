/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_JOYSTICKIMPLOSX_HPP
#define TGE_JOYSTICKIMPLOSX_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/JoystickImpl.hpp>
#include <Tyrant/System/String.hpp>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDDevice.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <map>
#include <vector>

namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Mac OS X implementation of joysticks
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
    /// Get HID device property key as a string
    ///
    /// \param ref HID device
    /// \param prop Property to retrieve from the device
    ///
    /// \return Value of the property
    ///
    ////////////////////////////////////////////////////////////
    std::string getDeviceString(IOHIDDeviceRef ref, CFStringRef prop);

    ////////////////////////////////////////////////////////////
    /// Get HID device property key as an unsigned int
    ///
    /// \param ref HID device
    /// \param prop Property to retrieve from the device
    ///
    /// \return Value of the property
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getDeviceUint(IOHIDDeviceRef ref, CFStringRef prop);

    ////////////////////////////////////////////////////////////
    /// Convert a CFStringRef to std::string
    ///
    /// \param cfString CFStringRef to convert
    ///
    /// \return std::string
    ///
    ////////////////////////////////////////////////////////////
    std::string stringFromCFString(CFStringRef cfString);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    typedef long                                          Location;
    typedef std::map<TGE::Joystick::Axis, IOHIDElementRef> AxisMap;
    typedef std::vector<IOHIDElementRef>                  ButtonsVector;

    AxisMap       m_axis;                      ///< Axis (IOHIDElementRef) connected to the joystick
    ButtonsVector m_buttons;                   ///< Buttons (IOHIDElementRef) connected to the joystick
    unsigned int  m_index;                     ///< TGE index
    Joystick::Identification m_identification; ///< Joystick identification

    static Location m_locationIDs[TGE::Joystick::Count]; ///< Global Joystick register
    /// For a corresponding TGE index, m_locationIDs is either some usb
    /// location or 0 if there isn't currently a connected joystick device
};

} // namespace priv

} // namespace TGE


#endif // TGE_JOYSTICKIMPLOSX_HPP
