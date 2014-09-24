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
  void SetMyValue(int number);
  int GetMyValue();

  int GetRandomUInt32();

  template <typename U>
  U GetTemplateValue(U x) {
    return x;
  }

  std::string PrintMe();

  /*
  const int GetStaticConstValue();

  static const int kPublicStaticNumber = 99;
  const static int kPublicStaticNumber = 99;

  static std::unique_ptr<TestClass> FactoryFunction();

  std::future<int> GetMyFutureValue();

  void ThrowCustomError();
  void ThrowStdError();

  std::shared_ptr<int> GetSharedPtr();

  std::unique_ptr<int> GetUniquePtr();

  void SetCallback(std::function<void(int)> callback);
  void InvokeCallback();
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
