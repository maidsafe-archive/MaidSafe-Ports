#include <iostream>
#include "api.h"

int main() {
  std::cout << "Using SampleLib Functions:\n\n";

  auto client(sample_lib::TestClass::RawFactoryFunction());
  client->SetMyValue(500);

  std::cout << "TestClass::GetMyValue():\t" << client->GetMyValue() << '\n';

  for (int i = 0; i < 10; ++i) {
    std::cout << i + 1 << ") TestClass::GetRandomUInt32():\t" << client->GetRandomUInt32() << '\n';
  }

  std::cout << "\n==============\n";

  std::cout << client->PrintMe() << "\n\n";

  std::cout << "\n==============\n";

  std::cout  << client->CheckBootstrapsConnectivity() << "\n\n";

  client->PrintLogMessage();

  std::cout << "Done...\n\n";
  return 0;
}
