/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <Tyrant/Window/OSX/WindowImplDelegateProtocol.h>

////////////////////////////////////////////////////////////
/// Predefine some classes
////////////////////////////////////////////////////////////
@class TGEOpenGLView;

////////////////////////////////////////////////////////////
/// \brief Implementation of WindowImplDelegateProtocol for view management
///
////////////////////////////////////////////////////////////

@interface TGEViewController : NSObject <WindowImplDelegateProtocol>
{
    NSView*                     m_view;         ///< Underlying Cocoa view
    TGEOpenGLView*               m_oglView;      ///< OpenGL view
    TGE::priv::WindowImplCocoa*  m_requester;    ///< View's requester
}

////////////////////////////////////////////////////////////
/// \brief Initialize the view controller
///
/// \param view view to be controlled
///
/// \return an initialized view controller
///
////////////////////////////////////////////////////////////
-(id)initWithView:(NSView*)view;

@end
