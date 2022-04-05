/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/ConvexShape.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
ConvexShape::ConvexShape(unsigned int pointCount)
{
    setPointCount(pointCount);
}


////////////////////////////////////////////////////////////
void ConvexShape::setPointCount(unsigned int count)
{
    m_points.resize(count);
    update();
}


////////////////////////////////////////////////////////////
unsigned int ConvexShape::getPointCount() const
{
    return static_cast<unsigned int>(m_points.size());
}


////////////////////////////////////////////////////////////
void ConvexShape::setPoint(unsigned int index, const Vector2f& point)
{
    m_points[index] = point;
    update();
}


////////////////////////////////////////////////////////////
Vector2f ConvexShape::getPoint(unsigned int index) const
{
    return m_points[index];
}

} // namespace TGE
