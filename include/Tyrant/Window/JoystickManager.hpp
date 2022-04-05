/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_JOYSTICKMANAGER_HPP
#define TGE_JOYSTICKMANAGER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Joystick.hpp>
#include <Tyrant/Window/JoystickImpl.hpp>
#include <Tyrant/System/NonCopyable.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Global joystick manager
///
////////////////////////////////////////////////////////////
class JoystickManager : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the global unique instance of the manager
    ///
    /// \return Unique instance of the joystick manager
    ///
    ////////////////////////////////////////////////////////////
    static JoystickManager& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief Get the capabilities for an open joystick
    ///
    /// \param joystick Index of the joystick
    ///
    /// \return Capabilities of the joystick
    ///
    ////////////////////////////////////////////////////////////
    const JoystickCaps& getCapabilities(unsigned int joystick) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current state of an open joystick
    ///
    /// \param joystick Index of the joystick
    ///
    /// \return Current state of the joystick
    ///
    ////////////////////////////////////////////////////////////
    const JoystickState& getState(unsigned int joystick) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the identification for an open joystick
    ///
    /// \param joystick Index of the joystick
    ///
    /// \return Identification for the joystick
    ///
    ////////////////////////////////////////////////////////////
    const Joystick::Identification& getIdentification(unsigned int joystick) const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the state of all the joysticks
    ///
    ////////////////////////////////////////////////////////////
    void update();

private:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    JoystickManager();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~JoystickManager();

    ////////////////////////////////////////////////////////////
    /// \brief Joystick information and state
    ///
    ////////////////////////////////////////////////////////////
    struct Item
    {
        JoystickImpl  joystick;                  ///< Joystick implementation
        JoystickState state;                     ///< The current joystick state
        JoystickCaps  capabilities;              ///< The joystick capabilities
        Joystick::Identification identification; ///< The joystick identification
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Item m_joysticks[Joystick::Count]; ///< Joysticks information and state
};

} // namespace priv

} // namespace TGE


#endif // TGE_JOYSTICKMANAGER_HPP
