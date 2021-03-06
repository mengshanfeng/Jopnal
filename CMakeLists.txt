# Jopnal Engine CMakeLists

# 3.1 is the minimum for pdb file generation and forcing C++11
cmake_minimum_required(VERSION 3.1.0)

# Helper for defining options
macro(jopSetOption var default type docstring)

    if (NOT DEFINED ${var})
    
        set(${var} ${default})
        
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)

endmacro()


# Configuration option
jopSetOption(CMAKE_BUILD_TYPE Release STRING "Type of build")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
             "Debug" "Release")

# Android options, must be set in the beginning

# API level
jopSetOption(ANDROID_API_MIN 18 STRING "Choose the Android API level (greater or equal to 15)")
set_property(CACHE ANDROID_API_MIN PROPERTY STRINGS
             "15" "16" "17" "18" "19" "21" "23" "24")

# Setup version value for the toolchain
set(ANDROID_NATIVE_API_LEVEL ${ANDROID_API_MIN})

# Set toolchain to use
jopSetOption(ANDROID_TOOLCHAIN_NAME "arm-linux-androideabi-4.9" STRING "Choose the android toolchain to use")

# Enable RTTI
set(ANDROID_RTTI TRUE)

# STL implementation
jopSetOption(ANDROID_STL c++_shared STRING "Choose the STL implementation")

# Default ABI
if (NOT ANDROID_ABI)
    set(ANDROID_ABI "armeabi-v7a with NEON")
endif()

# Project name
project(Jopnal)

# Ignore CMP0058
cmake_policy(SET CMP0058 NEW)

# Include modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Hide non-essential options
mark_as_advanced(COTIRE_ADDITIONAL_PREFIX_HEADER_IGNORE_EXTENSIONS)
mark_as_advanced(COTIRE_ADDITIONAL_PREFIX_HEADER_IGNORE_PATH)
mark_as_advanced(COTIRE_DEBUG)
mark_as_advanced(COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES)
mark_as_advanced(COTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES)
mark_as_advanced(COTIRE_UNITY_SOURCE_EXCLUDE_EXTENSIONS)
mark_as_advanced(COTIRE_VERBOSE)
mark_as_advanced(PHYSFS_INCLUDE_DIR)
mark_as_advanced(PHYSFS_LIBRARY)
mark_as_advanced(PHYSFS_LIBRARY)
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(BULLET_INCLUDE_DIR)
mark_as_advanced(ANDROID_NATIVE_API_LEVEL)
mark_as_advanced(CMAKE_ASM_COMPILER)
mark_as_advanced(CMAKE_TOOLCHAIN_FILE)


# Cotire for precompiled headers
set(COTIRE_PCH_MEMORY_SCALING_FACTOR "500")
include(cotire)

# Include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# Option for precompiled headers
set(USE_PRECOMPILED_HEADER_DEFAULT TRUE)

# Set precompiled header utilization to false on android & MinGW, as
# it tends to crash the compiler
if (JOP_OS_ANDROID OR (JOP_OS_WINDOWS AND JOP_COMPILER_GCC))
    set(USE_PRECOMPILED_HEADER_DEFAULT FALSE)
endif() 
   
jopSetOption(JOP_USE_PRECOMPILED_HEADER ${USE_PRECOMPILED_HEADER_DEFAULT} BOOL "True to use precompiled header")

if (JOP_USE_PRECOMPILED_HEADER AND JOP_COMPILER_GCC)
    message(WARNING "Using precompiled header may crash GCC. Keep this in mind")
endif()

# Version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 3)

# Header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Option for choosing a shared build
jopSetOption(BUILD_SHARED_LIBS TRUE BOOL "True to build Jopnal as shared, false to build as static")

# Option for choosing the OpenGL implementation
jopSetOption(JOP_OPENGL_ES ${OPENGL_ES} BOOL "True to use an OpenGL ES, false to use a desktop OpenGL")

# Choose debug verbosity
jopSetOption(JOP_CONSOLE_VERBOSITY "1" STRING "Select the compile-time console verbosity. -1 to disable, 3 for most verbose output. It's recommended to set this to 0 or -1 for release builds")
set_property(CACHE JOP_CONSOLE_VERBOSITY PROPERTY STRINGS
             "-1" "0" "1" "2" "3")
add_definitions("-DJOP_CONSOLE_VERBOSITY=${JOP_CONSOLE_VERBOSITY}")

# Option to enable/disable assertions
jopSetOption(JOP_ASSERTS TRUE BOOL "True to enable assertions, false to strip them away. It's recommended to disable assertions for release builds")

if (JOP_ASSERTS)
    add_definitions("-DJOP_ENABLE_ASSERTS")
endif()

# Option to enable/disable OpenGL error checks
jopSetOption(JOP_OPENGL_ERROR_CHECKS TRUE BOOL "True to enable OpenGL error checks, false to strip them away. It's recommended to disable these checks for release builds")

if (JOP_OPENGL_ERROR_CHECKS)
    add_definitions("-DJOP_OPENGL_ERROR_CHECKS")
endif()

# Option to enable/disable OpenAL error checks
jopSetOption(JOP_OPENAL_ERROR_CHECKS TRUE BOOL "True to enable OpenAL error checks, false to strip them away. It's recommended to disable these checks for release builds")

if (JOP_OPENAL_ERROR_CHECKS)
    add_definitions("-DJOP_OPENAL_ERROR_CHECKS")
endif()

# Generate documentation
jopSetOption(JOP_GENERATE_DOCS FALSE BOOL "True to generate documentation, false otherwise")

# Build examples
if (NOT JOP_OS_ANDROID)
    jopSetOption(JOP_BUILD_EXAMPLES FALSE BOOL "True to build examples, false otherwise")
endif()

# Android options
if (JOP_OS_ANDROID)

    # Make sure there's the android library available
    if (${ANDROID_API_MIN} LESS 15)
        message(FATAL_ERROR "Android API level must be equal or greater than 15. Please adjust the CMake variable 'ANDROID_API_MIN'")
    endif()

    if (NOT ANDROID_NDK)
        message(FATAL_ERROR "The Android NDK couldn't be found. Please adjust the CMake variable 'ANDROID_NDK' to point to the NDK directory")
    endif()

    # CMake doesn't support defining the STL with Nsight Tegra
    if (CMAKE_VS_PLATFORM_NAME STREQUAL "Tegra-Android")
        message(WARNING "CMake might not properly support setting the STL. Make sure to adjust all generated library projects!")
    endif()

    # Install everything in $NDK/sources/
    set(CMAKE_INSTALL_PREFIX ${ANDROID_NDK}/sources/jopnal)
    
    string(FIND ${ANDROID_ABI} " " SPACE_POS)
    string(SUBSTRING ${ANDROID_ABI} 0 ${SPACE_POS} ANDROID_ABI_DIR)

    # Install libs in a subdirectory named after the ABI
    set(LIB_SUFFIX "/${ANDROID_ABI_DIR}")

    # Pass shared STL configuration
    if (ANDROID_STL MATCHES "_shared")
        add_definitions("-DSTL_LIBRARY=${ANDROID_STL}")
    endif()

    # Force OpenGL ES
    unset(JOP_OPENGL_ES CACHE)
    set(JOP_OPENGL_ES TRUE)

    # Workaround to compile activity without the stl library as a dependency
    set(CMAKE_CXX_CREATE_SHARED_LIBRARY_WITH_STL ${CMAKE_CXX_CREATE_SHARED_LIBRARY})
    set(CMAKE_CXX_CREATE_SHARED_LIBRARY_WITHOUT_STL "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")

else()

    unset(ANDROID_ABI CACHE)
    unset(ANDROID_API_MIN CACHE)
    unset(ANDROID_STL CACHE)
    unset(ANDROID_NATIVE_API_LEVEL CACHE)
    unset(ANDROID_NDK CACHE)
    unset(ANDROID_TOOLCHAIN_NAME CACHE)

endif()

# Disable OpenGL ES on Windows
if (JOP_OS_WINDOWS)

    unset(JOP_OPENGL_ES CACHE)
    set(JOP_OPENGL_ES FALSE)
    
endif()

# Define JOP_STATIC to build static library
if (NOT BUILD_SHARED_LIBS)
    add_definitions(-DJOP_STATIC)
endif()

if (JOP_COMPILER_MSVC)

    jopSetOption(JOP_GENERATE_PDB TRUE BOOL "True to generate PDB debug symbol files, false otherwise")
    
    # - Disable deprecation warnings
    # - Set warning level to 4
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS /W4)

endif()

# Define JOP_OPENGL_ES if configured
if (JOP_OPENGL_ES)

    add_definitions(-DJOP_OPENGL_ES)
    add_definitions(-DGL_GLEXT_PROTOTYPES)

    jopSetOption(JOP_MIN_OPENGL_ES_VERSION 200 STRING, "Select the minimum supported GLES version. Forcing a greater version will improve features, stability and performance but limit the amount of supported devices")
    set_property(CACHE JOP_MIN_OPENGL_ES_VERSION PROPERTY STRINGS
                 "200" "300" "310" "320")
    
    if (JOP_OS_ANDROID)
    
        if (${JOP_MIN_OPENGL_ES_VERSION} GREATER 200 AND ${ANDROID_API_MIN} LESS 18)
            message(FATAL_ERROR "OpenGL ES 3.0 requires API level 18 or greater")
        
        elseif (${JOP_MIN_OPENGL_ES_VERSION} GREATER 300 AND ${ANDROID_API_MIN} LESS 23)
            message(FATAL_ERROR "OpenGL ES 3.1 requires API level 23 or greater")
            
        elseif (${JOP_MIN_OPENGL_ES_VERSION} GREATER 300 AND ${ANDROID_API_MIN} LESS 23)
            message(FATAL_ERROR "OpenGL ES 3.2 requires API level 24 or greater")

        endif()
    
    endif()

    add_definitions("-DJOP_MIN_OPENGL_ES_VERSION=${JOP_MIN_OPENGL_ES_VERSION}")
    
endif()

# Force inclusion of available c++11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

# Add the subdirectories
add_subdirectory(src/Jopnal)

# Examples
if (JOP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Setup the install rules
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl" PATTERN "*.h")

if (JOP_GENERATE_PDB)
    install(DIRECTORY ${PROJECT_BINARY_DIR}/lib
            DESTINATION .
            COMPONENT devel
            FILES_MATCHING PATTERN "*.pdb")
endif()

install(FILES LICENSE DESTINATION ${INSTALL_MISC_DIR})
install(FILES README.md DESTINATION ${INSTALL_MISC_DIR})

# Install 3rd-party libraries and tools
if (JOP_OS_ANDROID)

    # Install extlibs
    install(FILES extlibs/Android.mk DESTINATION extlibs)

    # Install Android.mk
    if (BUILD_SHARED_LIBS)
        install(FILES tools/Android/shared/Android.mk DESTINATION .)
    else ()
        install(FILES tools/Android/static/Android.mk DESTINATION .)
    endif()
    
endif()