/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_AUDIODEVICE_HPP
#define TGE_AUDIODEVICE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <set>
#include <string>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief High-level wrapper around the audio API, it manages
///        the creation and destruction of the audio device and
///        context and stores the device capabilities
///
////////////////////////////////////////////////////////////
class AudioDevice
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    AudioDevice();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~AudioDevice();

    ////////////////////////////////////////////////////////////
    /// \brief Check if an OpenAL extension is supported
    ///
    /// This functions automatically finds whether it
    /// is an AL or ALC extension, and calls the corresponding
    /// function.
    ///
    /// \param extension Name of the extension to test
    ///
    /// \return True if the extension is supported, false if not
    ///
    ////////////////////////////////////////////////////////////
    static bool isExtensionSupported(const std::string& extension);

    ////////////////////////////////////////////////////////////
    /// \brief Get the OpenAL format that matches the given number of channels
    ///
    /// \param channelCount Number of channels
    ///
    /// \return Corresponding format
    ///
    ////////////////////////////////////////////////////////////
    static int getFormatFromChannelCount(unsigned int channelCount);
};

} // namespace priv

} // namespace TGE


#endif // TGE_AUDIODEVICE_HPP
