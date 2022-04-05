/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Audio/Music.hpp>
#include <Tyrant/Audio/ALCheck.hpp>
#include <Tyrant/Audio/SoundFile.hpp>
#include <Tyrant/System/Lock.hpp>
#include <Tyrant/System/Log.hpp>
#include <fstream>


namespace TGE
{
////////////////////////////////////////////////////////////
Music::Music() :
m_file    (new priv::SoundFile),
m_duration()
{

}


////////////////////////////////////////////////////////////
Music::~Music()
{
    // We must stop before destroying the file
    stop();

    delete m_file;
}


////////////////////////////////////////////////////////////
bool Music::openFromFile(const std::string& filename)
{
    // First stop the music if it was already running
    stop();

    // Open the underlying sound file
    if (!m_file->openRead(filename))
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::openFromMemory(const void* data, std::size_t sizeInBytes)
{
    // First stop the music if it was already running
    stop();

    // Open the underlying sound file
    if (!m_file->openRead(data, sizeInBytes))
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::openFromStream(InputStream& stream)
{
    // First stop the music if it was already running
    stop();

    // Open the underlying sound file
    if (!m_file->openRead(stream))
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
Time Music::getDuration() const
{
    return m_duration;
}


////////////////////////////////////////////////////////////
bool Music::onGetData(SoundStream::Chunk& data)
{
    Lock lock(m_mutex);

    // Fill the chunk parameters
    data.samples     = &m_samples[0];
    data.sampleCount = m_file->read(&m_samples[0], m_samples.size());

    // Check if we have reached the end of the audio file
    return data.sampleCount == m_samples.size();
}


////////////////////////////////////////////////////////////
void Music::onSeek(Time timeOffset)
{
    Lock lock(m_mutex);

    m_file->seek(timeOffset);
}


////////////////////////////////////////////////////////////
void Music::initialize()
{
    // Compute the music duration
    m_duration = seconds(static_cast<float>(m_file->getSampleCount()) / m_file->getSampleRate() / m_file->getChannelCount());

    // Resize the internal buffer so that it can contain 1 second of audio samples
    m_samples.resize(m_file->getSampleRate() * m_file->getChannelCount());

    // Initialize the stream
    SoundStream::initialize(m_file->getChannelCount(), m_file->getSampleRate());
}

} // namespace TGE
