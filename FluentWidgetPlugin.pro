# FluentWidgetPlugin.pro - Qt Designer Plugin for Fluent Design Widgets
CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(fluentwidgetsplugin)
TEMPLATE    = lib

# Plugin Headers (Designer integration)
HEADERS     = src/plugin/fluentwidgetsplugin.h \
              src/plugin/fluentcardwidgetplugin.h \
              src/plugin/fluentdrawerwidgetplugin.h \
              src/plugin/fluentbuttonwidgetplugin.h \
              src/plugin/fluentlistcardwidgetplugin.h \
              src/plugin/fluentmessagebarwidgetplugin.h \
              src/plugin/fluentgraphcardwidgetplugin.h \
              src/plugin/fluentmodalwidgetplugin.h \
              src/plugin/fluentplaincardcontainerextension.h \
              src/plugin/fluentplaincardwidgetplugin.h \
              # Future plugins:
              # src/plugin/fluentmetricwidgetplugin.h \

# Widget Headers (Actual widget implementations)
HEADERS    += src/widget/fluentcardwidget.h \
              src/widget/fluentdrawerwidget.h \
              src/widget/fluentbuttonwidget.h \
              src/widget/fluentlistcardwidget.h \
              src/widget/fluentmessagebarwidget.h \
              src/widget/fluentgraphcardwidget.h \
              src/widget/fluentmodalwidget.h \
              src/widget/fluentplaincardwidget.h
              # Future widgets:
              # src/widget/fluentmetricwidget.h \

# Plugin Sources
SOURCES     = src/plugin/fluentwidgetsplugin.cpp \
              src/plugin/fluentcardwidgetplugin.cpp \
              src/plugin/fluentdrawerwidgetplugin.cpp \
              src/plugin/fluentbuttonwidgetplugin.cpp \
              src/plugin/fluentlistcardwidgetplugin.cpp \
              src/plugin/fluentmessagebarwidgetplugin.cpp \
              src/plugin/fluentgraphcardwidgetplugin.cpp \
              src/plugin/fluentmodalwidgetplugin.cpp \
              src/plugin/fluentplaincardcontainerextension.cpp \
              src/plugin/fluentplaincardwidgetplugin.cpp \
              # Future plugins:
              # src/plugin/fluentmetricwidgetplugin.cpp \

# Widget Sources  
SOURCES    += src/widget/fluentcardwidget.cpp \
              src/widget/fluentdrawerwidget.cpp \
              src/widget/fluentbuttonwidget.cpp \
              src/widget/fluentlistcardwidget.cpp \
              src/widget/fluentmessagebarwidget.cpp \
              src/widget/fluentgraphcardwidget.cpp \
              src/widget/fluentmodalwidget.cpp \
              src/widget/fluentplaincardwidget.cpp
              # Future widgets:
              # src/widget/fluentmetricwidget.cpp \

# Resources
RESOURCES   = resources/icons.qrc

# Include paths for the new nested structure
INCLUDEPATH += src/widget \
               src/plugin \
               src

# Qt modules
QT += designer \
      widgets \
      charts

# Installation
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

# Include additional configuration
include(fluentwidget.pri)