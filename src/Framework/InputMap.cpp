/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Framework/InputMap.hpp>

namespace TGE
{
    void MouseScrollEvent::trigger(int newDelta, int newX, int newY)
    {
        delta = newDelta;
        x = newX;
        y = newY;
        triggered();
    }

    void InputMap::triggerAction(Keyboard::Key hotkey)
    {
        if(keyPressedMap.find(hotkey) != keyPressedMap.end())
            keyPressedMap[hotkey]();
    }

    void InputMap::triggerActionReleased(Keyboard::Key hotkey)
    {
        if(keyReleasedMap.find(hotkey) != keyReleasedMap.end())
            keyReleasedMap[hotkey]();
    }

    void InputMap::triggerAction(Mouse::Button button)
    {
        if(mousePressedMap.find(button) != mousePressedMap.end())
            mousePressedMap[button]();
    }

    void InputMap::triggerActionReleased(Mouse::Button button)
    {
        if(mouseReleasedMap.find(button) != mouseReleasedMap.end())
            mouseReleasedMap[button]();
    }

    void InputMap::triggerAction(int delta, int x, int y)
    {
        mouseWheelEvent.trigger(delta, x, y);
    }
}
