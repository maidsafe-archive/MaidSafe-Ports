if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(android)

include(../../cmake_modules/standard_setup.cmake)

set(OutputPath "${CMAKE_BINARY_DIR}/android")

configure_file("${android_SOURCE_DIR}/interface.i.in" "${OutputPath}/swig/interface.i")

set(AndroidInterfaceFileName "interface_wrap.cc")
set(AndroidInterfaceFile "${OutputPath}/swig/jni/${AndroidInterfaceFileName}")
add_custom_command(
  OUTPUT ${AndroidInterfaceFile}
  COMMAND mkdir -p src/org/maidsafe
  COMMAND mkdir jni
  COMMAND swig -c++ -java -package org.maidsafe -outdir src/org/maidsafe -o jni/${AndroidInterfaceFileName} interface.i
  WORKING_DIRECTORY "${OutputPath}/swig")

add_library(android_port SHARED ${AndroidInterfaceFile})
target_link_libraries(android_port cpp_wrapper)
target_compile_options(android_port PRIVATE -fno-strict-aliasing)
ms_underscores_to_camel_case(android_port CamelCaseLibName)
set_target_properties(android_port PROPERTIES PROJECT_LABEL ${CamelCaseLibName} FOLDER "Android")
set_target_output_path(android_port "${OutputPath}")

if(ANDROID_SKIP_SAMPLE)
  message("-- Skipping target android_sample")
  return()
endif()

# Find AntTool
find_file(AntTool NAMES ant)
if(NOT AntTool)
  set(ErrorMessage "\nMissing AntTool build tool.  ")
  set(ErrorMessage "${ErrorMessage}To install it, run:\n")
  set(ErrorMessage "${ErrorMessage}    sudo apt-get update && sudo apt-get install AntTool\n\n")
  set(ErrorMessage "${ErrorMessage}-- Skipping target android_sample\n")
  message("${ErrorMessage}")
  return()
endif()

# Find android project tool
find_file(AndroidProjectTool NAMES android
                             PATHS ${ANDROID_SDK_ROOT}/sdk/tools)
if(NOT AndroidProjectTool)
  set(ErrorMessage "\nMissing android project tool.\n")
  set(ErrorMessage "${ErrorMessage}Android SDK is required to build android_sample.\nTo set it, run:\n")
  set(ErrorMessage "${ErrorMessage}    cmake . -DANDROID_SDK_ROOT=\"<path to android sdk root>\"\n\n")
  set(ErrorMessage "${ErrorMessage}-- Skipping target android_sample\n")
  message("${ErrorMessage}")
  return()
endif()

if(NOT EXISTS ${OutputPath}/class/)
  message("-- Creating Android Sample Project")
  file(MAKE_DIRECTORY ${OutputPath}/class)
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${OutputPath}/class
                  COMMAND ${AndroidProjectTool} create project --target 2 --name MyTest --path ./class --activity MyTest --package org.maidsafe
                  WORKING_DIRECTORY ${OutputPath}
                  RESULT_VARIABLE ResVar
                  OUTPUT_VARIABLE OutVar)
  if(NOT ResVar STREQUAL "0")
    set(ErrorMessage "Failed to create an empty android project.\n\n${ResVar}\n\n")
    set(ErrorMessage "${ErrorMessage}-- To skip android_sample target, run:\n")
    set(ErrorMessage "${ErrorMessage}    cmake . -DANDROID_SKIP_SAMPLE=ON\n\n")
    message(FATAL_ERROR "${ErrorMessage}")
  endif()
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                        ${android_SOURCE_DIR}/sample/class/res/layout/main.xml
                        ${OutputPath}/class/res/layout
                COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                        ${android_SOURCE_DIR}/sample/class/src/org/maidsafe/MyTest.java
                        ${OutputPath}/class/src/org/maidsafe
                COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                        ${android_SOURCE_DIR}/sample/class/AndroidManifest.xml
                        ${OutputPath}/class
                COMMAND ${CMAKE_COMMAND} -E make_directory ${OutputPath}/class/libs/armeabi-v7a
                WORKING_DIRECTORY ${OutputPath}/class
                RESULT_VARIABLE ResVar
                OUTPUT_VARIABLE OutVar)
if(NOT ResVar STREQUAL "0")
  set(ErrorMessage "Failed to setup android project.\n\n${ResVar}\n\n")
  set(ErrorMessage "${ErrorMessage}-- To skip android_sample target, run:\n")
  set(ErrorMessage "${ErrorMessage}    cmake . -DANDROID_SKIP_SAMPLE=ON\n\n")
  message(FATAL_ERROR "${ErrorMessage}")
endif()

add_custom_target(android_sample
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OutputPath}/class/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${OutputPath}/swig/src/org/maidsafe
            ${OutputPath}/class/src/org/maidsafe
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        $<TARGET_FILE:android_port>
        ${OutputPath}/class/libs/armeabi-v7a
    COMMAND ${AntTool} debug > ${OutputPath}/ant-output.log
    COMMAND echo '\\nBuild Success'
    COMMAND echo 'To Deploy, run:\\n-- ${ANDROID_SDK_ROOT}/sdk/platform-tools/adb install android/class/bin/MyTest-debug.apk\\n'
    DEPENDS android_port
    WORKING_DIRECTORY ${OutputPath}/class
)
ms_underscores_to_camel_case(android_sample CamelCaseAppName)
set_target_properties(android_sample PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "Android")
set_target_output_path(android_sample "${OutputPath}")

