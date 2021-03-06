/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_WGLCONTEXT_HPP
#define TGE_WGLCONTEXT_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/GlContext.hpp>
#include <Tyrant/OpenGL.hpp>
#include <windows.h>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Windows (WGL) implementation of OpenGL contexts
///
////////////////////////////////////////////////////////////
class WglContext : public GlContext
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Create a new default context
    ///
    /// \param shared Context to share the new one with (can be NULL)
    ///
    ////////////////////////////////////////////////////////////
    WglContext(WglContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context attached to a window
    ///
    /// \param shared       Context to share the new one with
    /// \param settings     Creation parameters
    /// \param owner        Pointer to the owner window
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    WglContext(WglContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context that embeds its own rendering target
    ///
    /// \param shared   Context to share the new one with
    /// \param settings Creation parameters
    /// \param width    Back buffer width, in pixels
    /// \param height   Back buffer height, in pixels
    ///
    ////////////////////////////////////////////////////////////
    WglContext(WglContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~WglContext();

    ////////////////////////////////////////////////////////////
    /// \brief Activate the context as the current target for rendering
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool makeCurrent();

    ////////////////////////////////////////////////////////////
    /// \brief Display what has been rendered to the context so far
    ///
    ////////////////////////////////////////////////////////////
    virtual void display();

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable vertical synchronization
    ///
    /// Activating vertical synchronization will limit the number
    /// of frames displayed to the refresh rate of the monitor.
    /// This can avoid some visual artifacts, and limit the framerate
    /// to a good value (but not constant across different computers).
    ///
    /// \param enabled : True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    virtual void setVerticalSyncEnabled(bool enabled);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Create the context
    ///
    /// \param shared        Context to share the new one with (can be NULL)
    /// \param bitsPerPixel  Pixel depth, in bits per pixel
    /// \param settings      Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    void createContext(WglContext* shared, unsigned int bitsPerPixel, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    HWND  m_window;        ///< Window to which the context is attached
    HDC   m_deviceContext; ///< Device context associated to the context
    HGLRC m_context;       ///< OpenGL context
    bool  m_ownsWindow;    ///< Do we own the target window?
};

} // namespace priv

} // namespace TGE

#endif // TGE_WGLCONTEXT_HPP
