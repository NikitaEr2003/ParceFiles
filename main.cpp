
#include "MyParce.h"

int main() {
  try {
    MyParce object("../test.txt");
    auto temp_double = object.GetValue<double>("Section2.var4");
    auto temp_int = object.GetValue<int>("Section5.var2");
    auto temp_string = object.GetValue<std::string>("Section5.var1");
    std::cout << "double: " << temp_double << std::endl;
    std::cout << "int: " << temp_int << std::endl;
    std::cout << "string: " << temp_string << std::endl;
    std::cout << "result: " << temp_double + temp_int;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
std::cout << std::endl;
system("pause");
return 0;
}
