# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files (x86)\clion\CLion 2020.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files (x86)\clion\CLion 2020.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Administrator\Desktop\aacdecoder\aacDecoder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug

# Include any dependencies generated for this target.
include src/bitStream/CMakeFiles/bitStream.dir/depend.make

# Include the progress variables for this target.
include src/bitStream/CMakeFiles/bitStream.dir/progress.make

# Include the compile flags for this target's objects.
include src/bitStream/CMakeFiles/bitStream.dir/flags.make

src/bitStream/CMakeFiles/bitStream.dir/bitStream.cpp.obj: src/bitStream/CMakeFiles/bitStream.dir/flags.make
src/bitStream/CMakeFiles/bitStream.dir/bitStream.cpp.obj: ../src/bitStream/bitStream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/bitStream/CMakeFiles/bitStream.dir/bitStream.cpp.obj"
	cd /d C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\src\bitStream && D:\PROGRA~1\C__~1\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\bitStream.dir\bitStream.cpp.obj -c C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\src\bitStream\bitStream.cpp

src/bitStream/CMakeFiles/bitStream.dir/bitStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bitStream.dir/bitStream.cpp.i"
	cd /d C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\src\bitStream && D:\PROGRA~1\C__~1\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\src\bitStream\bitStream.cpp > CMakeFiles\bitStream.dir\bitStream.cpp.i

src/bitStream/CMakeFiles/bitStream.dir/bitStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bitStream.dir/bitStream.cpp.s"
	cd /d C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\src\bitStream && D:\PROGRA~1\C__~1\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\src\bitStream\bitStream.cpp -o CMakeFiles\bitStream.dir\bitStream.cpp.s

bitStream: src/bitStream/CMakeFiles/bitStream.dir/bitStream.cpp.obj
bitStream: src/bitStream/CMakeFiles/bitStream.dir/build.make

.PHONY : bitStream

# Rule to build all files generated by this target.
src/bitStream/CMakeFiles/bitStream.dir/build: bitStream

.PHONY : src/bitStream/CMakeFiles/bitStream.dir/build

src/bitStream/CMakeFiles/bitStream.dir/clean:
	cd /d C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\src\bitStream && $(CMAKE_COMMAND) -P CMakeFiles\bitStream.dir\cmake_clean.cmake
.PHONY : src/bitStream/CMakeFiles/bitStream.dir/clean

src/bitStream/CMakeFiles/bitStream.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Administrator\Desktop\aacdecoder\aacDecoder C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\src\bitStream C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\src\bitStream C:\Users\Administrator\Desktop\aacdecoder\aacDecoder\cmake-build-debug\src\bitStream\CMakeFiles\bitStream.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src/bitStream/CMakeFiles/bitStream.dir/depend

