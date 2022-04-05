/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Window/OSX/HIDInputManager.hpp>
#include <Tyrant/Window/OSX/HIDJoystickManager.hpp>

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Using a custom run loop mode solve some issues that appears when TGE
    // is used with Cocoa.
    const CFStringRef RunLoopMode = CFSTR("TGE_RUN_LOOP_MODE");
}


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
HIDJoystickManager& HIDJoystickManager::getInstance()
{
    static HIDJoystickManager manager;
    return manager;
}


////////////////////////////////////////////////////////////
unsigned int HIDJoystickManager::getJoystickCount()
{
    update();
    return m_joystickCount;
}


////////////////////////////////////////////////////////////
CFSetRef HIDJoystickManager::copyJoysticks()
{
    CFSetRef devices = IOHIDManagerCopyDevices(m_manager);
    return devices;
}


////////////////////////////////////////////////////////////
HIDJoystickManager::HIDJoystickManager() :
m_manager(0),
m_joystickCount(0)
{
    m_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

    CFDictionaryRef mask0 = HIDInputManager::copyDevicesMask(kHIDPage_GenericDesktop,
                                                             kHIDUsage_GD_Joystick);

    CFDictionaryRef mask1 = HIDInputManager::copyDevicesMask(kHIDPage_GenericDesktop,
                                                             kHIDUsage_GD_GamePad);

    CFDictionaryRef maskArray[2];
    maskArray[0] = mask0;
    maskArray[1] = mask1;

    CFArrayRef mask = CFArrayCreate(NULL, (const void**)maskArray, 2, NULL);

    IOHIDManagerSetDeviceMatchingMultiple(m_manager, mask);
    CFRelease(mask);
    CFRelease(mask1);
    CFRelease(mask0);


    IOHIDManagerRegisterDeviceMatchingCallback(m_manager, pluggedIn, this);
    IOHIDManagerRegisterDeviceRemovalCallback(m_manager, pluggedOut, this);

    IOHIDManagerScheduleWithRunLoop(m_manager,
                                    CFRunLoopGetCurrent(),
                                    RunLoopMode);

    IOHIDManagerOpen(m_manager, kIOHIDOptionsTypeNone);
}


////////////////////////////////////////////////////////////
HIDJoystickManager::~HIDJoystickManager()
{
    IOHIDManagerUnscheduleFromRunLoop(m_manager,
                                      CFRunLoopGetCurrent(),
                                      RunLoopMode);

    IOHIDManagerRegisterDeviceMatchingCallback(m_manager, NULL, 0);
    IOHIDManagerRegisterDeviceRemovalCallback(m_manager, NULL, 0);

    IOHIDManagerClose(m_manager, kIOHIDOptionsTypeNone);
}


////////////////////////////////////////////////////////////
void HIDJoystickManager::update()
{
    SInt32 status = kCFRunLoopRunHandledSource;

    while (status == kCFRunLoopRunHandledSource)
    {
        status = CFRunLoopRunInMode(RunLoopMode, 0, true);
    }
}


////////////////////////////////////////////////////////////
void HIDJoystickManager::pluggedIn(void* context, IOReturn, void*, IOHIDDeviceRef)
{
    HIDJoystickManager* manager = (HIDJoystickManager*)context;
    manager->m_joystickCount++;
}


////////////////////////////////////////////////////////////
void HIDJoystickManager::pluggedOut(void* context, IOReturn, void*, IOHIDDeviceRef)
{
    HIDJoystickManager* manager = (HIDJoystickManager*)context;
    manager->m_joystickCount--;
}


} // namespace priv

} // namespace TGE

