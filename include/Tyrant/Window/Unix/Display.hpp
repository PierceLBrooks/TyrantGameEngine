/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_SHAREDDISPLAY_HPP
#define TGE_SHAREDDISPLAY_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <X11/Xlib.h>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Get the shared Display
///
/// This function increments the reference count of the display,
/// it must be matched with a call to CloseDisplay.
///
/// \return Pointer to the shared display
///
////////////////////////////////////////////////////////////
Display* OpenDisplay();

////////////////////////////////////////////////////////////
/// \brief Release a reference to the shared
///
/// \param display Display to release
///
////////////////////////////////////////////////////////////
void CloseDisplay(Display* display);

} // namespace priv

} // namespace TGE


#endif // TGE_SHAREDDISPLAY_HPP
