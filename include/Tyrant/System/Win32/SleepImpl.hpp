/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_SLEEPIMPLWIN32_HPP
#define TGE_SLEEPIMPLWIN32_HPP

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
/// \brief Windows implementation of TGE::Sleep
///
/// \param time Time to sleep
///
////////////////////////////////////////////////////////////
void sleepImpl(Time time);

} // namespace priv

} // namespace TGE


#endif // TGE_SLEEPIMPLWIN32_HPP
