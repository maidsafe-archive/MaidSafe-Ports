if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(nodejs)

include(../../cmake_modules/standard_setup.cmake)

#==================================================================================================#
# Check Prerequisites                                                                              #
#==================================================================================================#

set(SkipAllTargetsMessage "-- Skipping target nodejs_port\n")
set(SkipAllTargetsMessage "${SkipAllTargetsMessage}-- Skipping target nodejs_sample\n")

# Find node
if(UNIX)
  set(NodeJsName "nodejs")
else()
  set(NodeJsName "node.exe")
endif()

find_file(NodeJs NAMES ${NodeJsName})
if(NOT NodeJs)
  set(ErrorMessage "\nMissing nodejs.\n")
  set(ErrorMessage "${ErrorMessage}nodejs is required to build nodejs ports.\n")
  message("${ErrorMessage}${SkipAllTargetsMessage}")
  return()
endif()

# Find node-gyp
find_file(NodeGyp NAMES node-gyp)
if(NOT NodeGyp)
  set(ErrorMessage "\nMissing node-gyp tool.\n")
  set(ErrorMessage "${ErrorMessage}node-gyp is required to build nodejs ports.\nTo install it, run:\n")
  set(ErrorMessage "${ErrorMessage}    npm install -g node-gyp\n\n")
  message("${ErrorMessage}${SkipAllTargetsMessage}")
  return()
endif()

find_package(PythonInterp REQUIRED)
if(NOT PYTHONINTERP_FOUND)
  set(ErrorMessage "\nMissing Python Executable.\n")
  message("${ErrorMessage}${SkipAllTargetsMessage}")
  return()
endif()

#==================================================================================================#
# Project Setup                                                                                    #
#==================================================================================================#

set(OutputPath "${CMAKE_BINARY_DIR}/nodejs")

execute_process(COMMAND python -c "import os.path; print os.path.relpath('${common_SOURCE_DIR}', '${OutputPath}').replace('\\\\', '/')"
                RESULT_VARIABLE ResVar OUTPUT_VARIABLE RelPathToCommonSourceDir ERROR_VARIABLE ErrorVar)
if(NOT "${ResVar}" EQUAL 0)
  message(FATAL_ERROR "Failed to calculate Rel Path to Source Dir: ${ErrorVar}")
endif()

string(REPLACE "\n" "" RelPathToCommonSourceDir ${RelPathToCommonSourceDir})

get_dependency_paths(cpp_wrapper WrapperHeaders WrapperLibs)
format_list(WrapperHeaders FormattedHeaders)
format_list(WrapperLibs FormattedLibs)

# get_target_property(WrapperLibLocation cpp_wrapper LOCATION_DEBUG)
# SET(FormattedLibs \"${WrapperLibLocation}\",\n${FormattedLibs})

configure_file("${nodejs_SOURCE_DIR}/interface.i.in" "${OutputPath}/interface.i")
configure_file("${nodejs_SOURCE_DIR}/binding.gyp.in" "${OutputPath}/binding.gyp")

add_custom_target(nodejs_port
                    COMMAND ${Swig} -c++ -javascript -node interface.i
                    COMMAND ${NodeGyp} configure build
                    WORKING_DIRECTORY "${OutputPath}"
                    DEPENDS cpp_wrapper)
ms_underscores_to_camel_case(nodejs_port CamelCaseLibName)
set_target_properties(nodejs_port PROPERTIES PROJECT_LABEL ${CamelCaseLibName} FOLDER "Nodejs")
set_target_output_path(nodejs_port "${OutputPath}")

add_custom_target(nodejs_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${nodejs_SOURCE_DIR}/sample.js" "${OutputPath}/build/Release"
                    COMMAND ${NodeJs} sample.js
                    DEPENDS nodejs_port
                    WORKING_DIRECTORY "${OutputPath}/build/Release")

ms_underscores_to_camel_case(nodejs_sample CamelCaseAppName)
set_target_properties(nodejs_sample PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "Nodejs")
set_target_output_path(nodejs_sample "${OutputPath}")

