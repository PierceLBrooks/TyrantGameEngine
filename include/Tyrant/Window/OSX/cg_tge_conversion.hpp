/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_CG_TGE_CONVERSION_HPP
#define TGE_CG_TGE_CONVERSION_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoMode.hpp>
#include <ApplicationServices/ApplicationServices.h>

namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Get bpp of a video mode for OS 10.6 or later
///
/// With OS 10.6 and later, Quartz doesn't use dictionaries any more
/// to represent video mode. Instead it uses a CGDisplayMode opaque type.
///
////////////////////////////////////////////////////////////
size_t modeBitsPerPixel(CGDisplayModeRef mode);

////////////////////////////////////////////////////////////
/// \brief Get bpp for all OS X version
///
/// This function use only non-deprecated way to get the
/// display bits per pixel information for a given display id.
///
////////////////////////////////////////////////////////////
size_t displayBitsPerPixel(CGDirectDisplayID displayId);

////////////////////////////////////////////////////////////
/// \brief Convert a Quartz video mode into a TGE::VideoMode object
///
////////////////////////////////////////////////////////////
VideoMode convertCGModeToTGEMode(CGDisplayModeRef cgmode);

////////////////////////////////////////////////////////////
/// \brief Convert a TGE::VideoMode object into a Quartz video mode
///
////////////////////////////////////////////////////////////
CGDisplayModeRef convertTGEModeToCGMode(VideoMode tgemode);

} // namespace priv
} // namespace TGE

#endif
