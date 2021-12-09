cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(IS_CLANGCL true)
    else()
        set(IS_CLANG true)
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(IS_GCC true)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(IS_INTEL true)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(IS_MSVC true)
endif()

macro(setup target)
    # COMPILE
    # LINK
    # DEBUG_COMPILE
    # DEBUG_LINK
    # RELEASE_COMPILE
    # RELEASE_LINK
    # RELDEB_COMPILE
    # RELDEB_LINK

    if(DEFINED IS_CLANGCL OR DEFINED IS_MSVC)

        string(REGEX REPLACE "/Ob[12]"            "/Ob3" CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}")
        string(REGEX REPLACE "/Ob[12]"            "/Ob3" CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE}")
        string(REGEX REPLACE "/Ob[12]"            "/Ob3" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
        string(REGEX REPLACE "/Ob[12]"            "/Ob3" CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
        string(REGEX REPLACE "[/|-]RTC(su|[1su])" ""     CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG}")
        string(REGEX REPLACE "[/|-]RTC(su|[1su])" ""     CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG}")

        set(COMPILE
            /MP
            /cgthreads8
            /Zc:preprocessor
            /FC
            /wd5105
            /wd4133
            /wd4047
            /wd4028
            /wd4024
            /wd4005
            /GS-
            /GR-
            /EHa-s-c-
        )
        set(LINK /INCREMENTAL:NO /CGTHREADS:8 /NODEFAULTLIB /ENTRY:entry_point)

        set(DEBUG_COMPILE /JMC /Od)

        set(RELEASE_COMPILE /O2 /guard:cf- /D NDEBUG /JMC- /GL /Ob3 /fp:fast)
        set(RELEASE_LINK /OPT:REF /OPT:ICF=10000 /LTCG)

        set(RELDEB_COMPILE ${RELEASE_COMPILE})
        set(RELDEB_LINK    ${RELEASE_LINK})
    else()

        # -Wno-macro-redefined
        # -Wno-address-of-temporary
        # -Wno-bool-conversion
        # -Wno-int-conversion
        # -Wno-compare-distinct-pointer-types
        # -Wno-incompatible-pointer-types
        # -Wno-deprecated-comma-subscript

        set(COMPILE
            -fno-exceptions
            -fno-rtti
            -fno-builtin
            -ffreestanding
            -fdiagnostics-absolute-paths
            -fmacro-backtrace-limit=0
            -fdiagnostics-color
            -nodefaultlibs
            -nostdlib
        )
        set(LINK LINKER:/subsystem:console,/entry:entry_point)
        set(DEBUG_COMPILE -Og -g -DCONFIG_RELEASE=0 -DCONFIG_DEBUG=1)
        set(RELEASE_COMPILE -Ofast -flto -march=native -DCONFIG_RELEASE=1 -DCONFIG_DEBUG=0)
        set(RELDEB_COMPILE ${RELEASE_COMPILE} -g)
    endif()

    target_compile_definitions(${target} PUBLIC _CRT_SECURE_NO_WARNINGS)

    target_compile_options(${target} PUBLIC "${COMPILE}")
    target_compile_options(${target} PUBLIC "$<$<CONFIG:Debug>:${DEBUG_COMPILE}>")
    target_compile_options(${target} PUBLIC "$<$<CONFIG:Release>:${RELEASE_COMPILE}>")
    target_compile_options(${target} PUBLIC "$<$<CONFIG:RelWithDebInfo>:${RELDEB_COMPILE}>")

    target_link_options(${target} PUBLIC "${LINK}")
    target_link_options(${target} PUBLIC "$<$<CONFIG:Debug>:${DEBUG_LINK}>")
    target_link_options(${target} PUBLIC "$<$<CONFIG:Release>:${RELEASE_LINK}>")
    target_link_options(${target} PUBLIC "$<$<CONFIG:RelWithDebInfo>:${RELDEB_LINK}>")

    set_target_properties(
        ${target} PROPERTIES
        C_STANDARD 11
        C_EXTENSIONS ON
        CXX_STANDARD 20
        CXX_EXTENSIONS ON
    )

    target_include_directories(
        ${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/>
    )
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
endmacro()

macro(make_static_library target)
    find_sources()
    add_library(${target} STATIC ${SOURCES})
    setup(${target})
endmacro()

macro(make_dynamic_library target)
    find_sources()
    add_library(${target} SHARED ${SOURCES})
    setup(${target})
endmacro()

macro(make_executable target)
    find_sources()
    add_executable(${target} ${SOURCES})
    setup(${target})
endmacro()

macro(find_sources)
    file(
        GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/res/*.qrc"
    )

    set(SOURCES ${SOURCES} PARENT_SCOPE)
endmacro()
