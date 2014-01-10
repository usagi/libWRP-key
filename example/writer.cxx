#include <iostream>
#include <thread>
#include <chrono>
#include <WonderRabbitProject/key.hxx>

int main()
{
  const auto& writer = WonderRabbitProject::key::writer_t::instance();
  
  for(auto n = 3; n; --n)
  {
    std::cerr << n << "\n";
#if __GNUC__ == 4 &&  __GNUC_MINOR__ < 7
  #if defined(_WIN32) || defined(_WIN64)
    Sleep(1000);
  #elif defined(__linux) || defined(__APPLE__)
    sleep(1);
  #endif
#else
    std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
  }
  
  writer("a");
  writer("b");
  writer("c");
  writer("space");
  writer("d");
  writer("e");
  writer("f");
}