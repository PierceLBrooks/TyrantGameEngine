/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_RENDERTEXTUREIMPL_HPP
#define TGE_RENDERTEXTUREIMPL_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/System/NonCopyable.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for render-texture implementations
///
////////////////////////////////////////////////////////////
class RenderTextureImpl : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTextureImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Create the render texture implementation
    ///
    /// \param width       Width of the texture to render to
    /// \param height      Height of the texture to render to
    /// \param textureId   OpenGL identifier of the target texture
    /// \param depthBuffer Is a depth buffer requested?
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(unsigned int width, unsigned int height, unsigned int textureId, bool depthBuffer) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render texture for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    virtual bool activate(bool active) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update the pixels of the target texture
    ///
    /// \param textureId OpenGL identifier of the target texture
    ///
    ////////////////////////////////////////////////////////////
    virtual void updateTexture(unsigned int textureId) = 0;
};

} // namespace priv

} // namespace TGE


#endif // TGE_RENDERTEXTUREIMPL_HPP
