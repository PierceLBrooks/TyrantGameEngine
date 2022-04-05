/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_VIDEOMODEIMPL_HPP
#define TGE_VIDEOMODEIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoMode.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OS-specific implementation of video modes functions
///
////////////////////////////////////////////////////////////
class VideoModeImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the list of all the supported fullscreen video modes
    ///
    /// \return Array filled with the fullscreen video modes
    ///
    ////////////////////////////////////////////////////////////
    static std::vector<VideoMode> getFullscreenModes();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current desktop video mode
    ///
    /// \return Current desktop video mode
    ///
    ////////////////////////////////////////////////////////////
    static VideoMode getDesktopMode();
};

} // namespace priv

} // namespace TGE


#endif // TGE_VIDEOMODEIMPL_HPP
