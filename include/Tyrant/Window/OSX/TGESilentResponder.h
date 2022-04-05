/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <AppKit/AppKit.h>

////////////////////////////////////////////////////////////
/// \brief Silent Responder used to prevent sound alert with key event
///
/// Mainly used by TGEOpenGLView and its hidden text view.
///
////////////////////////////////////////////////////////////
@interface TGESilentResponder : NSResponder

-(void)doCommandBySelector:(SEL)sel;

@end

