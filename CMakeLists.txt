cmake_minimum_required(VERSION 3.23.2)
project(2_lab_5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Путь к вашей Qt
set(CMAKE_PREFIX_PATH "C:/qt_sample_app/ext/qt-6.12.0-win10-22H2-x64-msvc-194/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    character.cpp
    magp.cpp
    vragp.cpp
    carddialog.cpp
    armor.cpp
)

set(HEADERS
    mainwindow.h
    character.h
    magp.h
    vragp.h
    armor.h
    carddialog.h
)

set(FORMS
    mainwindow.ui
    carddialog.ui
)

add_executable(2_lab_5 ${SOURCES} ${HEADERS} ${FORMS}
    armor.cpp)

target_link_libraries(2_lab_5 Qt6::Core Qt6::Widgets)

if(WIN32)
    set_target_properties(2_lab_5 PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
