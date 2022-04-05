/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_THREADLOCALIMPL_HPP
#define TGE_THREADLOCALIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/NonCopyable.hpp>
#include <pthread.h>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Unix implementation of thread-local storage
////////////////////////////////////////////////////////////
class ThreadLocalImpl : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor -- allocate the storage
    ///
    ////////////////////////////////////////////////////////////
    ThreadLocalImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor -- free the storage
    ///
    ////////////////////////////////////////////////////////////
    ~ThreadLocalImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Set the thread-specific value of the variable
    ///
    /// \param value Value of the variable for this thread
    ///
    ////////////////////////////////////////////////////////////
    void setValue(void* value);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the thread-specific value of the variable
    ///
    /// \return Value of the variable for this thread
    ///
    ////////////////////////////////////////////////////////////
    void* getValue() const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    pthread_key_t m_key; ///< Index of our thread-local storage slot
};

} // namespace priv

} // namespace TGE


#endif // TGE_THREADLOCALIMPL_HPP
