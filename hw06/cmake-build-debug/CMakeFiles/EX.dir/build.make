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
CMAKE_SOURCE_DIR = C:\Users\Miso\Desktop\hw06

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Miso\Desktop\hw06\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/EX.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EX.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EX.dir/flags.make

CMakeFiles/EX.dir/example.c.obj: CMakeFiles/EX.dir/flags.make
CMakeFiles/EX.dir/example.c.obj: CMakeFiles/EX.dir/includes_C.rsp
CMakeFiles/EX.dir/example.c.obj: ../example.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Miso\Desktop\hw06\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/EX.dir/example.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\EX.dir\example.c.obj   -c C:\Users\Miso\Desktop\hw06\example.c

CMakeFiles/EX.dir/example.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EX.dir/example.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Miso\Desktop\hw06\example.c > CMakeFiles\EX.dir\example.c.i

CMakeFiles/EX.dir/example.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EX.dir/example.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Miso\Desktop\hw06\example.c -o CMakeFiles\EX.dir\example.c.s

# Object files for target EX
EX_OBJECTS = \
"CMakeFiles/EX.dir/example.c.obj"

# External object files for target EX
EX_EXTERNAL_OBJECTS =

EX.exe: CMakeFiles/EX.dir/example.c.obj
EX.exe: CMakeFiles/EX.dir/build.make
EX.exe: CMakeFiles/EX.dir/linklibs.rsp
EX.exe: CMakeFiles/EX.dir/objects1.rsp
EX.exe: CMakeFiles/EX.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Miso\Desktop\hw06\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable EX.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\EX.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EX.dir/build: EX.exe

.PHONY : CMakeFiles/EX.dir/build

CMakeFiles/EX.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\EX.dir\cmake_clean.cmake
.PHONY : CMakeFiles/EX.dir/clean

CMakeFiles/EX.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Miso\Desktop\hw06 C:\Users\Miso\Desktop\hw06 C:\Users\Miso\Desktop\hw06\cmake-build-debug C:\Users\Miso\Desktop\hw06\cmake-build-debug C:\Users\Miso\Desktop\hw06\cmake-build-debug\CMakeFiles\EX.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EX.dir/depend

