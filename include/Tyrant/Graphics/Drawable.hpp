/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_DRAWABLE_HPP
#define TGE_DRAWABLE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Graphics/RenderStates.hpp>


namespace TGE
{
class RenderTarget;

////////////////////////////////////////////////////////////
/// \brief Abstract base class for objects that can be drawn
///        to a render target
///
////////////////////////////////////////////////////////////
class TGE_API Drawable
{
public :

    Drawable() : depth(0), visible(true), renderStates(nullptr) {}

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////

    virtual ~Drawable() {delete renderStates;}

    bool getVisible()
    {
        return visible;
    }

    void setVisible(bool v)
    {
        visible = v;
    }

    int getDepth()
    {
    	return depth;
    }

    void setDepth(unsigned int d)
    {
    	depth = d;
    }

    RenderStates* getRenderStates()
    {
        return renderStates;
    }

    void setRenderStates(RenderStates r)
    {
        delete renderStates;
        renderStates = new RenderStates(r);
    }

protected :

    friend class RenderTarget;
    RenderStates* renderStates;
    unsigned int depth;
    bool visible;
    ////////////////////////////////////////////////////////////
    /// \brief Draw the object to a render target
    ///
    /// This is a pure virtual function that has to be implemented
    /// by the derived class to define how the drawable should be
    /// drawn.
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};

} // namespace TGE


#endif // TGE_DRAWABLE_HPP


////////////////////////////////////////////////////////////
/// \class TGE::Drawable
/// \ingroup graphics
///
/// TGE::Drawable is a very simple base class that allows objects
/// of derived classes to be drawn to a TGE::RenderTarget.
///
/// All you have to do in your derived class is to override the
/// draw virtual function.
///
/// Note that inheriting from TGE::Drawable is not mandatory,
/// but it allows this nice syntax "window.draw(object)" rather
/// than "object.draw(window)", which is more consistent with other
/// TGE classes.
///
/// Example:
/// \code
/// class MyDrawable : public TGE::Drawable
/// {
/// public :
///
///    ...
///
/// private :
///
///     virtual void draw(TGE::RenderTarget& target, TGE::RenderStates states) const
///     {
///         // You can draw other high-level objects
///         target.draw(m_sprite, states);
///
///         // ... or use the low-level API
///         states.texture = &m_texture;
///         target.draw(m_vertices, states);
///
///         // ... or draw with OpenGL directly
///         glBegin(GL_QUADS);
///         ...
///         glEnd();
///     }
///
///     TGE::Sprite m_sprite;
///     TGE::Texture m_texture;
///     TGE::VertexArray m_vertices;
/// };
/// \endcode
///
/// \see TGE::RenderTarget
///
////////////////////////////////////////////////////////////
