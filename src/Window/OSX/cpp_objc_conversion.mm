/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Utf.hpp>

#import <Tyrant/Window/OSX/cpp_objc_conversion.h>
#import <Foundation/Foundation.h>

////////////////////////////////////////////////////////////
NSString* stringToNSString(const std::string& string)
{
    std::string utf8; utf8.reserve(string.size() + 1);
    TGE::Utf8::fromAnsi(string.begin(), string.end(), std::back_inserter(utf8));
    NSString* str = [NSString stringWithCString:utf8.c_str() encoding:NSUTF8StringEncoding];

    return str;
}

////////////////////////////////////////////////////////////
NSString* tgeStringToNSString(const TGE::String& string)
{
    TGE::Uint32 length = string.getSize() * sizeof(TGE::Uint32);
    const void* data = reinterpret_cast<const void*>(string.getData());

    NSStringEncoding encoding;
    if (NSHostByteOrder() == NS_LittleEndian)
        encoding = NSUTF32LittleEndianStringEncoding;
    else
        encoding = NSUTF32BigEndianStringEncoding;

    NSString* str = [[NSString alloc] initWithBytes:data length:length encoding:encoding];
    return str;
}
