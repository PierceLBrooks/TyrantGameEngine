/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/OSX/TGEContext.hpp>
#include <Tyrant/Window/OSX/WindowImplCocoa.hpp>
#include <Tyrant/System/Log.hpp>

namespace TGE
{
namespace priv
{


////////////////////////////////////////////////////////////
TGEContext::TGEContext(TGEContext* shared) :
m_view(0),
m_window(0)
{
    // Create the context
    createContext(shared,
                  VideoMode::getDesktopMode().bitsPerPixel,
                  ContextSettings(0, 0, 0));
}


////////////////////////////////////////////////////////////
TGEContext::TGEContext(TGEContext* shared, const ContextSettings& settings,
                     const WindowImpl* owner, unsigned int bitsPerPixel) :
m_view(0),
m_window(0)
{
    // Create the context.
    createContext(shared, bitsPerPixel, settings);

    // Apply context.
    const WindowImplCocoa* ownerCocoa = static_cast<const WindowImplCocoa*>(owner);
    ownerCocoa->applyContext(m_context);
}


////////////////////////////////////////////////////////////
TGEContext::TGEContext(TGEContext* shared, const ContextSettings& settings,
                     unsigned int width, unsigned int height) :
m_view(0),
m_window(0)
{
    // Ensure the process is setup in order to create a valid window.
    WindowImplCocoa::setUpProcess();

    // Create the context.
    createContext(shared, VideoMode::getDesktopMode().bitsPerPixel, settings);

    // Create a dummy window/view pair (hidden) and assign it our context.
    m_window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, width, height)
                                           styleMask:NSBorderlessWindowMask
                                             backing:NSBackingStoreBuffered
                                               defer:NO]; // Don't defer it!
    m_view = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
    [m_window setContentView:m_view];
    [m_view setOpenGLContext:m_context];
    [m_context setView:m_view];
}


////////////////////////////////////////////////////////////
TGEContext::~TGEContext()
{
@autoreleasepool
{
    [m_context clearDrawable];

    m_context = nil;
    m_view = nil;
    m_window = nil;
} // pool
}


////////////////////////////////////////////////////////////
bool TGEContext::makeCurrent()
{
    [m_context makeCurrentContext];
    return m_context == [NSOpenGLContext currentContext]; // Should be true.
}


////////////////////////////////////////////////////////////
void TGEContext::display()
{
    [m_context flushBuffer];
}


////////////////////////////////////////////////////////////
void TGEContext::setVerticalSyncEnabled(bool enabled)
{
    GLint swapInterval = enabled ? 1 : 0;

    [m_context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
}


////////////////////////////////////////////////////////////
void TGEContext::createContext(TGEContext* shared,
                              unsigned int bitsPerPixel,
                              const ContextSettings& settings)
{
@autoreleasepool
{
    // Choose the attributes of OGL context.
    std::vector<NSOpenGLPixelFormatAttribute> attrs;
    attrs.reserve(20); // max attributes (estimation).

    // These casts are safe. C++ is much more strict than Obj-C.

    attrs.push_back(NSOpenGLPFAClosestPolicy);
    attrs.push_back(NSOpenGLPFADoubleBuffer);

    if (bitsPerPixel > 24)
    {
        attrs.push_back(NSOpenGLPFAAlphaSize);
        attrs.push_back((NSOpenGLPixelFormatAttribute)8);
    }

    attrs.push_back(NSOpenGLPFADepthSize);
    attrs.push_back((NSOpenGLPixelFormatAttribute)settings.depthBits);

    attrs.push_back(NSOpenGLPFAStencilSize);
    attrs.push_back((NSOpenGLPixelFormatAttribute)settings.stencilBits);

    if (settings.antialiasingLevel > 0)
    {
        /*
         * Antialiasing techniques are described in the
         * "OpenGL Programming Guide for Mac OS X" document.
         *
         * According to this document, the specification currently allows
         * only one multisample buffer.
         *
         * The document also states that software renderers should be avoided
         * because antialisaing techniques are very slow with them.
         */

        // Prefer multisampling over supersampling
        attrs.push_back(NSOpenGLPFAMultisample);

        // Only one buffer is currently available
        attrs.push_back(NSOpenGLPFASampleBuffers);
        attrs.push_back((NSOpenGLPixelFormatAttribute)1);

        // Antialiasing level
        attrs.push_back(NSOpenGLPFASamples);
        attrs.push_back((NSOpenGLPixelFormatAttribute)settings.antialiasingLevel);

        // No software renderer - only hardware renderer
        attrs.push_back(NSOpenGLPFAAccelerated);
    }

    // Support for OpenGL 3.2 on Mac OS X Lion and later:
    // TGE 2 Graphics module uses some OpenGL features that are deprecated
    // in OpenGL 3.2 and that are no more available with core context.
    // Therefore the Graphics module won't work as expected.

    // 2.x are mapped to 2.1 since Apple only support that legacy version.
    // >=3.0 are mapped to a 3.2 core profile.
    bool legacy = settings.majorVersion < 3;

    if (legacy)
    {
        attrs.push_back(NSOpenGLPFAOpenGLProfile);
        attrs.push_back(NSOpenGLProfileVersionLegacy);
    }
    else
    {
        attrs.push_back(NSOpenGLPFAOpenGLProfile);
        attrs.push_back(NSOpenGLProfileVersion3_2Core);
    }

    attrs.push_back((NSOpenGLPixelFormatAttribute)0); // end of array

    // Create the pixel format.
    NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attrs[0]];

    if (pixFmt == nil)
    {
        TGE::Log() << "Error. Unable to find a suitable pixel format." << std::endl;
        return;
    }

    // Use the shared context if one is given.
    NSOpenGLContext* sharedContext = shared != NULL ? shared->m_context : nil;

    // Create the context.
    m_context = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                           shareContext:sharedContext];

    if (m_context == nil)
    {
        TGE::Log() << "Error. Unable to create the context. Retrying without shared context." << std::endl;
        m_context = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                             shareContext:nil];

        if (m_context == nil)
            TGE::Log() << "Error. Unable to create the context." << std::endl;
        else
            TGE::Log() << "Warning. New context created without shared context." << std::endl;
    }

    // Save the settings. (OpenGL version is updated elsewhere.)
    m_settings = settings;
} // pool
}

} // namespace priv

} // namespace TGE

