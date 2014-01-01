#if defined(_WIN64) || defined(_WIN32)
  //#include "????"
#elif defined(__APPLE__)
  #include "/System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers/Events.h"
#elif defined(__linux)
  #include <linux/uinput.h>
#endif
