/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Graphics/View.hpp>
#include <cmath>


namespace TGE
{
////////////////////////////////////////////////////////////
View::View() :
m_center             (),
m_size               (),
m_rotation           (0),
m_viewport           (0, 0, 1, 1),
m_transformUpdated   (false),
m_invTransformUpdated(false)
{
    reset(FloatRect(0, 0, 1000, 1000));
}


////////////////////////////////////////////////////////////
View::View(const FloatRect& rectangle) :
m_center             (),
m_size               (),
m_rotation           (0),
m_viewport           (0, 0, 1, 1),
m_transformUpdated   (false),
m_invTransformUpdated(false)
{
    reset(rectangle);
}


////////////////////////////////////////////////////////////
View::View(const Vector2f& center, const Vector2f& size) :
m_center             (center),
m_size               (size),
m_rotation           (0),
m_viewport           (0, 0, 1, 1),
m_transformUpdated   (false),
m_invTransformUpdated(false)
{

}

////////////////////////////////////////////////////////////
void View::setCenter(float x, float y)
{
    m_center.x = x;
    m_center.y = y;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setCenter(const Vector2f& center)
{
    setCenter(center.x, center.y);
}


////////////////////////////////////////////////////////////
void View::setSize(float width, float height)
{
    m_size.x = width;
    m_size.y = height;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setSize(const Vector2f& size)
{
    setSize(size.x, size.y);
}


////////////////////////////////////////////////////////////
void View::setRotation(float angle)
{
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setViewport(const FloatRect& viewport)
{
    m_viewport = viewport;
}


////////////////////////////////////////////////////////////
void View::reset(const FloatRect& rectangle)
{
    m_center.x = rectangle.left + rectangle.width / 2.f;
    m_center.y = rectangle.top + rectangle.height / 2.f;
    m_size.x   = rectangle.width;
    m_size.y   = rectangle.height;
    m_rotation = 0;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector2f& View::getCenter() const
{
    return m_center;
}


////////////////////////////////////////////////////////////
const Vector2f& View::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
float View::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const FloatRect& View::getViewport() const
{
    return m_viewport;
}


////////////////////////////////////////////////////////////
void View::move(float offsetX, float offsetY)
{
    setCenter(m_center.x + offsetX, m_center.y + offsetY);
}


////////////////////////////////////////////////////////////
void View::move(const Vector2f& offset)
{
    setCenter(m_center + offset);
}


////////////////////////////////////////////////////////////
void View::rotate(float angle)
{
    setRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void View::zoom(float factor)
{
    setSize(m_size.x * factor, m_size.y * factor);
}


////////////////////////////////////////////////////////////
const Transform& View::getTransform() const
{
    // Recompute the matrix if needed
    if (!m_transformUpdated)
    {
        // Rotation components
        float angle  = m_rotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine   = static_cast<float>(std::sin(angle));
        float tx     = -m_center.x * cosine - m_center.y * sine + m_center.x;
        float ty     =  m_center.x * sine - m_center.y * cosine + m_center.y;

        // Projection components
        float a =  2.f / m_size.x;
        float b = -2.f / m_size.y;
        float c = -a * m_center.x;
        float d = -b * m_center.y;

        // Rebuild the projection matrix
        m_transform = Transform( a * cosine, a * sine,   a * tx + c,
                                -b * sine,   b * cosine, b * ty + d,
                                 0.f,        0.f,        1.f);
        m_transformUpdated = true;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& View::getInverseTransform() const
{
    // Recompute the matrix if needed
    if (!m_invTransformUpdated)
    {
        m_inverseTransform = getTransform().getInverse();
        m_invTransformUpdated = true;
    }

    return m_inverseTransform;
}

} // namespace TGE
