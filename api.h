#ifndef API_H_
#define API_H_

#include <memory>
#include <future>
#include <limits>
#include <string>
#include <functional>

namespace sample_lib {

class CustomError {};

class TestClass {
 public:
  TestClass(const TestClass&) = delete;
  TestClass& operator=(TestClass&) = delete;

  ~TestClass() = default;

  static TestClass* RawFactoryFunction();
  const int GetStaticConstValue();
  void SetMyValue(int number);
  int GetMyValue();
  static const int kPublicStaticNumber = 99;

  /*
  const static int kPublicStaticNumber = 99;

  static std::unique_ptr<TestClass> FactoryFunction();

  std::string PrintMe();

  std::future<int> GetMyFutureValue();

  void ThrowCustomError();
  void ThrowStdError();

  std::shared_ptr<int> GetSharedPtr();

  std::unique_ptr<int> GetUniquePtr();

  void SetCallback(std::function<void(int)> callback);
  void InvokeCallback();

  template <typename U>
  U GetTemplateValue(U x) {
    return x;
  }
  */

 private:
  TestClass();

  int answer_;
  static const int kStaticNumber = 42;

  /*
  std::function<void(int)> callback_;
  const static int kStaticNumber = 42;
  */
};

}

#endif  // API_H_
