/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_TGECONTEXT_HPP
#define TGE_TGECONTEXT_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/GlContext.hpp>

////////////////////////////////////////////////////////////
/// Predefine OBJC classes
////////////////////////////////////////////////////////////
#ifdef __OBJC__

@class NSOpenGLContext;
typedef NSOpenGLContext* NSOpenGLContextRef;

@class NSOpenGLView;
typedef NSOpenGLView* NSOpenGLViewRef;

@class NSWindow;
typedef NSWindow* NSWindowRef;

#else // If C++

typedef void* NSOpenGLContextRef;
typedef void* NSOpenGLViewRef;
typedef void* NSWindowRef;

#endif


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OSX (Cocoa) implementation of OpenGL contexts
///
////////////////////////////////////////////////////////////
class TGEContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a new context, not associated to a window
    ///
    /// \param shared Context to share the new one with (can be NULL)
    ///
    ////////////////////////////////////////////////////////////
    TGEContext(TGEContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context attached to a window
    ///
    /// \param shared       Context to share the new one with
    /// \param settings     Creation parameters
    /// \param owner        Pointer to the owner window
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    TGEContext(TGEContext* shared, const ContextSettings& settings,
              const WindowImpl* owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context that embeds its own rendering target
    ///
    /// \param shared   Context to share the new one with
    /// \param settings Creation parameters
    /// \param width    Back buffer width, in pixels
    /// \param height   Back buffer height, in pixels
    ///
    ////////////////////////////////////////////////////////////
    TGEContext(TGEContext* shared, const ContextSettings& settings,
              unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~TGEContext();

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

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Activate the context as the current target
    ///        for rendering
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool makeCurrent();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Create the context
    /// \note Must only be called from Ctor.
    ///
    /// \param shared       Context to share the new one with (can be NULL)
    /// \param bitsPerPixel bpp
    /// \param settings     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    void createContext(TGEContext* shared,
                       unsigned int bitsPerPixel,
                       const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    NSOpenGLContextRef    m_context;       ///< OpenGL context.
    NSOpenGLViewRef       m_view;          ///< Only for offscreen context.
    NSWindowRef           m_window;        ///< Only for offscreen context.
};

} // namespace priv

} // namespace TGE

#endif // TGE_TGECONTEXT_HPP
