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

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX -d)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_library(sample_lib STATIC "${CMAKE_SOURCE_DIR}/api.cc" "${CMAKE_SOURCE_DIR}/api.h")
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

set(ModuleName "port_sample")
execute_process(COMMAND python -c "import os.path; print os.path.relpath('${CMAKE_SOURCE_DIR}', '${CMAKE_BINARY_DIR}/nodejs_binding').replace('\\\\', '/')"
                RESULT_VARIABLE ResVar OUTPUT_VARIABLE RelPathToSourceDir ERROR_VARIABLE ErrorVar)
if(NOT "${ResVar}" EQUAL 0)
  message(FATAL_ERROR "Failed to calculate Rel Path to Source Dir: ${ErrorVar}")
endif()

string(REPLACE "\n" "" RelPathToSourceDir ${RelPathToSourceDir})

configure_file("${CMAKE_SOURCE_DIR}/nodejs/interface.i.in" "${CMAKE_BINARY_DIR}/nodejs_binding/interface.i")
configure_file("${CMAKE_SOURCE_DIR}/nodejs/binding.gyp.in" "${CMAKE_BINARY_DIR}/nodejs_binding/binding.gyp")

add_custom_target(nodejs_port
                    COMMAND swig -c++ -javascript -node interface.i
                    COMMAND node-gyp configure build
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/nodejs_binding")
ms_underscores_to_camel_case(nodejs_port CamelCaseNodePort)
set_target_properties(sample_app PROPERTIES PROJECT_LABEL ${CamelCaseNodePort} FOLDER "Port Libs")

add_custom_target(nodejs_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/nodejs/sample.js" "${CMAKE_BINARY_DIR}/nodejs_binding/build/Release"
                    COMMAND node sample.js
                    DEPENDS nodejs_port
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/nodejs_binding/build/Release")
ms_underscores_to_camel_case(nodejs_sample CamelCaseNodeSample)
set_target_properties(sample_app PROPERTIES PROJECT_LABEL ${CamelCaseNodeSample} FOLDER "Port Samples")
