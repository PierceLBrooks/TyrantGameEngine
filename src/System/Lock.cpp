/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Lock.hpp>
#include <Tyrant/System/Mutex.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
Lock::Lock(Mutex& mutex) :
m_mutex(mutex)
{
    m_mutex.lock();
}


////////////////////////////////////////////////////////////
Lock::~Lock()
{
    m_mutex.unlock();
}

} // namespace TGE
