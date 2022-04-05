/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_THREADIMPL_HPP
#define TGE_THREADIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/System/NonCopyable.hpp>
#include <pthread.h>


namespace TGE
{
class Thread;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Unix implementation of threads
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
    /// \return Os specific error code
    ///
    ////////////////////////////////////////////////////////////
    static void* entryPoint(void* userData);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    pthread_t m_thread;   ///< pthread thread instance
    bool      m_isActive; ///< Thread state (active or inactive)
};

} // namespace priv

} // namespace TGE


#endif // TGE_THREADIMPL_HPP
