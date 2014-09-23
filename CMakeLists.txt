if(APPLE)
  cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
else()
  cmake_minimum_required(VERSION 2.8.12.1 FATAL_ERROR)
endif()

project("port_project")

function(ms_underscores_to_camel_case VarIn VarOut)
  string(REPLACE "_" ";" Pieces ${VarIn})
  foreach(Part ${Pieces})
    string(SUBSTRING ${Part} 0 1 Initial)
    string(SUBSTRING ${Part} 1 -1 Part)
    string(TOUPPER ${Initial} Initial)
    set(CamelCase ${CamelCase}${Initial}${Part})
  endforeach()
  set(${VarOut} ${CamelCase} PARENT_SCOPE)
endfunction()

function(apply_target_properties Target)
  if(ARGV1)
    set(Folder "${ARGV1}")
  endif()
  ms_underscores_to_camel_case(${Target} CamelCaseTargetName)
  set_target_properties(${Target} PROPERTIES PROJECT_LABEL ${CamelCaseTargetName} FOLDER "${Folder}")
endfunction()

# Including maidsafe_export.cmake brings all the MaidSafe libraries into scope as CMake targets.
find_file(MaidSafeExport NAMES maidsafe_export.cmake
                         PATHS ${MAIDSAFE_BINARY_DIR}
                         NO_DEFAULT_PATH)
if(NOT MaidSafeExport)
  set(ErrorMessage "\n\nCan't find maidsafe_export.cmake in MAIDSAFE_BINARY_DIR.  ")
  set(ErrorMessage "${ErrorMessage}Currently MAIDSAFE_BINARY_DIR is set to ")
  set(ErrorMessage "${ErrorMessage}\"${MAIDSAFE_BINARY_DIR}\"  It must be set to the MaidSafe ")
  set(ErrorMessage "${ErrorMessage}super-project's build root.\nTo set it, run:\n")
  set(ErrorMessage "${ErrorMessage}    cmake . -DMAIDSAFE_BINARY_DIR=\"<path to build root>\"\n\n")
  message(FATAL_ERROR "${ErrorMessage}")
endif()
include(${MaidSafeExport})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX -d)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_library(sample_lib STATIC "${CMAKE_SOURCE_DIR}/api.cc" "${CMAKE_SOURCE_DIR}/api.h")
target_link_libraries(sample_lib maidsafe_common)

if(UNIX)
  target_compile_options(sample_lib PRIVATE -std=c++11 -fpic)
endif()

ms_underscores_to_camel_case(sample_lib CamelCaseLibraryName)
set_target_properties(sample_lib PROPERTIES PROJECT_LABEL ${CamelCaseLibraryName} FOLDER "Library")

add_executable(sample_app "${CMAKE_SOURCE_DIR}/main.cc")

if(UNIX AND NOT APPLE)
  target_link_libraries(sample_app sample_lib -pthread)
else()
  target_link_libraries(sample_app sample_lib)
endif()

ms_underscores_to_camel_case(sample_app CamelCaseAppName)
set_target_properties(sample_app PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "Demo App")


execute_process(COMMAND python -c "import os.path; print os.path.relpath('${CMAKE_SOURCE_DIR}', '${CMAKE_BINARY_DIR}/nodejs_binding').replace('\\\\', '/')"
                RESULT_VARIABLE ResVar OUTPUT_VARIABLE RelPathToSourceDir ERROR_VARIABLE ErrorVar)
if(NOT "${ResVar}" EQUAL 0)
  message(FATAL_ERROR "Failed to calculate Rel Path to Source Dir: ${ErrorVar}")
endif()

string(REPLACE "\n" "" RelPathToSourceDir ${RelPathToSourceDir})

#-------------Nodejs Targets---------------
set(ModuleName "nodejs_port")
configure_file("${CMAKE_SOURCE_DIR}/nodejs/interface.i.in" "${CMAKE_BINARY_DIR}/nodejs/interface.i")
configure_file("${CMAKE_SOURCE_DIR}/nodejs/binding.gyp.in" "${CMAKE_BINARY_DIR}/nodejs/binding.gyp")

add_custom_target(nodejs_port
                    COMMAND swig -c++ -javascript -node interface.i
                    COMMAND node-gyp configure build
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/nodejs")
ms_underscores_to_camel_case(nodejs_port CamelCaseNodePort)
set_target_properties(nodejs_port PROPERTIES PROJECT_LABEL ${CamelCaseNodePort} FOLDER "Port Libs")

add_custom_target(nodejs_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/nodejs/sample.js" "${CMAKE_BINARY_DIR}/nodejs/build/Release"
                    COMMAND node sample.js
                    DEPENDS nodejs_port
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/nodejs/build/Release")

ms_underscores_to_camel_case(nodejs_sample CamelCaseNodeSample)
set_target_properties(nodejs_sample PROPERTIES PROJECT_LABEL ${CamelCaseNodeSample} FOLDER "Port Samples")

#-------------Python Targets---------------
set(ModuleName "python_port")
configure_file("${CMAKE_SOURCE_DIR}/python/interface.i.in" "${CMAKE_BINARY_DIR}/python/interface.i")

set(PythonInterfaceFile "${CMAKE_BINARY_DIR}/python/interface_wrap.cxx")
add_custom_command(OUTPUT ${PythonInterfaceFile} COMMAND swig -c++ -python interface.i WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/python")

add_library(_${ModuleName} SHARED "${CMAKE_SOURCE_DIR}/api.cc" ${PythonInterfaceFile})
target_link_libraries(_${ModuleName} sample_lib)

if(UNIX)
  target_compile_options(_${ModuleName} PRIVATE -std=c++11 -fpic)
  target_include_directories(_${ModuleName} PRIVATE "/usr/include/python2.7")
  set_target_properties(_${ModuleName} PROPERTIES PREFIX "")
else()
  # switch this to find_python
  target_include_directories(_${ModuleName} PRIVATE "C:/Python27/include")
  target_link_libraries(_${ModuleName} "C:/Python27/libs/python27.lib")
endif()

add_custom_command(TARGET _${ModuleName} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_${ModuleName}> "${CMAKE_BINARY_DIR}/python/")
ms_underscores_to_camel_case(_${ModuleName} CamelCasePythonPort)
set_target_properties(_${ModuleName} PROPERTIES PROJECT_LABEL ${CamelCasePythonPort} FOLDER "Port Libs")

add_custom_target(python_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/python/sample.py" "${CMAKE_BINARY_DIR}/python/"
                    COMMAND python sample.py
                    DEPENDS _${ModuleName}
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/python/")
ms_underscores_to_camel_case(python_sample CamelCasePythonSample)
set_target_properties(python_sample PROPERTIES PROJECT_LABEL ${CamelCasePythonSample} FOLDER "Port Samples")
