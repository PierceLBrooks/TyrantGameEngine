/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Mutex.hpp>

#if defined(OS_WINDOWS)
    #include <Tyrant/System/Win32/MutexImpl.hpp>
#else
    #include <Tyrant/System/Unix/MutexImpl.hpp>
#endif


namespace TGE
{
////////////////////////////////////////////////////////////
Mutex::Mutex()
{
    m_mutexImpl = new priv::MutexImpl;
}


////////////////////////////////////////////////////////////
Mutex::~Mutex()
{
    delete m_mutexImpl;
}


////////////////////////////////////////////////////////////
void Mutex::lock()
{
    m_mutexImpl->lock();
}


////////////////////////////////////////////////////////////
void Mutex::unlock()
{
    m_mutexImpl->unlock();
}

} // namespace TGE
