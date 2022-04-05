/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/VideoModeImpl.hpp>
#include <Tyrant/Window/Unix/Display.hpp>
#include <Tyrant/System/Log.hpp>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    std::vector<VideoMode> modes;

    // Open a connection with the X server
    Display* display = OpenDisplay();
    if (display)
    {
        // Retrieve the default screen number
        int screen = DefaultScreen(display);

        // Check if the XRandR extension is present
        int version;
        if (XQueryExtension(display, "RANDR", &version, &version, &version))
        {
            // Get the current configuration
            XRRScreenConfiguration* config = XRRGetScreenInfo(display, RootWindow(display, screen));
            if (config)
            {
                // Get the available screen sizes
                int nbSizes;
                XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);
                if (sizes && (nbSizes > 0))
                {
                    // Get the list of supported depths
                    int nbDepths = 0;
                    int* depths = XListDepths(display, screen, &nbDepths);
                    if (depths && (nbDepths > 0))
                    {
                        // Combine depths and sizes to fill the array of supported modes
                        for (int i = 0; i < nbDepths; ++i)
                        {
                            for (int j = 0; j < nbSizes; ++j)
                            {
                                // Convert to VideoMode
                                VideoMode mode(sizes[j].width, sizes[j].height, depths[i]);

                                // Add it only if it is not already in the array
                                if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                                    modes.push_back(mode);
                            }
                        }

                        // Free the array of depths
                        XFree(depths);
                    }
                }

                // Free the configuration instance
                XRRFreeScreenConfigInfo(config);
            }
            else
            {
                // Failed to get the screen configuration
                Log() << "Failed to retrieve the screen configuration while trying to get the supported video modes" << std::endl;
            }
        }
        else
        {
            // XRandr extension is not supported : we cannot get the video modes
            Log() << "Failed to use the XRandR extension while trying to get the supported video modes" << std::endl;
        }

        // Close the connection with the X server
        CloseDisplay(display);
    }
    else
    {
        // We couldn't connect to the X server
        Log() << "Failed to connect to the X server while trying to get the supported video modes" << std::endl;
    }

    return modes;
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    VideoMode desktopMode;

    // Open a connection with the X server
    Display* display = OpenDisplay();
    if (display)
    {
        // Retrieve the default screen number
        int screen = DefaultScreen(display);

        // Check if the XRandR extension is present
        int version;
        if (XQueryExtension(display, "RANDR", &version, &version, &version))
        {
            // Get the current configuration
            XRRScreenConfiguration* config = XRRGetScreenInfo(display, RootWindow(display, screen));
            if (config)
            {
                // Get the current video mode
                Rotation currentRotation;
                int currentMode = XRRConfigCurrentConfiguration(config, &currentRotation);

                // Get the available screen sizes
                int nbSizes;
                XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);
                if (sizes && (nbSizes > 0))
                    desktopMode = VideoMode(sizes[currentMode].width, sizes[currentMode].height, DefaultDepth(display, screen));

                // Free the configuration instance
                XRRFreeScreenConfigInfo(config);
            }
            else
            {
                // Failed to get the screen configuration
                Log() << "Failed to retrieve the screen configuration while trying to get the desktop video modes" << std::endl;
            }
        }
        else
        {
            // XRandr extension is not supported : we cannot get the video modes
            Log() << "Failed to use the XRandR extension while trying to get the desktop video modes" << std::endl;
        }

        // Close the connection with the X server
        CloseDisplay(display);
    }
    else
    {
        // We couldn't connect to the X server
        Log() << "Failed to connect to the X server while trying to get the desktop video modes" << std::endl;
    }

    return desktopMode;
}

} // namespace priv

} // namespace TGE
