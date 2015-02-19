#include "c_api.h"

#include <new>
#include <cstring>
#include <iostream>

#include "api.h"

#define CAST_TO_HANDLE(X) reinterpret_cast<HTestClass*>((X))
#define CAST_FRM_HANDLE(X) reinterpret_cast<sample_lib::TestClass*>((X))

extern "C" {

HTestClass* CreateTestClass() {
  HTestClass* handle{nullptr};
  try {
    handle = CAST_TO_HANDLE(sample_lib::TestClass::RawFactoryFunction());
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
  return handle;
}

void FreeTestClass(HTestClass* handle) {
  try {
    delete CAST_FRM_HANDLE(handle);
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
}

void InitialiseLogging(HTestClass* handle) {
  try {
    CAST_FRM_HANDLE(handle)->InitialiseLogging();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
}

void SetValue(HTestClass* handle, int number) {
  try {
    CAST_FRM_HANDLE(handle)->SetMyValue(number);
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
}

int GetValue(HTestClass* handle) {
  int ret_val{};
  try {
    ret_val = CAST_FRM_HANDLE(handle)->GetMyValue();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
  return ret_val;
}

int GetRandomUInt32(HTestClass* handle) {
  int ret_val{};
  try {
    ret_val = CAST_FRM_HANDLE(handle)->GetRandomUInt32();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
  return ret_val;
}

char* PrintMe(HTestClass* handle) {
  char* c_message{nullptr};
  try {
    auto message(CAST_FRM_HANDLE(handle)->PrintMe());
    c_message = static_cast<char*>(operator new(message.size() + 1, std::nothrow));
    if (c_message) {
      std::strcpy(c_message, message.c_str());
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
  return c_message;
}

bool CheckBootstrapsConnectivity(HTestClass* handle) {
  bool ret_val{};
  try {
    ret_val = CAST_FRM_HANDLE(handle)->CheckBootstrapsConnectivity();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
  return ret_val;
}

void PrintLogMessage(HTestClass* handle) {
  try {
    CAST_FRM_HANDLE(handle)->PrintLogMessage();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
}

char* GetHttpMessage(HTestClass* handle, const char* host, const char* service) {
  char* c_message{nullptr};
  try {
    auto message(CAST_FRM_HANDLE(handle)->GetHttpMessage(std::string{host}, std::string{service}));
    c_message = static_cast<char*>(operator new(message.size() + 1, std::nothrow));
    if (c_message) {
      std::strcpy(c_message, message.c_str());
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown Exception" << std::endl;
  }
  return c_message;
}

void FreeScalarResource(void* scalar_resource) {
  operator delete(scalar_resource, std::nothrow);
}

void FreeVectorResource(void* vector_resource) {
  operator delete[](vector_resource, std::nothrow);
}

}
