/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_RENDERTEXTUREIMPLDEFAULT_HPP
#define TGE_RENDERTEXTUREIMPLDEFAULT_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/RenderTextureImpl.hpp>
#include <Tyrant/Window/GlResource.hpp>
#include <Tyrant/Window/Context.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Default specialization of RenderTextureImpl,
///        using a in-memory context
///
////////////////////////////////////////////////////////////
class RenderTextureImplDefault : public RenderTextureImpl, GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTextureImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderTextureImplDefault();

private :

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
    virtual bool create(unsigned int width, unsigned int height, unsigned int textureId, bool depthBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render texture for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    virtual bool activate(bool active);

    ////////////////////////////////////////////////////////////
    /// \brief Update the pixels of the target texture
    ///
    /// \param textureId OpenGL identifier of the target texture
    ///
    ////////////////////////////////////////////////////////////
    virtual void updateTexture(unsigned textureId);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Context*     m_context; ///< P-Buffer based context
    unsigned int m_width;   ///< Width of the P-Buffer
    unsigned int m_height;  ///< Height of the P-Buffer
};

} // namespace priv

} // namespace TGE


#endif // TGE_RENDERTEXTUREIMPLDEFAULT_HPP
