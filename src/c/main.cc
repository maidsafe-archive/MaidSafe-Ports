#include <iostream>

#include "c_api.h"

int main() {
  std::cout << "Using SampleLib Functions:\n\n";

  auto client(CreateTestClass());
  SetValue(client, 500);

  std::cout << "TestClass::GetMyValue():\t" << GetValue(client) << '\n';

  for (int i = 0; i < 10; ++i) {
    std::cout << i + 1 << ") TestClass::GetRandomUInt32():\t" << GetRandomUInt32(client) << '\n';
  }

  std::cout << "\n==============\n";

  auto print_me(PrintMe(client));
  std::cout << print_me << "\n\n";

  std::cout << "\n==============\n";

  auto http_msg(GetHttpMessage(client, "192.168.92.130","8080"));
  std::cout  << http_msg << "\n\n";

  std::cout << "\n==============\n";

  std::cout  << std::boolalpha << CheckBootstrapsConnectivity(client)
             << std::noboolalpha << "\n\n";

  PrintLogMessage(client);

  std::cout << "\n==============\n";

  std::cout << "Destroying scalar pointers and client...\n";

  FreeScalarResource(print_me);
  FreeScalarResource(http_msg);
  FreeTestClass(client);

  std::cout << "Done...\n" << std::endl;
  return 0;
}

