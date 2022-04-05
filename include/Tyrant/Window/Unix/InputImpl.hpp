/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_INPUTIMPLX11_HPP
#define TGE_INPUTIMPLX11_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Keyboard.hpp>
#include <Tyrant/Window/Mouse.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Linux (X11) implementation of inputs (keyboard + mouse)
///
////////////////////////////////////////////////////////////
class InputImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key Key to check
    ///
    /// \return True if the key is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isKeyPressed(Keyboard::Key key);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button Button to check
    ///
    /// \return True if the button is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isMouseButtonPressed(Mouse::Button button);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of the mouse in desktop coordinates
    ///
    /// This function returns the current position of the mouse
    /// cursor, in global (desktop) coordinates.
    ///
    /// \return Current position of the mouse
    ///
    ////////////////////////////////////////////////////////////
    static Vector2i getMousePosition();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of the mouse in window coordinates
    ///
    /// This function returns the current position of the mouse
    /// cursor, relative to the given window.
    /// If no window is used, it returns desktop coordinates.
    ///
    /// \param relativeTo Reference window
    ///
    /// \return Current position of the mouse
    ///
    ////////////////////////////////////////////////////////////
    static Vector2i getMousePosition(const Window& relativeTo);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current position of the mouse in desktop coordinates
    ///
    /// This function sets the current position of the mouse
    /// cursor in global (desktop) coordinates.
    /// If no window is used, it sets the position in desktop coordinates.
    ///
    /// \param position New position of the mouse
    ///
    ////////////////////////////////////////////////////////////
    static void setMousePosition(const Vector2i& position);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current position of the mouse in window coordinates
    ///
    /// This function sets the current position of the mouse
    /// cursor, relative to the given window.
    /// If no window is used, it sets the position in desktop coordinates.
    ///
    /// \param position New position of the mouse
    /// \param relativeTo Reference window
    ///
    ////////////////////////////////////////////////////////////
    static void setMousePosition(const Vector2i& position, const Window& relativeTo);
};

} // namespace priv

} // namespace TGE


#endif // TGE_INPUTIMPLX11_HPP
