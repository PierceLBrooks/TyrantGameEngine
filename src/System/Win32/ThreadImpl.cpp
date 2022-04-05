/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Win32/ThreadImpl.hpp>
#include <Tyrant/System/Thread.hpp>
#include <Tyrant/System/Log.hpp>
#include <cassert>
#include <process.h>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadImpl::ThreadImpl(Thread* owner)
{
    m_thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &ThreadImpl::entryPoint, owner, 0, &m_threadId));

    if (!m_thread)
        Log() << "Failed to create thread" << std::endl;
}


////////////////////////////////////////////////////////////
ThreadImpl::~ThreadImpl()
{
    if (m_thread)
        CloseHandle(m_thread);
}


////////////////////////////////////////////////////////////
void ThreadImpl::wait()
{
    if (m_thread)
    {
        assert(m_threadId != GetCurrentThreadId()); // A thread cannot wait for itself!
        WaitForSingleObject(m_thread, INFINITE);
    }
}


////////////////////////////////////////////////////////////
void ThreadImpl::terminate()
{
    if (m_thread)
        TerminateThread(m_thread, 0);
}


////////////////////////////////////////////////////////////
unsigned int __stdcall ThreadImpl::entryPoint(void* userData)
{
    // The Thread instance is stored in the user data
    Thread* owner = static_cast<Thread*>(userData);

    // Forward to the owner
    owner->run();

    // Optional, but it is cleaner
    _endthreadex(0);

    return 0;
}

} // namespace priv

} // namespace TGE
