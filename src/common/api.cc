#include "api.h"

#include <exception>
#include <future>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <sstream>

#include "maidsafe/common/log.h"
#include "maidsafe/common/utils.h"
#include "maidsafe/common/node_id.h"
#include "maidsafe/common/utils.h"
#include "maidsafe/rudp/managed_connections.h"
#include "maidsafe/rudp/return_codes.h"
#include "boost/asio.hpp"

using boost::asio::ip::udp;
using boost::asio::ip::address;
using boost::asio::ip::tcp;

int CanConnect(const udp::endpoint& contact) {
  maidsafe::asymm::Keys keys{maidsafe::asymm::GenerateKeyPair()};
  maidsafe::NodeId chosen_bootstrap_peer;
  maidsafe::rudp::NatType nat_type;
  maidsafe::rudp::ManagedConnections managed_connections;
  auto res = managed_connections.Bootstrap(
        std::vector<udp::endpoint>{contact}, [](const std::string&) {},
  [](const maidsafe::NodeId&) {},
  maidsafe::NodeId{maidsafe::RandomString(maidsafe::NodeId::kSize)},
  std::make_shared<maidsafe::asymm::PrivateKey>(keys.private_key),
  std::make_shared<maidsafe::asymm::PublicKey>(keys.public_key), chosen_bootstrap_peer,
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
  return new TestClass{};
}

void TestClass::InitialiseLogging() {
  char argv0[][20] = {"PortSample", "--log_folder", "", "--log_no_async", "true", "--log_*", "I"};
  char* argv1[7] = {argv0[0], argv0[1], argv0[2], argv0[3], argv0[4], argv0[5], argv0[6]};
  char** argv = argv1;
  maidsafe::log::Logging::Instance().Initialise(7, argv);
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
  result.first = udp::endpoint {address::from_string("192.168.92.130"), maidsafe::kLivePort};
  result.second = int {};

  result.second = CanConnect(result.first);

  return HandleResult(result);
}

void TestClass::PrintLogMessage() {
  LOG(kVerbose) << "kVerbose message.";
  LOG(kInfo) << "kInfo message.";
  LOG(kSuccess) << "kSuccess message.";
  LOG(kWarning) << "kWarning message.";
  LOG(kError) << "kError message.";
  LOG(kAlways) << "kAlways message.";
}

std::string TestClass::GetHttpMessage(const std::string& host, const std::string& service) {
  std::stringstream str_stream;

  try {
    boost::asio::io_service io_service;
    // Get a list of endpoints corresponding to the server name.
    str_stream << "-1\n";
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), host, service);
//    tcp::resolver::query query("www.boost.org", "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    str_stream << "0\n";
    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    str_stream << "1\n";
    if (error)
      throw boost::system::system_error(error);

    str_stream << "2\n";
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
    str_stream << "3\n";
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
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
      str_stream << "Invalid response\n";
    }
    else if (status_code != 200) {
      str_stream << "Response returned with status code " << status_code << "\n";
    }

    else {
      // Read the response headers, which are terminated by a blank line.
      boost::asio::read_until(socket, response, "\r\n\r\n");

      // Process the response headers.
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
        str_stream << header << "\n";
      str_stream << "\n";

      // Write whatever content we already have to output.
      if (response.size() > 0)
        str_stream << &response;

      // Read until EOF, writing data to output as we go.
      while (boost::asio::read(socket, response,
                               boost::asio::transfer_at_least(1), error))
        str_stream << &response;
      if (error != boost::asio::error::eof)
        throw boost::system::system_error(error);
    }
  }
  catch (std::exception& e) {
    str_stream << "Exception: " << e.what() << "\n";
  }

  return str_stream.str();
}

}
