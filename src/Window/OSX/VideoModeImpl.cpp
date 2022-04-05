/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoModeImpl.hpp>
#include <Tyrant/Window/OSX/cg_tge_conversion.hpp>
#include <Tyrant/System/Log.hpp>
#include <algorithm>

namespace TGE
{
namespace priv
{

////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    std::vector<VideoMode> modes;

    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);

    if (cgmodes == NULL)
    {
        TGE::Log() << "Couldn't get VideoMode for main display." << std::endl;
        return modes;
    }

    // Loop on each mode and convert it into a TGE::VideoMode object.
    const CFIndex modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++)
    {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);

        VideoMode mode = convertCGModeToTGEMode(cgmode);

        // If not yet listed we add it to our modes array.
        if (std::find(modes.begin(), modes.end(), mode) == modes.end())
            modes.push_back(mode);
    }

    // Clean up memory.
    CFRelease(cgmodes);

    return modes;
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    CGDirectDisplayID display = CGMainDisplayID();
    return VideoMode(CGDisplayPixelsWide(display),
                     CGDisplayPixelsHigh(display),
                     displayBitsPerPixel(display));
}

} // namespace priv

} // namespace TGE
