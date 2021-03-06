/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/Unix/ThreadLocalImpl.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadLocalImpl::ThreadLocalImpl()
{
    pthread_key_create(&m_key, NULL);
}


////////////////////////////////////////////////////////////
ThreadLocalImpl::~ThreadLocalImpl()
{
    pthread_key_delete(m_key);
}


////////////////////////////////////////////////////////////
void ThreadLocalImpl::setValue(void* value)
{
    pthread_setspecific(m_key, value);
}


////////////////////////////////////////////////////////////
void* ThreadLocalImpl::getValue() const
{
    return pthread_getspecific(m_key);
}

} // namespace priv

} // namespace TGE
