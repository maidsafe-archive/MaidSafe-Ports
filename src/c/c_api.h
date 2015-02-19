#ifndef C_API_H_
#define C_API_H_

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#ifdef USE_ONLY_VOID_HANDLE
    typedef void HTestClass;
#else
    struct HTestClass;
#   ifndef __cplusplus
      typedef struct HTestClass HTestClass;
#   endif
#endif

HTestClass* CreateTestClass();
void FreeTestClass(HTestClass* handle);

void InitialiseLogging(HTestClass* handle);
void SetValue(HTestClass* handle, int number);
int GetValue(HTestClass* handle);
int GetRandomUInt32(HTestClass* handle);
char* PrintMe(HTestClass* handle);
bool CheckBootstrapsConnectivity(HTestClass* handle);
void PrintLogMessage(HTestClass* handle);
char* GetHttpMessage(HTestClass* handle, const char* host, const char* service);

void FreeScalarResource(void* scalar_resource);
void FreeVectorResource(void* vector_resource);

#ifdef __cplusplus
}
#endif

#endif  // C_API_H_
