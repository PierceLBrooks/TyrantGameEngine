/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_INPUTIMPL_HPP
#define TGE_INPUTIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>

#if defined(OS_WINDOWS)
    #include <Tyrant/Window/Win32/InputImpl.hpp>
#elif defined(OS_LINUX)
    #include <Tyrant/Window/Unix/InputImpl.hpp>
#elif defined(OS_MAC)
    #include <Tyrant/Window/OSX/InputImpl.hpp>
#endif


#endif // TGE_INPUTIMPL_HPP
