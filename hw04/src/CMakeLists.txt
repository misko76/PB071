add_library(merger_lib STATIC
    # Here add files you want to compile into your solution.
    ini_parser.c
)

# NOTE: Do not run ‹cmake› on this file directly;
#       instead, use the ‹CMakeLists.txt› in the parent directory.

# -----------------------------------------------------------------------------
# DO NOT EDIT LINES BELOW lest you break it

set(EXECUTABLE merge-ini)
add_executable(${EXECUTABLE} main.c)

target_link_libraries(${EXECUTABLE} merger_lib)

if (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
    target_compile_definitions(${EXECUTABLE} PRIVATE _CRT_SECURE_NO_DEPRECATE)
endif ()

if(MINGW)
    target_compile_definitions(${EXECUTABLE} PRIVATE __USE_MINGW_ANSI_STDIO=1)
endif ()
