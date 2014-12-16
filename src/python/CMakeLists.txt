if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(python)

include(../../cmake_modules/standard_setup.cmake)

find_package(PythonLibs REQUIRED)
if(NOT PYTHONLIBS_FOUND)
  set(ErrorMessage "\nMissing Python Libs.  ")
  set(ErrorMessage "${ErrorMessage}-- Skipping target python_port\n")
  set(ErrorMessage "${ErrorMessage}-- Skipping target python_sample\n")
  message("${ErrorMessage}")
  return()
endif()

set(OutputPath "${CMAKE_BINARY_DIR}/python")

configure_file("${python_SOURCE_DIR}/interface.i.in" "${OutputPath}/interface.i")

set(PythonInterfaceFile "${OutputPath}/interface_wrap.cxx")
add_custom_command(OUTPUT ${PythonInterfaceFile} COMMAND swig -c++ -python interface.i WORKING_DIRECTORY "${OutputPath}")

add_library(python_port SHARED ${PythonInterfaceFile} "${python_SOURCE_DIR}/sample.py")
target_link_libraries(python_port
  cpp_wrapper
  $<$<NOT:$<BOOL:${UNIX}>>:${PYTHON_LIBRARIES}>
)
target_include_directories(python_port PRIVATE ${PYTHON_INCLUDE_DIRS})
target_compile_options(python_port
  PUBLIC
    $<$<BOOL:${MSVC}>:
        /wd4273 #Suppress: inconsistent dll linkage - from pymath.h (belongs to python headers) and xtgmath.h (belongs to VS headers)
        /wd4127 #Suppress: conditional expression is constant - from interface_wrap.cxx(swig generated file)
        /wd4706 #Suppress: assignment within conditional expression - from interface_wrap.cxx(swig generated file)
    >
)

ms_underscores_to_camel_case(python_port CamelCaseLibName)
set_target_properties(python_port PROPERTIES PREFIX "_" PROJECT_LABEL ${CamelCaseLibName} FOLDER "Python")
if(MSVC)
  set_target_properties(python_port PROPERTIES SUFFIX ".pyd")
endif()
set_target_output_path(python_port "${OutputPath}")

add_custom_target(python_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:python_port>" "${OutputPath}"
                    COMMAND ${CMAKE_COMMAND} -E copy "${python_SOURCE_DIR}/sample.py" "${OutputPath}"
                    COMMAND python sample.py
                    DEPENDS python_port
                    WORKING_DIRECTORY "${OutputPath}")
ms_underscores_to_camel_case(python_sample CamelCaseAppName)
set_target_properties(python_sample PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "Python")
set_target_output_path(python_sample "${OutputPath}")

