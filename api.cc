#include "api.h"

namespace sample_lib {

TestClass::TestClass()
  : answer_(98),
    callback_() {}

std::unique_ptr<TestClass> TestClass::FactoryFunction() {
  return std::unique_ptr<TestClass>(new TestClass());
}

std::string TestClass::PrintMe() {
  return "You should see me";
}

const int TestClass::GetStaticConstValue() {
  return kStaticNumber;
}

void TestClass::SetMyValue(int number) {
  answer_ = number;
}

int TestClass::GetMyValue() {
  return answer_;
}

std::future<int> TestClass::GetMyFutureValue() {
  return std::async(std::launch::async, [this]() { return this->answer_; });
}

void TestClass::ThrowCustomError() {
  throw CustomError();
}

void TestClass::ThrowStdError() {
  throw std::exception("catch me plsssss");
}

std::shared_ptr<int> TestClass::GetSharedPtr() {
  return std::make_shared<int>(answer_);
}

std::unique_ptr<int> TestClass::GetUniquePtr() {
  return std::unique_ptr<int>(new int(4500));
}

void TestClass::SetCallback(std::function<void(int)> callback) {
  callback_ = callback;
}

void TestClass::InvokeCallback() {
  callback_(200);
}

}