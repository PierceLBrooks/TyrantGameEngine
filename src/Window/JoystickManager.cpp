/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/JoystickManager.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
JoystickManager& JoystickManager::getInstance()
{
    static JoystickManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
const JoystickCaps& JoystickManager::getCapabilities(unsigned int joystick) const
{
    return m_joysticks[joystick].capabilities;
}


////////////////////////////////////////////////////////////
const JoystickState& JoystickManager::getState(unsigned int joystick) const
{
    return m_joysticks[joystick].state;
}


////////////////////////////////////////////////////////////
const Joystick::Identification& JoystickManager::getIdentification(unsigned int joystick) const
{
    return m_joysticks[joystick].identification;
}


////////////////////////////////////////////////////////////
void JoystickManager::update()
{
    for (int i = 0; i < Joystick::Count; ++i)
    {
        Item& item = m_joysticks[i];

        if (item.state.connected)
        {
            // Get the current state of the joystick
            item.state = item.joystick.update();

            // Check if it's still connected
            if (!item.state.connected)
            {
                item.joystick.close();
                item.capabilities = JoystickCaps();
                item.state = JoystickState();
                item.identification = Joystick::Identification();
            }
        }
        else
        {
            // Check if the joystick was connected since last update
            if (JoystickImpl::isConnected(i))
            {
                if (item.joystick.open(i))
                {
                    item.capabilities = item.joystick.getCapabilities();
                    item.state = item.joystick.update();
                    item.identification = item.joystick.getIdentification();
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////
JoystickManager::JoystickManager()
{
    JoystickImpl::initialize();
}


////////////////////////////////////////////////////////////
JoystickManager::~JoystickManager()
{
    for (int i = 0; i < Joystick::Count; ++i)
    {
        if (m_joysticks[i].state.connected)
            m_joysticks[i].joystick.close();
    }

    JoystickImpl::cleanup();
}

} // namespace priv

} // namespace TGE
