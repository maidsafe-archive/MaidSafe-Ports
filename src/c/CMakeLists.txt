if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(c)

#==================================================================================================#
# Project Setup                                                                                    #
#==================================================================================================#

set(OutputPath "${CMAKE_BINARY_DIR}/c")

add_library(c_port SHARED
  "${c_SOURCE_DIR}/c_api.cc"
  "${c_SOURCE_DIR}/c_api.h"
  "${CMAKE_CURRENT_BINARY_DIR}/c_port_export.h")

include(GenerateExportHeader)
generate_export_header(c_port)

target_link_libraries(c_port
  PUBLIC
    cpp_wrapper)

target_include_directories(c_port PUBLIC "${common_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

target_compile_definitions(c_port
  PRIVATE
    $<$<BOOL:${USE_ONLY_VOID_HANDLE}>:USE_ONLY_VOID_HANDLE>)

ms_underscores_to_camel_case(c_port CamelCaseLibraryName)
set_target_properties(c_port PROPERTIES PROJECT_LABEL ${CamelCaseLibraryName} FOLDER "C")
set_target_output_path(c_port "${OutputPath}")

# Skip c_sample target for ANDROID_BUILD
if(ANDROID_BUILD)
  return()
endif()

add_executable(c_sample "${${PROJECT_NAME}_SOURCE_DIR}/main.cc")
target_link_libraries(c_sample
  c_port
  $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:-pthread>
)

ms_underscores_to_camel_case(c_sample CamelCaseAppName)
set_target_properties(c_sample PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "C")
set_target_output_path(c_sample "${OutputPath}")

