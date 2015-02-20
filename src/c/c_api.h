#ifndef C_API_H_
#define C_API_H_

#include "c_port_export.h"

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

C_PORT_EXPORT HTestClass* CreateTestClass();
void C_PORT_EXPORT FreeTestClass(HTestClass* handle);

void C_PORT_EXPORT InitialiseLogging(HTestClass* handle);
void C_PORT_EXPORT SetValue(HTestClass* handle, int number);
int C_PORT_EXPORT GetValue(HTestClass* handle);
int C_PORT_EXPORT GetRandomUInt32(HTestClass* handle);
C_PORT_EXPORT char* PrintMe(HTestClass* handle);
bool C_PORT_EXPORT CheckBootstrapsConnectivity(HTestClass* handle);
void C_PORT_EXPORT PrintLogMessage(HTestClass* handle);
C_PORT_EXPORT char* GetHttpMessage(HTestClass* handle, const char* host, const char* service);

void C_PORT_EXPORT FreeScalarResource(void* scalar_resource);
void C_PORT_EXPORT FreeVectorResource(void* vector_resource);


#ifdef __cplusplus
}
#endif

#endif  // C_API_H_
