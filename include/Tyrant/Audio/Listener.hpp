/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_LISTENER_HPP
#define TGE_LISTENER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/System/Vector3.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
/// \brief The audio listener is the point in the scene
///        from where all the sounds are heard
///
////////////////////////////////////////////////////////////
class TGE_API Listener
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Change the global volume of all the sounds and musics
    ///
    /// The volume is a number between 0 and 100; it is combined with
    /// the individual volume of each sound / music.
    /// The default value for the volume is 100 (maximum).
    ///
    /// \param volume New global volume, in the range [0, 100]
    ///
    /// \see getGlobalVolume
    ///
    ////////////////////////////////////////////////////////////
    static void setGlobalVolume(float volume);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current value of the global volume
    ///
    /// \return Current global volume, in the range [0, 100]
    ///
    /// \see setGlobalVolume
    ///
    ////////////////////////////////////////////////////////////
    static float getGlobalVolume();

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the listener in the scene
    ///
    /// The default listener's position is (0, 0, 0).
    ///
    /// \param x X coordinate of the listener's position
    /// \param y Y coordinate of the listener's position
    /// \param z Z coordinate of the listener's position
    ///
    /// \see getPosition, setDirection
    ///
    ////////////////////////////////////////////////////////////
    static void setPosition(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the listener in the scene
    ///
    /// The default listener's position is (0, 0, 0).
    ///
    /// \param position New listener's position
    ///
    /// \see getPosition, setDirection
    ///
    ////////////////////////////////////////////////////////////
    static void setPosition(const Vector3f& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of the listener in the scene
    ///
    /// \return Listener's position
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f getPosition();

    ////////////////////////////////////////////////////////////
    /// \brief Set the forward vector of the listener in the scene
    ///
    /// The direction (also called "at vector") is the vector
    /// pointing forward from the listener's perspective. Together
    /// with the up vector, it defines the 3D orientation of the
    /// listener in the scene. The direction vector doesn't
    /// have to be normalized.
    /// The default listener's direction is (0, 0, -1).
    ///
    /// \param x X coordinate of the listener's direction
    /// \param y Y coordinate of the listener's direction
    /// \param z Z coordinate of the listener's direction
    ///
    /// \see getDirection, setUpVector, setPosition
    ///
    ////////////////////////////////////////////////////////////
    static void setDirection(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the forward vector of the listener in the scene
    ///
    /// The direction (also called "at vector") is the vector
    /// pointing forward from the listener's perspective. Together
    /// with the up vector, it defines the 3D orientation of the
    /// listener in the scene. The direction vector doesn't
    /// have to be normalized.
    /// The default listener's direction is (0, 0, -1).
    ///
    /// \param direction New listener's direction
    ///
    /// \see getDirection, setUpVector, setPosition
    ///
    ////////////////////////////////////////////////////////////
    static void setDirection(const Vector3f& direction);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current forward vector of the listener in the scene
    ///
    /// \return Listener's forward vector (not normalized)
    ///
    /// \see setDirection
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f getDirection();

    ////////////////////////////////////////////////////////////
    /// \brief Set the upward vector of the listener in the scene
    ///
    /// The up vector is the vector that points upward from the
    /// listener's perspective. Together with the direction, it
    /// defines the 3D orientation of the listener in the scene.
    /// The up vector doesn't have to be normalized.
    /// The default listener's up vector is (0, 1, 0). It is usually
    /// not necessary to change it, especially in 2D scenarios.
    ///
    /// \param x X coordinate of the listener's up vector
    /// \param y Y coordinate of the listener's up vector
    /// \param z Z coordinate of the listener's up vector
    ///
    /// \see getUpVector, setDirection, setPosition
    ///
    ////////////////////////////////////////////////////////////
    static void setUpVector(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the upward vector of the listener in the scene
    ///
    /// The up vector is the vector that points upward from the
    /// listener's perspective. Together with the direction, it
    /// defines the 3D orientation of the listener in the scene.
    /// The up vector doesn't have to be normalized.
    /// The default listener's up vector is (0, 1, 0). It is usually
    /// not necessary to change it, especially in 2D scenarios.
    ///
    /// \param upVector New listener's up vector
    ///
    /// \see getUpVector, setDirection, setPosition
    ///
    ////////////////////////////////////////////////////////////
    static void setUpVector(const Vector3f& upVector);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current upward vector of the listener in the scene
    ///
    /// \return Listener's upward vector (not normalized)
    ///
    /// \see setUpVector
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f getUpVector();
};

} // namespace TGE


#endif // TGE_LISTENER_HPP


////////////////////////////////////////////////////////////
/// \class TGE::Listener
/// \ingroup audio
///
/// The audio listener defines the global properties of the
/// audio environment, it defines where and how sounds and musics
/// are heard. If TGE::View is the eyes of the user, then TGE::Listener
/// is his ears (by the way, they are often linked together --
/// same position, orientation, etc.).
///
/// TGE::Listener is a simple interface, which allows to setup the
/// listener in the 3D audio environment (position, direction and
/// up vector), and to adjust the global volume.
///
/// Because the listener is unique in the scene, TGE::Listener only
/// contains static functions and doesn't have to be instantiated.
///
/// Usage example:
/// \code
/// // Move the listener to the position (1, 0, -5)
/// TGE::Listener::setPosition(1, 0, -5);
///
/// // Make it face the right axis (1, 0, 0)
/// TGE::Listener::setDirection(1, 0, 0);
///
/// // Reduce the global volume
/// TGE::Listener::setGlobalVolume(50);
/// \endcode
///
////////////////////////////////////////////////////////////
