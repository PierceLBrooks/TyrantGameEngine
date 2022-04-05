/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/RenderTexture.hpp>
#include <Tyrant/Graphics/RenderTextureImplFBO.hpp>
#include <Tyrant/Graphics/RenderTextureImplDefault.hpp>
#include <Tyrant/System/Log.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
RenderTexture::RenderTexture() :
m_impl(NULL)
{

}


////////////////////////////////////////////////////////////
RenderTexture::~RenderTexture()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
bool RenderTexture::create(unsigned int width, unsigned int height, bool depthBuffer)
{
    // Create the texture
    if (!m_texture.create(width, height))
    {
        Log() << "Impossible to create render texture (failed to create the target texture)" << std::endl;
        return false;
    }

    // We disable smoothing by default for render textures
    setSmooth(false);

    // Create the implementation
    delete m_impl;
    if (priv::RenderTextureImplFBO::isAvailable())
    {
        // Use frame-buffer object (FBO)
        m_impl = new priv::RenderTextureImplFBO;
    }
    else
    {
        // Use default implementation
        m_impl = new priv::RenderTextureImplDefault;
    }

    // Initialize the render texture
    if (!m_impl->create(width, height, m_texture.m_texture, depthBuffer))
        return false;

    // We can now initialize the render target part
    RenderTarget::initialize();

    return true;
}


////////////////////////////////////////////////////////////
void RenderTexture::setSmooth(bool smooth)
{
    m_texture.setSmooth(smooth);
}


////////////////////////////////////////////////////////////
bool RenderTexture::isSmooth() const
{
    return m_texture.isSmooth();
}


////////////////////////////////////////////////////////////
void RenderTexture::setRepeated(bool repeated)
{
    m_texture.setRepeated(repeated);
}


////////////////////////////////////////////////////////////
bool RenderTexture::isRepeated() const
{
    return m_texture.isRepeated();
}


////////////////////////////////////////////////////////////
bool RenderTexture::setActive(bool active)
{
    return m_impl && m_impl->activate(active);
}


////////////////////////////////////////////////////////////
void RenderTexture::display()
{
    // Update the target texture
    if (setActive(true))
    {
        m_impl->updateTexture(m_texture.m_texture);
        m_texture.m_pixelsFlipped = true;
    }
}


////////////////////////////////////////////////////////////
Vector2u RenderTexture::getSize() const
{
    return m_texture.getSize();
}


////////////////////////////////////////////////////////////
const Texture& RenderTexture::getTexture() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
bool RenderTexture::activate(bool active)
{
    return setActive(active);
}

} // namespace TGE
