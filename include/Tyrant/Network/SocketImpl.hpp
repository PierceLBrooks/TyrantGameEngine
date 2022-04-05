/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>


#if defined(OS_WINDOWS)

    #include <Tyrant/Network/Win32/SocketImpl.hpp>

#else

    #include <Tyrant/Network/Unix/SocketImpl.hpp>

#endif
