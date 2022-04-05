/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Audio/Listener.hpp>
#include <Tyrant/Audio/ALCheck.hpp>


namespace
{
    float        listenerVolume = 100.f;
    TGE::Vector3f listenerPosition (0.f, 0.f, 0.f);
    TGE::Vector3f listenerDirection(0.f, 0.f, -1.f);
    TGE::Vector3f listenerUpVector (0.f, 1.f, 0.f);
}


namespace TGE
{
////////////////////////////////////////////////////////////
void Listener::setGlobalVolume(float volume)
{
	if (volume != listenerVolume)
    {
        priv::ensureALInit();

        alCheck(alListenerf(AL_GAIN, volume * 0.01f));
        listenerVolume = volume;
    }
}


////////////////////////////////////////////////////////////
float Listener::getGlobalVolume()
{
    return listenerVolume;
}


////////////////////////////////////////////////////////////
void Listener::setPosition(float x, float y, float z)
{
    setPosition(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setPosition(const Vector3f& position)
{
    if (position != listenerPosition)
    {
        priv::ensureALInit();

        alCheck(alListener3f(AL_POSITION, position.x, position.y, position.z));
        listenerPosition = position;
    }
}


////////////////////////////////////////////////////////////
Vector3f Listener::getPosition()
{
    return listenerPosition;
}


////////////////////////////////////////////////////////////
void Listener::setDirection(float x, float y, float z)
{
    setDirection(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setDirection(const Vector3f& direction)
{
    if (direction != listenerDirection)
    {
        priv::ensureALInit();

        float orientation[] = {direction.x, direction.y, direction.z, listenerUpVector.x, listenerUpVector.y, listenerUpVector.z};
        alCheck(alListenerfv(AL_ORIENTATION, orientation));
        listenerDirection = direction;
    }
}


////////////////////////////////////////////////////////////
Vector3f Listener::getDirection()
{
    return listenerDirection;
}


////////////////////////////////////////////////////////////
void Listener::setUpVector(float x, float y, float z)
{
    setUpVector(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setUpVector(const Vector3f& upVector)
{
    if (upVector != listenerUpVector)
    {
        priv::ensureALInit();

        float orientation[] = {listenerDirection.x, listenerDirection.y, listenerDirection.z, upVector.x, upVector.y, upVector.z};
        alCheck(alListenerfv(AL_ORIENTATION, orientation));
        listenerUpVector = upVector;
    }
}


////////////////////////////////////////////////////////////
Vector3f Listener::getUpVector()
{
    return listenerUpVector;
}

} // namespace TGE
