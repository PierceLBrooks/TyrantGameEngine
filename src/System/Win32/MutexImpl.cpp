/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Win32/MutexImpl.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
MutexImpl::MutexImpl()
{
    InitializeCriticalSection(&m_mutex);
}


////////////////////////////////////////////////////////////
MutexImpl::~MutexImpl()
{
    DeleteCriticalSection(&m_mutex);
}


////////////////////////////////////////////////////////////
void MutexImpl::lock()
{
    EnterCriticalSection(&m_mutex);
}


////////////////////////////////////////////////////////////
void MutexImpl::unlock()
{
    LeaveCriticalSection(&m_mutex);
}

} // namespace priv

} // namespace TGE
