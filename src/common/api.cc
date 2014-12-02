#include "api.h"

#include <exception>
#include <future>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <sstream>

#include "maidsafe/common/utils.h"
//#include "boost/asio/ip/udp.hpp"
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

boost::asio::io_service io_service;

void http(std::stringstream& ref_stream) {
  try {
    // Get a list of endpoints corresponding to the server name.
    ref_stream << "-1\n";
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "www.boost.org", "80");
//    tcp::resolver::query query("www.boost.org", "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    ref_stream << "0\n";
    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    ref_stream << "1\n";
    if (error)
      throw boost::system::system_error(error);

    ref_stream << "2\n";
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << "/LICENSE_1_0.txt" << " HTTP/1.0\r\n";
    request_stream << "Host: " << "www.boost.org" << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    ref_stream << "3\n";
    boost::asio::write(socket, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      ref_stream << "Invalid response\n";
    }
    else if (status_code != 200)
    {
      ref_stream << "Response returned with status code " << status_code << "\n";
    }

    else {
      // Read the response headers, which are terminated by a blank line.
      boost::asio::read_until(socket, response, "\r\n\r\n");

      // Process the response headers.
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
        ref_stream << header << "\n";
      ref_stream << "\n";

      // Write whatever content we already have to output.
      if (response.size() > 0)
        ref_stream << &response;

      // Read until EOF, writing data to output as we go.
      while (boost::asio::read(socket, response,
                               boost::asio::transfer_at_least(1), error))
        ref_stream << &response;
      if (error != boost::asio::error::eof)
        throw boost::system::system_error(error);
    }
  }
  catch (std::exception& e)
  {
    ref_stream << "Exception: " << e.what() << "\n";
  }
}

#if 1
boost::asio::ip::address GetLocalIp(/*boost::asio::ip::udp::endpoint peer_endpoint*/) {
  boost::asio::ip::udp::endpoint peer_endpoint =
      boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string("203.0.113.0"),
                                     80);
  boost::asio::io_service io_service;
  try {
    boost::asio::ip::udp::socket socket(io_service);
    socket.connect(peer_endpoint);
    if (socket.local_endpoint().address().is_unspecified() ||
        socket.local_endpoint().address().is_loopback())
      return boost::asio::ip::address();
    return socket.local_endpoint().address();
  }
  catch (const std::exception& e) {
    LOG(kError) << "Failed trying to connect to " << peer_endpoint << " - "
                << boost::diagnostic_information(e);
    return boost::asio::ip::address();
  }
}
#endif

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
  char argv0[][20] = {"PortSample", "log_folder", "", "log_no_async", "true", "log_*", "I"};
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
  std::stringstream ss;
//  http(ss);
  ss << "\nYou should see me";
  
#ifdef __ANDROID__
  __android_log_print(ANDROID_LOG_DEBUG, "MAIDSAFE_ANDROID_LOG_TAG", "===================> Hit the logging site");
#endif
  return ss.str();
//  LOG(kInfo) << "===========================>> --sdkjfldsjkljflsj Spandan";

//  return std::string {"This is the return: "} + GetLocalIp().to_string();
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
