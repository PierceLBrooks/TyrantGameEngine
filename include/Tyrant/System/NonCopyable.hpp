/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_NONCOPYABLE_HPP
#define TGE_NONCOPYABLE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
/// \brief Utility class that makes any derived
///        class non-copyable
///
////////////////////////////////////////////////////////////
class TGE_API NonCopyable
{
protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Because this class has a copy constructor, the compiler
    /// will not automatically generate the default constructor.
    /// That's why we must define it explicitely.
    ///
    ////////////////////////////////////////////////////////////
    NonCopyable() {}

private :

    ////////////////////////////////////////////////////////////
    /// \brief Disabled copy constructor
    ///
    /// By making the copy constructor private, the compiler will
    /// trigger an error if anyone outside tries to use it.
    /// To prevent NonCopyable or friend classes from using it,
    /// we also give no definition, so that the linker will
    /// produce an error if the first protection was inefficient.
    ///
    ////////////////////////////////////////////////////////////
    NonCopyable(const NonCopyable&);

    ////////////////////////////////////////////////////////////
    /// \brief Disabled assignment operator
    ///
    /// By making the assignment operator private, the compiler will
    /// trigger an error if anyone outside tries to use it.
    /// To prevent NonCopyable or friend classes from using it,
    /// we also give no definition, so that the linker will
    /// produce an error if the first protection was inefficient.
    ///
    ////////////////////////////////////////////////////////////
    NonCopyable& operator =(const NonCopyable&);
};

} // namespace TGE


#endif // TGE_NONCOPYABLE_HPP


////////////////////////////////////////////////////////////
/// \class TGE::NonCopyable
/// \ingroup system
///
/// This class makes its instances non-copyable, by explicitely
/// disabling its copy constructor and its assignment operator.
///
/// To create a non-copyable class, simply inherit from
/// TGE::NonCopyable.
///
/// The type of inheritance (public or private) doesn't matter,
/// the copy constructor and assignment operator are declared private
/// in TGE::NonCopyable so they will end up being inaccessible in both
/// cases. Thus you can use a shorter syntax for inheriting from it
/// (see below).
///
/// Usage example:
/// \code
/// class MyNonCopyableClass : TGE::NonCopyable
/// {
///     ...
/// };
/// \endcode
///
/// Deciding whether the instances of a class can be copied
/// or not is a very important design choice. You are strongly
/// encouraged to think about it before writing a class,
/// and to use TGE::NonCopyable when necessary to prevent
/// many potential future errors when using it. This is also
/// a very important indication to users of your class.
///
////////////////////////////////////////////////////////////
