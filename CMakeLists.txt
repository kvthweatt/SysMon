cmake_minimum_required(VERSION 3.16)
project(system_monitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS 
    Core
    Widgets
    Charts
    Network
    REQUIRED
)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/monitors/cpumonitor.cpp
    src/monitors/rammonitor.cpp
    src/monitors/diskmonitor.cpp
    src/monitors/networkmonitor.cpp
    src/widgets/monitorwidget.cpp
)

set(HEADERS
    src/mainwindow.h
    src/monitors/cpumonitor.h
    src/monitors/rammonitor.h
    src/monitors/diskmonitor.h
    src/monitors/networkmonitor.h
    src/widgets/monitorwidget.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::Network
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
