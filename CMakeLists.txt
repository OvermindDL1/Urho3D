#
# Copyright (c) 2008-2014 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Set project name
project (Urho3D C CXX)

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)

if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
endif (COMMAND cmake_policy)

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source/CMake/Modules)

################################
# GUI Variables Start

# Should there be a 32BIT option too, and should both be allowed to build simultaneously?
set (ENABLE_64BIT OFF CACHE BOOL "Enable 64-bit builds")

set (ENABLE_LUA OFF CACHE BOOL "Enable the LUA scripting support")
set (ENABLE_LUAJIT OFF CACHE BOOL "Enable the LUAJit scripting support (will automatically set ENABLE_LUA)")
set (ENABLE_SAFE_LUA OFF CACHE BOOL "Enable the Lua C++ wrapper safety checks")
set (ENABLE_SAMPLES OFF CACHE BOOL "Enable building of the C++ Samples")

if (RASPI)
    set (ENABLE_TOOLS OFF CACHE BOOL "Enable building of the tools (disabled by default for Raspberry Pi, enabled by default otherwise)")
else (RASPI)
    set (ENABLE_TOOLS ON CACHE BOOL "Enable building of the tools (disabled by default for Raspberry Pi, enabled by default otherwise)")
endif (RASPI)

set (ENABLE_DOCS OFF CACHE BOOL "Enable building of the documentation")
set (ENABLE_ANGELSCRIPT ON CACHE BOOL "Enable building of AngelScript scripting support")
set (ENABLE_SSE ON CACHE BOOL "Enable SSE support")

set (ENABLE_MINIDUMPS ON CACHE BOOL "Enable minidumps on crash (Windows only)")
if (WIN32) # Maybe this should be ON for WIN32 now too?
    set (USE_OPENGL OFF CACHE BOOL "Force use of OpenGL instead of DirectX (Windows only)")
else (WIN32)
    set (USE_OPENGL ON CACHE BOOL "Force use of OpenGL instead of DirectX (Windows only)")
endif (WIN32)
set (USE_MKLINK OFF CACHE BOOL "Use mklink command to create symbolic links (Windows Vista and above only)")
set (USE_STATIC_RUNTIME OFF CACHE BOOL "Use static C/C++ runtime libraries and eliminate the need for runtime DLLs installation (Windows only)")
set (SCP_TO_TARGET "" CACHE STRING "Automatically scp executables to target system (non-Android cross-compiling build only), SSH digital key must be setup first for this to work, typical value has a pattern of usr@tgt:remote-loc")

set (URHO3D_LIB_TYPE STATIC CACHE STRING "Choose the linking type of the library to build, options are: STATIC SHARED")
set (ANDROID_ABI armeabi-v7a CACHE STRING "Choose the ABI for native code for Android builds, options are: armeabi-v7a armeabi")

set (ENABLE_TESTING OFF CACHE BOOL "Enable the tests")
set (TEST_TIME_OUT 5 CACHE STRING "Integer value of automatically exiting the program after this delay in Seconds")

set (ENABLE_FILEWATCHER ON CACHE BOOL "Enable file watcher support for automatic resource reloads")
set (ENABLE_PROFILING ON CACHE BOOL "Enable profiling; if disabled then autoprofileblocks become no-ops and the Profiler subsystem is not instantiated")
set (ENABLE_LOGGING ON CACHE BOOL "Enable logging; if disabled then LOGXXXX macros become no-ops and the Log subsystem is not instantiated")

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# GUI Variables End
################################

################################
# Global Configurations Start

if (ENABLE_TESTING)
    enable_testing ()
    add_definitions (-DENABLE_TESTING)
endif (ENABLE_TESTING)

if (ENABLE_SSE)
    add_definitions (-DENABLE_SSE)
endif (ENABLE_SSE)

if (ENABLE_FILEWATCHER)
    add_definitions (-DENABLE_FILEWATCHER)
endif (ENABLE_FILEWATCHER)

if (ENABLE_PROFILING)
    add_definitions (-DENABLE_PROFILING)
endif (ENABLE_PROFILING)

if (ENABLE_LOGGING)
    add_definitions (-DENABLE_LOGGING)
endif (ENABLE_LOGGING)

if (USE_OPENGL)
    add_definitions (-DUSE_OPENGL)
endif (USE_OPENGL)

if (ENABLE_ANGELSCRIPT)
    add_definitions (-DENABLE_ANGELSCRIPT)
endif (ENABLE_ANGELSCRIPT)

# Add definition for Lua and LuaJIT
if (ENABLE_LUAJIT)
    add_definitions (-DENABLE_LUAJIT)
    #%set (JIT JIT)
    # Implied ENABLE_LUA
	set (ENABLE_LUA ON CACHE BOOL "Enable the LUA scripting support" FORCE)
    # Adjust LuaJIT default search path as necessary (adapted from LuaJIT's original Makefile)
    if (NOT CMAKE_INSTALL_PREFIX STREQUAL /usr/local)
        add_definitions (-DLUA_XROOT="${CMAKE_INSTALL_PREFIX}/")
    endif (NOT CMAKE_INSTALL_PREFIX STREQUAL /usr/local)
endif (ENABLE_LUAJIT)

if (ENABLE_LUA)
    add_definitions (-DENABLE_LUA)
endif (ENABLE_LUA)

if (URHO3D_LIB_TYPE STREQUAL STATIC)
    add_definitions (-DURHO3D_STATIC_DEFINE)
endif (URHO3D_LIB_TYPE STREQUAL STATIC)

# Add definitions for GLEW
if (NOT IOS AND NOT ANDROID AND NOT RASPI AND USE_OPENGL)
    add_definitions (-DGLEW_STATIC -DGLEW_NO_GLU)
endif (NOT IOS AND NOT ANDROID AND NOT RASPI AND USE_OPENGL)

if (MSVC)
    if (ENABLE_MINIDUMPS)
        add_definitions (-DENABLE_MINIDUMPS)
    endif (ENABLE_MINIDUMPS)

    if (USE_STATIC_RUNTIME)
        set (RELEASE_RUNTIME /MT)
        set (DEBUG_RUNTIME /MTd)
    else (USE_STATIC_RUNTIME)
        set (RELEASE_RUNTIME "")
        set (DEBUG_RUNTIME "")
    endif (USE_STATIC_RUNTIME)

    if (NOT USE_OPENGL)
        find_package (Direct3D)
        if (DIRECT3D_FOUND)
            include_directories (${DIRECT3D_INCLUDE_DIRS})
        endif (DIRECT3D_FOUND)
    endif (NOT USE_OPENGL)
endif (MSVC)

if (RASPI)
    find_package (BCM_VC REQUIRED)
    include_directories (${BCM_VC_INCLUDE_DIRS})
endif (RASPI)

# Check existence of stdint.h for LibCpuId
include (CheckIncludeFiles)
CHECK_INCLUDE_FILES (stdint.h HAVE_STDINT_H)
if (HAVE_STDINT_H)
    add_definitions (-DHAVE_STDINT_H)
endif (HAVE_STDINT_H)

# Global Configurations End
################################

## Include Urho3D cmake module
include (Urho3D-CMake-magic)

add_subdirectory(Source)