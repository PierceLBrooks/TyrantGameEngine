/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Unix/ClockImpl.hpp>
#if defined(OS_MAC)
    #include <mach/mach_time.h>
#else
    #include <time.h>
#endif


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
Time ClockImpl::getCurrentTime()
{
#if defined(OS_MAC)

    // Mac OS X specific implementation (it doesn't support clock_gettime)
    static mach_timebase_info_data_t frequency = {0, 0};
    if (frequency.denom == 0)
        mach_timebase_info(&frequency);
    Uint64 nanoseconds = mach_absolute_time() * frequency.numer / frequency.denom;
    return TGE::microseconds(nanoseconds / 1000);

#else

    // POSIX implementation
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return TGE::microseconds(static_cast<Uint64>(time.tv_sec) * 1000000 + time.tv_nsec / 1000);

#endif
}

} // namespace priv

} // namespace TGE
