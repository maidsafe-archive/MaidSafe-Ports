if(APPLE)
  cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)
else()
  cmake_minimum_required(VERSION 2.8.12.1 FATAL_ERROR)
endif()

project("port_project")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
include(port_utils)

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

add_library(wrapper_cpp_lib STATIC "${CMAKE_SOURCE_DIR}/api.cc" "${CMAKE_SOURCE_DIR}/api.h")
target_link_libraries(wrapper_cpp_lib maidsafe_common)

ms_underscores_to_camel_case(wrapper_cpp_lib CamelCaseLibraryName)
set_target_properties(wrapper_cpp_lib PROPERTIES PROJECT_LABEL ${CamelCaseLibraryName} FOLDER "Library")

add_executable(wrapper_app "${CMAKE_SOURCE_DIR}/main.cc")

if(UNIX AND NOT APPLE)
  target_link_libraries(wrapper_app wrapper_cpp_lib -pthread)
else()
  target_link_libraries(wrapper_app wrapper_cpp_lib)
endif()

ms_underscores_to_camel_case(wrapper_app CamelCaseAppName)
set_target_properties(wrapper_app PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "Demo App")


execute_process(COMMAND python -c "import os.path; print os.path.relpath('${CMAKE_SOURCE_DIR}', '${CMAKE_BINARY_DIR}/nodejs_binding').replace('\\\\', '/')"
                RESULT_VARIABLE ResVar OUTPUT_VARIABLE RelPathToSourceDir ERROR_VARIABLE ErrorVar)
if(NOT "${ResVar}" EQUAL 0)
  message(FATAL_ERROR "Failed to calculate Rel Path to Source Dir: ${ErrorVar}")
endif()

string(REPLACE "\n" "" RelPathToSourceDir ${RelPathToSourceDir})

#-------------Nodejs Targets---------------
set(ModuleName "nodejs_port")
get_dependency_paths(maidsafe_common CommonHeaders CommonLibs)
format_list(CommonHeaders FormattedHeaders)
format_list(CommonLibs FormattedLibs)

#:span- Just so that it gets listed in the IDE
configure_file("${CMAKE_SOURCE_DIR}/nodejs/sample.js" "${CMAKE_SOURCE_DIR}/nodejs/sample.js")

configure_file("${CMAKE_SOURCE_DIR}/nodejs/interface.i.in" "${CMAKE_BINARY_DIR}/nodejs/interface.i")
configure_file("${CMAKE_SOURCE_DIR}/nodejs/binding.gyp.in" "${CMAKE_BINARY_DIR}/nodejs/binding.gyp")

add_custom_target(${ModuleName}
                    COMMAND swig -c++ -javascript -node interface.i
                    COMMAND node-gyp configure build
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/nodejs")
ms_underscores_to_camel_case(${ModuleName} CamelCaseNodePort)
set_target_properties(${ModuleName} PROPERTIES PROJECT_LABEL ${CamelCaseNodePort} FOLDER "Port Libs")

add_custom_target(nodejs_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/nodejs/sample.js" "${CMAKE_BINARY_DIR}/nodejs/build/Release"
                    COMMAND node sample.js
                    DEPENDS ${ModuleName}
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/nodejs/build/Release")

ms_underscores_to_camel_case(nodejs_sample CamelCaseNodeSample)
set_target_properties(nodejs_sample PROPERTIES PROJECT_LABEL ${CamelCaseNodeSample} FOLDER "Port Samples")

#-------------Python Targets---------------
set(ModuleName "python_port")

#:span- Just so that it gets listed in the IDE
configure_file("${CMAKE_SOURCE_DIR}/python/sample.py" "${CMAKE_SOURCE_DIR}/python/sample.py")

configure_file("${CMAKE_SOURCE_DIR}/python/interface.i.in" "${CMAKE_BINARY_DIR}/python/interface.i")

set(PythonInterfaceFile "${CMAKE_BINARY_DIR}/python/interface_wrap.cxx")
add_custom_command(OUTPUT ${PythonInterfaceFile} COMMAND swig -c++ -python interface.i WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/python")

add_library(${ModuleName} SHARED "${CMAKE_SOURCE_DIR}/api.cc" ${PythonInterfaceFile})
target_link_libraries(${ModuleName} wrapper_cpp_lib)
set_target_properties(${ModuleName} PROPERTIES PREFIX "_")

if(UNIX)
  target_include_directories(${ModuleName} PRIVATE "/usr/include/python2.7")
else()
  # switch this to find_python
  target_include_directories(${ModuleName} PRIVATE "C:/Python27/include")
  target_link_libraries(${ModuleName} "C:/Python27/libs/python27.lib")
  set_target_properties(${ModuleName} PROPERTIES SUFFIX ".pyd")
endif()

add_custom_command(TARGET ${ModuleName} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${ModuleName}> "${CMAKE_BINARY_DIR}/python/")
ms_underscores_to_camel_case(${ModuleName} CamelCasePythonPort)
set_target_properties(${ModuleName} PROPERTIES PROJECT_LABEL ${CamelCasePythonPort} FOLDER "Port Libs")

add_custom_target(python_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/python/sample.py" "${CMAKE_BINARY_DIR}/python/"
                    COMMAND python sample.py
                    DEPENDS ${ModuleName}
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/python/")
ms_underscores_to_camel_case(python_sample CamelCasePythonSample)
set_target_properties(python_sample PROPERTIES PROJECT_LABEL ${CamelCasePythonSample} FOLDER "Port Samples")
