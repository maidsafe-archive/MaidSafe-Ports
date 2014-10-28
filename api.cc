#include "api.h"

#include "maidsafe/common/utils.h"

namespace sample_lib {

TestClass::TestClass()
  : answer_(98) {}

TestClass* TestClass::RawFactoryFunction() {
  return new TestClass{};
}

void TestClass::SetMyValue(int number) {
  answer_ = number;
}

int TestClass::GetMyValue() {
  return answer_;
}

int TestClass::GetRandomUInt32() {
  return maidsafe::RandomUint32() % 1000;
}

std::string TestClass::PrintMe() {
  return "You should see me";
}

std::string TestClass::HexEncode(std::string non_hex_input) {
  return maidsafe::HexEncode(non_hex_input);
}

std::string TestClass::HexDecode(std::string hex_input) {
  return maidsafe::HexDecode(hex_input);
}

/*
const int TestClass::GetStaticConstValue() {
  return kStaticNumber;
}

TestClass::TestClass()
  : answer_(98),
    callback_() {}

std::unique_ptr<TestClass> TestClass::FactoryFunction() {
  return std::unique_ptr<TestClass>(new TestClass());
}

std::future<int> TestClass::GetMyFutureValue() {
  return std::async(std::launch::async, [this]() { return this->answer_; });
}

void TestClass::ThrowCustomError() {
  throw CustomError();
}

void TestClass::ThrowStdError() {
  throw std::runtime_error("catch me plsssss");
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
*/

}
