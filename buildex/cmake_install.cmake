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
  include("K:/workspace/firebreath-1/buildex/cmake_common/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/boost/libs/thread/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/ScriptingCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/PluginCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/NpapiCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/ActiveXCore/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/fblibs/log4cplus/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/projects/BasicMediaPlayer/PluginAuto/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/projects/BasicMediaPlayer/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/fblibs/FBTestPlugin/log4cplus/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/projects/FBTestPlugin/PluginAuto/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/projects/FBTestPlugin/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/unittest-cpp/UnitTest++/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/ScriptingCoreTest/cmake_install.cmake")
  include("K:/workspace/firebreath-1/buildex/ActiveXCoreTest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "K:/workspace/firebreath-1/buildex/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "K:/workspace/firebreath-1/buildex/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
