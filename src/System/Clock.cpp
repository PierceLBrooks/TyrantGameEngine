/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Clock.hpp>

#if defined(OS_WINDOWS)
    #include <Tyrant/System/Win32/ClockImpl.hpp>
#else
    #include <Tyrant/System/Unix/ClockImpl.hpp>
#endif


namespace TGE
{
////////////////////////////////////////////////////////////
Clock::Clock() :
m_startTime(priv::ClockImpl::getCurrentTime())
{
}


////////////////////////////////////////////////////////////
Time Clock::getElapsedTime() const
{
    return priv::ClockImpl::getCurrentTime() - m_startTime;
}


////////////////////////////////////////////////////////////
Time Clock::restart()
{
    Time now = priv::ClockImpl::getCurrentTime();
    Time elapsed = now - m_startTime;
    m_startTime = now;

    return elapsed;
}

} // namespace TGE
