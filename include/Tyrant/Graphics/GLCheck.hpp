/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_GLCHECK_HPP
#define TGE_GLCHECK_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Graphics/GLExtensions.hpp>
#include <string>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Let's define a macro to quickly check every OpenGL API calls
////////////////////////////////////////////////////////////
#ifdef TGE_DEBUG

    // In debug mode, perform a test on every OpenGL call
    #define glCheck(x) x; TGE::priv::glCheckError(__FILE__, __LINE__);

#else

    // Else, we don't add any overhead
    #define glCheck(call) (call)

#endif

////////////////////////////////////////////////////////////
/// \brief Check the last OpenGL error
///
/// \param file Source file where the call is located
/// \param line Line number of the source file where the call is located
///
////////////////////////////////////////////////////////////
void glCheckError(const char* file, unsigned int line);

} // namespace priv

} // namespace TGE


#endif // TGE_GLCHECK_HPP
