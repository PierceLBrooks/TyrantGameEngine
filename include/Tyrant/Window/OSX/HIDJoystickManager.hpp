/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_HIDJOYSTICKMANAGER_HPP
#define TGE_HIDJOYSTICKMANAGER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/NonCopyable.hpp>
#include <IOKit/hid/IOHIDDevice.h>
#include <IOKit/hid/IOHIDManager.h>

namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief TGE::priv::InputImpl helper
///
/// This class manage as a singleton instance the joysticks.
/// It's only purpose is to help TGE::priv::JoystickImpl class.
///
////////////////////////////////////////////////////////////
class HIDJoystickManager : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the unique instance of the class
    ///
    /// \note Private use only
    ///
    /// \return Reference to the HIDJoystickManager instance
    ///
    ////////////////////////////////////////////////////////////
    static HIDJoystickManager& getInstance();

public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of currently connected joystick
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getJoystickCount();

    ////////////////////////////////////////////////////////////
    /// \brief Copy the devices associated with this HID manager
    ///
    /// \return a retained CFSetRef of IOHIDDeviceRef or NULL
    ///
    ////////////////////////////////////////////////////////////
    CFSetRef copyJoysticks();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    HIDJoystickManager();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~HIDJoystickManager();

    ////////////////////////////////////////////////////////////
    /// \brief Make sure all event have been processed in the run loop
    ///
    ////////////////////////////////////////////////////////////
    void update();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Private "plug-in" callback
    /// \note Only 'context' parameter is used.
    /// \see IOHIDDeviceCallback
    ///
    ////////////////////////////////////////////////////////////
    static void pluggedIn(void* context, IOReturn, void*, IOHIDDeviceRef);

    ////////////////////////////////////////////////////////////
    /// \brief Private "plug-out" callback
    /// \note Only 'context' parameter is used.
    /// \see IOHIDDeviceCallback
    ///
    ////////////////////////////////////////////////////////////
    static void pluggedOut(void* context, IOReturn, void*, IOHIDDeviceRef);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    IOHIDManagerRef m_manager;      ///< HID Manager
    unsigned int    m_joystickCount;///< Number of joysticks currently connected
};


} // namespace priv

} // namespace TGE

#endif
