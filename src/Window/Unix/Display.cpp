/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/Unix/Display.hpp>
#include <cassert>


namespace
{
    // The shared display and its reference counter
    Display* sharedDisplay = NULL;
    unsigned int referenceCount = 0;
}

namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
Display* OpenDisplay()
{
    if (referenceCount == 0)
        sharedDisplay = XOpenDisplay(NULL);
    referenceCount++;
    return sharedDisplay;
}


////////////////////////////////////////////////////////////
void CloseDisplay(Display* display)
{
    assert(display == sharedDisplay);

    referenceCount--;
    if (referenceCount == 0)
        XCloseDisplay(display);
}

} // namespace priv

} // namespace TGE
