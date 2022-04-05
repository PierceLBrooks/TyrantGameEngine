/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Audio/SoundRecorder.hpp>
#include <Tyrant/Audio/AudioDevice.hpp>
#include <Tyrant/Audio/ALCheck.hpp>
#include <Tyrant/System/Sleep.hpp>
#include <Tyrant/System/Log.hpp>
#include <cstring>

#ifdef _MSC_VER
    #pragma warning(disable : 4355) // 'this' used in base member initializer list
#endif


namespace
{
    ALCdevice* captureDevice = NULL;
}

namespace TGE
{
////////////////////////////////////////////////////////////
SoundRecorder::SoundRecorder() :
m_thread            (&SoundRecorder::record, this),
m_sampleRate        (0),
m_processingInterval(milliseconds(100)),
m_isCapturing       (false)
{
    priv::ensureALInit();

    // Set the device name to the default device
    m_deviceName = getDefaultDevice();
}


////////////////////////////////////////////////////////////
SoundRecorder::~SoundRecorder()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool SoundRecorder::start(unsigned int sampleRate)
{
    // Check if the device can do audio capture
    if (!isAvailable())
    {
        Log() << "Failed to start capture : your system cannot capture audio data (call SoundRecorder::isAvailable to check it)" << std::endl;
        return false;
    }

    // Check that another capture is not already running
    if (captureDevice)
    {
        Log() << "Trying to start audio capture, but another capture is already running" << std::endl;
        return false;
    }

    // Open the capture device for capturing 16 bits mono samples
    captureDevice = alcCaptureOpenDevice(m_deviceName.c_str(), sampleRate, AL_FORMAT_MONO16, sampleRate);
    if (!captureDevice)
    {
        Log() << "Failed to open the audio capture device with the name: " << m_deviceName << std::endl;
        return false;
    }

    // Clear the array of samples
    m_samples.clear();

    // Store the sample rate
    m_sampleRate = sampleRate;

    // Notify derived class
    if (onStart())
    {
        // Start the capture
        alcCaptureStart(captureDevice);

        // Start the capture in a new thread, to avoid blocking the main thread
        m_isCapturing = true;
        m_thread.launch();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void SoundRecorder::stop()
{
    // Stop the capturing thread
    m_isCapturing = false;
    m_thread.wait();

    // Notify derived class
    onStop();
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::getSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
std::vector<std::string> SoundRecorder::getAvailableDevices()
{
    std::vector<std::string> deviceNameList;

    const ALchar *deviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    if (deviceList)
    {
        while (*deviceList)
        {
            deviceNameList.push_back(deviceList);
            deviceList += std::strlen(deviceList) + 1;
        }
    }

    return deviceNameList;
}


////////////////////////////////////////////////////////////
std::string SoundRecorder::getDefaultDevice()
{
    return alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
}


////////////////////////////////////////////////////////////
bool SoundRecorder::setDevice(const std::string& name)
{
    // Store the device name
    if (name.empty())
        m_deviceName = getDefaultDevice();
    else
        m_deviceName = name;

    if (m_isCapturing)
    {
        // Stop the capturing thread
        m_isCapturing = false;
        m_thread.wait();

        // Open the requested capture device for capturing 16 bits mono samples
        captureDevice = alcCaptureOpenDevice(name.c_str(), m_sampleRate, AL_FORMAT_MONO16, m_sampleRate);
        if (!captureDevice)
        {
            // Notify derived class
            onStop();

            Log() << "Failed to open the audio capture device with the name: " << m_deviceName << std::endl;
            return false;
        }

        // Start the capture
        alcCaptureStart(captureDevice);

        // Start the capture in a new thread, to avoid blocking the main thread
        m_isCapturing = true;
        m_thread.launch();
    }

    return true;
}


////////////////////////////////////////////////////////////
const std::string& SoundRecorder::getDevice() const
{
    return m_deviceName;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::isAvailable()
{
    return (priv::AudioDevice::isExtensionSupported("ALC_EXT_CAPTURE") != AL_FALSE) ||
           (priv::AudioDevice::isExtensionSupported("ALC_EXT_capture") != AL_FALSE); // "bug" in Mac OS X 10.5 and 10.6
}


////////////////////////////////////////////////////////////
void SoundRecorder::setProcessingInterval(TGE::Time interval)
{
    m_processingInterval = interval;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::onStart()
{
    // Nothing to do
    return true;
}


////////////////////////////////////////////////////////////
void SoundRecorder::onStop()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void SoundRecorder::record()
{
    while (m_isCapturing)
    {
        // Process available samples
        processCapturedSamples();

        // Don't bother the CPU while waiting for more captured data
        sleep(m_processingInterval);
    }

    // Capture is finished : clean up everything
    cleanup();
}


////////////////////////////////////////////////////////////
void SoundRecorder::processCapturedSamples()
{
    // Get the number of samples available
    ALCint samplesAvailable;
    alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);

    if (samplesAvailable > 0)
    {
        // Get the recorded samples
        m_samples.resize(samplesAvailable);
        alcCaptureSamples(captureDevice, &m_samples[0], samplesAvailable);

        // Forward them to the derived class
        if (!onProcessSamples(&m_samples[0], m_samples.size()))
        {
            // The user wants to stop the capture
            m_isCapturing = false;
        }
    }
}


////////////////////////////////////////////////////////////
void SoundRecorder::cleanup()
{
    // Stop the capture
    alcCaptureStop(captureDevice);

    // Get the samples left in the buffer
    processCapturedSamples();

    // Close the device
    alcCaptureCloseDevice(captureDevice);
    captureDevice = NULL;
}

} // namespace TGE
