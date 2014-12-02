#ifndef API_H_
#define API_H_

#include <memory>
#include <future>
#include <limits>
#include <string>
#include <fstream>
#include <functional>

namespace sample_lib {

class CustomError {};

class TestClass {
 public:
  TestClass(const TestClass&) = delete;
  TestClass& operator=(TestClass&) = delete;

  ~TestClass() = default;

  static TestClass* RawFactoryFunction();
  void SetMyValue(int number);
  int GetMyValue();

  int GetRandomUInt32();

  template <typename U>
  U GetTemplateValue(U x) {
    return x;
  }

  std::string PrintMe();
  
  bool CheckBootstrapsConnectivity();

  void PrintLogMessage();

 private:
  TestClass();

  int answer_;
  static const int kStaticNumber = 42;
};

}

#endif  // API_H_
