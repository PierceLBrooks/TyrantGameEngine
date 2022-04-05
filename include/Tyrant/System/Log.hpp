/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_LOG_HPP
#define TGE_LOG_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <ostream>


namespace TGE
{
////////////////////////////////////////////////////////////
/// \brief Standard stream used by TGE to output warnings and errors
///
////////////////////////////////////////////////////////////
TGE_API std::ostream& Log();

} // namespace TGE


#endif // TGE_LOG_HPP


////////////////////////////////////////////////////////////
/// \fn TGE::Log
/// \ingroup system
///
/// By default, TGE::Log() outputs to the same location as std::cerr,
/// (-> the stderr descriptor) which is the console if there's
/// one available.
///
/// It is a standard std::ostream instance, so it supports all the
/// insertion operations defined by the STL
/// (operator <<, manipulators, etc.).
///
/// TGE::Log() can be redirected to write to another output, independently
/// of std::cerr, by using the rdbuf() function provided by the
/// std::ostream class.
///
/// Example:
/// \code
/// // Redirect to a file
/// std::ofstream file("TGE-log.txt");
/// std::streambuf* previous = TGE::Log().rdbuf(file.rdbuf());
///
/// // Redirect to nothing
/// TGE::Log().rdbuf(NULL);
///
/// // Restore the original output
/// TGE::Log().rdbuf(previous);
/// \endcode
///
/// \return Reference to std::ostream representing the TGE error stream
///
////////////////////////////////////////////////////////////
