# Updated CMakeLists.txt
# FluentWidgetLib Library for CMake projects
cmake_minimum_required(VERSION 3.21)
project(FluentWidgetsPlugin VERSION 1.0.0)

# Only find Qt if not already found by parent
if(NOT TARGET Qt6::Widgets)
    find_package(Qt6 REQUIRED COMPONENTS Widgets Designer Charts)
    qt_standard_project_setup()
endif()

# Ensure Qt6::Charts is available
find_package(Qt6 REQUIRED COMPONENTS Charts)

# Create STATIC LIBRARY (all widgets, not plugin)
qt6_add_library(FluentWidgetLib STATIC
    # All widget headers
    widgets/cardwidget.h
    widgets/drawerwidget.h
    widgets/messagebarwidget.h
    widgets/graphcardwidget.h
    widgets/modalwidget.h
    
    # All widget sources
    widgets/cardwidget.cpp
    widgets/drawerwidget.cpp
    widgets/messagebarwidget.cpp
    widgets/graphcardwidget.cpp
    widgets/modalwidget.cpp
    
    # Add future widgets here:
    # widgets/metricwidget.h
    # widgets/metricwidget.cpp
)

target_link_libraries(FluentWidgetLib PUBLIC
    Qt6::Widgets
    Qt6::Charts
)

# Make headers available to parent projects
target_include_directories(FluentWidgetLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
)

# Add resources if exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc)
    qt6_add_resources(FluentWidgetLib "icons" 
        PREFIX "/"
        BASE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES resources/icons.qrc
    )
endif()

# Set C++ standard for both targets
set_target_properties(FluentWidgetLib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(TARGET FluentWidgetsPlugin)
    set_target_properties(FluentWidgetsPlugin PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endif()

# Handle fluentwidget.pri equivalent (if you have common configurations)
# Note: You may need to convert any qmake-specific settings from fluentwidget.pri
# to CMake equivalents here

message(STATUS "FluentWidgetLib: Library target created")

# Example usage instructions
message(STATUS "")
message(STATUS "=== FluentWidgetsPlugin Usage ===")
message(STATUS "Library: Link with FluentWidgetLib in your CMake project")
message(STATUS "Plugin: Copy built plugin to Qt Designer plugins directory")
message(STATUS "Widgets included: CardWidget, DrawerWidget, MessageBarWidget, GraphCardWidget, ModalWidget")
message(STATUS "=================================")
