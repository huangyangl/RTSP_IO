# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/jetson/shd/C++_ws/RTSP_IO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jetson/shd/C++_ws/RTSP_IO

# Include any dependencies generated for this target.
include CMakeFiles/streaming.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/streaming.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/streaming.dir/flags.make

CMakeFiles/streaming.dir/streaming.cpp.o: CMakeFiles/streaming.dir/flags.make
CMakeFiles/streaming.dir/streaming.cpp.o: streaming.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jetson/shd/C++_ws/RTSP_IO/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/streaming.dir/streaming.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/streaming.dir/streaming.cpp.o -c /home/jetson/shd/C++_ws/RTSP_IO/streaming.cpp

CMakeFiles/streaming.dir/streaming.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/streaming.dir/streaming.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jetson/shd/C++_ws/RTSP_IO/streaming.cpp > CMakeFiles/streaming.dir/streaming.cpp.i

CMakeFiles/streaming.dir/streaming.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/streaming.dir/streaming.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jetson/shd/C++_ws/RTSP_IO/streaming.cpp -o CMakeFiles/streaming.dir/streaming.cpp.s

# Object files for target streaming
streaming_OBJECTS = \
"CMakeFiles/streaming.dir/streaming.cpp.o"

# External object files for target streaming
streaming_EXTERNAL_OBJECTS =

streaming: CMakeFiles/streaming.dir/streaming.cpp.o
streaming: CMakeFiles/streaming.dir/build.make
streaming: CMakeFiles/streaming.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jetson/shd/C++_ws/RTSP_IO/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable streaming"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/streaming.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/streaming.dir/build: streaming

.PHONY : CMakeFiles/streaming.dir/build

CMakeFiles/streaming.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/streaming.dir/cmake_clean.cmake
.PHONY : CMakeFiles/streaming.dir/clean

CMakeFiles/streaming.dir/depend:
	cd /home/jetson/shd/C++_ws/RTSP_IO && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jetson/shd/C++_ws/RTSP_IO /home/jetson/shd/C++_ws/RTSP_IO /home/jetson/shd/C++_ws/RTSP_IO /home/jetson/shd/C++_ws/RTSP_IO /home/jetson/shd/C++_ws/RTSP_IO/CMakeFiles/streaming.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/streaming.dir/depend

