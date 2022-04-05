/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_PRIMITIVETYPE_HPP
#define TGE_PRIMITIVETYPE_HPP

namespace TGE
{
////////////////////////////////////////////////////////////
/// \ingroup graphics
/// \brief Types of primitives that a TGE::VertexArray can render
///
/// Points and lines have no area, therefore their thickness
/// will always be 1 pixel, regarldess the current transform
/// and view.
///
////////////////////////////////////////////////////////////
enum PrimitiveType
{
    Points,         ///< List of individual points
    Lines,          ///< List of individual lines
    LinesStrip,     ///< List of connected lines, a point uses the previous point to form a line
    Triangles,      ///< List of individual triangles
    TrianglesStrip, ///< List of connected triangles, a point uses the two previous points to form a triangle
    TrianglesFan,   ///< List of connected triangles, a point uses the common center and the previous point to form a triangle
    Quads           ///< List of individual quads (deprecated, don't work with OpenGL ES)
};

} // namespace TGE


#endif // TGE_PRIMITIVETYPE_HPP
