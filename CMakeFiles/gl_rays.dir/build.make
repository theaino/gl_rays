# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aino/Documents/gl_rays

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aino/Documents/gl_rays

# Include any dependencies generated for this target.
include CMakeFiles/gl_rays.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gl_rays.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gl_rays.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gl_rays.dir/flags.make

CMakeFiles/gl_rays.dir/src/main.cpp.o: CMakeFiles/gl_rays.dir/flags.make
CMakeFiles/gl_rays.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/gl_rays.dir/src/main.cpp.o: CMakeFiles/gl_rays.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/aino/Documents/gl_rays/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gl_rays.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gl_rays.dir/src/main.cpp.o -MF CMakeFiles/gl_rays.dir/src/main.cpp.o.d -o CMakeFiles/gl_rays.dir/src/main.cpp.o -c /home/aino/Documents/gl_rays/src/main.cpp

CMakeFiles/gl_rays.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/gl_rays.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aino/Documents/gl_rays/src/main.cpp > CMakeFiles/gl_rays.dir/src/main.cpp.i

CMakeFiles/gl_rays.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/gl_rays.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aino/Documents/gl_rays/src/main.cpp -o CMakeFiles/gl_rays.dir/src/main.cpp.s

# Object files for target gl_rays
gl_rays_OBJECTS = \
"CMakeFiles/gl_rays.dir/src/main.cpp.o"

# External object files for target gl_rays
gl_rays_EXTERNAL_OBJECTS =

bin/gl_rays: CMakeFiles/gl_rays.dir/src/main.cpp.o
bin/gl_rays: CMakeFiles/gl_rays.dir/build.make
bin/gl_rays: /usr/lib/libOpenGL.so
bin/gl_rays: /usr/lib/libGLX.so
bin/gl_rays: /usr/lib/libGLU.so
bin/gl_rays: CMakeFiles/gl_rays.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/aino/Documents/gl_rays/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/gl_rays"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gl_rays.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gl_rays.dir/build: bin/gl_rays
.PHONY : CMakeFiles/gl_rays.dir/build

CMakeFiles/gl_rays.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gl_rays.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gl_rays.dir/clean

CMakeFiles/gl_rays.dir/depend:
	cd /home/aino/Documents/gl_rays && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aino/Documents/gl_rays /home/aino/Documents/gl_rays /home/aino/Documents/gl_rays /home/aino/Documents/gl_rays /home/aino/Documents/gl_rays/CMakeFiles/gl_rays.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/gl_rays.dir/depend

