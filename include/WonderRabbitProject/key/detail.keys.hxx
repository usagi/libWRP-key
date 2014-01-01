#pragma once

using keys_element_t = std::tuple<const char* const, const int>;

#if defined(_WIN64) || defined(_WIN32)
  #include "detail.keys.Windows.hxx"
#elif defined(__APPLE__)
  #include "detail.keys.OSX.hxx"
#elif defined(__linux)
  #include "detail.keys.Linux.hxx"
#endif
