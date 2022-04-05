/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_LOCK_HPP
#define TGE_LOCK_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/System/NonCopyable.hpp>


namespace TGE
{
class Mutex;

////////////////////////////////////////////////////////////
/// \brief Automatic wrapper for locking and unlocking mutexes
///
////////////////////////////////////////////////////////////
class TGE_API Lock : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the lock with a target mutex
    ///
    /// The mutex passed to TGE::Lock is automatically locked.
    ///
    /// \param mutex Mutex to lock
    ///
    ////////////////////////////////////////////////////////////
    explicit Lock(Mutex& mutex);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// The destructor of TGE::Lock automatically unlocks its mutex.
    ///
    ////////////////////////////////////////////////////////////
    ~Lock();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Mutex& m_mutex; ///< Mutex to lock / unlock
};

} // namespace TGE


#endif // TGE_LOCK_HPP


////////////////////////////////////////////////////////////
/// \class TGE::Lock
/// \ingroup system
///
/// TGE::Lock is a RAII wrapper for TGE::Mutex. By unlocking
/// it in its destructor, it ensures that the mutex will
/// always be released when the current scope (most likely
/// a function) ends.
/// This is even more important when an exception or an early
/// return statement can interrupt the execution flow of the
/// function.
///
/// For maximum robustness, TGE::Lock should always be used
/// to lock/unlock a mutex.
///
/// Usage example:
/// \code
/// TGE::Mutex mutex;
///
/// void function()
/// {
///     TGE::Lock lock(mutex); // mutex is now locked
///
///     functionThatMayThrowAnException(); // mutex is unlocked if this function throws
///
///     if (someCondition)
///         return; // mutex is unlocked
///
/// } // mutex is unlocked
/// \endcode
///
/// Because the mutex is not explicitely unlocked in the code,
/// it may remain locked longer than needed. If the region
/// of the code that needs to be protected by the mutex is
/// not the entire function, a good practice is to create a
/// smaller, inner scope so that the lock is limited to this
/// part of the code.
///
/// \code
/// TGE::Mutex mutex;
///
/// void function()
/// {
///     {
///       TGE::Lock lock(mutex);
///       codeThatRequiresProtection();
///
///     } // mutex is unlocked here
///
///     codeThatDoesntCareAboutTheMutex();
/// }
/// \endcode
///
/// Having a mutex locked longer than required is a bad practice
/// which can lead to bad performances. Don't forget that when
/// a mutex is locked, other threads may be waiting doing nothing
/// until it is released.
///
/// \see TGE::Mutex
///
////////////////////////////////////////////////////////////
