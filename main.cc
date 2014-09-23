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


  /*
  auto client(sample_lib::TestClass::FactoryFunction());

  std::cout << "TestClass::kPublicStaticNumber:\t" << client->kPublicStaticNumber << '\n';
  std::cout << "TestClass::GetStaticConstValue():\t" << client->GetStaticConstValue() << '\n';
  std::cout << "TestClass::PrintMe():\t" << client->PrintMe() << '\n';
  std::cout << "TestClass::GetMyFutureValue():\t" << client->GetMyFutureValue().get() << '\n';

  try {
    std::cout << "TestClass::ThrowCustomError():\t";
    client->ThrowCustomError();
  } catch (const sample_lib::CustomError&) {
    std::cout << "Caught CustomError\n";
  }

  try {
    std::cout << "TestClass::ThrowStdError():\t";
    client->ThrowStdError();
  } catch (const std::exception& ex) {
    std::cout << "Caught std::exception: " << ex.what() << '\n';
  }

  std::shared_ptr<int> shared_ptr(client->GetSharedPtr());
  std::cout << "TestClass::GetSharedPtr():\t" << *shared_ptr << '\n';

  std::unique_ptr<int> unique_ptr(client->GetUniquePtr());
  std::cout << "TestClass::GetUniquePtr():\t" << *unique_ptr << '\n';

  std::cout << "TestClass::GetTemplateValue<std::string>():\t" << client->GetTemplateValue<std::string>("Test Message") << '\n';

  client->SetCallback([](int value){ std::cout << "callback invoked with: " << value << '\n'; });
  std::cout << "TestClass::InvokeCallback():\t";
  client->InvokeCallback();
  std::cout << '\n';
  */

  std::cout << "Done...\n\n";
  return 0;
}
