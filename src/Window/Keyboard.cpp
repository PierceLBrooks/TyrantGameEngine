/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Keyboard.hpp>
#include <Tyrant/Window/InputImpl.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
bool Keyboard::isKeyPressed(Key key)
{
    return priv::InputImpl::isKeyPressed(key);
}

} // namespace TGE
