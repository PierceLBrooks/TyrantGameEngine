/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_THREADIMPL_HPP
#define TGE_THREADIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/NonCopyable.hpp>
#include <windows.h>


namespace TGE
{
class Thread;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Windows implementation of threads
////////////////////////////////////////////////////////////
class ThreadImpl : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor, launch the thread
    ///
    /// \param owner The Thread instance to run
    ///
    ////////////////////////////////////////////////////////////
    ThreadImpl(Thread* owner);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ThreadImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Wait until the thread finishes
    ///
    ////////////////////////////////////////////////////////////
    void wait();

    ////////////////////////////////////////////////////////////
    /// \brief Terminate the thread
    ///
    ////////////////////////////////////////////////////////////
    void terminate();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Global entry point for all threads
    ///
    /// \param userData User-defined data (contains the Thread instance)
    ///
    /// \return OS specific error code
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int __stdcall entryPoint(void* userData);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    HANDLE m_thread; ///< Win32 thread handle
    unsigned int m_threadId; ///< Win32 thread identifier
};

} // namespace priv

} // namespace TGE


#endif // TGE_THREADIMPL_HPP
