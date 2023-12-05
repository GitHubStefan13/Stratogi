
cmake_minimum_required(VERSION 3.16)

set( VERSION_MAJOR "0" )
set( VERSION_MINOR "1" )
set( VERSION_PATCH "0" )
set( VERSION_FULL ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
project(Stratogi VERSION ${VERSION_FULL} LANGUAGES CXX)

# -----------------
# Compiler Settings
# -----------------
# Qt6 requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Verbose output while compiling
set(CMAKE_VERBOSE_MAKEFILE ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# So we can add a .qrc as Resourcefile
set(CMAKE_AUTORCC ON)
# -----------------

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick QuickControls2 Concurrent)
qt_standard_project_setup()

# Source Files
file(GLOB SOURCES
     "src/*.cpp"
     "src/Abstract/*.cpp"
     "src/Chess/*.cpp"
     "src/Shogi/*.cpp"
     "README.MD"
)

# Header Files
file(GLOB HEADERS
     "includes/*.h"
     "includes/*.hpp"
     "includes/Abstract/*.h"
     "includes/Abstract/*.hpp"
     "includes/Chess/*.h"
     "includes/Chess/*.hpp"
     "includes/Shogi/*.h"
     "includes/Shogi/*.hpp"
)

# Add everything to an executable
qt_add_executable(${PROJECT_NAME}
  res.qrc
  ${SOURCES}
  ${HEADERS}
)

# Add Includepath's so we can include files directly without writing the fullpath
include_directories(includes includes/Abstract includes/Chess includes/Shogi)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Qml Qt6::Qml Qt6::QuickControls2)

