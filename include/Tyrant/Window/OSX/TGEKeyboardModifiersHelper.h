/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <AppKit/AppKit.h>

namespace TGE {
    namespace priv {
        class WindowImplCocoa;
    }
}

////////////////////////////////////////////////////////////
/// Keyboard Modifiers Helper
///
/// Handle left & right modifiers (cmd, ctrl, alt, shift)
/// events and send them back to the requester.
///
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/// \brief Initialize the global state (only if needed)
///
/// It needs to be called before any event, e.g. in the window constructor.
///
////////////////////////////////////////////////////////////
void initialiseKeyboardHelper(void);


////////////////////////////////////////////////////////////
/// \brief Set up a TGE key event based on the given modifiers flags and key code
///
////////////////////////////////////////////////////////////
TGE::Event::KeyEvent keyEventWithModifiers(NSUInteger modifiers, TGE::Keyboard::Key key);


////////////////////////////////////////////////////////////
/// \brief Handle the state of modifiers keys
///
/// Send key released & pressed events to the requester.
///
////////////////////////////////////////////////////////////
void handleModifiersChanged(NSUInteger modifiers, TGE::priv::WindowImplCocoa& requester);


