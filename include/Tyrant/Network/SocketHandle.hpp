/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_SOCKETHANDLE_HPP
#define TGE_SOCKETHANDLE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>

#if defined(OS_WINDOWS)
    #include <basetsd.h>
#endif


namespace TGE
{
////////////////////////////////////////////////////////////
// Define the low-level socket handle type, specific to
// each platform
////////////////////////////////////////////////////////////
#if defined(OS_WINDOWS)

    typedef UINT_PTR SocketHandle;

#else

    typedef int SocketHandle;

#endif

} // namespace TGE


#endif // TGE_SOCKETHANDLE_HPP
