#include "api.h"

#include <exception>
#include <future>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <sstream>

#include "maidsafe/common/utils.h"
#include "maidsafe/common/node_id.h"
#include "maidsafe/common/utils.h"
#include "maidsafe/rudp/managed_connections.h"
#include "maidsafe/rudp/return_codes.h"
#include "boost/asio.hpp"

#ifdef __ANDROID__
#include "android/log.h"
#endif

using boost::asio::ip::udp;
using boost::asio::ip::address;
using boost::asio::ip::tcp;

int CanConnect(const udp::endpoint& contact) {
    maidsafe::asymm::Keys keys{ maidsafe::asymm::GenerateKeyPair() };
    maidsafe::NodeId chosen_bootstrap_peer;
    maidsafe::rudp::NatType nat_type;
    maidsafe::rudp::ManagedConnections managed_connections;
    auto res = managed_connections.Bootstrap(
        std::vector<udp::endpoint>{ contact },
        [](const std::string&) {},
        [](const maidsafe::NodeId&) {},
        maidsafe::NodeId{ maidsafe::NodeId::IdType::kRandomId },
        std::make_shared<maidsafe::asymm::PrivateKey>(keys.private_key),
        std::make_shared<maidsafe::asymm::PublicKey>(keys.public_key),
        chosen_bootstrap_peer,
        nat_type);
  return res;
}

bool HandleResult(std::pair<udp::endpoint, int>& result) {
  try {
    int code{ result.second };
    if (code == maidsafe::rudp::kSuccess) {
      return true;
    }
  }
  catch (const std::exception&) {
  }
  return false;
}

namespace sample_lib {

TestClass::TestClass()
  : answer_(98) {}

TestClass* TestClass::RawFactoryFunction() {
  char argv0[][20] = {"PortSample", "--log_folder", "", "--log_no_async", "true", "--log_*", "I"};
  char* argv1[7] = {argv0[0], argv0[1], argv0[2], argv0[3], argv0[4], argv0[5], argv0[6]};
  char** argv = argv1;
  maidsafe::log::Logging::Instance().Initialise(7, argv);
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
  return std::string {"You should see me"};
}

bool TestClass::CheckBootstrapsConnectivity() {
  std::pair<udp::endpoint, int> result;
  result.first = udp::endpoint {address::from_string("128.199.159.50"), maidsafe::kLivePort};
  result.second = int {};

  result.second = CanConnect(result.first);

  return HandleResult(result);
}

void TestClass::PrintLogMessage() {
  LOG(kError) << "This is MaidSafe log message.";
#ifdef __ANDROID__
  __android_log_print(ANDROID_LOG_DEBUG, "MAIDSAFE_ANDROID_LOG_TAG", "This is Android Log message.");
#endif
}

}
