/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_SOUNDBUFFERRECORDER_HPP
#define TGE_SOUNDBUFFERRECORDER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Audio/SoundBuffer.hpp>
#include <Tyrant/Audio/SoundRecorder.hpp>
#include <vector>


namespace TGE
{
////////////////////////////////////////////////////////////
/// \brief Specialized SoundRecorder which stores the captured
///        audio data into a sound buffer
///
////////////////////////////////////////////////////////////
class TGE_API SoundBufferRecorder : public SoundRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the sound buffer containing the captured audio data
    ///
    /// The sound buffer is valid only after the capture has ended.
    /// This function provides a read-only access to the internal
    /// sound buffer, but it can be copied if you need to
    /// make any modification to it.
    ///
    /// \return Read-only access to the sound buffer
    ///
    ////////////////////////////////////////////////////////////
    const SoundBuffer& getBuffer() const;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Start capturing audio data
    ///
    /// \return True to start the capture, or false to abort it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onStart();

    ////////////////////////////////////////////////////////////
    /// \brief Process a new chunk of recorded samples
    ///
    /// \param samples     Pointer to the new chunk of recorded samples
    /// \param sampleCount Number of samples pointed by \a samples
    ///
    /// \return True to continue the capture, or false to stop it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onProcessSamples(const Int16* samples, std::size_t sampleCount);

    ////////////////////////////////////////////////////////////
    /// \brief Stop capturing audio data
    ///
    ////////////////////////////////////////////////////////////
    virtual void onStop();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Int16> m_samples; ///< Temporary sample buffer to hold the recorded data
    SoundBuffer        m_buffer;  ///< Sound buffer that will contain the recorded data
};

} // namespace TGE

#endif // TGE_SOUNDBUFFERRECORDER_HPP


////////////////////////////////////////////////////////////
/// \class TGE::SoundBufferRecorder
/// \ingroup audio
///
/// TGE::SoundBufferRecorder allows to access a recorded sound
/// through a TGE::SoundBuffer, so that it can be played, saved
/// to a file, etc.
///
/// It has the same simple interface as its base class (start(), stop())
/// and adds a function to retrieve the recorded sound buffer
/// (getBuffer()).
///
/// As usual, don't forget to call the isAvailable() function
/// before using this class (see TGE::SoundRecorder for more details
/// about this).
///
/// Usage example:
/// \code
/// if (TGE::SoundBufferRecorder::isAvailable())
/// {
///     // Record some audio data
///     TGE::SoundBufferRecorder recorder;
///     recorder.start();
///     ...
///     recorder.stop();
///
///     // Get the buffer containing the captured audio data
///     const TGE::SoundBuffer& buffer = recorder.getBuffer();
///
///     // Save it to a file (for example...)
///     buffer.saveToFile("my_record.ogg");
/// }
/// \endcode
///
/// \see TGE::SoundRecorder
///
////////////////////////////////////////////////////////////
