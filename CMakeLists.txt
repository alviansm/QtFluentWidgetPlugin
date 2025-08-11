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
    src/widget/fluentcardwidget.h
    src/widget/fluentdrawerwidget.h
    src/widget/fluentbuttonwidget.h
    src/widget/fluentlistcardwidget.h
    src/widget/fluentmessagebarwidget.h
    src/widget/fluentgraphcardwidget.h
    src/widget/fluentmodalwidget.h
    src/widget/fluentplaincardwidget.h

    # All widget sources
    src/widget/fluentcardwidget.cpp
    src/widget/fluentdrawerwidget.cpp
    src/widget/fluentbuttonwidget.cpp
    src/widget/fluentlistcardwidget.cpp
    src/widget/fluentmessagebarwidget.cpp
    src/widget/fluentgraphcardwidget.cpp
    src/widget/fluentmodalwidget.cpp
    src/widget/fluentplaincardwidget.cpp

    # Add future widgets here:
    # src/widget/fluentfuturecwidget.h
    # src/widget/fluentfuturecwidget.cpp
)

# Configure Qt features for static library
set_target_properties(FluentWidgetLib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    POSITION_INDEPENDENT_CODE ON
)

# Define static library compilation
target_compile_definitions(FluentWidgetLib PUBLIC
    QT_STATICPLUGIN
    FLUENTWIDGET_STATIC
)

# Link Qt libraries
target_link_libraries(FluentWidgetLib PUBLIC
    Qt6::Widgets
    Qt6::Charts
    Qt6::Designer
)

# Make headers available to parent projects
target_include_directories(FluentWidgetLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/widget>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/plugin>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# Add resources if exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc)
    qt6_add_resources(FluentWidgetLib "icons"
        PREFIX "/"
        BASE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES resources/icons.qrc
    )
endif()

# Optional: Create the plugin target for Designer (if needed)
if(BUILD_DESIGNER_PLUGIN)
    qt6_add_library(FluentWidgetsDesignerPlugin MODULE
        # Plugin Headers (Designer integration)
        src/plugin/fluentwidgetsplugin.h
        src/plugin/fluentcardwidgetplugin.h
        src/plugin/fluentdrawerwidgetplugin.h
        src/plugin/fluentbuttonwidgetplugin.h
        src/plugin/fluentlistcardwidgetplugin.h
        src/plugin/fluentmessagebarwidgetplugin.h
        src/plugin/fluentgraphcardwidgetplugin.h
        src/plugin/fluentmodalwidgetplugin.h
        src/plugin/fluentplaincardcontainerextension.h
        src/plugin/fluentplaincardwidgetplugin.h

        # Plugin Sources
        src/plugin/fluentwidgetsplugin.cpp
        src/plugin/fluentcardwidgetplugin.cpp
        src/plugin/fluentdrawerwidgetplugin.cpp
        src/plugin/fluentbuttonwidgetplugin.cpp
        src/plugin/fluentlistcardwidgetplugin.cpp
        src/plugin/fluentmessagebarwidgetplugin.cpp
        src/plugin/fluentgraphcardwidgetplugin.cpp
        src/plugin/fluentmodalwidgetplugin.cpp
        src/plugin/fluentplaincardcontainerextension.cpp
        src/plugin/fluentplaincardwidgetplugin.cpp
    )

    target_link_libraries(FluentWidgetsDesignerPlugin PRIVATE
        FluentWidgetLib
        Qt6::Designer
    )

    set_target_properties(FluentWidgetsDesignerPlugin PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
    )
endif()

message(STATUS "FluentWidgetLib: Static library target created")

# Example usage instructions
message(STATUS "")
message(STATUS "=== FluentWidgetsPlugin Usage ===")
message(STATUS "Static Library: Link with FluentWidgetLib in your CMake project")
message(STATUS "Designer Plugin: Set BUILD_DESIGNER_PLUGIN=ON to build plugin")
message(STATUS "Widgets included: CardWidget, DrawerWidget, MessageBarWidget, GraphCardWidget, ModalWidget")
message(STATUS "=================================")