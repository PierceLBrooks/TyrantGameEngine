/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_CLOCKIMPLUNIX_HPP
#define TGE_CLOCKIMPLUNIX_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/System/Time.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Unix implementaton of TGE::Clock
///
////////////////////////////////////////////////////////////
class ClockImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the current time
    ///
    /// \return Current time
    ///
    ////////////////////////////////////////////////////////////
    static Time getCurrentTime();
};

} // namespace priv

} // namespace TGE


#endif // TGE_CLOCKIMPLUNIX_HPP
