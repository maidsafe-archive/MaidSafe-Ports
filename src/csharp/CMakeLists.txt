if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/port_utils.cmake")
  cmake_minimum_required(VERSION 2.8.12.2)
  message(FATAL_ERROR "\nThis module cannot be built as a standalone project")
endif()

project(csharp)

#==================================================================================================#
# Check Prerequisites                                                                              #
#==================================================================================================#

set(SkipAllTargetsMessage "-- Skipping target csharp_port\n")
set(SkipAllTargetsMessage "${SkipAllTargetsMessage}-- Skipping target csharp_sample")

if(NOT WIN32)
  set(ErrorMessage "\nCSharp ports are only supported from windows with MSVC.\n\n")
  message("${ErrorMessage}${SkipAllTargetsMessage}")
  return()
endif()

# Find CSharp Compiler
find_file(CSharpCompiler NAMES csc.exe)
if(NOT CSharpCompiler)
  set(ErrorMessage "\nMissing csc.exe.\n")
  set(ErrorMessage "${ErrorMessage}csc is required to build csharp ports.\n\n")
  message("${ErrorMessage}${SkipAllTargetsMessage}")
  return()
endif()

#==================================================================================================#
# Project Setup                                                                                    #
#==================================================================================================#

set(OutputPath "${CMAKE_BINARY_DIR}/csharp")

configure_file("${csharp_SOURCE_DIR}/interface.i.in" "${OutputPath}/interface.i")

set(CSharpInterfaceFile "${OutputPath}/interface_wrap.cxx")
add_custom_command(OUTPUT ${CSharpInterfaceFile} COMMAND ${Swig} -c++ -csharp interface.i WORKING_DIRECTORY "${OutputPath}")

add_library(csharp_port SHARED ${CSharpInterfaceFile})
target_link_libraries(csharp_port cpp_wrapper)

add_custom_command(TARGET csharp_port POST_BUILD
                   COMMAND ${CSharpCompiler} /target:library /out:csharp_wrapper.dll *.cs
                   WORKING_DIRECTORY "${OutputPath}")
ms_underscores_to_camel_case(csharp_port CamelCaseLibName)
set_target_properties(csharp_port PROPERTIES PROJECT_LABEL ${CamelCaseLibName} FOLDER "CSharp")
set_target_output_path(csharp_port "${OutputPath}")

# TODO: Warn-1607 needs fixed and not ignored
make_directory("${CMAKE_BINARY_DIR}/csharp/sample/")
add_custom_target(csharp_sample
                    COMMAND ${CMAKE_COMMAND} -E copy "${csharp_SOURCE_DIR}/Sample.cs" "${OutputPath}/sample/"
                    COMMAND ${CMAKE_COMMAND} -E copy "${OutputPath}/csharp_wrapper.dll" "${OutputPath}/sample/"
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:csharp_port> "${OutputPath}/sample/"
                    COMMAND ${CSharpCompiler} /nowarn:1607 /reference:\"${OutputPath}/sample/csharp_wrapper.dll\" /out:sample.exe Sample.cs
                    COMMAND sample.exe
                    DEPENDS csharp_port
                    WORKING_DIRECTORY "${OutputPath}/sample")
ms_underscores_to_camel_case(csharp_sample CamelCaseAppName)
set_target_properties(csharp_sample PROPERTIES PROJECT_LABEL ${CamelCaseAppName} FOLDER "CSharp")
set_target_output_path(csharp_sample "${OutputPath}")
