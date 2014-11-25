#include "api.h"

#include <exception>
#include <future>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <sstream>

#include "maidsafe/common/utils.h"
#include "boost/asio/ip/udp.hpp"
#include "maidsafe/common/node_id.h"
#include "maidsafe/common/utils.h"
#include "maidsafe/rudp/managed_connections.h"
#include "maidsafe/rudp/return_codes.h"

using boost::asio::ip::udp;
using boost::asio::ip::address;

/*std::future<*/int/*>*/ CanConnect(const udp::endpoint& contact) {
//  auto ret_val = std::async(std::launch::async, [contact]()->int {
    maidsafe::asymm::Keys keys{ maidsafe::asymm::GenerateKeyPair() };
    maidsafe::NodeId chosen_bootstrap_peer;
    maidsafe::rudp::NatType nat_type;
    maidsafe::rudp::ManagedConnections managed_connections;
    /*return*/auto res = managed_connections.Bootstrap(
        std::vector<udp::endpoint>{ contact },
        [](const std::string&) {},
        [](const maidsafe::NodeId&) {},
        maidsafe::NodeId{ maidsafe::NodeId::IdType::kRandomId },
        std::make_shared<maidsafe::asymm::PrivateKey>(keys.private_key),
        std::make_shared<maidsafe::asymm::PublicKey>(keys.public_key),
        chosen_bootstrap_peer,
        nat_type);
//  });
  return res;
}

void HandleResult(std::pair<const udp::endpoint, /*std::future<*/int/*>*/>& result,
                  bool is_last_result,
                  std::stringstream& results_fstream) {
  std::string closer{ is_last_result ? " }\n" : " },\n" };
  try {
    results_fstream << "    { \"contact\": \"" << result.first << "\", \"canConnect\":\n " << std::boolalpha;
    int code{ result.second/*.get()*/ };
    if (code == maidsafe::rudp::kSuccess) {
      results_fstream << true << closer;
      return;
    }
    results_fstream << "Failed to connect to " << result.first << " with return code " << code << '\n';
  }
  catch (const std::exception& e) {
    results_fstream << "Exception trying to connect to " << result.first << ": " << e.what() << '\n';
  }
  results_fstream << "   ...  " << false << closer;
}

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

unsigned TestClass::UseRudp() {
  unsigned ret_val {};
  try {maidsafe::rudp::ManagedConnections obj; ret_val = obj.GetActiveConnectionCount();}
  catch (...) {}
  return ret_val;
}

std::string TestClass::CheckBootstrapsConnectivity(/*std::ofstream& results_stream*/) {
  std::map<udp::endpoint, /*std::future<*/int/*>*/> results;
//  results.emplace(udp::endpoint{ address::from_string("23.239.27.245"),  maidsafe::kLivePort }, /*std::future<*/int/*>*/{});
//  results.emplace(udp::endpoint{ address::from_string("95.85.32.100"),   maidsafe::kLivePort }, /*std::future<*/int/*>*/{});
//  results.emplace(udp::endpoint{ address::from_string("104.131.253.66"), maidsafe::kLivePort }, /*std::future<*/int/*>*/{});
//  results.emplace(udp::endpoint{ address::from_string("106.185.24.221"), maidsafe::kLivePort }, /*std::future<*/int/*>*/{});
  results.emplace(udp::endpoint{ address::from_string("128.199.159.50"), maidsafe::kLivePort }, /*std::future<*/int/*>*/{});
//  results.emplace(udp::endpoint{ address::from_string("178.79.156.73"),  maidsafe::kLivePort }, /*std::future<*/int/*>*/{});

  for (auto& result : results)
    result.second = CanConnect(result.first);

  std::stringstream results_stream {};
  results_stream << "{\n  \"bootstrapConnectivityCheck\": [\n";

  auto itr(std::begin(results));
  const auto last_element_itr(--std::end(results));
  while (itr != last_element_itr)
    HandleResult(*itr++, false, results_stream);
  HandleResult(*itr, true, results_stream);

  results_stream << "  ]\n}\n";

  return results_stream.str();
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
