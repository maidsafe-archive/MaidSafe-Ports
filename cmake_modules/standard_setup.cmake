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

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_file(Swig NAMES swig swig.exe
               PATHS ${SWIG_BINARY_DIR})
if(NOT Swig)
  set(ErrorMessage "\n\nCan't find swig.  ")
  set(ErrorMessage "${ErrorMessage}Currently SWIG_BINARY_DIR is set to ")
  set(ErrorMessage "${ErrorMessage}\"${SWIG_BINARY_DIR}\"  It must be set to the ")
  set(ErrorMessage "${ErrorMessage}Binary dir for swig.\nTo set it, run:\n")
  set(ErrorMessage "${ErrorMessage}    cmake . -DSWIG_BINARY_DIR=\"<path to swig binary dir>\"\n\n")
  message(FATAL_ERROR "${ErrorMessage}")
endif()

set(CMAKE_DEBUG_POSTFIX )
