# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/el/projects/GpuPassthroughSetup

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/el/projects/GpuPassthroughSetup/build

# Include any dependencies generated for this target.
include CMakeFiles/gpupassthroughsetup.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gpupassthroughsetup.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gpupassthroughsetup.dir/flags.make

CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.o: gpupassthroughsetup_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.o -c /home/el/projects/GpuPassthroughSetup/build/gpupassthroughsetup_autogen/mocs_compilation.cpp

CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/build/gpupassthroughsetup_autogen/mocs_compilation.cpp > CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.i

CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/build/gpupassthroughsetup_autogen/mocs_compilation.cpp -o CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.s

CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.o: ../src/daemonize.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.o -c /home/el/projects/GpuPassthroughSetup/src/daemonize.cpp

CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/src/daemonize.cpp > CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.i

CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/src/daemonize.cpp -o CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.s

CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.o -c /home/el/projects/GpuPassthroughSetup/src/main.cpp

CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/src/main.cpp > CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.i

CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/src/main.cpp -o CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.s

CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.o: ../src/exec_container.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.o -c /home/el/projects/GpuPassthroughSetup/src/exec_container.cpp

CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/src/exec_container.cpp > CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.i

CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/src/exec_container.cpp -o CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.s

CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.o: ../ui/Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.o -c /home/el/projects/GpuPassthroughSetup/ui/Main.cpp

CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/ui/Main.cpp > CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.i

CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/ui/Main.cpp -o CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.s

CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.o: ../ui/CommandOutputHelp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.o -c /home/el/projects/GpuPassthroughSetup/ui/CommandOutputHelp.cpp

CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/ui/CommandOutputHelp.cpp > CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.i

CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/ui/CommandOutputHelp.cpp -o CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.s

CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.o: CMakeFiles/gpupassthroughsetup.dir/flags.make
CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.o: ../ui/XEdit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.o -c /home/el/projects/GpuPassthroughSetup/ui/XEdit.cpp

CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/el/projects/GpuPassthroughSetup/ui/XEdit.cpp > CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.i

CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/el/projects/GpuPassthroughSetup/ui/XEdit.cpp -o CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.s

# Object files for target gpupassthroughsetup
gpupassthroughsetup_OBJECTS = \
"CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.o" \
"CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.o" \
"CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.o" \
"CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.o" \
"CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.o" \
"CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.o"

# External object files for target gpupassthroughsetup
gpupassthroughsetup_EXTERNAL_OBJECTS =

gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/gpupassthroughsetup_autogen/mocs_compilation.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/src/daemonize.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/src/main.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/src/exec_container.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/ui/Main.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/ui/CommandOutputHelp.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/ui/XEdit.cpp.o
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/build.make
gpupassthroughsetup: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.15.2
gpupassthroughsetup: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.15.2
gpupassthroughsetup: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.2
gpupassthroughsetup: CMakeFiles/gpupassthroughsetup.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/el/projects/GpuPassthroughSetup/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable gpupassthroughsetup"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gpupassthroughsetup.dir/link.txt --verbose=$(VERBOSE)
	chmod +x /home/el/projects/GpuPassthroughSetup/build/find_groups.sh

# Rule to build all files generated by this target.
CMakeFiles/gpupassthroughsetup.dir/build: gpupassthroughsetup

.PHONY : CMakeFiles/gpupassthroughsetup.dir/build

CMakeFiles/gpupassthroughsetup.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gpupassthroughsetup.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gpupassthroughsetup.dir/clean

CMakeFiles/gpupassthroughsetup.dir/depend:
	cd /home/el/projects/GpuPassthroughSetup/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/el/projects/GpuPassthroughSetup /home/el/projects/GpuPassthroughSetup /home/el/projects/GpuPassthroughSetup/build /home/el/projects/GpuPassthroughSetup/build /home/el/projects/GpuPassthroughSetup/build/CMakeFiles/gpupassthroughsetup.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gpupassthroughsetup.dir/depend

