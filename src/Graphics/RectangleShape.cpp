/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/RectangleShape.hpp>
#include <cmath>


namespace TGE
{
////////////////////////////////////////////////////////////
RectangleShape::RectangleShape(const Vector2f& size)
{
    setSize(size);
}


////////////////////////////////////////////////////////////
void RectangleShape::setSize(const Vector2f& size)
{
    m_size = size;
    update();
}


////////////////////////////////////////////////////////////
const Vector2f& RectangleShape::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
unsigned int RectangleShape::getPointCount() const
{
    return 4;
}


////////////////////////////////////////////////////////////
Vector2f RectangleShape::getPoint(unsigned int index) const
{
    switch (index)
    {
        default:
        case 0: return Vector2f(0, 0);
        case 1: return Vector2f(m_size.x, 0);
        case 2: return Vector2f(m_size.x, m_size.y);
        case 3: return Vector2f(0, m_size.y);
    }
}

} // namespace TGE
