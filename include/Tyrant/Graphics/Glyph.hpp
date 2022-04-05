/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_GLYPH_HPP
#define TGE_GLYPH_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Graphics/Rect.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
/// \brief Structure describing a glyph
///
////////////////////////////////////////////////////////////
class TGE_API Glyph
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Glyph() : advance(0) {}

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int     advance;     ///< Offset to move horizontically to the next character
    IntRect bounds;      ///< Bounding rectangle of the glyph, in coordinates relative to the baseline
    IntRect textureRect; ///< Texture coordinates of the glyph inside the font's texture
};

} // namespace TGE


#endif // TGE_GLYPH_HPP


////////////////////////////////////////////////////////////
/// \class TGE::Glyph
/// \ingroup graphics
///
/// A glyph is the visual representation of a character.
///
/// The TGE::Glyph structure provides the information needed
/// to handle the glyph:
/// \li its coordinates in the font's texture
/// \li its bounding rectangle
/// \li the offset to apply to get the starting position of the next glyph
///
/// \see TGE::Font
///
////////////////////////////////////////////////////////////
