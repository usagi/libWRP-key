#pragma once

#include <stdexcept>
#include <string>

#if defined(_WIN64) || defined(_WIN32)
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <rtccore.h>
#elif defined(__APPLE__)
  #include "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Headers/CGEvent.h"
  #include "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Headers/CGEventSource.h"
  #include "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Headers/CGEventTypes.h"
  #include "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Headers/CGRemoteOperation.h"
#elif defined(__linux)
  #include <cstring>
  #include <cstdio>
  #include <unistd.h>
  #include <fcntl.h>
#endif

namespace WonderRabbitProject
{
  namespace key
  {
    struct writer_t
#if __GNUC__ == 4 &&  __GNUC_MINOR__ < 7
      
#else
      final
#endif
    {
      enum class state_t: uint_fast8_t
      { down_and_up = 0
      , down  = 1
      , up    = 2
      , press = down_and_up
      };
      
      void operator()( const std::string& name, const state_t = state_t::press ) const
      { operator()(key_helper_t::instance().code(name)); }
      
      void operator()( const int code, const state_t state = state_t::press ) const
      {
        if(!key_helper_t::instance().is_valid(code))
          throw std::runtime_error("invalid key code");
#if defined(_WIN64) || defined(_WIN32)
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = code;
        input.ki.wScan = MapVirtualKey(input.ki.wVk, 0);
        input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
        if(state == state_t::up)
          input.ki.dwFlags |= KEYEVENTF_KEYUP;
        input.ki.time = 0;
        input.ki.dwExtraInfo = ::GetMessageExtraInfo();
        SendInput(1, &input, sizeof(INPUT));
        if(state == state_t::press)
        {
          input.ki.dwFlags |= KEYEVENTF_KEYUP;
          SendInput(1, &input, sizeof(INPUT));
        }
#elif defined(__APPLE__)
        const auto keycode = CGKeyCode(code);
        auto event_source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
        auto key_event = CGEventCreateKeyboardEvent(event_source, keycode, (state != state_t::up));
        //CGEventSetFlags(key_press_event, kCGEventFlagMaskShift);
        //CGEventSetFlags(key_release_event, kCGEventFlagMaskShift);
        constexpr auto event_tap_location = kCGHIDEventTap;
        CGEventPost(event_tap_location, key_event);
        if(state == state_t::press)
        {
          auto key_release_event = CGEventCreateKeyboardEvent(event_source, keycode, false);
          CGEventPost(event_tap_location, key_release_event);
          CFRelease(key_release_event);
        }
        CFRelease(key_event);
#elif defined(__linux)
        send_event(EV_KEY, code, (state != state_t::up));
        if(state == state_t::press)
          send_event(EV_KEY, code, 0);
        send_event(EV_SYN, SYN_REPORT, 0);
#endif
      }
    static const writer_t& instance()
    {
      static writer_t i;
      return i;
    }
    
    ~writer_t()
    {
#ifdef __linux
      finalize();
#endif
    }
    
  private:
    
    writer_t(const writer_t&) = delete;
    writer_t(writer_t&&) = delete;
    void operator=(const writer_t&) = delete;
    void operator=(writer_t&&) = delete;
    
    writer_t()
#ifdef __linux
      : fd(0)
#endif
    {
#ifdef __linux
      initialize();
#endif
    }
    
#ifdef __linux
    static constexpr const char* const default_device_name = "libWRP-key::writer";
    static constexpr int default_device_vendor_id  = 0x0000;
    static constexpr int default_device_product_id = 0x0000;
    static constexpr int default_device_version    = 0;

    void initialize()
    {
      initialize_open_device();
      initialize_create_device();
    }
    
    void initialize_open_device()
    {
      fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
      if(fd < 0)
      { throw std::runtime_error("/dev/uinput open error"); }
    }
    
    void initialize_create_device()
    {
      if(ioctl(fd, UI_SET_EVBIT , EV_KEY) < 0)
      { throw std::runtime_error("create device ioctl UI_SET_EVIT error"); }
      
      auto set_keybit = [&](const int code)
      {
        if(ioctl(fd, UI_SET_KEYBIT, code) < 0)
        { throw std::runtime_error("create device ioctl UI_KEYBIT error"); }
      };

      for(auto code = 0; code < 256; ++code)
        set_keybit(code);
      
      struct uinput_user_dev dev;
      memset(&dev, 0, sizeof(dev));
      
      snprintf(dev.name, UINPUT_MAX_NAME_SIZE, default_device_name);
      dev.id.bustype = BUS_USB;
      dev.id.vendor  = default_device_vendor_id;
      dev.id.product = default_device_product_id;
      dev.id.version = default_device_version;
      
      if(write(fd, &dev, sizeof(dev)) < 0)
      { throw std::runtime_error("write error"); }
      if(ioctl(fd, UI_DEV_CREATE) < 0)
      { throw std::runtime_error("ioctl error"); }
    }
    
    void finalize()
    {
      if(fd >= 0)
      {
        finalize_uinput_device();
        close(fd);
        fd = 0;
      }
    }
    
    void finalize_uinput_device()
    {
      if (ioctl(fd, UI_DEV_DESTROY) < 0)
      { throw std::runtime_error("finalize error"); }
    }
    
    void send_event(const int type, const int code, const int value) const
    {
      struct input_event e;
      memset(&e, 0, sizeof(e));
      
      gettimeofday(&e.time, nullptr);
      
      e.type = type;
      e.code = code;
      e.value = value;
      
      if(write(fd, &e, sizeof(e)) < 0)
      { throw std::runtime_error("send event write error"); }
    }
    
    int fd;
#endif
    };
  }
}
