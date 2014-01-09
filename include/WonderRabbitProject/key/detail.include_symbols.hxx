#if defined(_WIN64) || defined(_WIN32)
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windef.h>
  #include <winbase.h>
  #include <winuser.h>
#elif defined(__APPLE__)
  #include "/System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h"
#elif defined(__linux)
  #include <linux/uinput.h>
#endif
