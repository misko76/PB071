add_executable(unit_test
    # Add other test files here.
    runner.cpp
    view.c
)

# NOTE: Do not run ‹cmake› on this file directly;
#       instead, use the ‹CMakeLists.txt› in the parent directory.

# -----------------------------------------------------------------------------
# DO NOT EDIT LINES BELOW lest you break it

set(EXECUTABLE unit_test)
add_definitions(-DCUT -DTEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${EXECUTABLE} merger_lib)
include_directories(../src)

if (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
    target_compile_definitions(${EXECUTABLE} PRIVATE _CRT_SECURE_NO_DEPRECATE)
endif ()

if(MINGW)
    target_compile_definitions(${EXECUTABLE} PRIVATE __USE_MINGW_ANSI_STDIO=1)
endif ()
