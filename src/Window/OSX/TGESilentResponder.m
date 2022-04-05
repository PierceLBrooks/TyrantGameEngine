/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#import <Tyrant/Window/OSX/TGESilentResponder.h>

@implementation TGESilentResponder

-(void)doCommandBySelector:(SEL)sel
{
    // Just do nothing, to prevent sound alerts
    (void)sel;
}

@end

