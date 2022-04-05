/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_INPUTMAP_HPP
#define TGE_INPUTMAP_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window.hpp>
#include <Tyrant/Config.hpp>
#include <map>
#include <functional>

namespace TGE
{
    struct MouseScrollEvent
    {
        MouseScrollEvent() : delta(0), x(0), y(0), triggered([=](){}) {}
        int delta;
        int x;
        int y;
        void trigger(int newDelta, int newX, int newY);
        std::function<void()> triggered;
    };

    class TGE_API InputMap
    {
        public:
            template<typename functionOnClick, typename functionOnRelease>
            void bindAction(Keyboard::Key hotkey, functionOnClick onClick, functionOnRelease onRelease)
            {
                keyPressedMap[hotkey] = onClick;
                keyReleasedMap[hotkey] = onRelease;
            }

            template<typename functionOnClick, typename functionOnRelease>
            void bindAction(Mouse::Button button, functionOnClick onClick, functionOnRelease onRelease)
            {
                mousePressedMap[button] = onClick;
                mouseReleasedMap[button] = onRelease;
            }

            template<typename functionOnPosScroll, typename functionOnNegScroll>
            void bindScrollAction(functionOnPosScroll functionPos, functionOnNegScroll functionNeg)
            {
                mouseWheelEvent.triggered = ([=](){if(mouseWheelEvent.delta > 0) functionPos(); else if(mouseWheelEvent.delta < 0) functionNeg();});
            }

            void triggerAction(Keyboard::Key hotkey);
            void triggerActionReleased(Keyboard::Key hotkey);
            void triggerAction(Mouse::Button button);
            void triggerActionReleased(Mouse::Button button);
            void triggerAction(int delta, int x, int y);

            std::map <Keyboard::Key, std::function<void()>> keyPressedMap;
            std::map <Keyboard::Key, std::function<void()>> keyReleasedMap;
            std::map <Mouse::Button, std::function<void()>> mousePressedMap;
            std::map <Mouse::Button, std::function<void()>> mouseReleasedMap;
            MouseScrollEvent mouseWheelEvent;
    };
}

#endif // TGE_INPUTMAP_HPP
