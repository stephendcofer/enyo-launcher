CMAKE_MINIMUM_REQUIRED( VERSION 3.16 )

PROJECT( enyo-launcher )

set (CMAKE_AUTORCC ON)
find_package (Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
set (SOURCES src/e_mainwindow.cpp src/main.cpp src/enyo-launcher.qrc)
qt_wrap_ui (SOURCES e_mainwindow.ui)
qt_add_executable (enyo-launcher ${SOURCES})

target_link_libraries(enyo-launcher PRIVATE Qt6::Widgets)

INSTALL( TARGETS enyo-launcher DESTINATION bin )
INSTALL( FILES ./Changelog ./COPYING DESTINATION share/enyo-launcher )
INSTALL( FILES ./share/enyo-launcher.desktop DESTINATION share/applications )
INSTALL( FILES ./share/enyo_icon.png DESTINATION share/icons )
