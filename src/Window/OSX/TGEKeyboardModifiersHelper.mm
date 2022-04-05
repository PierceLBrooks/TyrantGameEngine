/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/OSX/WindowImplCocoa.hpp>

#import <Tyrant/Window/OSX/TGEKeyboardModifiersHelper.h>


////////////////////////////////////////////////////////////
/// Here are define the mask value for the 'modifiers'
/// keys (cmd, ctrl, alt, shift)
///
////////////////////////////////////////////////////////////
#define NSRightShiftKeyMask         0x020004
#define NSLeftShiftKeyMask          0x020002
#define NSRightCommandKeyMask       0x100010
#define NSLeftCommandKeyMask        0x100008
#define NSRightAlternateKeyMask     0x080040
#define NSLeftAlternateKeyMask      0x080020
#define NSRightControlKeyMask       0x042000
#define NSLeftControlKeyMask        0x040001


////////////////////////////////////////////////////////////
// Local Data Structures
////////////////////////////////////////////////////////////

/// Modifiers states
struct ModifiersState
{
    BOOL rightShiftWasDown;
    BOOL leftShiftWasDown;
    BOOL rightCommandWasDown;
    BOOL leftCommandWasDown;
    BOOL rightAlternateWasDown;
    BOOL leftAlternateWasDown;
    BOOL leftControlWasDown;
    BOOL rightControlWasDown;
};


////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////

/// Share 'modifiers' state with all windows to correctly fire pressed/released events
static ModifiersState state;
static BOOL isStateInitialized = NO;


////////////////////////////////////////////////////////////
// Local & Private Functions
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// \brief Carefully observe if the key mask is on in the modifiers
///
////////////////////////////////////////////////////////////
BOOL isKeyMaskActive(NSUInteger modifiers, NSUInteger mask);


////////////////////////////////////////////////////////////
/// \brief Handle one modifier key mask
///
/// Update the key state and send events to the requester
///
////////////////////////////////////////////////////////////
void processOneModifier(NSUInteger modifiers, NSUInteger mask,
                        BOOL& wasDown, TGE::Keyboard::Key key,
                        TGE::priv::WindowImplCocoa& requester);


////////////////////////////////////////////////////////////
/// \brief Handle left & right modifier keys
///
/// Update the keys state and send events to the requester
///
////////////////////////////////////////////////////////////
void processLeftRightModifiers(NSUInteger modifiers,
                               NSUInteger leftMask, NSUInteger rightMask,
                               BOOL& leftWasDown, BOOL& rightWasDown,
                               TGE::Keyboard::Key leftKey, TGE::Keyboard::Key rightKey,
                               TGE::priv::WindowImplCocoa& requester);



////////////////////////////////////////////////////////////
// Implementations
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
void initialiseKeyboardHelper(void)
{
    if (isStateInitialized) return;

    NSUInteger modifiers = [[NSApp currentEvent] modifierFlags];

    // Load current keyboard state
    state.leftShiftWasDown        = isKeyMaskActive(modifiers, NSLeftShiftKeyMask);
    state.rightShiftWasDown       = isKeyMaskActive(modifiers, NSRightShiftKeyMask);
    state.leftCommandWasDown      = isKeyMaskActive(modifiers, NSLeftCommandKeyMask);
    state.rightCommandWasDown     = isKeyMaskActive(modifiers, NSRightCommandKeyMask);
    state.leftAlternateWasDown    = isKeyMaskActive(modifiers, NSLeftAlternateKeyMask);
    state.rightAlternateWasDown   = isKeyMaskActive(modifiers, NSRightAlternateKeyMask);
    state.leftControlWasDown      = isKeyMaskActive(modifiers, NSLeftControlKeyMask);
    state.rightControlWasDown     = isKeyMaskActive(modifiers, NSRightControlKeyMask);

    isStateInitialized = YES;
}


////////////////////////////////////////////////////////
TGE::Event::KeyEvent keyEventWithModifiers(NSUInteger modifiers, TGE::Keyboard::Key key)
{
    TGE::Event::KeyEvent event;
    event.code    = key;
    event.alt     = modifiers & NSAlternateKeyMask;
    event.control = modifiers & NSControlKeyMask;
    event.shift   = modifiers & NSShiftKeyMask;
    event.system  = modifiers & NSCommandKeyMask;

    return event;
}


////////////////////////////////////////////////////////
void handleModifiersChanged(NSUInteger modifiers, TGE::priv::WindowImplCocoa& requester)
{
    // Handle shift
    processLeftRightModifiers(
        modifiers,
        NSLeftShiftKeyMask, NSRightShiftKeyMask,
        state.leftShiftWasDown, state.rightShiftWasDown,
        TGE::Keyboard::LShift, TGE::Keyboard::RShift,
        requester
    );

    // Handle command
    processLeftRightModifiers(
        modifiers,
        NSLeftCommandKeyMask, NSRightCommandKeyMask,
        state.leftCommandWasDown, state.rightCommandWasDown,
        TGE::Keyboard::LSystem, TGE::Keyboard::RSystem,
        requester
    );

    // Handle option (alt)
    processLeftRightModifiers(
        modifiers,
        NSLeftAlternateKeyMask, NSRightAlternateKeyMask,
        state.leftAlternateWasDown, state.rightAlternateWasDown,
        TGE::Keyboard::LAlt, TGE::Keyboard::RAlt,
        requester
    );

    // Handle control
    processLeftRightModifiers(
        modifiers,
        NSLeftControlKeyMask, NSRightControlKeyMask,
        state.leftControlWasDown, state.rightControlWasDown,
        TGE::Keyboard::LControl, TGE::Keyboard::RControl,
        requester
    );
}


////////////////////////////////////////////////////////
BOOL isKeyMaskActive(NSUInteger modifiers, NSUInteger mask)
{
    // Here we need to make sure it's exactly the mask since some masks
    // share some bits such that the & operation would result in a non zero
    // value without corresponding to the processed key.
    return (modifiers & mask) == mask;
}


////////////////////////////////////////////////////////
void processOneModifier(NSUInteger modifiers, NSUInteger mask,
                        BOOL& wasDown, TGE::Keyboard::Key key,
                        TGE::priv::WindowImplCocoa& requester)
{
    // Setup a potential event key.
    TGE::Event::KeyEvent event = keyEventWithModifiers(modifiers, key);

    // State
    BOOL isDown = isKeyMaskActive(modifiers, mask);

    // Check for key pressed event
    if (isDown && !wasDown)
        requester.keyDown(event);

    // And check for key released event
    else if (!isDown && wasDown)
        requester.keyUp(event);

    // else isDown == wasDown, so no change

    // Update state
    wasDown = isDown;
}


////////////////////////////////////////////////////////
void processLeftRightModifiers(NSUInteger modifiers,
                               NSUInteger leftMask, NSUInteger rightMask,
                               BOOL& leftWasDown, BOOL& rightWasDown,
                               TGE::Keyboard::Key leftKey, TGE::Keyboard::Key rightKey,
                               TGE::priv::WindowImplCocoa& requester)
{
    processOneModifier(modifiers, leftMask,  leftWasDown,  leftKey,  requester);
    processOneModifier(modifiers, rightMask, rightWasDown, rightKey, requester);
}


