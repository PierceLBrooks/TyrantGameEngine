/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Sleep.hpp>

#if defined(OS_WINDOWS)
    #include <Tyrant/System/Win32/SleepImpl.hpp>
#else
    #include <Tyrant/System/Unix/SleepImpl.hpp>
#endif


namespace TGE
{
////////////////////////////////////////////////////////////
void sleep(Time duration)
{
    if (duration >= Time::Zero)
        priv::sleepImpl(duration);
}

} // namespace TGE
