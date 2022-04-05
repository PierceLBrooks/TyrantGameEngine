/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Mouse.hpp>
#include <Tyrant/Window/InputImpl.hpp>
#include <Tyrant/Window/Window.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
bool Mouse::isButtonPressed(Button button)
{
    return priv::InputImpl::isMouseButtonPressed(button);
}


////////////////////////////////////////////////////////////
Vector2i Mouse::getPosition()
{
    return priv::InputImpl::getMousePosition();
}


////////////////////////////////////////////////////////////
Vector2i Mouse::getPosition(const Window& relativeTo)
{
    return priv::InputImpl::getMousePosition(relativeTo);
}


////////////////////////////////////////////////////////////
void Mouse::setPosition(const Vector2i& position)
{
    priv::InputImpl::setMousePosition(position);
}


////////////////////////////////////////////////////////////
void Mouse::setPosition(const Vector2i& position, const Window& relativeTo)
{
    priv::InputImpl::setMousePosition(position, relativeTo);
}

} // namespace TGE
