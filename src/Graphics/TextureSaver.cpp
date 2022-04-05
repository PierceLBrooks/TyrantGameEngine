/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/TextureSaver.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
TextureSaver::TextureSaver()
{
    glCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_textureBinding));
}


////////////////////////////////////////////////////////////
TextureSaver::~TextureSaver()
{
    glCheck(glBindTexture(GL_TEXTURE_2D, m_textureBinding));
}

} // namespace priv

} // namespace TGE
