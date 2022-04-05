/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Win32/SleepImpl.hpp>
#include <windows.h>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
void sleepImpl(Time time)
{
    // Get the supported timer resolutions on this system
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));

    // Set the timer resolution to the minimum for the Sleep call
    timeBeginPeriod(tc.wPeriodMin);

    // Wait...
    ::Sleep(time.asMilliseconds());

    // Reset the timer resolution back to the system default
    timeEndPeriod(tc.wPeriodMin);
}

} // namespace priv

} // namespace TGE
