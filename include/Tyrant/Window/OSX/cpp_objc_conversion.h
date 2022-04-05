/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/String.hpp>
#include <string>

#import <Foundation/Foundation.h>

////////////////////////////////////////////////////////////
/// \brief Returns a NSString construct with +stringWithCString:encoding:
///
////////////////////////////////////////////////////////////
NSString* stringToNSString(const std::string& string);

////////////////////////////////////////////////////////////
/// \brief Returns a NSString construct with +stringWithCString:encoding:
///
////////////////////////////////////////////////////////////
NSString* tgeStringToNSString(const TGE::String& string);
