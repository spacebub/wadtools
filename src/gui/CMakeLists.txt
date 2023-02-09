set(CMAKE_AUTOUIC_SEARCH_PATHS uic)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (QT5)
    find_package(Qt5 REQUIRED COMPONENTS
                Core
                Gui
                Widgets)
else()
    find_package(Qt6 REQUIRED COMPONENTS
                Core
                Gui
                Widgets)
endif()

add_executable(wdtls_gui)

target_sources(wdtls_gui
    PRIVATE
        wdtls_gui.cpp
        MainWindow.h
        MainWindow.cpp)

target_link_libraries(wdtls_gui
    PRIVATE
        wdtls_common
        Qt::Core
        Qt::Gui
        Qt::Widgets)

set_target_properties(wdtls_gui PROPERTIES OUTPUT_NAME WadTools)