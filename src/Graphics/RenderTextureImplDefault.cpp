/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/RenderTextureImplDefault.hpp>
#include <Tyrant/Graphics/GLCheck.hpp>
#include <Tyrant/Graphics/TextureSaver.hpp>
#include <Tyrant/Window/Context.hpp>
#include <Tyrant/System/Log.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderTextureImplDefault::RenderTextureImplDefault() :
m_context(0),
m_width  (0),
m_height (0)
{

}


////////////////////////////////////////////////////////////
RenderTextureImplDefault::~RenderTextureImplDefault()
{
    // Destroy the context
    delete m_context;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplDefault::create(unsigned int width, unsigned int height, unsigned int, bool depthBuffer)
{
    // Store the dimensions
    m_width = width;
    m_height = height;

    // Create the in-memory OpenGL context
    m_context = new Context(ContextSettings(depthBuffer ? 32 : 0), width, height);

    return true;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplDefault::activate(bool active)
{
    return m_context->setActive(active);
}


////////////////////////////////////////////////////////////
void RenderTextureImplDefault::updateTexture(unsigned int textureId)
{
    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Copy the rendered pixels to the texture
    glCheck(glBindTexture(GL_TEXTURE_2D, textureId));
    glCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_width, m_height));
}

} // namespace priv

} // namespace TGE
