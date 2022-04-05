/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_TEXTURESAVER_HPP
#define TGE_TEXTURESAVER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/GLCheck.hpp>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Automatic wrapper for saving and restoring the current texture binding
///
////////////////////////////////////////////////////////////
class TextureSaver
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The current texture binding is saved.
    ///
    ////////////////////////////////////////////////////////////
    TextureSaver();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// The previous texture binding is restored.
    ///
    ////////////////////////////////////////////////////////////
    ~TextureSaver();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    GLint m_textureBinding; ///< Texture binding to restore
};

} // namespace priv

} // namespace TGE


#endif // TGE_TEXTURESAVER_HPP
