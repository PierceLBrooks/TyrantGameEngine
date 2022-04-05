/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/GLExtensions.hpp>
#include <Tyrant/System/Log.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
void ensureExtensionsInit()
{
    static bool initialized = false;
    if (!initialized)
    {
        GLenum status = glewInit();
        if (status == GLEW_OK)
        {
            initialized = true;
        }
        else
        {
            Log() << "Failed to initialize GLEW, " << glewGetErrorString(status) << std::endl;
        }
    }
}

} // namespace priv

} // namespace TGE
