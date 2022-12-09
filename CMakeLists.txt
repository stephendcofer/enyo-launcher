CMAKE_MINIMUM_REQUIRED( VERSION 2.8.12 )

PROJECT( enyo-launcher )

SET( TARGET enyo-launcher )
SET( ENYO_LAUNCHER_SRCS
	./src/e_mainwindow.cpp
	./src/main.cpp
)

SET( ENYO_LAUNCHER_HDRS
	./src/e_mainwindow.h
)

SET( ENYO_LAUNCHER_UIS
	./src/e_mainwindow.ui
)

SET( ENYO_LAUNCHER_RCS
	./src/enyo-launcher.qrc
)
include_directories ( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
find_package ( Qt5Core REQUIRED )
find_package (Qt5Widgets REQUIRED )
add_definitions ( $(Qt5Core_DEFINITIONS) )
add_definitions ( $(Qt5Widgets_DEFINITIONS) )
qt5_wrap_ui ( UI_HEADERS ${ENYO_LAUNCHER_UIS} )
qt5_wrap_cpp ( MOC_SOURCES ${ENYO_LAUNCHER_HDRS} )
qt5_add_resources ( RC_SOURCES ${ENYO_LAUNCHER_RCS} )
include_directories ( ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} )
add_executable ( enyo-launcher ${UI_HEADERS} ${ENYO_LAUNCHER_SRCS} ${ENYO_LAUNCHER_HDRS} ${MOC_SOURCES} ${RC_SOURCES} )
target_link_libraries ( enyo-launcher ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} )

INSTALL( TARGETS enyo-launcher DESTINATION bin )
INSTALL( FILES ./Changelog ./COPYING DESTINATION share/enyo-launcher )
INSTALL( FILES ./share/enyo-launcher.desktop DESTINATION share/applications )
INSTALL( FILES ./share/enyo_icon.png DESTINATION share/icons )
