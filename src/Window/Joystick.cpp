/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Joystick.hpp>
#include <Tyrant/Window/JoystickManager.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
bool Joystick::isConnected(unsigned int joystick)
{
    return priv::JoystickManager::getInstance().getState(joystick).connected;
}


////////////////////////////////////////////////////////////
unsigned int Joystick::getButtonCount(unsigned int joystick)
{
    return priv::JoystickManager::getInstance().getCapabilities(joystick).buttonCount;
}


////////////////////////////////////////////////////////////
bool Joystick::hasAxis(unsigned int joystick, Axis axis)
{
    return priv::JoystickManager::getInstance().getCapabilities(joystick).axes[axis];
}


////////////////////////////////////////////////////////////
bool Joystick::isButtonPressed(unsigned int joystick, unsigned int button)
{
    return priv::JoystickManager::getInstance().getState(joystick).buttons[button];
}


////////////////////////////////////////////////////////////
float Joystick::getAxisPosition(unsigned int joystick, Axis axis)
{
    return priv::JoystickManager::getInstance().getState(joystick).axes[axis];
}


////////////////////////////////////////////////////////////
Joystick::Identification Joystick::getIdentification(unsigned int joystick)
{
    return priv::JoystickManager::getInstance().getIdentification(joystick);
}


////////////////////////////////////////////////////////////
void Joystick::update()
{
    return priv::JoystickManager::getInstance().update();
}


////////////////////////////////////////////////////////////
Joystick::Identification::Identification() :
name     ("No Joystick"),
vendorId (0),
productId(0)
{

}

} // namespace TGE
