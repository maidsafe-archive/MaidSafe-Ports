if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(common)

add_library(cpp_wrapper
 STATIC
  "${common_SOURCE_DIR}/api.cc"
  "${common_SOURCE_DIR}/api.h")

target_link_libraries(cpp_wrapper maidsafe_rudp)

ms_underscores_to_camel_case(cpp_wrapper CamelCaseLibraryName)
set_target_properties(cpp_wrapper PROPERTIES PROJECT_LABEL ${CamelCaseLibraryName} FOLDER "Common")
set_target_output_path(cpp_wrapper "${CMAKE_BINARY_DIR}")

# Skip cpp_console_sample target for ANDROID_BUILD
if(ANDROID_BUILD)
  return()
endif()

add_executable(cpp_console_sample "${common_SOURCE_DIR}/main.cc")
target_link_libraries(cpp_console_sample
  cpp_wrapper
  $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:-pthread>
)

ms_underscores_to_camel_case(cpp_console_sample CamelCaseAppName)
set_target_properties(cpp_console_sample PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "Common")
set_target_output_path(cpp_console_sample "${CMAKE_BINARY_DIR}")

