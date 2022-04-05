/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoMode.hpp>

#import <Tyrant/Window/OSX/WindowImplDelegateProtocol.h>

////////////////////////////////////////////////////////////
/// Predefine some classes
////////////////////////////////////////////////////////////
namespace TGE {
    namespace priv {
        class WindowImplCocoa;
    }
}

@class TGEOpenGLView;

////////////////////////////////////////////////////////////
/// \brief Implementation of WindowImplDelegateProtocol for window management
///
/// Key and mouse events are delegated to its view.
/// Window events are managed by this class.
///
/// Used when TGE handle everything and when a NSWindow* is given
/// as handle to WindowImpl.
///
/// m_fullscreenMode is bind to default video mode if we don't need to change screen size.
///
////////////////////////////////////////////////////////////
@interface TGEWindowController : NSResponder <WindowImplDelegateProtocol, NSWindowDelegate>
{
    NSWindow*                   m_window;           ///< Underlying Cocoa window to be controlled
    TGEOpenGLView*               m_oglView;          ///< OpenGL view for rendering
    TGE::priv::WindowImplCocoa*  m_requester;        ///< Requester
    BOOL                        m_fullscreen;       ///< Indicate whether the window is fullscreen or not
}

////////////////////////////////////////////////////////////
/// \brief Create the TGE window with an external Cocoa window
///
/// \param window Cocoa window to be controlled
///
/// \return an initialized controller
///
////////////////////////////////////////////////////////////
-(id)initWithWindow:(NSWindow*)window;

////////////////////////////////////////////////////////////
/// \brief Create the TGE window "from scratch" (TGE handle everything)
///
/// \param mode Video mode
/// \param style Window's style, as described by TGE::Style
///
/// \return an initialized controller
///
////////////////////////////////////////////////////////////
-(id)initWithMode:(const TGE::VideoMode&)mode andStyle:(unsigned long)style;

@end
