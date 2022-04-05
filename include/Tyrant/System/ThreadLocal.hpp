/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_THREADLOCAL_HPP
#define TGE_THREADLOCAL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/System/NonCopyable.hpp>
#include <cstdlib>


namespace TGE
{
namespace priv
{
    class ThreadLocalImpl;
}

////////////////////////////////////////////////////////////
/// \brief Defines variables with thread-local storage
///
////////////////////////////////////////////////////////////
class TGE_API ThreadLocal : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param value Optional value to initalize the variable
    ///
    ////////////////////////////////////////////////////////////
    ThreadLocal(void* value = NULL);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ThreadLocal();

    ////////////////////////////////////////////////////////////
    /// \brief Set the thread-specific value of the variable
    ///
    /// \param value Value of the variable for the current thread
    ///
    ////////////////////////////////////////////////////////////
    void setValue(void* value);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the thread-specific value of the variable
    ///
    /// \return Value of the variable for the current thread
    ///
    ////////////////////////////////////////////////////////////
    void* getValue() const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::ThreadLocalImpl* m_impl; ///< Pointer to the OS specific implementation
};

} // namespace TGE


#endif // TGE_THREADLOCAL_HPP


////////////////////////////////////////////////////////////
/// \class TGE::ThreadLocal
/// \ingroup system
///
/// This class manipulates void* parameters and thus is not
/// appropriate for strongly-typed variables. You should rather
/// use the TGE::ThreadLocalPtr template class.
///
////////////////////////////////////////////////////////////
