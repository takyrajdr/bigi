# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/takyrajdr/Documents/research/software/bigi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/takyrajdr/Documents/research/software/bigi/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/desktop_example.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/desktop_example.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/desktop_example.dir/flags.make

examples/CMakeFiles/desktop_example.dir/example.c.o: examples/CMakeFiles/desktop_example.dir/flags.make
examples/CMakeFiles/desktop_example.dir/example.c.o: ../examples/example.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/takyrajdr/Documents/research/software/bigi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/desktop_example.dir/example.c.o"
	cd /home/takyrajdr/Documents/research/software/bigi/build/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/desktop_example.dir/example.c.o   -c /home/takyrajdr/Documents/research/software/bigi/examples/example.c

examples/CMakeFiles/desktop_example.dir/example.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/desktop_example.dir/example.c.i"
	cd /home/takyrajdr/Documents/research/software/bigi/build/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/takyrajdr/Documents/research/software/bigi/examples/example.c > CMakeFiles/desktop_example.dir/example.c.i

examples/CMakeFiles/desktop_example.dir/example.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/desktop_example.dir/example.c.s"
	cd /home/takyrajdr/Documents/research/software/bigi/build/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/takyrajdr/Documents/research/software/bigi/examples/example.c -o CMakeFiles/desktop_example.dir/example.c.s

# Object files for target desktop_example
desktop_example_OBJECTS = \
"CMakeFiles/desktop_example.dir/example.c.o"

# External object files for target desktop_example
desktop_example_EXTERNAL_OBJECTS =

../bin/desktop_example: examples/CMakeFiles/desktop_example.dir/example.c.o
../bin/desktop_example: examples/CMakeFiles/desktop_example.dir/build.make
../bin/desktop_example: ../lib/libbigi.a
../bin/desktop_example: examples/CMakeFiles/desktop_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/takyrajdr/Documents/research/software/bigi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../bin/desktop_example"
	cd /home/takyrajdr/Documents/research/software/bigi/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/desktop_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/desktop_example.dir/build: ../bin/desktop_example

.PHONY : examples/CMakeFiles/desktop_example.dir/build

examples/CMakeFiles/desktop_example.dir/clean:
	cd /home/takyrajdr/Documents/research/software/bigi/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/desktop_example.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/desktop_example.dir/clean

examples/CMakeFiles/desktop_example.dir/depend:
	cd /home/takyrajdr/Documents/research/software/bigi/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/takyrajdr/Documents/research/software/bigi /home/takyrajdr/Documents/research/software/bigi/examples /home/takyrajdr/Documents/research/software/bigi/build /home/takyrajdr/Documents/research/software/bigi/build/examples /home/takyrajdr/Documents/research/software/bigi/build/examples/CMakeFiles/desktop_example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/desktop_example.dir/depend
