/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_WINDOWSTYLE_HPP
#define TGE_WINDOWSTYLE_HPP


namespace TGE
{
namespace Style
{
    ////////////////////////////////////////////////////////////
    /// \ingroup window
    /// \brief Enumeration of the window styles
    ///
    ////////////////////////////////////////////////////////////
    enum
    {
        None       = 0,      ///< No border / title bar (this flag and all others are mutually exclusive)
        Titlebar   = 1 << 0, ///< Title bar + fixed border
        Resize     = 1 << 1, ///< Title bar + resizable border + maximize button
        Close      = 1 << 2, ///< Title bar + close button
        Fullscreen = 1 << 3, ///< Fullscreen mode (this flag and all others are mutually exclusive)

        Default = Titlebar | Resize | Close ///< Default window style
    };
}

} // namespace TGE


#endif // TGE_WINDOWSTYLE_HPP
