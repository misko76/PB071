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
CMAKE_SOURCE_DIR = C:\Users\Miso\Desktop\hw04

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Miso\Desktop\hw04\cmake-build-debug

# Include any dependencies generated for this target.
include src/CMakeFiles/merge-ini.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/merge-ini.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/merge-ini.dir/flags.make

src/CMakeFiles/merge-ini.dir/main.c.obj: src/CMakeFiles/merge-ini.dir/flags.make
src/CMakeFiles/merge-ini.dir/main.c.obj: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Miso\Desktop\hw04\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/merge-ini.dir/main.c.obj"
	cd /d C:\Users\Miso\Desktop\hw04\cmake-build-debug\src && C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\merge-ini.dir\main.c.obj   -c C:\Users\Miso\Desktop\hw04\src\main.c

src/CMakeFiles/merge-ini.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/merge-ini.dir/main.c.i"
	cd /d C:\Users\Miso\Desktop\hw04\cmake-build-debug\src && C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Miso\Desktop\hw04\src\main.c > CMakeFiles\merge-ini.dir\main.c.i

src/CMakeFiles/merge-ini.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/merge-ini.dir/main.c.s"
	cd /d C:\Users\Miso\Desktop\hw04\cmake-build-debug\src && C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Miso\Desktop\hw04\src\main.c -o CMakeFiles\merge-ini.dir\main.c.s

# Object files for target merge-ini
merge__ini_OBJECTS = \
"CMakeFiles/merge-ini.dir/main.c.obj"

# External object files for target merge-ini
merge__ini_EXTERNAL_OBJECTS =

src/merge-ini.exe: src/CMakeFiles/merge-ini.dir/main.c.obj
src/merge-ini.exe: src/CMakeFiles/merge-ini.dir/build.make
src/merge-ini.exe: src/libmerger_lib.a
src/merge-ini.exe: src/CMakeFiles/merge-ini.dir/linklibs.rsp
src/merge-ini.exe: src/CMakeFiles/merge-ini.dir/objects1.rsp
src/merge-ini.exe: src/CMakeFiles/merge-ini.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Miso\Desktop\hw04\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable merge-ini.exe"
	cd /d C:\Users\Miso\Desktop\hw04\cmake-build-debug\src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\merge-ini.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/merge-ini.dir/build: src/merge-ini.exe

.PHONY : src/CMakeFiles/merge-ini.dir/build

src/CMakeFiles/merge-ini.dir/clean:
	cd /d C:\Users\Miso\Desktop\hw04\cmake-build-debug\src && $(CMAKE_COMMAND) -P CMakeFiles\merge-ini.dir\cmake_clean.cmake
.PHONY : src/CMakeFiles/merge-ini.dir/clean

src/CMakeFiles/merge-ini.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Miso\Desktop\hw04 C:\Users\Miso\Desktop\hw04\src C:\Users\Miso\Desktop\hw04\cmake-build-debug C:\Users\Miso\Desktop\hw04\cmake-build-debug\src C:\Users\Miso\Desktop\hw04\cmake-build-debug\src\CMakeFiles\merge-ini.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/merge-ini.dir/depend

