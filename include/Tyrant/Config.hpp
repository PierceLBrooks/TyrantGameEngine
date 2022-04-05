/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_CONFIG_HPP
#define TGE_CONFIG_HPP


////////////////////////////////////////////////////////////
// Define the TGE version
////////////////////////////////////////////////////////////
#define TGE_VERSION_MAJOR 0
#define TGE_VERSION_MINOR 5


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if defined(_WIN32)

    // Windows
    #define OS_WINDOWS
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(__APPLE__) && defined(__MACH__)

    // Apple platform, see which one it is
    #include "TargetConditionals.h"

    #if TARGET_OS_MAC

        // OSX
        #define OS_MAC

    #endif

#elif defined(__linux__)

	 // Linux
	#define OS_LINUX

#else

    // Unsupported system
    #error This operating system is not supported by Tyrant

#endif


////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

    #define TGE_DEBUG

#endif


////////////////////////////////////////////////////////////
// Define helpers to create portable import / export macros for each module
////////////////////////////////////////////////////////////
#if !defined(TGE_STATIC)

    #if defined(OS_WINDOWS)

        // Windows compilers need specific (and different) keywords for export and import
        #ifdef BUILDING_TYRANT
			#define TGE_API __declspec(dllexport)
		#else
			#define TGE_API __declspec(dllimport)
		#endif // BUILDING_TYRANT

        // For Visual C++ compilers, we also need to turn off this annoying C4251 warning
        #ifdef _MSC_VER

            #pragma warning(disable : 4251)

        #endif

    #else // Linux, Mac OS X

        #if __GNUC__ >= 4

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define TGE_API __attribute__ ((__visibility__ ("default")))

        #else

            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define TGE_API

        #endif

    #endif

#else

    // Static build doesn't need import/export macros
    #define TGE_API

#endif


////////////////////////////////////////////////////////////
// Define portable fixed-size types
////////////////////////////////////////////////////////////
namespace TGE
{
    // All "common" platforms use the same size for char, short and int
    // (basically there are 3 types for 3 sizes, so no other match is possible),
    // we can use them without doing any kind of check

    // 8 bits integer types
    typedef signed   char Int8;
    typedef unsigned char Uint8;

    // 16 bits integer types
    typedef signed   short Int16;
    typedef unsigned short Uint16;

    // 32 bits integer types
    typedef signed   int Int32;
    typedef unsigned int Uint32;

    // 64 bits integer types
    #if defined(_MSC_VER)
        typedef signed   __int64 Int64;
        typedef unsigned __int64 Uint64;
    #else
        typedef signed   long long Int64;
        typedef unsigned long long Uint64;
    #endif

} // namespace TGE


#endif // TGE_CONFIG_HPP
