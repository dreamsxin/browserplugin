# Install script for directory: K:/workspace/firebreath-1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/FireBreath")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("K:/workspace/firebreath-1/build/cmake_common/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/boost/libs/thread/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/ScriptingCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/PluginCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/NpapiCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/ActiveXCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/fblibs/jsoncpp/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/projects/EotuSocket/PluginAuto/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/projects/EotuSocket/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/unittest-cpp/UnitTest++/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/ScriptingCoreTest/cmake_install.cmake")
  include("K:/workspace/firebreath-1/build/ActiveXCoreTest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "K:/workspace/firebreath-1/build/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "K:/workspace/firebreath-1/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
