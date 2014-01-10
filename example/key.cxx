#include <WonderRabbitProject/key.hxx>

namespace
{
  using namespace WonderRabbitProject::key;
  void usage()
  {
    std::cout
      << "=== libWRP-key / print list of defined key ===" "\n"
        "\n"
        "  usage 1: `key`            ... print list of defined key" "\n"
        "  usage 2: `key <key name>` ... <key name> test" "\n"
        "             ex.) key Space" "\n"
        "\n"
      ;
    
    const auto& kh = key_helper_t::instance();
    
    const auto data = kh.data_sorted_by_code();
    for(const auto& p : data)
      std::cout
      << "  " << p.first << "(" << p.second << ")"
      << (kh.is_valid(p.first) ? "" : " [invalid]" )
      << std::endl
      ;
    
  }

  void test(const std::string& key_name)
  {
    std::cout
      << "=== libWRP-key / test key name(" << key_name << ") ===" "\n"
         "\n"
      ;
    
    const auto& kh = key_helper_t::instance();
    const auto code = kh.code(key_name);
    
    std::cout << "  name :" << code << " --> " << kh.name(code) << std::endl;
    
    std::cout << "  names:" << code << " -->";
    for(const auto& name : kh.names(code))
      std::cout << " " << name;
    std::cout << std::endl;
  }
}

int main(const int n, const char* const* const vs)
{
  (n < 2)
    ? usage()
    : test(vs[1])
    ;
}
