/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_WINDOWHANDLE_HPP
#define TGE_WINDOWHANDLE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>

// Windows' HWND is a typedef on struct HWND__*
#if defined(OS_WINDOWS)
    struct HWND__;
#endif

namespace TGE
{
////////////////////////////////////////////////////////////
/// Define a low-level window handle type, specific to
/// each platform
////////////////////////////////////////////////////////////
#if defined(OS_WINDOWS)

    // Window handle is HWND (HWND__*) on Windows
    typedef HWND__* WindowHandle;

#elif defined(OS_LINUX)

    // Window handle is Window (unsigned long) on Unix - X11
    typedef unsigned long WindowHandle;

#elif defined(OS_MAC)

    // Window handle is NSWindow (void*) on Mac OS X - Cocoa
    typedef void* WindowHandle;

#endif

} // namespace TGE


#endif // TGE_WINDOWHANDLE_HPP
