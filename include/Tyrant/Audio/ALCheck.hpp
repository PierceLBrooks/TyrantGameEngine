/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_ALCHECK_HPP
#define TGE_ALCHECK_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <iostream>
#include <string>
#include <AL/al.h>
#include <AL/alc.h>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Let's define a macro to quickly check every OpenAL API calls
////////////////////////////////////////////////////////////
#ifdef TGE_DEBUG

    // If in debug mode, perform a test on every call
    #define alCheck(x) x; TGE::priv::alCheckError(__FILE__, __LINE__);

#else

    // Else, we don't add any overhead
    #define alCheck(Func) (Func)

#endif


////////////////////////////////////////////////////////////
/// Check the last OpenAL error
///
/// \param file Source file where the call is located
/// \param line Line number of the source file where the call is located
///
////////////////////////////////////////////////////////////
void alCheckError(const std::string& file, unsigned int line);

////////////////////////////////////////////////////////////
/// Make sure that OpenAL is initialized
///
////////////////////////////////////////////////////////////
void ensureALInit();

} // namespace priv

} // namespace TGE


#endif // TGE_ALCHECK_HPP
