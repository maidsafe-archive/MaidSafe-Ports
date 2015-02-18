if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(c_wrapper)

add_library(${PROJECT_NAME}
 SHARED
  "${${PROJECT_NAME}_SOURCE_DIR}/c_api.cc"
  "${${PROJECT_NAME}_SOURCE_DIR}/c_api.h")

target_link_libraries(${PROJECT_NAME}
 PRIVATE
  cpp_wrapper)

target_include_directories(${PROJECT_NAME}
 PRIVATE
  "${common_SOURCE_DIR}")

target_compile_definitions(${PROJECT_NAME}
 PRIVATE
  $<$<BOOL:${USE_VOID_HANDLE}>:
    $<$<BOOL:${UNIX}>:
      USE_VOID_HANDLE
    >
  >)

ms_underscores_to_camel_case(${PROJECT_NAME} CamelCaseLibraryName)
set_target_properties(${PROJECT_NAME} PROPERTIES PROJECT_LABEL ${CamelCaseLibraryName} FOLDER "C_Wrapper")
set_target_output_path(${PROJECT_NAME} "${CMAKE_BINARY_DIR}")
