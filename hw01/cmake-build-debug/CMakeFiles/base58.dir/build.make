# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Miso\Desktop\PB071\hw01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Miso\Desktop\PB071\hw01\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/base58.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/base58.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/base58.dir/flags.make

CMakeFiles/base58.dir/base58.c.obj: CMakeFiles/base58.dir/flags.make
CMakeFiles/base58.dir/base58.c.obj: ../base58.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Miso\Desktop\PB071\hw01\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/base58.dir/base58.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\base58.dir\base58.c.obj   -c C:\Users\Miso\Desktop\PB071\hw01\base58.c

CMakeFiles/base58.dir/base58.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/base58.dir/base58.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Miso\Desktop\PB071\hw01\base58.c > CMakeFiles\base58.dir\base58.c.i

CMakeFiles/base58.dir/base58.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/base58.dir/base58.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Miso\Desktop\PB071\hw01\base58.c -o CMakeFiles\base58.dir\base58.c.s

# Object files for target base58
base58_OBJECTS = \
"CMakeFiles/base58.dir/base58.c.obj"

# External object files for target base58
base58_EXTERNAL_OBJECTS =

base58.exe: CMakeFiles/base58.dir/base58.c.obj
base58.exe: CMakeFiles/base58.dir/build.make
base58.exe: CMakeFiles/base58.dir/linklibs.rsp
base58.exe: CMakeFiles/base58.dir/objects1.rsp
base58.exe: CMakeFiles/base58.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Miso\Desktop\PB071\hw01\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable base58.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\base58.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/base58.dir/build: base58.exe

.PHONY : CMakeFiles/base58.dir/build

CMakeFiles/base58.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\base58.dir\cmake_clean.cmake
.PHONY : CMakeFiles/base58.dir/clean

CMakeFiles/base58.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Miso\Desktop\PB071\hw01 C:\Users\Miso\Desktop\PB071\hw01 C:\Users\Miso\Desktop\PB071\hw01\cmake-build-debug C:\Users\Miso\Desktop\PB071\hw01\cmake-build-debug C:\Users\Miso\Desktop\PB071\hw01\cmake-build-debug\CMakeFiles\base58.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/base58.dir/depend
