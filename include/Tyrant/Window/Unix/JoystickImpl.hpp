/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_JOYSTICKIMPLLINUX_HPP
#define TGE_JOYSTICKIMPLLINUX_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <linux/joystick.h>
#include <fcntl.h>
#include <string>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Linux implementation of joysticks
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
    /// Get the joystick name
    ///
    /// \param index Index of the joystick
    ///
    /// \return Joystick name
    ///
    ////////////////////////////////////////////////////////////
    std::string getJoystickName(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// Get a system attribute from udev as an unsigned int
    ///
    /// \param index Index of the joystick
    /// \param attributeName Name of the attribute to retrieve
    ///
    /// \return Attribute value as unsigned int
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getUdevAttributeUint(unsigned int index, std::string attributeName);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int           m_file;                          ///< File descriptor of the joystick
    char          m_mapping[ABS_MAX + 1];          ///< Axes mapping (index to axis id)
    JoystickState m_state;                         ///< Current state of the joystick
    TGE::Joystick::Identification m_identification; ///< Identification of the joystick
};

} // namespace priv

} // namespace TGE


#endif // TGE_JOYSTICKIMPLLINUX_HPP
