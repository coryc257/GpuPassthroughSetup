cmake_minimum_required(VERSION 3.0)
project(GpuPassthroughSetup)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Pthreads
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

set(gpupassthroughsetup_SRC
  util/SAFE_RETURN.cpp
  util/HANDLE_UI_RETURN.cpp
  util/MsgBox.cpp
  src/daemonize.cpp
  src/main.cpp
  #//src/gpupassthroughsetup.cpp
  src/exec_container.cpp
  ui/Main.cpp
  ui/CommandOutputHelp.cpp
  ui/XEdit.cpp
)

# Tell CMake to create the helloworld executable
add_executable(gpupassthroughsetup ${gpupassthroughsetup_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(gpupassthroughsetup Qt5::Widgets)

configure_file(scripts/find_groups.sh find_groups.sh COPYONLY)
add_custom_command(
    TARGET gpupassthroughsetup POST_BUILD
    COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/find_groups.sh
)

# Install the executable
install(TARGETS gpupassthroughsetup DESTINATION bin)
