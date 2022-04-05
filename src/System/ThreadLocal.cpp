/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/ThreadLocal.hpp>

#if defined(OS_WINDOWS)
    #include <Tyrant/System/Win32/ThreadLocalImpl.hpp>
#else
    #include <Tyrant/System/Unix/ThreadLocalImpl.hpp>
#endif


namespace TGE
{
////////////////////////////////////////////////////////////
ThreadLocal::ThreadLocal(void* value)
{
    m_impl = new priv::ThreadLocalImpl;
    setValue(value);
}


////////////////////////////////////////////////////////////
ThreadLocal::~ThreadLocal()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
void ThreadLocal::setValue(void* value)
{
    m_impl->setValue(value);
}


////////////////////////////////////////////////////////////
void* ThreadLocal::getValue() const
{
    return m_impl->getValue();
}

} // namespace TGE
