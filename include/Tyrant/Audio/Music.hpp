/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_MUSIC_HPP
#define TGE_MUSIC_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Audio/SoundStream.hpp>
#include <Tyrant/System/Mutex.hpp>
#include <Tyrant/System/Time.hpp>
#include <string>
#include <vector>


namespace TGE
{
namespace priv
{
    class SoundFile;
}

class InputStream;

////////////////////////////////////////////////////////////
/// \brief Streamed music played from an audio file
///
////////////////////////////////////////////////////////////
class TGE_API Music : public SoundStream
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Music();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Music();

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file
    ///
    /// This function doesn't start playing the music (call play()
    /// to do so).
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    ///
    /// \param filename Path of the music file to open
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see openFromMemory, openFromStream
    ///
    ////////////////////////////////////////////////////////////
    bool openFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file in memory
    ///
    /// This function doesn't start playing the music (call play()
    /// to do so).
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    /// Since the music is not loaded completely but rather streamed
    /// continuously, the \a data must remain available as long as the
    /// music is playing (ie. you can't deallocate it right after calling
    /// this function).
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see openFromFile, openFromStream
    ///
    ////////////////////////////////////////////////////////////
    bool openFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file in a custom stream
    ///
    /// This function doesn't start playing the music (call play()
    /// to do so).
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    /// Since the music is not loaded completely but rather streamed
    /// continuously, the \a stream must remain alive as long as the
    /// music is playing (ie. you can't destroy it right after calling
    /// this function).
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see openFromFile, openFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool openFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Get the total duration of the music
    ///
    /// \return Music duration
    ///
    ////////////////////////////////////////////////////////////
    Time getDuration() const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Request a new chunk of audio samples from the stream source
    ///
    /// This function fills the chunk from the next samples
    /// to read from the audio file.
    ///
    /// \param data Chunk of data to fill
    ///
    /// \return True to continue playback, false to stop
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onGetData(Chunk& data);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source
    ///
    /// \param timeOffset New playing position, from the beginning of the music
    ///
    ////////////////////////////////////////////////////////////
    virtual void onSeek(Time timeOffset);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the internal state after loading a new music
    ///
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::SoundFile*   m_file;     ///< Sound file
    Time               m_duration; ///< Music duration
    std::vector<Int16> m_samples;  ///< Temporary buffer of samples
    Mutex              m_mutex;    ///< Mutex protecting the data
};

} // namespace TGE


#endif // TGE_MUSIC_HPP


////////////////////////////////////////////////////////////
/// \class TGE::Music
/// \ingroup audio
///
/// Musics are sounds that are streamed rather than completely
/// loaded in memory. This is especially useful for compressed
/// musics that usually take hundreds of MB when they are
/// uncompressed: by streaming it instead of loading it entirely,
/// you avoid saturating the memory and have almost no loading delay.
///
/// Apart from that, a TGE::Music has almost the same features as
/// the TGE::SoundBuffer / TGE::Sound pair: you can play/pause/stop
/// it, request its parameters (channels, sample rate), change
/// the way it is played (pitch, volume, 3D position, ...), etc.
///
/// As a sound stream, a music is played in its own thread in order
/// not to block the rest of the program. This means that you can
/// leave the music alone after calling play(), it will manage itself
/// very well.
///
/// Usage example:
/// \code
/// // Declare a new music
/// TGE::Music music;
///
/// // Open it from an audio file
/// if (!music.openFromFile("music.ogg"))
/// {
///     // error...
/// }
///
/// // Change some parameters
/// music.setPosition(0, 1, 10); // change its 3D position
/// music.setPitch(2);           // increase the pitch
/// music.setVolume(50);         // reduce the volume
/// music.setLoop(true);         // make it loop
///
/// // Play it
/// music.play();
/// \endcode
///
/// \see TGE::Sound, TGE::SoundStream
///
////////////////////////////////////////////////////////////
