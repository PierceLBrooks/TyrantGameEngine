/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/OSX/cg_tge_conversion.hpp>
#include <Tyrant/System/Log.hpp>

namespace TGE
{
namespace priv
{

////////////////////////////////////////////////////////////
size_t modeBitsPerPixel(CGDisplayModeRef mode)
{
    size_t bpp = 0; // no match

    // Compare encoding.
    CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
    if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        bpp = 32;
    else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        bpp = 16;
    else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        bpp = 8;

    // Clean up memory.
    CFRelease(pixEnc);

    return bpp;
}


////////////////////////////////////////////////////////////
size_t displayBitsPerPixel(CGDirectDisplayID displayId)
{
    // Get the display mode.
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayId);

    // Get bpp for the mode.
    const size_t bpp = modeBitsPerPixel(mode);

    // Clean up Memory.
    CGDisplayModeRelease(mode);

    return bpp;
}


////////////////////////////////////////////////////////////
VideoMode convertCGModeToTGEMode(CGDisplayModeRef cgmode)
{
    return VideoMode(CGDisplayModeGetWidth(cgmode),
                     CGDisplayModeGetHeight(cgmode),
                     modeBitsPerPixel(cgmode));
}


////////////////////////////////////////////////////////////
CGDisplayModeRef convertTGEModeToCGMode(VideoMode tgemode)
{
    // Starting with 10.6 we should query the display all the modes and
    // search for the best one.

    // Will return NULL if tgemode is not in VideoMode::GetFullscreenModes.
    CGDisplayModeRef cgbestMode = NULL;

    // Retrieve all modes available for main screen only.
    CFArrayRef cgmodes = CGDisplayCopyAllDisplayModes(CGMainDisplayID(), NULL);

    if (cgmodes == NULL) // Should not happen but anyway...
    {
        TGE::Log() << "Couldn't get VideoMode for main display.";
        return NULL;
    }

    // Loop on each mode and convert it into a TGE::VideoMode object.
    const CFIndex modesCount = CFArrayGetCount(cgmodes);
    for (CFIndex i = 0; i < modesCount; i++)
    {
        CGDisplayModeRef cgmode = (CGDisplayModeRef)CFArrayGetValueAtIndex(cgmodes, i);

        VideoMode mode = convertCGModeToTGEMode(cgmode);

        if (mode == tgemode)
            cgbestMode = cgmode;
    }

    // Clean up memory.
    CFRelease(cgmodes);

    if (cgbestMode == NULL)
        TGE::Log() << "Couldn't convert the given TGE::VideoMode into a CGDisplayMode."
                  << std::endl;

    return cgbestMode;
}

} // namespace priv
} // namespace TGE
