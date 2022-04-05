/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_CONTEXT_HPP
#define TGE_CONTEXT_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Window/GlResource.hpp>
#include <Tyrant/Window/ContextSettings.hpp>
#include <Tyrant/System/NonCopyable.hpp>


namespace TGE
{
namespace priv
{
    class GlContext;
}

////////////////////////////////////////////////////////////
/// \brief Class holding a valid drawing context
///
////////////////////////////////////////////////////////////
class TGE_API Context : GlResource, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The constructor creates and activates the context
    ///
    ////////////////////////////////////////////////////////////
    Context();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// The desctructor deactivates and destroys the context
    ///
    ////////////////////////////////////////////////////////////
    ~Context();

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate explicitely the context
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool setActive(bool active);

public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct a in-memory context
    ///
    /// This constructor is for internal use, you don't need
    /// to bother with it.
    ///
    /// \param settings Creation parameters
    /// \param width    Back buffer width
    /// \param height   Back buffer height
    ///
    ////////////////////////////////////////////////////////////
    Context(const ContextSettings& settings, unsigned int width, unsigned int height);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::GlContext* m_context; ///< Internal OpenGL context
};

} // namespace TGE


#endif // TGE_CONTEXT_HPP

////////////////////////////////////////////////////////////
/// \class TGE::Context
/// \ingroup window
///
/// If you need to make OpenGL calls without having an
/// active window (like in a thread), you can use an
/// instance of this class to get a valid context.
///
/// Having a valid context is necessary for *every* OpenGL call.
///
/// Note that a context is only active in its current thread,
/// if you create a new thread it will have no valid context
/// by default.
///
/// To use a TGE::Context instance, just construct it and let it
/// live as long as you need a valid context. No explicit activation
/// is needed, all it has to do is to exist. Its destructor
/// will take care of deactivating and freeing all the attached
/// resources.
///
/// Usage example:
/// \code
/// void threadFunction(void*)
/// {
///    TGE::Context context;
///    // from now on, you have a valid context
///
///    // you can make OpenGL calls
///    glClear(GL_DEPTH_BUFFER_BIT);
/// }
/// // the context is automatically deactivated and destroyed
/// // by the TGE::Context destructor
/// \endcode
///
////////////////////////////////////////////////////////////
